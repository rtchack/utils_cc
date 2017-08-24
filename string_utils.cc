/**
 * Created by xing on 4/7/16.
 */

#include "base/string_utils.h"

namespace base{
	namespace str_util{

		void UperCase(char *str) noexcept {
			while(*str){
				if(*str <= 122 && *str >= 97){
					*str -= 32;
				}else if('-' == *str){
					*str = '_';
				}

				++str;
			}
		}

		template<typename T>
		std::string ToHexStr(T c) noexcept {
			char str[4];
			snprintf(str, 4, "%02x", c);
			return str;
		}

		std::string CharsToHexStr(const std::vector<int8_t> &chars) noexcept {
			std::string s;
			s.reserve((chars.size() + 1) << 2);
			for(auto c : chars){
				s.append(ToHexStr(c));
			}
			return s;
		}

		std::string CharsToHexStr(const std::vector<int8_t> &chars,
		                          const std::string &delimiter) noexcept {
			return CharsToHexStr(chars, delimiter, 1);
		}

		std::string CharsToHexStr(const std::vector<int8_t> &chars,
		                          const std::string &delimiter,
		                          uint16_t pace) noexcept {
			std::string s;
			s.reserve((chars.size() + 1) << 2);

			auto sz = chars.size() - 1;
			uint16_t step{0};
			for(size_t i{0}; i < sz; ++i){
				s.append(ToHexStr(chars[i]));
				if(++step >= pace){
					s.append(delimiter);
					step = 0;
				}
			}
			s.append(ToHexStr(chars[sz]));
			return s;
		}

		std::string UcharsToHexStr(const std::vector<uint8_t> &chars) noexcept {
			std::string s;
			s.reserve((chars.size() + 1) << 2);
			for(auto c : chars){
				s.append(ToHexStr(c));
			}
			return s;
		}

		std::string UcharsToHexStr(const std::vector<uint8_t> &chars,
		                           const std::string &delimiter) noexcept {
			return UcharsToHexStr(chars, delimiter, 1);
		}

		std::string UcharsToHexStr(const std::vector<uint8_t> &chars,
		                           const std::string &delimiter,
		                           uint16_t pace) noexcept {
			std::string s;
			s.reserve((chars.size() + 1) << 2);

			auto sz = chars.size() - 1;
			uint16_t step{0};
			for(size_t i{0}; i < sz; ++i){
				s.append(ToHexStr(chars[i]));
				if(++step >= pace){
					s.append(delimiter);
					step = 0;
				}
			}
			s.append(ToHexStr(chars[sz]));
			return s;
		}

	}
}
