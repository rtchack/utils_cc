/**
 * Created on: 6/4/16
 *     Author: xing
 */

#include "base/buf_pool.h"

namespace base{

	Ret Buffer::Write(const uint8_t *src, size_t length) noexcept {
		if(!src) return Ret::E_ARG_NULL;
		if(length > size) return Ret::E_ARG;

		memcpy(data, src, length);
		len = length;

		return Ret::OK;
	}

	Ret Buffer::Read(uint8_t *dst, size_t &length) const noexcept{
		if(!dst) return Ret::E_ARG_NULL;
		if(!len) return Ret::NO;

		length = (length > len) ? len : length;

		memcpy(dst, data, length);

		return Ret::OK;
	}

	BufferPool::BufferPool(size_t buf_count, size_t buf_size, const std::string &name):
			Module{name}, buf_count{buf_count}, buf_size{buf_size}{
		BASE_RISE_IF((buf_count <= 0 || buf_size < sizeof(nodeptr)), Ret::E_ARG)

		size_t sz = BASE_ROUND(buf_size + sizeof(Buffer), sizeof(nodeptr));
		mem = new uint8_t[buf_count * sz];
		BASE_RISE_UNLESS(mem, Ret::E_MEM);

		free_mem = (nodeptr)mem;

		sz /= sizeof(nodeptr);
		nodeptr tmp = free_mem;
		while(--buf_count){
			tmp->next = tmp + sz;
			tmp = tmp->next;
		}

		tmp->next = nullptr;
	}

	Buffer *BufferPool::Alloc() noexcept{
		stat.Total();

		if(!free_mem){
			return nullptr;
		}

		stat.Succ();

		auto b = (Buffer *)free_mem;
		free_mem = free_mem->next;
		b->Init(buf_size, 0);

		return b;
	};

	CBufferPool::CBufferPool(size_t buf_count, size_t buf_size, const std::string &name):
			Module{name}, buf_count{buf_count}, buf_size{buf_size}{
		BASE_RISE_IF((buf_count <= 0 || buf_size < sizeof(nodeptr)), Ret::E_ARG)

		size_t sz = BASE_ROUND(buf_size + sizeof(Buffer), sizeof(nodeptr));
		mem = new uint8_t[buf_count * sz];
		BASE_RISE_UNLESS(mem, Ret::E_MEM);

		free_mem = (nodeptr)mem;

		sz /= sizeof(nodeptr);
		nodeptr tmp = free_mem;
		while(--buf_count){
			tmp->next = tmp + sz;
			tmp = tmp->next;
		}

		tmp->next = nullptr;
	}

	Buffer *CBufferPool::Alloc() noexcept{
		stat.Total();

		if(!free_mem){
			return nullptr;
		}

		{
			std::lock_guard<std::mutex> bar{mut};
			auto b = (Buffer *)free_mem;
			free_mem = free_mem->next;
			b->Init(buf_size, 0);
			stat.Succ();
			return b;
		}
	};

}

