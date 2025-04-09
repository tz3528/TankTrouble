#pragma once

#include "TimerManager.hpp"

#include <mutex>
#include <functional>
#include <map>
#include <string>
#include <chrono>
#include <stack>
#include <thread>
#include <source_location>
#include <fstream>
#include <filesystem>

constexpr short ONC			= 1;
constexpr short TWO			= 2;
constexpr short THREE		= 3;
constexpr short FOUR		= 4;
constexpr short FIVE		= 5;
constexpr short SIX			= 6;
constexpr short SEVEN		= 7;
constexpr short EIGHT		= 8;
constexpr short NINE		= 9;
constexpr short TEN			= 10;
//这里考虑到自然溢出，执行次数仍然是有限的
constexpr short THROUGHOUT	= -1;

/**
 * @brief 监测器，用于监测某段代码的运行时间或是某个函数的运行次数
 * 日志信息的格式为:
 * 日志类型,函数名,日志信息,行号,线程编号,文件名
 */
class Monitor {
public:
	Monitor(std::string path) {
		auto now = std::chrono::system_clock::now();
		auto time_t_now = std::chrono::system_clock::to_time_t(now);
		std::tm tm_now;

	#ifdef _WIN32
		localtime_s(&tm_now, &time_t_now); // Windows 平台
	#else
		localtime_r(&time_t_now, &tm_now); // POSIX 平台
	#endif
		path += "/Log";
		std::filesystem::create_directories(path);
		std::ostringstream oss;
		oss << path << "/Log" 
			<< std::put_time(&tm_now, "%Y-%m-%d_%H-%M-%S")
			<< ".csv";

		 filename = oss.str();

	}

	~Monitor() = default;

	
	inline void BeginPerRun(int times=THROUGHOUT,const std::source_location& location = std::source_location::current());
	inline void EndPerRun(const std::source_location& location = std::source_location::current());


	inline void PerSecond(int duration, short times=THROUGHOUT,const std::source_location& location = std::source_location::current());
private:
	using TimePoint = high_resolution_clock::time_point;

	TimerManager timerManager = TimerManager();

	static std::stack<TimePoint>& GetStack() {
		thread_local std::stack<TimePoint> stack;
		return stack;
	}
	inline void LogDuration(
		string functionName,int duration,
		const std::source_location& BeginLocation,
        const std::source_location& EndLocation,
        std::thread::id thread_id
	);
	inline void LogTimer(string functionName,int count,const std::source_location& location,std::thread::id thread_id);

	std::string filename;
	std::unordered_map <string, int> RunTable;
	std::unordered_map <string, std::source_location> FunctionInfo;
	std::unordered_map <string, int> SecondTable;
    std::unordered_map <string, int> Count;

	std::mutex mutex;

};
/**
 * @brief 插入开始计时标记
 * @param f 
 * @param times 
 */
inline void Monitor::BeginPerRun(int times,const std::source_location& location) {
	auto function = std::string(location.file_name());

	if (RunTable.find(function) == RunTable.end()) {
		RunTable[function] = times;
		FunctionInfo[function]= location;
	}
	if (RunTable[function] == 0) return;
	RunTable[function]--;
	auto& stack = GetStack();
	stack.push(std::chrono::high_resolution_clock::now());
}
/**
 * @brief 插入结束计时标记，记录当前时刻距离运行开始时刻的时间
 */
inline void Monitor::EndPerRun(const std::source_location& location) {
	auto function = std::string(location.file_name());

	if (RunTable[function] == 0) return;

	auto end_time = std::chrono::high_resolution_clock::now();
	auto& stack = GetStack();

	if (stack.empty()) {
		//LogError("Unmatched end() call detected");
		return;
	}

	auto start_time = stack.top();
	stack.pop();
	LogDuration(
		location.function_name(),
		std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count(), 
		FunctionInfo[function], 
		location, 
		std::this_thread::get_id()
	);
}
/**
 * @brief			用于记录每段时间间隔运行次数
 * @param duration	时间间隔
 * @param times		剩余运行次数
 * @param location	运行函数信息
 */
inline void Monitor::PerSecond(int duration, short times,const std::source_location& location) {
	auto function = location.function_name();

	if (SecondTable.find(function) == SecondTable.end()) {
		SecondTable[function] = times;
		timerManager.addTask(
			std::hash<std::thread::id>{}(std::this_thread::get_id())+std::hash<std::string>{}(location.function_name()), duration,
			[this, duration, function, location]() ->int {
				if (SecondTable[function] == 0) {
					return 0;
				}
				LogTimer(
					location.function_name(),
					Count[function],
					location,
					std::this_thread::get_id()
				);
				Count[function] = 0;
				return duration;
			});
	}
	if (SecondTable[function] == 0) return;
	SecondTable[function]--;
	Count[function]++;
}
/**
 * @brief					用于记录某段代码的运行时间
 * @param functionName		代码片段所属的函数名
 * @param duration			运行时间
 * @param BeginLocation		开始记录时的信息
 * @param EndLocation		结束记录时的信息
 * @param thread_id			线程id
 */
inline void Monitor::LogDuration(string functionName, int duration,const std::source_location& BeginLocation,const std::source_location& EndLocation,std::thread::id thread_id){
	std::lock_guard<std::mutex> lock(mutex);

	// 打开日志文件
	std::ofstream logFile(filename, std::ios::app);
	if (!logFile.is_open()) {
		// 如果文件无法打开，可以选择抛出异常或记录错误
		return;
	}

	// 写入日志信息
	logFile << "LogDuration,"	<< functionName << "," 
			<< "Duration: "		<< duration << "μs,"
			<< "filename: "		<< BeginLocation.file_name() << ","
			<< "Begin: "		<< BeginLocation.line() << ","
			<< "End: "			<< EndLocation.line() << ","
			<< "ThreadID: "		<< thread_id << ","
			<< std::endl;

	logFile.close();

}
/**
 * @brief				用于记录个函数在固定时间间隔内的运行次数
 * @param functionName	运行函数名
 * @param count			运行次数
 * @param location		运行函数信息
 * @param thread_id		线程id
 */
inline void Monitor::LogTimer(string functionName,int count,const std::source_location& location,std::thread::id thread_id) {
	std::lock_guard<std::mutex> lock(mutex);

	// 打开日志文件
	std::ofstream logFile(filename, std::ios::app);
	if (!logFile.is_open()) {
		// 如果文件无法打开，可以选择抛出异常或记录错误
		return;
	}

	// 写入日志信息
	logFile << "LogTimer,"	<< functionName << ","
			<< "Count: "	<< count << "EPS" << ","
			<< "filename: " << location.file_name() << ","
			<< "Location: " << location.line() << ","
			<< "ThreadID: " << thread_id << ","
			<< std::endl;

	logFile.close();
}

