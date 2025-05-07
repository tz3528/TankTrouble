#pragma once


#include <mutex>
#include <memory>
#include <list>


constexpr size_t DEFAULT_BLOCK_SIZE = 1024;
constexpr size_t MAX_BLOCK_SIZE = 128 * 1024;

class MemoryBlock {
public:
	std::unique_ptr<uint8_t[]> data;
	size_t current_size = 0;
	size_t capacity;

	MemoryBlock(size_t size = DEFAULT_BLOCK_SIZE) 
        : data(std::make_unique<uint8_t[]>(size)), 
          capacity(size) {}

	size_t remain() const { return capacity - current_size; }
private:
	
	
};

class MemoryPool {
public:
	explicit MemoryPool(size_t max_blocks = MAX_BLOCK_SIZE);
	~MemoryPool() = default;

	std::unique_ptr<MemoryBlock> allocate();
    /**
     * @brief 用于回收内存块，参数必须使用move移交指针的所有权
     * @param block 被回收的内存块
     */
    void deallocate(std::unique_ptr<MemoryBlock> block);
	/**
	 * @brief 将链式的内存块回收，参数必须使用move移交指针的所有权
	 * @param blocks 被回收的内存块
	 */
	void deallocate(std::list<std::unique_ptr<MemoryBlock>> blocks);

private:
	std::mutex mutex_;
    std::list<std::unique_ptr<MemoryBlock>> blocks_;
	size_t max_blocks_;
};
