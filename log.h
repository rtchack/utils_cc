/**
 *  Created on: Mar 8, 2016
 *      Author: xing
 */

#pragma once

#include <iostream>
#include <cstdlib>

#define UTILS_SEVERITY_DBG 3
#define UTILS_SEVERITY_INF 2
#define UTILS_SEVERITY_WAR 1
#define UTILS_SEVERITY_ERR 0

#if NDEBUG
#define UTILS_CURRENT_SEVERITY UTILS_SEVERITY_INF
#else
#define UTILS_CURRENT_SEVERITY UTILS_SEVERITY_DBG
#endif


#define UTILS_STD_LOGGER(msg) std::cout << msg << std::endl;
#define UTILS_INIT_LOG UTILS_STD_LOGGER("Hello.")
#define UTILS_INIT_LOG_WITH(file_name) \
  UTILS_STD_LOGGER("Use STD_COUT instead of " << file_name)
#define UTILS_SHUTDOWN_LOG


// Debugging logger
#if UTILS_CURRENT_SEVERITY >= UTILS_SEVERITY_DBG
#define UTILS_DBG_ONLY(act) {act;}
#define lDbg(msg) UTILS_STD_LOGGER( \
  "DBG [" << __FUNCTION__ << " " << __LINE__ << "] " << msg);
#define printDbg(fmt, ...) CstyleLog(base::LogSeverity::DBG,\
  "[%s %d] " fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#else
#define lDbg(msg) ;
#define printDbg(fmt, ...) ;
#define UTILS_DBG_ONLY(act) ;
#endif

// Informative logger
#if UTILS_CURRENT_SEVERITY >= UTILS_SEVERITY_INF

#define lInf(msg) UTILS_STD_LOGGER( \
   "INF [" << __FUNCTION__ << "] " << msg);

#define printInf(fmt, ...) CstyleLog(base::LogSeverity::INF,\
   "[%s] " fmt, __FUNCTION__, ##__VA_ARGS__);
#else
#define lInf(msg) ;
#define printInf(fmt, ...) ;
#endif

// Warning logger
#if UTILS_CURRENT_SEVERITY >= UTILS_SEVERITY_WAR

#define lWar(msg) UTILS_STD_LOGGER( \
   "WAR [" << __FUNCTION__ << "] " << msg);

#define printWar(fmt, ...) CstyleLog(base::LogSeverity::WAR,\
  "[%s] " fmt, __FUNCTION__, ##__VA_ARGS__);
#else
#define lWar(msg) ;
#define printWar(fmt, ...) ;
#endif

// Erroneous logger
#define lErr(msg) UTILS_STD_LOGGER( \
   "ERR [" << __FUNCTION__ << "] " << msg);

#define printErr(fmt, ...) CstyleLog(base::LogSeverity::ERR,\
  "[%s] " fmt, __FUNCTION__, ##__VA_ARGS__);

#define lFatal(msg) {lErr(msg) exit(1);}
#define printFatal(fmt, ...) {printErr(fmt, ##__VA_ARGS__) exit(1);}


#define cDbg(msg) lDbg( "[" << Get_name() << "] " << msg)
#define cInf(msg) lInf( "[" << Get_name() << "] " << msg)
#define cWar(msg) lWar( "[" << Get_name() << "] " << msg)
#define cErr(msg) lErr( "[" << Get_name() << "] " << msg)
#define cFatal(msg) lFatal( "[" << Get_name() << "] " << msg)


namespace utils {

constexpr size_t UTILS_MAX_LOG_MSG_LENGTH{512};

enum class LogSeverity {
  DBG, INF, WAR, ERR
};

void CstyleLog(LogSeverity severity, const char *fmt, ...);

void PrintBinary(const char *tag, const void *buf, size_t buf_len);

#define lBinary(buf, buf_len) PrintBinary(__FUNCTION__, buf, buf_len);

}
