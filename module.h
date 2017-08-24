/**
 *  Created on: Mar 5, 2016
 *      Author: xing
 */

#ifndef BASE_COMMON_INC_MODULE_H_
#define BASE_COMMON_INC_MODULE_H_

#include <string>
#include <memory>

#include "base/common.h"
#include "base/log.h"
#include "base/string_utils.h"

namespace base{

	class Module: public StringAble{
	public:
		Module() = default;

		explicit Module(const std::string &name): name{name} {}

		virtual ~Module() = default;

		virtual void Inspect() const {cInf(ToString())}

		virtual void PutStat() const {cInf(ToString())}

		virtual std::string ToString() const {
			return name + ": ToString has not been implemented.";
		}

	private:
		BASE_ACCESSOR(std::string, name){""};
	};

}

#endif /* BASE_COMMON_INC_MODULE_H_ */
