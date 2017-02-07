/**
 *  Created on: Mar 8, 2016
 *      Author: xing
 */

#ifndef BASE_COMMON_INC_STATISTIC_H_
#define BASE_COMMON_INC_STATISTIC_H_

#include <string>
#include <memory>

#include "base/macro_utils.h"

//#define ENABLE_TIME_STAT

#ifdef ENABLE_TIME_STAT
#include "base/timer.h"
#endif


namespace base{

	class Statistic{
	public:
		Statistic() = default;

		inline void Succ() noexcept {++succ_count;}

		inline void Total() noexcept {++total_count;}

		std::string ToString() const noexcept;

		inline void Start() noexcept {
#ifdef ENABLE_TIME_STAT
			timer.Reset();
#endif
		}

		inline void Stop() noexcept {
#ifdef ENABLE_TIME_STAT
			tmp = timer.Lap();
#endif
		}

		inline void Count() noexcept {
#ifdef ENABLE_TIME_STAT
			dur += tmp;
#endif
		}

	private:

	BASE_ACCESSOR(std::string, succ_tag){"succ:"};
	BASE_ACCESSOR(std::string, total_tag){"total:"};

	BASE_READER(uint64_t, succ_count){0};
	BASE_READER(uint64_t, total_count){0};

#ifdef ENABLE_TIME_STAT
		Timer timer{};
		Timer::Duration tmp{};
		Timer::Duration dur{};
#endif
	};

}

#endif /* BASE_COMMON_INC_STATISTIC_H_ */
