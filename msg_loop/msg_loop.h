/**
 * Created on: 6/5/16
 *     Author: xing
 */

#ifndef GTEST_WITH_CLION_MSG_LOOP_H
#define GTEST_WITH_CLION_MSG_LOOP_H

#include <base/module.h>
#include "base/msg_loop/msg_pump.h"

namespace base{

	class MessageLoop: public MessagePump::Delegate, public Module{
	public:
		enum class Type{DEFAULT, IO};

		explicit MessageLoop(Type type = Type::DEFAULT);
		explicit MessageLoop(std::unique_ptr<MessagePump> &&pump);
		~MessageLoop() override;

		// TODO: not safe enough
		static MessageLoop *current();

		static std::unique_ptr<MessagePump> CreateMessagePumpForType(Type type);

		// notified before destroy
		class DestructionObserver{
		public:
			virtual void WillDestroyCurrentMessageLoop() = 0;
		protected:
			virtual ~DestructionObserver();
		};

		void AddDestructionObserver(DestructionObserver *observer);
		void RemoveDestructionObserver(DestructionObserver *observer);

		// notified before the first task is processed
		class NestingObserver{
		public:
			virtual void OnBeginNestedMessageLoop() = 0;
		protected:
			virtual ~NestingObserver();
		};

		void AddNestingObserver(NestingObserver *observer);
		void RemoveNestingObserver(NestingObserver *observer);

		// Deprecated: prefer task runner() and the TaskRunner interface
		void PostTask(const std::function<void()> &task);
		// Deprecated: prefer task runner() and the TaskRunner interface
		void PostTask(const std::function<void()> &task, const Timer::Duration &delay);

		/**
		 * TODO: implement DeleteSoon and ReleaseSoon if needed
		template <class T>
		void DeleteSoon(const tracked_objects::Location& from_here, const T* object) {
			base::subtle::DeleteHelperInternal<T, void>::DeleteViaSequencedTaskRunner(
					this, from_here, object);
		}
		template <class T>
		void ReleaseSoon(const tracked_objects::Location& from_here,
		                 const T* object) {
			base::subtle::ReleaseHelperInternal<T, void>::ReleaseViaSequencedTaskRunner(
					this, from_here, object);
		}
		 */

		// Deprecated: use RunLoop instead
		void Run();
		// Deprecated: use Runloop instead
		void RunUntilIdle();
		// Deprecated: use Runloop instead
		void QuitWhenIdle();
		// Deprecated: use RunLoop instead
		void QuitNow();
		// Deprecated: use RunLoop instead
		static std::function<void()> QuitWhenIdleClosure();

		virtual bool IsType(Type type) const noexcept;

	private:
		std::unique_ptr<MessagePump> pump;

		BASE_READER(Type, type);
	};

}

#endif //GTEST_WITH_CLION_MSG_LOOP_H
