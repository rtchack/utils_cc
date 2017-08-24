/**
 * Created by xing on 4/7/16.
 */

#ifndef CPPBASE_STRING_UTILS_H_H
#define CPPBASE_STRING_UTILS_H_H

#include <cstring>
#include <string>
#include <sstream>
#include <vector>

#include "base/macro_utils.h"

namespace base{

	class StringAble{
	public:
		virtual std::string ToString() const = 0;
	};

	inline std::string to_string(const StringAble &sa){
		return sa.ToString();
	}

	namespace str_util{

		void UperCase(char *str) noexcept;

		template<typename T>
		std::string ToHexStr(T c) noexcept;

		std::string CharsToHexStr(const std::vector<int8_t> &uchars) noexcept;

		std::string CharsToHexStr(const std::vector<int8_t> &uchars,
		                          const std::string &delimiter) noexcept;

		std::string CharsToHexStr(const std::vector<int8_t> &uchars,
		                          const std::string &delimiter,
		                          uint16_t pace) noexcept;

		std::string UcharsToHexStr(const std::vector<uint8_t> &uchars) noexcept;

		std::string UcharsToHexStr(const std::vector<uint8_t> &uchars,
		                           std::string delimiter) noexcept;

		std::string UcharsToHexStr(const std::vector<uint8_t> &uchars,
		                           const std::string &delimiter,
		                           uint16_t pace) noexcept;

	}
}
#endif //CPPBASE_STRING_UTILS_H_H
