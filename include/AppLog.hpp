/*
*------------------------------------------------------------------------
*AppLog.hpp
*
* This code was developed by Shunguang Wu in his spare time. No government
* or any client funds were used.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and its documentation, to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the software, and
* to permit persons to whom the software is furnished to do so, subject to
* the following conditions:
*
* 1. The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the software.
*
* 2.THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES, OR
* OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT, OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.*
*
* Shunguang Wu, All Right Reserved, 02/23/2025
*-------------------------------------------------------------------------
*/

#pragma once
#include "appLog/spdlog/include/spdlog/spdlog.h"
#include "appLog/spdlog/include/spdlog/sinks/dist_sink.h"

#define SPD_LOG_STRING(var)	#var

//uinveral API, you may add __FUNCTION__ into it
#define APP_LOG(L,fmt, ...)	 AppLog::getInstance()->logMsg(L, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

//specific APIs, you may add __FUNCTION__ into them
#define APP_TRACE(fmt, ...) AppLog::getInstance()->logMsg(0, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define APP_DEBUG(fmt, ...) AppLog::getInstance()->logMsg(1, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define APP_INFO(fmt, ...)  AppLog::getInstance()->logMsg(2, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define APP_WARN(fmt, ...)  AppLog::getInstance()->logMsg(3, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define APP_ERR(fmt, ...)   AppLog::getInstance()->logMsg(4, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define APP_CRITICAL(fmt, ...)   AppLog::getInstance()->logMsg(5, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

//global API functions
void setupAppLog(const std::string& logger_file_path, const std::string& logger_name, 
	int logger_level = (int)spdlog::level::debug, uint64_t file_size_MB = 1000, uint64_t num_files = 50, bool enableConsoleSink=false);

class AppLog
{
protected:
	//Singleton pattern
	AppLog();
	AppLog(AppLog& other) = delete;
	void operator=(const AppLog&) = delete;

public:
	/**
	 * This is the static method that controls the access to the AppLog
	 * instance. On the first run, it creates a singleton object and places it
	 * into the static field. On subsequent runs, it returns the client existing
	 * object stored in the static field.
	 */
	static AppLog* getInstance();
	static spdlog::level::level_enum findLogLevelEnum(const std::string& LogLevel);
	static std::string findLogLevelString(const spdlog::level::level_enum LogLevelEnum);
	static inline const char* get_filename(const char* path) {
		// Look for the last occurrence of '/' or '\\'
		const char* file = std::strrchr(path, '/');
		if (!file) {
			file = std::strrchr(path, '\\');
		}
		// If a separator is found, return the substring after it; otherwise return the whole path.
		return file ? file + 1 : path;
	}

	void setupDistSinkAndLoggingLevel(
		const std::string& logFileStr, 
		const std::string& logger_name,
		const spdlog::level::level_enum loggingLevel,
		const uint64_t file_size_MB, 
		const uint64_t num_files,
		const bool enableConsoleSink=false);

	void updateLoggingLevel(const int loggingLevel); //TODO:: need to do any checks to make sure this comes after Setup??

	inline std::string getFilePath()  
	{ 
		std::lock_guard<std::mutex> lock(m_instance_mutex);
		return m_log_file_path;
	}

	inline std::string getName()  
	{ 
		std::lock_guard<std::mutex> lock(m_instance_mutex);
		return m_logger_name;
	}
	inline int getLevel()
	{
		std::lock_guard<std::mutex> lock(m_instance_mutex);
		return static_cast<int>(m_log_level);
	}

	void logMsg(const int level, const char* filePath, int line, const char* fmt, ...);
	void logMsg(const int level, const char* msg);

protected:

	//Static Members : Tied to the class itself, shared across all instances, and independent of the singleton object.
	static std::unique_ptr<AppLog> m_log_intstance;
	static std::mutex m_instance_mutex;

	//Non-Static Members: Tied to the singleton instance, encapsulated, and instance-specific
	std::string m_logger_name = "appLog";
	bool m_setup_complete= false;
	std::string m_log_file_path = "./log.txt";
	spdlog::level::level_enum m_log_level = spdlog::level::info;
	std::shared_ptr<spdlog::sinks::dist_sink_mt> m_dist_sink=nullptr;

	std::shared_ptr<spdlog::logger> m_logger_ptrs=nullptr;
};

