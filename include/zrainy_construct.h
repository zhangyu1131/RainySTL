/*
 * 基本构造和析构函数
 * 析构函数分两种，一种是接收一个简单指针的，另一种是接收一对迭代器，
 * 对[first,last)范围内的对象都进行析构
 * 
 * by Zrainy
 */

#ifndef ZRAINY_CONSTRUCT_H_
#define ZRAINY_CONSTRUCT_H_

#include "zrainy_iterator.h"
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


	//接收一对迭代器的析构函数
	template<class ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last){
		__destroy(first, last, value_type(first));
		//for(; first != last; ++first){
			//destroy(& *first);
		//}
	}
	//
	template <class ForwardIterator, class T>
	inline void __destroy(ForwardIterator first, ForwardIterator last, T*){
	}
	//TODO 尚未针对traits型别进行完善，后续引入traits后完善。

}// end namespace ZRainySTL

#endif
