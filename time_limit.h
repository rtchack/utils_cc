/**
 * Created on: 5/31/17
 *     Author: xing
 */

#ifndef BASE_DEV_TIMELIMIT_H
#define BASE_DEV_TIMELIMIT_H

#include <time.h>

#include "base/log.h"
#include "base/common.h"

namespace base{

	/**
	 * @note not thread safe
	 */
	void CheckTime(time_t due_time){
		const auto now = time(nullptr);
		lInf("NOW: " << ctime(&now) << "\tDUE: " << ctime(&due_time))
		if(now >= due_time){
			lInf("Out-dated!!")
			exit(1);
		}
	}

}

#endif //BASE_DEV_TIMELIMIT_H
