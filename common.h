/**
 *  Created on: Mar 5, 2016
 *      Author: xing
 */

#ifndef COMMON_INC_COMMON_MY_H_
#define COMMON_INC_COMMON_MY_H_

#include <string>
#include <stdexcept>
#include <sstream>

#include "base/macro_utils.h"

namespace base{

	/**
	 * return value
	 */
	enum class Ret{
#define BASE_RET_ERROR 0x8000;

		OK = 0, // OK
		NO, // no

		E_ARG, // parameter error
		E_ARG_NULL, // nil parameter detected

		E_MEM, // memory allocation error
		E_INIT, // not properly initialized yet

		E_FILE_OP, // file operation error
		E_FILE_CON, // file content error

		E_UNSUP, // unsupported type
		E_ALG, // failed on algorithm
		E_BUF_OVERFLOW, // buffer overflow

		E_IO_PENDING, // operation could not be completed synchronously

		E_SOCKET_NOT_CONNECTED, // socket is not currently connected

		E_GENERAL // general error
	};

	std::string to_string(const Ret value) noexcept;

	constexpr bool Failed(const Ret ret) noexcept{
		return ret > Ret::NO;
	}

	constexpr bool Succeed(const Ret ret) noexcept{
		return !Failed(ret);
	}


	/**
	 * exception
	 */
	class Excep: public std::exception{
	public:
		Excep() = delete;

		Excep(Ret ret): msg{to_string(ret)} {}

		Excep(const std::string &message): msg{message} {}

		Excep(Ret ret, const std::string &message): Excep(ret) {
				msg += ": ";
				msg += message;
		};

		const char *what() const _GLIBCXX_USE_NOEXCEPT override {
			return msg.c_str();
		};

	private:
		std::string msg;
	};


#define BASE_RISE_LOCATED(ret, msg) \
	std::stringstream base_excep_located_s;\
	base_excep_located_s << "" << __FILE__ << ", " << __LINE__\
	<< " ~ " << base::to_string(base::ret) << ": " << msg;\
	throw base::Excep(base_excep_located_s.str());

	// if v is true, throw ret
#define BASE_RISE_ON_TRUE(v, ret) \
	if(v) {BASE_RISE_LOCATED(ret, #v " should not be ture")}

	// if v is false, throw ret
#define BASE_RISE_ON_FALSE(v, ret) \
	BASE_RISE_ON_TRUE(!(v), ret)


	/**
	 * ulimit, enable core dump
	 */
	void EnableCoreDump() noexcept;

}

#endif /* COMMON_INC_COMMON_MY_H_ */
