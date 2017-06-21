/**
 * Created on: 6/20/17
 *     Author: xing
 */

#ifndef BASE_DEV_ALGORITHM_H
#define BASE_DEV_ALGORITHM_H

#include <array>
#include "base/log.h"
#include "base/helper.h"
#include "base/macro_utils.h"


namespace base{

	template<typename T>
	void QuickSort(T *v, int32_t left, int32_t right){
		if(left >= right){
			return;
		}

		auto last = left;
		for(auto i = left + 1; i <= right; ++i){
			if(v[i] < v[left] && i != ++last){
				std::swap(v[i], v[last]);
			}
		}
		unless(left == last){
			std::swap(v[left], v[last]);
		}
		QuickSort(v, left, last - 1);
		QuickSort(v, last + 1, right);
	}

	template<typename T, size_t N>
	void QuickSort(T (&v)[N]){
		QuickSort(v, 0, (int32_t)(N-1));
	};

}

#endif //BASE_DEV_ALGORITHM_H
