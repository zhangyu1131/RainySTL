/*
 * 基本构造和析构函数
 * 析构函数分两种，一种是接收一个简单指针的，另一种是接收一对迭代器，
 * 对[first,last)范围内的对象都进行析构
 * 
 * by Zrainy
 */

#ifndef ZRAINY_CONSTRUCT_H_
#define ZRAINY_CONSTRUCT_H_

//#include "zrainy_iterator.h"
#include "zrainy_type_traits.h"
#include <new> //要使用placement new，必须包含该头文件


namespace ZRainySTL{
	//基本构造函数
	template<class T1, class T2>
	inline void construct(T1 *ptr, const T2& value){
		new(ptr) T1(value);
	}

	//基本析构函数
	template<class T>
	inline void destroy(T *ptr){
		ptr->~T();
	}


	//接收一对迭代器的析构函数，根据_type_traits进行不同调用
	template<class ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last){
		typedef typename _type_traits<ForwardIterator>::is_POD_type is_POD_type;
		__destroy(first, last, is_POD_type());
	}

	template <class ForwardIterator>
	inline void __destroy(ForwardIterator first, ForwardIterator last, _true_type){}

	template <class ForwardIterator>
	inline void __destroy(ForwardIterator first, ForwardIterator last, _false_type){
		for(; first != last; ++first){
			destroy(& *first);
		}
	}

}// end namespace ZRainySTL

#endif
