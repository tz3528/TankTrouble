#pragma once

#include <unordered_map>
#include <list>
#include <iterator>
#include <string>
#include <mutex>
#include <boost/pfr/core.hpp>
#include <shared_mutex>

using std::unordered_map;
using std::string;
using namespace boost;

/**
 * @brief 将任意数据结构按照属性序列化为基本类型，再将序列元素转换为字符串后拼接
 */
template <typename T>
string toString(const T& t) {
    if constexpr (std::is_fundamental_v<T> || std::is_enum_v<T>) {
        string value(sizeof(T), '\0');
        memcpy(value.data(), &t, sizeof(T));
        return value;
    }
    else {
        auto tuple = pfr::structure_to_tuple(t);
        std::string value;
        std::apply([&value](auto&&... args) -> void {
            ((value += toString(args)), ...);
            }, tuple);
        return value;
    }
}

template <typename T>
struct ContainerHash {
    size_t operator()(const T& t) const {
        if constexpr (std::is_fundamental_v<T> || std::is_enum_v<T>) {
            return std::hash<T>{}(t);
        }
        else {
            auto tuple = pfr::structure_to_tuple(t);
            size_t hash_value = 0;
            std::apply([&hash_value](auto&&... args) -> void {
                ((hash_value ^= ContainerHash<std::decay_t<decltype(args)>>{}(args)+0x9e3779b9 + (hash_value << 6) + (hash_value >> 2)), ...);
                }, tuple);
            return hash_value;
        }
    }
};

template <typename T>
struct ContainerEqual {
    bool operator()(const T& lhs, const T& rhs) const {
        if constexpr (std::is_fundamental_v<T> || std::is_enum_v<T>) {
            return lhs == rhs;
        }
        else {
            auto tuple_lhs = pfr::structure_to_tuple(lhs);
            auto tuple_rhs = pfr::structure_to_tuple(rhs);
            return std::apply([&](auto&&... lhs_elems) {
                return std::apply([&](auto&&... rhs_elems) {
                    // 使用逻辑与的折叠表达式实现短路比较
                    return (... && ContainerEqual<std::decay_t<decltype(lhs_elems)>>{}(
                        lhs_elems,
                        rhs_elems
                        ));
                    }, tuple_rhs);
                }, tuple_lhs);
            // 直接展开两个元组进行逐元素比较

        }
    }
};

template <typename T>
class Container {
public:
    Container() = default;

    ~Container() = default;

    typename std::list<T>::iterator begin() {
        std::shared_lock<std::shared_mutex> lock(mutex);
        return data.begin();
    }

    typename std::list<T>::iterator end() {
        std::shared_lock<std::shared_mutex> lock(mutex);
        return data.end();
    }

    void insert(const T& value) {
        std::unique_lock<std::shared_mutex> lock(mutex);
        auto it = data.insert(data.end(), value);
        index[value] = it;
    }

    void emplace_back(const T& value) {
        std::unique_lock<std::shared_mutex> lock(mutex);
        auto it = data.emplace(data.end(), value);
        index[value] = it;
    }

    T back() {
        return data.back();
    }

    typename std::list<T>::iterator  erase(const T& value) {
        std::unique_lock<std::shared_mutex> lock(mutex);
        auto it = index.find(value);
        if (it != index.end()) {
            data.erase(it->second);
            index.erase(it);
        }
        return (it != index.end()) ? std::next(it->second) : data.end();
    }

    bool empty() {
        std::shared_lock<std::shared_mutex> lock(mutex);
        return data.empty();
    }

    int size() {
        std::shared_lock<std::shared_mutex> lock(mutex);
        return data.size();
    }

    void clear() {
        std::unique_lock<std::shared_mutex> lock(mutex);
        data.clear();
        index.clear();
    }

    std::shared_mutex& getMutex() {
        return mutex;
    }
    /**
     * @brief 将容器内容打包成字符串
     * @return 一个字符串
     */
    string toMessage() {
        std::shared_lock<std::shared_mutex> lock(mutex);
        string message;
        int length = data.size();
        message += toString(length);
        for (auto& item : data) {
            message += toString(item);
        }
        return message;
    }

private:
    std::shared_mutex mutex;
    std::list<T> data;
    std::unordered_map<T, typename std::list<T>::iterator, ContainerHash<T>, ContainerEqual<T>> index; // 使用自定义哈希函数
};

