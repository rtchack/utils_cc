/**
 *  Created on: Mar 9, 2016
 *      Author: xing
 */

#include "base/log.h"
#include "base/macro_utils.h"

#include <stdarg.h>


#ifdef BASE_USE_LOG4CPLUS
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#endif

namespace base{

#ifdef BASE_USE_LOG4CPLUS
	using namespace log4cplus;

	const static std::string LOG_LAYOUT{"%d{%m%d %H:%M:%S} %p - %m\n"};

	void InitLog() {
		SharedAppenderPtr cap{new ConsoleAppender()};
		cap->setName("std");
		std::unique_ptr<Layout> clay{new PatternLayout(LOG_LAYOUT)};
		cap->setLayout(std::move(clay));
		auto r_logger = Logger::getRoot();
		r_logger.addAppender(cap);
	}

	void InitLog(const std::string &log_file){
		InitLog();
		SharedAppenderPtr fap{new RollingFileAppender(log_file,
		                                              1 << 24,
		                                              8)};
		fap->setName(log_file);
		std::unique_ptr<Layout> flay{new PatternLayout(LOG_LAYOUT)};
		fap->setLayout(std::move(flay));
		auto r_logger = Logger::getRoot();
		r_logger.addAppender(fap);
	}

#endif

#define LOG_TRUNCATED_NOTIFY_MSG " [truncated!]"

	constexpr size_t TRUNCAT_NOTIFY_MSG_LEN{16};
	constexpr size_t REAL_BASE_MAX_LOG_MSG_LENGTH{
			BASE_MAX_LOG_MSG_LENGTH - TRUNCAT_NOTIFY_MSG_LEN};
	constexpr size_t REAL_BASE_MAX_LOG_MSG_LENGTH_MINUS_1{
			REAL_BASE_MAX_LOG_MSG_LENGTH - 1};

	void CstyleLog(LogSeverity severity, const char *fmt, ...){
		char msg[BASE_MAX_LOG_MSG_LENGTH];
		va_list args;
		va_start(args, fmt);
		if(REAL_BASE_MAX_LOG_MSG_LENGTH_MINUS_1 <= vsnprintf(
				msg, REAL_BASE_MAX_LOG_MSG_LENGTH, fmt, args)){
			snprintf(msg + REAL_BASE_MAX_LOG_MSG_LENGTH_MINUS_1 - 1,
			         TRUNCAT_NOTIFY_MSG_LEN,
			         LOG_TRUNCATED_NOTIFY_MSG);
		}
		va_end(args);

		switch(severity){
			case LogSeverity::DBG:
#ifdef BASE_USE_LOG4CPLUS
				LOG4CPLUS_DEBUG(Logger::getRoot(), msg);
#else
				BASE_STD_LOGGER("DBG " << msg);
#endif
				return;
			case LogSeverity::INF:
#ifdef BASE_USE_LOG4CPLUS
				LOG4CPLUS_INFO(Logger::getRoot(), msg);
#else
				BASE_STD_LOGGER("INF " << msg);
#endif
				return;
			case LogSeverity::WAR:
#ifdef BASE_USE_LOG4CPLUS
				LOG4CPLUS_WARN(Logger::getRoot(), msg);
#else
				BASE_STD_LOGGER("WAR " << msg);
#endif
				return;
			case LogSeverity::ERR:
#ifdef BASE_USE_LOG4CPLUS
				LOG4CPLUS_ERROR(Logger::getRoot(), msg);
#else
				BASE_STD_LOGGER("ERR " << msg);
#endif
				return;
		}
	}

	void PrintBinary(const char *tag, const void *buf, size_t buf_len){

		char msg[BASE_MAX_LOG_MSG_LENGTH];
		auto *tmp_buf = (const unsigned char *)buf;
		size_t index = 0;
		size_t i = 0;

		unless(tmp_buf) return;

#define LOG_BINARY_PUTN(fmt, ...) \
	index += snprintf(msg+index, (REAL_BASE_MAX_LOG_MSG_LENGTH_MINUS_1 - index), fmt, ##__VA_ARGS__);\
	if(REAL_BASE_MAX_LOG_MSG_LENGTH_MINUS_1 <= index){\
		snprintf(msg+index, 24, "...");\
		goto print_point;\
	}

		LOG_BINARY_PUTN("[%s] {\n", tag)

		while(i<buf_len){
			LOG_BINARY_PUTN("%02x", tmp_buf[i])
			unless((++i % 8)){
				LOG_BINARY_PUTN("  ")
			}else{
				LOG_BINARY_PUTN(" ")
			}
			unless((i % 16)){
				LOG_BINARY_PUTN("\n")
			}
		}

		print_point:
		CstyleLog(LogSeverity::INF, "%s\n}\n", msg);
	}

}

