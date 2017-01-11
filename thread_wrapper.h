/**
 * Created on:  4/25/16
 *     Author:  xing
 */

#ifndef CPPBASE_THREAD_WRAPPER_H_H
#define CPPBASE_THREAD_WRAPPER_H_H

#include <thread>

#include "base/common.h"
#include "base/macro_utils.h"

namespace base{

	class ThreadWrapper final{
	public:
		ThreadWrapper() = default;

		ThreadWrapper(std::thread &&thread):
				thread{std::forward<std::thread &&>(thread)} {};

		ThreadWrapper(ThreadWrapper &&oth) noexcept {
			thread = std::move(oth.thread);};

		~ThreadWrapper() {Reset();};

		inline void Attach(std::thread &&th) noexcept {
			Reset();
			thread = std::forward<std::thread &&>(th);
		}

		inline void operator=(std::thread &&th) noexcept {
			Attach(std::forward<std::thread &&>(th));}

		inline void Reset() noexcept {
			if(thread.joinable())
				thread.join();
		}

	private:
		BASE_DISALLOW_COPY_AND_ASSIGN(ThreadWrapper)

		std::thread thread{};
	};

}

#endif //CPPBASE_THREAD_WRAPPER_H_H
