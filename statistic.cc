/*
 *  Created on: Apr 5, 2016
 *      Author: xing
 */

#include "base/statistic.h"
#include "base/log.h"

namespace base{

	std::string Statistic::ToString() const noexcept {
		std::string s;
		s.reserve(64);

		s.append(succ_tag);
		s.append("/");
		s.append(total_tag);
		s.append(": ");
		s.append(std::to_string(succ_count));
		s.append("/");
		s.append(std::to_string(total_count));

#ifdef ENABLE_TIME_STAT
		if(succ_count){
			s.append(". ");
			s.append("Total ");

			auto tm = SSTimer::of_microseconds(dur);
			s.append(std::to_string(tm));
			s.append("micsec, ");
			s.append(std::to_string(tm / succ_count));
			s.append("mpp");
		}
#endif

		return s;
	};

}
