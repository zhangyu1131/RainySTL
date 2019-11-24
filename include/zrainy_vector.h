#ifndef _ZRAINY_VECTOR_H_
#define _ZRAINY_VECTOR_H_


//#include "zrainy_type_traits.h"
#include "zrainy_allocate.h"
#include "zrainy_construct.h"
#include "zrainy_iterator.h"
#include "zrainy_uninitialized.h"


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
		zvector(const size_type n, const T& value);
		~zvector(){
			destroy(start, finish);
			_deallocate();
		}

		//基本操作
		iterator begin(){return start;}
		iterator end(){return finish;}
		size_type size(){return (size_type)(finish - start);}
		bool empty() const {return begin() == end();}

		void push_back(const T& v);
		void pop_back();
	
	private:
		void _deallocate(){
			if(start){
				data_allocator::deallocate(start, end_of_storage - start);
			}
		}

		void _fill_initialize(const size_type n, const T& value);

		iterator _alloc_and_fill(size_type n, const T& value);

		void _insert_aux(iterator position, const T& value);
		
	};// end class zvector
}// end namespace ZRainySTL

#endif //end ifnde

