#ifndef _ZRAINY_VECTOR_IMP_H
#define _ZRAINY_VECTOR_IMP_H

#include <algorithm>//为了使用copy_backward暂时加这个头文件，后面自行实现，TODO

namespace ZRainySTL{

	// 构造相关
	template<class T, class Alloc>
	zvector<T, Alloc>::zvector(const size_type n, const T& value){
		_fill_initialize(n, value);
	}

	template<class T, class Alloc>
	void zvector<T, Alloc>::_fill_initialize(const size_type n, const T& value){
		start = _alloc_and_fill(n, value);
		finish = start + n;
		end_of_storage = finish;
	}

	template<class T, class Alloc>
	typename zvector<T, Alloc>::iterator zvector<T, Alloc>::_alloc_and_fill(size_type n, const T& value){
		iterator result = data_allocator::allocator(n);
		uninitialized_fill_n(result, n, value);
		return result;
	}


	// vector相关操作
	template<class T, class Alloc>
	void zvector<T, Alloc>::push_back(const T& v){
		//判断是否有空余空间
		if (finish != end_of_storage) {
			construct(finish, v);
			++finish;
		} else {
			//没有空余空间
			_insert_aux(end(), v);
		}
	}

	template<class T, class Alloc>
	void zvector<T, Alloc>::_insert_aux(iterator position, const T& value){
		if (finish != end_of_storage) {
			//有备用空间，则在备用空间的第一个地方构造一个元素，用vector最后一个元素值做初值
			construct(finish, *(finish - 1));
			//调整指针
			++finish;
			//初始化一个值的副本，因为value是引用传递的
			T v_copy = value;
			//把vector插入位置position之后的元素往后移一位
			std::copy_backward(position, finish - 2, finish - 1);
			*position = v_copy;
		} else {
			//没有可用空间了，则需要新开辟空间
			//开辟新空间的长度默认是原长度的两倍
			const size_type old_size = size();
			const size_type new_size = old_size != 0 ? 2 * old_size : 1;

			iterator new_start = data_allocator::allocator(new_size);
			iterator new_finish = new_start;

			try{
				//将原来的vector的内容拷贝到新vector
				new_finish = uninitialized_copy(start, position, new_start);
				//为新的元素设定初值
				construct(new_finish, value);
				++new_finish;
				//将原vector中的后半段也copy到新vector中
				new_finish = uninitialized_copy(position, finish, new_finish);
			} catch (...) {
				//异常，回滚状态
				destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, new_size);
				throw;
			}


			//析构并释放原vector
			destroy(begin(), end());
			_deallocate();

			//调整迭代器
			start = new_start;
			finish = new_finish;
			end_of_storage = start + new_size;
		}
	}

	template<class T, class Alloc>
	void zvector<T, Alloc>::pop_back(){
		--finish;
		destroy(finish);
	}

} // end namespace ZRainySTL
#endif //end ifndef
