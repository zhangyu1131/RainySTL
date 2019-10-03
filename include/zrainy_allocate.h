/*
 * 空间配置器，包含了zrainy_alloc.h和zrainy_construct.h两个头文件。
 * 这是对外的配置器头文件，容器直接使用的配置器。
 * by Zrainy
 */

#ifndef _ZRAINY_ALLOCATOR_H_
#define _ZRAINY_ALLOCATOR_H_

#include "zrainy_alloc.h"
#include "zrainy_construct.h"

#include <new>
//#include <cassert>
#include <cstddef>

namespace ZRainySTL{
	//空间配置器
	template<class T>
	class allocator{
		public:
			typedef T			value_type;
			typedef T*			pointer;
			typedef const T*	const_pointer;
			typedef T&			reference;
			typedef const T&	const_reference;
			typedef size_t		size_type;
			typedef ptrdiff_t	difference_type;
		public:
			//内存的分配与释放
			static T* allocate();
			static T* allocate(size_t n);
			static void deallocate(T* ptr);
			static void deallocate(T* pyt, size_t n);

			//对象的构造和析构
			//按照STL规定，配置器必须拥有名为construct()和destroy()的两个成员函数
			static void construct(T* ptr);
			static void construct(T* ptr, const T& value);
			static void destroy(T* ptr);
			static void destroy(T* first, T* last);
	};//end class allocator

	//分配和释放的方法只是简单地转发到alloc中
	template <class T>
	T* allocator<T>::allocate(){
		return static_cast<T*>(alloc::allocate(sizeof(T)));
	}
	template <class T>
	T* allocator<T>::allocate(size_t n){
		//注意0字节
		if(n == 0)
			return 0;
		return static_cast<T*>(alloc::allocate(n * (sizeof(T))));
	}
	template <class T>
	void allocator<T>::deallocate(T* ptr){
		alloc::deallocate(static_cast<void*>(ptr), sizeof(T));
	}
	template <class T>
	void allocator<T>::deallocate(T* ptr, size_t n){
		if(n == 0)
			return;
		alloc::deallocate(static_cast<void*>(ptr), n * sizeof(T));
	}

	template <class T>
	void allocator<T>::construct(T* ptr){
		new(ptr)T();
	}
	template <class T>
	void allocator<T>::construct(T* ptr, const T& value){
		new(ptr)T(value);
	}
	template <class T>
	void allocator<T>::destroy(T* ptr){
		ptr->~T();
	}
	template <class T>
	void allocator<T>::destroy(T* first, T* last){
		for(; first != last; ++first){
			first->~T();
		}
	}

};//end namespace ZRainySTL

#endif //end _ZRAINY_ALLOCATOR_H_
