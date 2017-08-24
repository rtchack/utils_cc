/**
 * Created by xing on 4/12/16.
 */

#ifndef NATIVE264DEC_LOOPER_H
#define NATIVE264DEC_LOOPER_H

#include <queue>
#include <list>
#include <cstdint>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "base/macro_utils.h"
#include "base/module.h"
#include "base/common.h"
#include "base/thread_wrapper.h"
#include "base/timer.h"

namespace base{

	class Looper: public Module{
	public:

		BASE_DISALLOW_COPY_AND_ASSIGN(Looper)

		/**
		 * @return ture to quit the loop, false to continue
		 */
		typedef std::function<void()> Task;
		typedef std::list<Task> MsgQueue;


		Looper(): Looper{""} {}

		explicit Looper(const std::string &name):
				Module{name}, worker{Get_name()} {}

		~Looper() override {
			Deactivate();
			PutStat();
		}

		/**
		 * Post a method
		 */
		void Post(const Task &tsk) noexcept {
			Post(tsk, false);
		}

		/**
		 * Post a method
		 */
		void Post(const Task &task, bool flush) noexcept;

		// TODO: implement
		//void Post(const Task &task, SSTimer::Duration &delay) noexcept;

		/**
		 * start loop
		 */
		void Activate() noexcept;

		/**
		 * stop loop
		 */
		void Deactivate() noexcept;

		bool IsActive() const noexcept {return running;}

	protected:
		/**
		 * before loop gets started
		 */
		virtual void PreActivate() {cDbg("")}

		/**
		 * after loop gets started
		 */
		virtual void PostActivate() {cDbg("")}

		/**
		 * before the quiting of loop
		 */
		virtual void PreDeactivate() {cDbg("")}

		/**
		 * after loop gets stopped
		 */
		virtual void PostDeactivate() {cDbg("")}

		std::string ToString() const noexcept override {
			BASE_STR_S(16)
			s += Get_name();
			s += ": processed ";
			s += std::to_string(stat.processed);
			return s;
		}

	private:

		void Entry() noexcept;


		struct{
			uint64_t processed{};
		} stat{};

		MsgQueue msg_queue{};
		ThreadWrapper worker;
		std::mutex run_mut{};
		std::mutex op_mut{};
		volatile bool running{false};
		volatile bool looping{false};
		std::condition_variable cv{};
	};

}

#endif //NATIVE264DEC_LOOPER_H
