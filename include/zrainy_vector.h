#ifndef _ZRAINY_VECTOR_H_
#define _ZRAINY_VECTOR_H_


//#include "zrainy_type_traits.h"
#include "zrainy_allocate.h"
#include "zrainy_construct.h"
#include "zrainy_iterator.h"
#include "zrainy_uninitialized.h"


#include <algorithm>

namespace ZRainySTL{
	
	template<class T, class Alloc = allocator<T>>
	//为了与stl中的容器区分，ZRainySTL中的所有容器添加前缀z
	class zvector{
	public:
		//zvector的嵌套型别定义
		typedef T					value_type;
		typedef value_type*			pointer;
		typedef value_type&			reference;
		typedef const value_type&	const_reference;
		typedef value_type*			iterator;
		typedef const value_type*	const_iterator;
		typedef size_t				size_type;
		typedef ptrdiff_t			difference_type;
		
	private:
		//空间配置器，这里我就直接使用allocator了
		typedef Alloc data_allocator;
		iterator start;//表示目前使用空间的头
		iterator finish;//表示目前使用空间的尾
		iterator end_of_storage;//表示目前可用空间的尾
	
	public:
		//构造和析构函数,TODO
		zvector():start(nullptr), finish(nullptr), end_of_storage(nullptr){}
		zvector(const size_type n, const T& value){
			_fill_initialize(n, value);
		}
		~zvector(){
			destroy(start, finish);
			_deallocate();
		}

		//基本操作
		iterator begin(){return start;}
		iterator end(){return finish;}
		size_type size(){return (size_type)(finish - start);}
		bool empty() const {return begin() == end();}

		void push_back(const T& v){
			//判断是否有空余空间
			if (finish != end_of_storage) {
				construct(finish, v);
				++finish;
			} else {
				//没有空余空间
				_insert_aux(end(), v);
			}
		}
		void pop_back(){
			--finish;
			destroy(finish);
		}
	
	private:
		void _deallocate(){
			if(start){
				data_allocator::deallocate(start, end_of_storage - start);
			}
		}

		void _fill_initialize(const size_type n, const T& value){
			start = _alloc_and_fill(n, value);
			finish = start + n;
			end_of_storage = finish;
		}

		iterator _alloc_and_fill(size_type n, const T& value){
			iterator result = data_allocator::allocate(n);
			uninitialized_fill_n(result, n, value);
			return result;
		}

		void _insert_aux(iterator position, const T& value){
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
			    const size_type new_size = (old_size != 0) ? 2 * old_size : 1;

				iterator new_start = data_allocator::allocate(new_size);
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
		
	};// end class zvector
}// end namespace ZRainySTL

#endif //end ifnde

