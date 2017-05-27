/**
 *  Created on: Mar 24, 2016
 *      Author: xing
 */

#ifndef COMMON_INC_FILE_WRAPPER_H_
#define COMMON_INC_FILE_WRAPPER_H_

#include <cstdio>
#include <cstdarg>

#include "base/common.h"
#include "base/macro_utils.h"

namespace base{

	class FileWrapper final{
	public:
		FileWrapper(FileWrapper &&other): fl{other.fl}{
			other.fl = NULL;
		}

		void operator=(FileWrapper &&other){
			if(fl){
				fclose(fl);
			}
			fl = other.fl;
			other.fl = NULL;
		}

		FileWrapper(const char *name, const char *mode):
				fl(fopen(name, mode)) {
			if(!fl)
				throw Excep(Ret::E_FILE_OP, name);
		};

		~FileWrapper() noexcept {fclose(fl);};

		inline size_t Read(void *__restrict ptr,
		                   size_t size, size_t n) noexcept {
			return fread(ptr, size, n, fl);
		}

		inline size_t Read(void *__restrict ptr, size_t size) noexcept {
			return fread(ptr, size, 1, fl);
		}

		inline size_t Write(const void *__restrict ptr,
		                    size_t size, size_t n) noexcept {
			return fwrite(ptr, size, n, fl);
		}

		inline size_t Write(const void *__restrict ptr, size_t size) noexcept {
			return fwrite(ptr, size, 1, fl);
		}

		inline int Seek(long int off, int whence) noexcept {
			return fseek(fl, off, whence);
		}

		inline long int Tell() noexcept {
			return ftell(fl);
		}

		inline int Flush() noexcept {
			return fflush(fl);
		}

		inline int GetC() noexcept {
			return fgetc(fl);
		}

		inline long int Size() noexcept {
			Seek(0, SEEK_END);
			long int len{Tell()};
			Seek(0, SEEK_SET);
			return len;
		}

		inline int ReadAt(int offset) noexcept {
			Seek(offset, SEEK_SET);
			return GetC();
		}

		inline int Printf(const char *__restrict fmt, ...) noexcept {
			va_list vl;
			va_start(vl, fmt);
			int a = vfprintf(fl, fmt, vl);
			va_end(vl);
			return a;
		}

		/**
		 * evaluate each char until f returns false
		 */
		inline void EachChar(std::function<void(int)> f) noexcept {
			Seek(0, SEEK_SET);
			int c;
			while(EOF != (c = GetC())){
				f(c);
			}
		}

	private:
		BASE_DISALLOW_COPY_AND_ASSIGN(FileWrapper);

		FILE *fl;
	};

}

#endif /* COMMON_INC_FILE_WRAPPER_H_ */
