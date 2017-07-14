/**
 *  Created on: Mar 8, 2016
 *      Author: xing
 */

#ifndef BASE_COMMON_INC_LOG_H_
#define BASE_COMMON_INC_LOG_H_

#include <iostream>
#include <cstdlib>

#define BASE_SEVERITY_DBG 3
#define BASE_SEVERITY_INF 2
#define BASE_SEVERITY_WAR 1
#define BASE_SEVERITY_ERR 0

#define BASE_CURRENT_SEVERITY BASE_SEVERITY_INF

#define BASE_USE_LOG4CPLUS 1

#ifdef BASE_USE_LOG4CPLUS
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#endif


#ifdef BASE_USE_LOG4CPLUS
#define BASE_INIT_LOG base::InitLog(); lInf("Hello.");
#define BASE_INIT_LOG_WITH(file_name) \
	base::InitLog(file_name); lInf("Hello.");
#define BASE_SHUTDOWN_LOG log4cplus::Logger::getRoot().shutdown();
#else
#define BASE_STD_LOGGER(msg) std::cout << msg << std::endl;
#define BASE_INIT_LOG BASE_STD_LOGGER("Hello.")
#define BASE_INIT_LOG_WITH(file_name) \
	BASE_STD_LOGGER("Use STD_COUT instead of " << file_name)
#define BASE_SHUTDOWN_LOG
#endif


	// Debugging logger
#if BASE_CURRENT_SEVERITY >= BASE_SEVERITY_DBG

#ifdef BASE_USE_LOG4CPLUS
#define lDbg(msg) LOG4CPLUS_DEBUG(log4cplus::Logger::getRoot(),\
	"[" << __FILE__ << " " << __LINE__ << "] " << msg);
#else
#define lDbg(msg) BASE_STD_LOGGER( \
	"DBG [" << __FILE__ << " " << __LINE__ << "] " << msg);
#endif

#define printDbg(fmt, ...) CstyleLog(base::LogSeverity::DBG,\
	"[%s %d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__);
#else
#define lDbg(msg) ;
#define printDbg(fmt, ...) ;
#endif

	// Informative logger
#if BASE_CURRENT_SEVERITY >= BASE_SEVERITY_INF

#ifdef BASE_USE_LOG4CPLUS
#define lInf(msg) LOG4CPLUS_INFO(log4cplus::Logger::getRoot(),\
	 "[" << __FUNCTION__ << "] " << msg);
#else
#define lInf(msg) BASE_STD_LOGGER( \
	 "INF [" << __FUNCTION__ << "] " << msg);
#endif

#define printInf(fmt, ...) CstyleLog(base::LogSeverity::INF,\
	 "[%s] " fmt, __FUNCTION__, ##__VA_ARGS__);
#else
#define lInf(msg) ;
#define printInf(fmt, ...) ;
#endif

	// Warning logger
#if BASE_CURRENT_SEVERITY >= BASE_SEVERITY_WAR

#ifdef BASE_USE_LOG4CPLUS
#define lWar(msg) LOG4CPLUS_WARN(log4cplus::Logger::getRoot(),\
	 "[" << __FUNCTION__ << "] " << msg);
#else
#define lWar(msg) BASE_STD_LOGGER( \
	 "WAR [" << __FUNCTION__ << "] " << msg);
#endif

#define printWar(fmt, ...) CstyleLog(base::LogSeverity::WAR,\
	"[%s] " fmt, __FUNCTION__, ##__VA_ARGS__);
#else
#define lWar(msg) ;
#define printWar(fmt, ...) ;
#endif

	// Erroneous logger
#ifdef BASE_USE_LOG4CPLUS
#define lErr(msg) LOG4CPLUS_ERROR(log4cplus::Logger::getRoot(),\
	 "[" << __FUNCTION__ << "] " << msg);
#else
#define lErr(msg) BASE_STD_LOGGER( \
	 "ERR [" << __FUNCTION__ << "] " << msg);
#endif

#define printErr(fmt, ...) CstyleLog(base::LogSeverity::ERR,\
	"[%s] " fmt, __FUNCTION__, ##__VA_ARGS__);

#define lFatal(msg) {lErr(msg) exit(1);}
#define printFatal(fmt, ...) {printErr(fmt, ##__VA_ARGS__) exit(1);}


#define cDbg(msg) lDbg( "[" << Get_name() << "] " << msg)
#define cInf(msg) lInf( "[" << Get_name() << "] " << msg)
#define cWar(msg) lWar( "[" << Get_name() << "] " << msg)
#define cErr(msg) lErr( "[" << Get_name() << "] " << msg)
#define cFatal(msg) lFatal( "[" << Get_name() << "] " << msg)


namespace base{

#ifdef BASE_USE_LOG4CPLUS
	void InitLog();

	void InitLog(const std::string log_file);
#endif

	enum class LogSeverity {DBG, INF, WAR, ERR};
	constexpr size_t BASE_MAX_LOG_MSG_LENGTH{512};
	void CstyleLog(LogSeverity severity, const char *fmt, ...);

	void PrintBinary(const char *tag, const void *buf, size_t buf_len);

#define lBinary(buf, buf_len) PrintBinary(__FUNCTION__, buf, buf_len);

}

#endif /* BASE_COMMON_INC_LOG_H_ */
