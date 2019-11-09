/*
 * STL中的一些高级算法实现
 *
 * by ZRainy
 */

#ifndef ZRAINY_ALGORITHM_H
#define ZRAINY_ALGORITHM_H

namespace ZRainySTL{
	template <class ForwardIterator, class T>
	void
	fill(ForwardIterator first, ForwardIterator last, const T& v){
		for(; first != last; first++){
			*first = v;
		}
	}

	//如果n超越了容器现有大小，会有问题
	template <class ForwardIterator, class Size, class T>
	ForwardIterator
	fill_n(ForwardIterator first, Size n, const T& v){
		for(; n > 0; n--,first++){
			*first = v;
		}
		return first;
	}


}// end namespace ZRainySTL
#endif //end ZRAINY_ALGORITHM_H
