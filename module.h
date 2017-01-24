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

namespace base{

	class Module{
	public:
		Module() = default;

		Module(const std::string &name): name{name} {}

		virtual ~Module() {}

		virtual void Inspect() const noexcept {lInf(ToString())}

		virtual void PutStat() const noexcept {lInf(ToString())}

		virtual std::string ToString() const noexcept {
			return name + " " + stat.ToString();
		}

	protected:
		Statistic stat{};

	private:
		BASE_ACCESSOR(std::string, name){" "};
	};

}

#endif /* BASE_COMMON_INC_MODULE_H_ */
