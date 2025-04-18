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

#include <stdarg.h>  
#include "appLog/include/AppLog.hpp"

std::unique_ptr<AppLog> AppLog::m_log_intstance = nullptr;
std::mutex AppLog::m_instance_mutex;

//-----------------------------------
// static public functions 
//-----------------------------------
spdlog::level::level_enum AppLog::findLogLevelEnum(const std::string& LogLevel)
{
	if (LogLevel == "off") { return spdlog::level::off; }
	else    if (LogLevel == "trace") { return spdlog::level::trace; }
	else    if (LogLevel == "debug") { return spdlog::level::debug; }
	else    if (LogLevel == "info") { return spdlog::level::info; }
	else    if (LogLevel == "warn") { return spdlog::level::warn; }
	else    if (LogLevel == "err") { return spdlog::level::err; }
	else    if (LogLevel == "critical") { return spdlog::level::critical; }
	else
	{
		std::string err_str = "findLogLevelEnum -> log_level must be one of: off, trace, debug, info, warn, err, critical.";
		throw std::runtime_error(err_str);
	}
}

std::string AppLog::findLogLevelString(const spdlog::level::level_enum LogLevelEnum)
{
	if (LogLevelEnum == spdlog::level::off) { return "off"; }
	else    if (LogLevelEnum == spdlog::level::trace) { return "trace"; }
	else    if (LogLevelEnum == spdlog::level::debug) { return "debug"; }
	else    if (LogLevelEnum == spdlog::level::info) { return "info"; }
	else    if (LogLevelEnum == spdlog::level::warn) { return "warn"; }
	else    if (LogLevelEnum == spdlog::level::err) { return "err"; }
	else    if (LogLevelEnum == spdlog::level::critical) { return "critical"; }
	else
	{
		std::string err_str =
			"findLogLevelString -> log enum not recognized... log_level must be one of: off, trace, debug, info, warn, err, critical.";
		throw std::runtime_error(err_str);
	}
}

/**
 * The first time we call getInstance we will lock the storage location
 *      and then we make sure again that the variable is null and then we
 *      set the logFileStr. RU:
 */
AppLog* AppLog::getInstance()
{
	if (m_log_intstance == nullptr)
	{
		m_log_intstance.reset( new AppLog() );
	}
	return m_log_intstance.get();
}


//-----------------------------------
//  AppLog class functions 
//-----------------------------------

AppLog::AppLog()
{
	AppLog::m_setup_complete = false;
}

void AppLog::setupDistSinkAndLoggingLevel(
	const std::string& logFileStr,
	const std::string& logger_name,
	const spdlog::level::level_enum loggingLevel,
	const uint64_t file_size_MB, 
	const uint64_t num_files,
	const bool enableConsoleSink)
{
	std::lock_guard<std::mutex> lock(m_instance_mutex);

	if (m_setup_complete) {
		return;
	}

	/***************************/
	// Set up log file str and logging level
	/***************************/
	m_log_file_path = logFileStr;
	m_logger_name = logger_name;
	m_log_level = loggingLevel;

	/***************************/
	// Set up dist sink
	/***************************/
	// configure so that each log message gets sent to all sinks.
	m_dist_sink = std::make_shared<spdlog::sinks::dist_sink_mt>();

	// console sink
	if (enableConsoleSink) {
		m_dist_sink->add_sink(spdlog::sinks::stdout_sink_mt::instance());
	}

	// all messages file sink.  <num_files> x <file_size_MB> MB rotating logs 
	m_dist_sink->add_sink(
		std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
			AppLog::m_log_file_path.c_str(),
			1024 * 1024 * file_size_MB,
			num_files));

	/***************************/
	// Save set up boolean
	/***************************/
	m_setup_complete = true;

	m_logger_ptrs = spdlog::get(logger_name);
	if (m_logger_ptrs == nullptr)
	{
		spdlog::register_logger(std::make_shared<spdlog::logger>(logger_name, m_dist_sink));
		m_logger_ptrs = spdlog::get(logger_name);
		m_logger_ptrs->flush_on(spdlog::level::err);

		std::string log_level_str = findLogLevelString(m_log_level);

		m_logger_ptrs->set_level(m_log_level);

		m_logger_ptrs->trace(
			"SpdLogHelper::GetLogger -> Set {} logging level to {}",
			logger_name, log_level_str);
	}
}

void AppLog::updateLoggingLevel( const int loggingLevel)
{
	{
		std::lock_guard<std::mutex> lock(m_instance_mutex);
		AppLog::m_log_level = static_cast<spdlog::level::level_enum>(loggingLevel);
	}

	m_logger_ptrs->info(
		"updateLoggingLevel -> Logging level updated to {}",
		AppLog::findLogLevelString(AppLog::m_log_level));
}

void AppLog::logMsg(const int level, const char* msg)
{
	spdlog::level::level_enum L = static_cast<spdlog::level::level_enum>(level);
	if (L == spdlog::level::trace)
		m_logger_ptrs->trace(msg);
	else if (L == spdlog::level::debug)
		m_logger_ptrs->debug(msg);
	else if (L == spdlog::level::info)
		m_logger_ptrs->info(msg);
	else if (L == spdlog::level::warn)
		m_logger_ptrs->warn(msg);
	else if (L == spdlog::level::err)
		m_logger_ptrs->error(msg);
	else if (L == spdlog::level::critical)
		m_logger_ptrs->critical(msg);

	m_logger_ptrs->flush();
}

void AppLog::logMsg(const int level, const char* filePath, int line, const char* fmt, ...)
{
	std::lock_guard<std::mutex> lock(m_instance_mutex);  // Ensure thread-safety
	if (level < static_cast<int>(m_log_level)) {
		return;
	}

	static char buffer[1024];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, 1024, fmt, args);
	va_end(args);

	const char* fileName = get_filename(filePath);

	std::string msg = std::string(fileName) + ":" + std::to_string(line) + " - " + buffer;
	this->logMsg(level, msg.c_str());
}

//--------------- global funcs -------------------
void setupAppLog(const std::string& logger_file_path, const std::string& logger_name, int logger_level, uint64_t file_size_MB, uint64_t num_files, bool enableConsoleSink)
{
	//todo: check existence and make logging_directory
	spdlog::level::level_enum logLevel;
	if (logger_level < (int)spdlog::level::trace)
		logLevel = spdlog::level::trace;
	else if (logger_level > (int)spdlog::level::off)
		logLevel = spdlog::level::off;
	else
		logLevel = (spdlog::level::level_enum)logger_level;

	// Get instance of spdloghelper and create the dist sink
	auto slh = AppLog::getInstance();
	slh->setupDistSinkAndLoggingLevel(logger_file_path, logger_name, logLevel, file_size_MB, num_files, enableConsoleSink);

	//spdlog::set_pattern("[source %s] [function %!] [line %#] %v");

	std::string msg = std::string(__FILE__) + ":" + std::to_string(__LINE__) + " - " + "=== start log:" + logger_file_path + "===";
	slh->logMsg(0, msg.c_str());

	msg = std::string(__FILE__) + ":" + std::to_string(__LINE__) + " - " + "logger_name=" + logger_name + "logLevel=" + AppLog::findLogLevelString(logLevel);
	slh->logMsg(0, msg.c_str());
}
