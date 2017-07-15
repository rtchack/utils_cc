/**
 *  Created on: Mar 5, 2016
 *      Author: xing
 */

#ifndef BASE_COMMON_INC_MODULE_H_
#define BASE_COMMON_INC_MODULE_H_

#include <string>
#include <memory>

#include "base/common.h"
#include "base/statistic.h"
#include "base/log.h"
#include "base/string_utils.h"

namespace base{

	class Module: public StringAble{
	public:
		Module() = default;

		Module(const std::string &name): name{name} {}

		virtual ~Module() {}

		virtual void Inspect() const {cInf(ToString())}

		virtual void PutStat() const {cInf(ToString())}

		virtual std::string ToString() const override {
			return std::string("\n\t") + name + " " + stat.ToString();
		}

	protected:
		Statistic stat{};

	private:
		BASE_ACCESSOR(std::string, name){" "};
	};

}

#endif /* BASE_COMMON_INC_MODULE_H_ */
