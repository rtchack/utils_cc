/**
 *  Created on: Mar 9, 2016
 *      Author: xing
 */

#include "base/log.h"

#ifdef BASE_USE_LOG4CPLUS
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#endif

namespace base{

#ifdef BASE_USE_LOG4CPLUS
	using namespace log4cplus;

	void InitLog() {
		SharedAppenderPtr fap{new FileAppender("default.log")};
		fap->setName("filelog");

		SharedAppenderPtr cap{new ConsoleAppender()};
		cap->setName("consolelog");

		std::unique_ptr<Layout> flay{new PatternLayout("%d{%m%d %H:%M:%S} %p - %m\n")};
		fap->setLayout(std::move(flay));
		std::unique_ptr<Layout> clay{new PatternLayout("%d{%m%d %H:%M:%S} %p - %m\n")};
		cap->setLayout(std::move(clay));

		Logger r_logger = Logger::getRoot();
		r_logger.addAppender(fap);
		r_logger.addAppender(cap);
	}
#endif

	void CstyleLog(LogSeverity severity, const char *fmt, ...){
		char msg[BASE_MAX_LOG_MSG_LENGTH];
		va_list args;
		va_start(args, fmt);
		vsnprintf(msg, BASE_MAX_LOG_MSG_LENGTH, fmt, args);
		va_end(args);

		switch(severity){
			case LogSeverity::DBG:
				lDbg(msg);
				return;
			case LogSeverity::INF:
				lInf(msg);
				return;
			case LogSeverity::WAR:
				lWar(msg);
				return;
			case LogSeverity::ERR:
				lErr(msg);
				return;
		}
	}

	void PrintBinary(const char *tag, const void *buf, size_t buf_len){
		char msg[1024];
		unsigned char *tmp_buf = (unsigned char *)buf;
		size_t index = 0;
		size_t i = 0;

		if(!tmp_buf) return;

#define LOG_BINARY_PUTN(fmt, ...) \
	index += snprintf(msg+index, 1000-index, fmt, ##__VA_ARGS__);\
	if(1000 <= index){\
		snprintf(msg+index, 24, "...");\
		goto print_point;\
	}

		LOG_BINARY_PUTN("[%s] {\n", tag)

		while(i<buf_len){
			LOG_BINARY_PUTN("%02x", tmp_buf[i])
			if(!(++i % 8)){
				LOG_BINARY_PUTN("  ")
			}else{
				LOG_BINARY_PUTN(" ")
			}
			if(!(i % 16)){
				LOG_BINARY_PUTN("\n")
			}
		}

		print_point:
		CstyleLog(LogSeverity::INF, "%s\n}\n", msg);
	}

}

