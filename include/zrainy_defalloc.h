/*
 * 原始的HP defalut allocator，提供它只是为了回溯兼容。
 * 我这里照着写一遍这个文件只是为了体验和了解一下。
 * 本项目暂时没有用到
 *
 *
*/


#ifndef ZRAINY_DEFALLOC_H
#define ZRAINY_DEFALLOC_H

#include <new.h>
#include <cstddef.h>
#include <stdlib.h>
#include <iostream.h>
#include <algobase.h>

template <class T>
//ptrdiff_t是一种有符号整数类型，用于存放同一数组中两个指针之间的差距，返回结果以数据元素长度为单位，位于头文件stddef.h中。
inline T* allocator(ptrdiff_t size, T*){
	//set_new_handler，当默认内存分配失败时调用指定函数进行处理，传入空指针或0表示直接抛出bad_alloc异常，在new.h里。
	set_new_handler(0);
	T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
	if(tmp == 0){
		cerr << "out of memory" << endl;
		exit(1);
	}
	return tmp;
}

template <class T>
inline void deallocate(T* buffer){
	::operator delete(buffer);
}


template <class T>
class allocator{
public:
	//定义各种type
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	pointer allocator(size_type n){
		return ::allocator((difference_type)n, (pointer)0);
	}

	void deallocate(pointer p){
		::deallocate(p);
	}

	pointer address(reference x) { return (pointer)&x; }
	const_pointer const_address(const_reference x){
		return (const_reference)&x;
	}

	size_type init_page_size(){
		return max(size_type(1), size_type(4096/sizeof(T)));
	}
	//返回可成功配置的最大值
	size_type max_size() const {
		return max(size_type(1), size_type(UNIX_MAX/sizeof(T)));
	}
};

//特化
class allocator<void>{
public:
	typedef void* pointer;
};

#endif
