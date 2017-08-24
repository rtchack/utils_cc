/**
 *  Created on: Mar 5, 2016
 *      Author: xing
 */

#include "base/common.h"

#include <sys/resource.h>

namespace base{

	std::string to_string(Ret v) noexcept{
#define CR_DESCRIBE(s) \
  case Ret::s:\
  return #s

		switch(v){
			CR_DESCRIBE(OK);
			CR_DESCRIBE(NO);
			CR_DESCRIBE(E_INIT);
			CR_DESCRIBE(E_ARG);
			CR_DESCRIBE(E_ARG_NULL);
			CR_DESCRIBE(E_FILE_OP);
			CR_DESCRIBE(E_FILE_CON);
			CR_DESCRIBE(E_MEM);
			CR_DESCRIBE(E_UNSUP);
			CR_DESCRIBE(E_ALG);
			CR_DESCRIBE(E_BUF_OVERFLOW);
			CR_DESCRIBE(E_IO_PENDING);
			CR_DESCRIBE(E_SOCKET_NOT_CONNECTED);
			CR_DESCRIBE(E_GENERAL);
			default:
				return "UNEXPECTED_RETURN";
		}
	}

	void EnableCoreDump() noexcept{
#ifdef NDEBUG
		return;
#else
		rlimit lim;

		unless(getrlimit(RLIMIT_CORE, &lim)){
			lim.rlim_cur = RLIM_INFINITY;
			lim.rlim_max = RLIM_INFINITY;
			setrlimit(RLIMIT_CORE, &lim);
		}
#endif
	}

}
