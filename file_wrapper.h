/**
 *  Created on: Mar 24, 2016
 *      Author: xing
 */

#ifndef COMMON_INC_FILE_WRAPPER_H_
#define COMMON_INC_FILE_WRAPPER_H_

#include <stdio.h>
#include <stdarg.h>

#include "base/common.h"
#include "base/macro_utils.h"

namespace base{

	class FileWrapper final{
	public:
		FileWrapper(FileWrapper &&other): fl{other.fl}{
			BASE_RAISE_UNLESS(fl)
			other.fl = NULL;
		}

		FileWrapper(FILE *file): fl{file} {
			BASE_RAISE_UNLESS(fl)
		}

		FileWrapper(const char *name, const char *mode):
				fl(fopen(name, mode)) {
			BASE_RAISE_VERBOSE_UNLESS(fl, name << ":" << mode)
		}

		FileWrapper(const std::string &name, const char *mode):
				FileWrapper(name.c_str(), mode) {}

		FileWrapper(const std::string &name, const std::string &mode):
				FileWrapper(name.c_str(), mode.c_str()) {}

		~FileWrapper() noexcept {
			fclose(fl);
		};

		void operator=(FileWrapper &&other){
			if(fl){
				fclose(fl);
			}
			fl = other.fl;
			other.fl = NULL;
		}

		inline std::string Read(){
			const auto size = (size_t)Size();
			auto mem = new char[size + 1];
			mem[size] = '\0';
			unless(size == Read(mem, size)){
				delete[] mem;
				return std::string{};
			}
			std::string str{mem};
			delete[] mem;
			return str;
		}

		inline size_t Read(void *__restrict ptr,
		                   size_t size, size_t n) noexcept {
			return fread(ptr, size, n, fl);
		}

		inline size_t Read(void *__restrict ptr, size_t size) noexcept {
			return fread(ptr, 1, size, fl);
		}

		inline size_t Write(const std::string &str) noexcept {
			return Write(str.c_str(), str.size());
		}

		inline size_t Write(const void *__restrict ptr,
		                    size_t size, size_t n) noexcept {
			return fwrite(ptr, size, n, fl);
		}

		inline size_t Write(const void *__restrict ptr, size_t size) noexcept {
			return fwrite(ptr, 1, size, fl);
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
