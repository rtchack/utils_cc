/**
 *  Created on: Mar 8, 2016
 *      Author: xing
 */

#ifndef BASE_COMMON_INC_LOG_H_
#define BASE_COMMON_INC_LOG_H_

#include <cstdlib>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

namespace base{

#define BASE_SEVERITY_DBG 3
#define BASE_SEVERITY_INF 2
#define BASE_SEVERITY_WAR 1
#define BASE_SEVERITY_ERR 0

#define BASE_CURRENT_SEVERITY BASE_SEVERITY_DBG

#define INIT_LOG base::InitLog(); lInf("Hello.");

#define LOG_PUT_ATTR(i) << "\n\t"#i " " << i
#define LOG_PUT_ITER(i, name) << ", "#i "-" << *(name + i)

#define BASE_CURRENT_LOGGER (log4cplus::Logger::getRoot())


#if BASE_CURRENT_SEVERITY >= BASE_SEVERITY_DBG
#define lDbg(msg) LOG4CPLUS_DEBUG(BASE_CURRENT_LOGGER,\
	"[" << __FUNCTION__ << " " << __LINE__ << "] " << msg);
#define printDbg(fmt, ...) CstyleLog(LogSeverity::DBG,\
	"[%s %d] " fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#else
#define lDbg(msg) ;
#define printDbg(fmt, ...) ;
#endif

#if BASE_CURRENT_SEVERITY >= BASE_SEVERITY_INF
#define lInf(msg) LOG4CPLUS_INFO(BASE_CURRENT_LOGGER,\
	 "[" << __FUNCTION__ << "] " << msg);
#define printInf(fmt, ...) CstyleLog(LogSeverity::INF,\
	 "[%s %d] " fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#else
#define lInf(msg) ;
#define printInf(fmt, ...) ;
#endif

#if BASE_CURRENT_SEVERITY >= BASE_SEVERITY_WAR
#define lWar(msg) LOG4CPLUS_WARN(BASE_CURRENT_LOGGER,\
	 "[" << __FILE__ << " " << __LINE__ << "] " << msg);
#define printWar(fmt, ...) CstyleLog(LogSeverity::WAR,\
	"[%s %d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__);
#else
#define lWar(msg) ;
#define printWar(fmt, ...) ;
#endif

#define lErr(msg) LOG4CPLUS_ERROR(BASE_CURRENT_LOGGER,\
	 "[" << __FILE__ << " " << __LINE__ << "] " << msg);
#define printErr(fmt, ...) CstyleLog(LogSeverity::ERR,\
	"[%s %d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__);

#define lFatal(msg) {lErr(msg) exit(1);}
#define printFatal(fmt, ...) {printErr(fmt, ##__VA_ARGS__) exit(1);}

#define lBinary(buf, buf_len) PrintBinary(__FUNCTION__, buf, buf_len);


#define cDbg(msg) lDbg( "[" << Get_name() << "] " << msg)
#define cInf(msg) lInf( "[" << Get_name() << "] " << msg)
#define cWar(msg) lWar( "[" << Get_name() << "] " << msg)
#define cErr(msg) lErr( "[" << Get_name() << "] " << msg)
#define cFatal(msg) lFatal( "[" << Get_name() << "] " << msg)

#define iDbg(inst, msg) lDbg( "[" << inst->Get_name() << "] " << msg)
#define iInf(inst, msg) lInf( "[" << inst->Get_name() << "] " << msg)
#define iWar(inst, msg) lWar( "[" << inst->Get_name() << "] " << msg)
#define iErr(inst, msg) lErr( "[" << inst->Get_name() << "] " << msg)
#define iFatal(inst, msg) lFatal( "[" << inst->Get_name() << "] " << msg)


#define BASE_ASSERT_FALSE(act) \
	if((act)){\
		lFatal("#act")\
	}
#define BASE_ASSERT_TRUE(act) BASE_ASSERT_FALSE(!(act))

#define BASE_EXPECT_FALSE(act) \
	if((act)){\
		lErr("#act")\
	}
#define BASE_EXPECT_TRUE(act) BASE_EXPECT_FALSE(!(act))


	constexpr size_t BASE_MAX_LOG_MSG_LENGTH{512};

	enum class LogSeverity {DBG, INF, WAR, ERR};

	void CstyleLog(LogSeverity severity, const char *fmt, ...);

	void InitLog();

	void PrintBinary(const char *tag, const void *buf, size_t buf_len);

}

#endif /* BASE_COMMON_INC_LOG_H_ */
