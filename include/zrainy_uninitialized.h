/*
 * 内存基本处理工具
 * 包括uninitialized_copy/uninitialized_fill/uninitialized_fill_n三个全局方法
 *
 * by Zrainy
 */

#ifndef ZRAINY_UNINITIALIZED_H
#define ZRAINY_UNINITIALIZED_H

#include "zrainy_construct.h"
#include "zrainy_type_traits.h"
#include "zrainy_iterator.h"
#include "zrainy_algorithm.h"

namespace ZRainySTL{
	//start uninitialized_copy
	
	//针对_true_type的内部copy方法，外部不要直接调用
	template <class InputIterator, class ForwardIterator>
	ForwardIterator
	_uninitialized_copy_aux(InputIterator first, InputIterator last,ForwardIterator result,  _true_type){
		memcpy(result, first, (last - first) * sizeof(*first));
		return result + (last - first);
	}
	
	//针对_false_type的内部copy方法，外部不要直接调用
	template <class InputIterator, class ForwardIterator>
	ForwardIterator
	_uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result,  _false_type){
		int i = 0;
		for(; first != last; ++first){
			construct(result + i, *first);
			i++;
		}
		return (result + i);
	}

	//内部调用的copy方法，在这个方法里萃取出is_POD特性，然后分别调用上述两个方法
	template <class InputIterator, class ForwardIterator, class T>
	inline ForwardIterator
	_uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*){
		typedef typename _type_traits<T>::is_POD_type is_POD;
		return _uninitialized_copy_aux(first, last, result, is_POD()/*只是调用下构造函数生成一个匿名对象*/);
	}
	
	//真正对外的uninitialized_copy
	template <class InputIterator, class ForwardIterator>
	ForwardIterator
	uninitialized_copy(InputIterator first, InputIterator last,	
						ForwardIterator result){
		return _uninitialized_copy(first, last, result, value_type(result));
	}
	// 书上还有针对char和wchar_t的特化版本，这里暂时不提供
	// end uninitialized_copy
	
	//*************************************************************************//	
	// start uninitialized_fill
	
	//针对_true_type的内部fill方法，外部不要直接调用
	template <class ForwardIterator, class T>
	void
	_uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& v, _true_type){
		fill(first, last, v);
	}

	//针对_false_type的内部fill方法，外部不要直接调用
	template <class ForwardIterator, class T>
	void
	_uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& v, _false_type){
		ForwardIterator cur = first;
		for(; cur != last; cur++){
			construct(&*cur, v);
		}
	}

	//内部调用的fill方法，在这个方法里萃取出is_POD特性，然后分别调用上述两个方法
	template <class ForwardIterator, class T, class T1>
	void
	_uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& v, T1*){
		typedef typename _type_traits<T1>::is_POD_type is_POD;
		_uninitialized_fill_aux(first, last, v, is_POD());
	}

	//真正对外的uninitialized_fill
	template <class ForwardIterator, class T>
	void
	uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& v){
		_uninitialized_fill(first, last, v, value_type(first));
	}

	// end uninitialized_fill
	
	//*************************************************************************//	
	// start uninitialized_fill_n
	
	//针对_true_type的内部fill_n方法，外部不要直接调用
	template <class ForwardIterator, class Size, class T>
	ForwardIterator
	_uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& v, _true_type){
		return fill_n(first, n, v);
	}

	//针对_false_type的内部fill_n方法，外部不要直接调用
	template <class ForwardIterator, class Size, class T>
	ForwardIterator
	_uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& v, _false_type){
		ForwardIterator cur = first;
		for(; n > 0; --n, cur++){
			construct(&*cur, v);
		}
		return cur;
	}

	//内部调用的fill_n方法，在这个方法里萃取出is_POD特性，然后分别调用上述两个方法
	template <class ForwardIterator, class Size, class T, class T1>
	ForwardIterator
	_uninitialized_fill_n(ForwardIterator first, Size n, const T& v, T1*){
		typedef typename _type_traits<T1>::is_POD_type is_POD;
		return _uninitialized_fill_n_aux(first, n, v, is_POD());
	}
	
	//真正对外的uninitialized_fill_n
	template <class ForwardIterator, class Size, class T>
	ForwardIterator
	uninitialized_fill_n(ForwardIterator first, Size n, const T& v){
		return _uninitialized_fill_n(first, n, v, value_type(first));
	}
	// end uninitialized_fill_n
	
}//end namespace ZRainySTL

#endif //end ZRAINY_UNINITIALIZED_H

