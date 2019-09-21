/*
 * 迭代器实现文件
 *
 * by ZRainy
 */ 

#ifndef ZRAINY_ITERATOR_H
#define ZRAINY_ITERATOR_H

#include <stddef.h>

namespace ZRainySTL{
	//定义五种迭代器类型
	struct input_iterator_tag{};
	struct output_iterator_tag{};
	struct forward_iterator_tag : public input_iterator_tag{};
	struct bidirectional_iterator_tag : public forward_iterator_tag{};
	struct random_access_iterator_tag : public bidirectional_iterator_tag{};

	//为避免写代码时挂一漏万，自行开发的迭代器最好继承自下面这个iterator类
	template <class Category, class T, class Distance = ptrdiff_t,
			 class Pointer = T*, class Reference = T&>
	struct iterator{
		typedef Category	iterator_category;
		typedef T			value_type;
		typedef Distance	difference_type;
		typedef Pointer		pointer;
		typedef Reference	reference;
	};

	//定义五种iterator
	template <class T, class Distance>
	struct input_iterator{
		typedef input_iterator_tag	iterator_category;
		typedef T					value_type;
		typedef Distance			difference_type;
		typedef T*					pointer;
		typedef T&					reference;
	};

	struct output_iterator{
		typedef output_iterator_tag		iterator_category;
		typedef void					value_type;
		typedef void					difference_type;
		typedef void					pointer;
		typedef void					reference;
	};

	template <class T, class Distance>
	struct forward_iterator{
		typedef forward_iterator_tag	iterator_category;
		typedef T						value_type;
		typedef Distance				difference_type;
		typedef T*						pointer;
		typedef T&						reference;
	};

	template <class T, class Distance>
	struct bidirectional_iterator{
		typedef bidirectional_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef Distance					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};

	template <class T, class Distance>
	struct random_access_iterator{
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef Distance					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};
	
	//特性萃取traits
	template <class Iterator>
	struct iterator_traits{
		typedef typename Iterator::iterator_category	iterator_category;
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference			reference;
	};
	//如果只有上述结构体，则只能假设所有迭代器都是class type
	//但原生指针就不是，如果不是class type，就不能为它定义内嵌性别，所以需要对原生指针和const原生指针提供偏特化。
	template <class T>
	struct iterator_traits<T*>{
		typedef random_access_iterator_tag		iterator_category;
		typedef T								value_type;
		typedef ptrdiff_t						difference_type;
		typedef T*								pointer;
		typedef T&								reference;
	};
	template <class T>
	struct iterator_traits<const T*>{
		typedef random_access_iterator_tag		iterator_category;
		typedef T								value_type;
		typedef ptrdiff_t						difference_type;
		typedef const T*						pointer;
		typedef const T&						reference;
	};


	//本函数可以得出某个迭代器的类型
	template <class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
	iterator_category(const Iterator& It){
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	}

	//得出某个迭代器的value_type
	template <class Iterator>
	inline typename iterator_traits<Iterator>::value_type*
	value_type(const Iterator& It){
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}

	//得出某个迭代器的difference_type
	template <class Iterator>
	inline typename iterator_traits<Iterator>::difference_type*
	difference_type(const Iterator& It){
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}


	//__distance方法，根据不同iterator_type有不同实现，这也是iterator_type存在的意义
	template <class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
	__distance(InputIterator first, InputIterator last, input_iterator_tag){
		typename iterator_traits<InputIterator>::difference_type n = 0;
		while(first != last){
			++first;
			++n;
		}
		return n;
	}
	template <class RandomAccessIterator>
	inline typename iterator_traits<RandomAccessIterator>::difference_type
	__distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag){
		return last - first;
	}

	//对外提供的distance方法，内在实现是靠萃取器和函数重载的方式来调用上述两个不同的__distance方法。
	template <class Iterator>
	inline typename iterator_traits<Iterator>::difference_type
	distance(Iterator first, Iterator last){
		return __distance(first, last, iterator_category(first));
	}
	

	//以下是类似的advance方法，用来将迭代器向前移动n步
	template <class InputIterator, class Distance>
	inline void __advance(InputIterator& i, Distance n, input_iterator_tag){
		while(n--) ++i;
	}
	template <class BidirectionIterator, class Distance>
	inline void __advance(BidirectionIterator& i, Distance n, bidirectional_iterator_tag){
		if(n > 0){
			while(n--) ++i;
		}else{
			while(n++) --i;
		}
	}
	template <class RandomAccessIterator, class Distance>
	inline void __advance(RandomAccessIterator& i, Distance n, random_access_iterator_tag){
		i += n;
	}
	//对外提供的真正advance方法
	template <class Iterator, class Distance>
	inline void advance(Iterator& i, Distance n){
		__advance(i, n, iterator_category(i));
	}
}//end namespace ZRainySTL
#endif
