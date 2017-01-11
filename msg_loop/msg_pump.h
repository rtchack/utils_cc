/**
 * Created on: 6/5/16
 *     Author: xing
 */

#ifndef GTEST_WITH_CLION_MSG_PUMP_H
#define GTEST_WITH_CLION_MSG_PUMP_H

#include "base/timer.h"

namespace base{

	class MessagePump{
	public:
		class Delegate{
		public:
			virtual ~Delegate(){}

			virtual bool Dowork() noexcept = 0;

			virtual bool DoDelayedWork(const Timer::Duration &delay) noexcept = 0;

			virtual bool DoIdleWork() noexcept = 0;
		};

		MessagePump() = default;
		virtual ~MessagePump() {}

		virtual void Run(Delegate &delegate) = 0;

		virtual void Quit() = 0;

		virtual void ScheduleWork() = 0;

		virtual void ScheduleDelayedWork(const Timer::Duration &delay) = 0;

		// TODO implement TimerSlack
		//virtual void SetTimerSlack(TimerSlack slack) noexcept;
	};

}

#endif //GTEST_WITH_CLION_MSG_PUMP_H
