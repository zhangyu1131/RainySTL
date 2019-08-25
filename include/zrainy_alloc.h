/*
 * 空间配置器
 *
 * by Zrainy
 */

#ifndef ZRAINY_ALLOC_H
#define ZRAINY_ALLOC_H

#include <cstdlib> //该头文件包含了size_t类型的定义

namespace ZRainySTL{
	class alloc{
	private:
		//小型区块的上调边界
		enum ZAlign {__ALIGN = 8};
		//小型区块的大小上限
		enum ZMaxBytes {__MAX_BYTES = 128};
		//free-lists的个数，是上述两个值的商
		enum ZNewFreeLists {__NFREELISTS = ZMaxBytes::__MAX_BYTES / ZAlign::__ALIGN};
	private:
		//free-lists节点
		//Note：这里的使用也可以借鉴在其他需要节省内存的地方
		union obj{
			union obj* next;//未分配出去时，指向链表下一个
			char client_data[1];//分配出去后指向实际区块，用union是为了节省空间，相当于一个存的是char数组的首地址，一个是char数组的内容
		};

		static obj *free_list[ZNewFreeLists::__NFREELISTS];
	
	private:
		static char* start_free;//内存池起始位置
		static char* end_free;//内存池结束位置
		static size_t heap_size;
	
	private:
		//将byte上调至8的倍数
		static size_t ROUND_UP(size_t bytes){
			return ((bytes + ZAlign::__ALIGN - 1) & ~(ZAlign::__ALIGN - 1));
		}

		//根据区块大小，决定使用第n号free-list
		//Note：这里的计算方法可以用在任何计算类似数组下标的地方，很简洁
		static size_t FREELIST_INDEX(size_t bytes){
			return ((bytes + ZAlign::__ALIGN - 1) / ZAlign::__ALIGN - 1);
		}

		//返回一个大小为n的对象，并可能加入大小为n的其他区块到free-list
		static void* refill(size_t n);
		//配置一块大空间，可容纳nobjs个大小为size的区块
		//如果配置nobjs个区块有问题，可能会改变nobjs的大小，所以参数是引用
		static char* chunk_alloc(size_t size, size_t& nobjs);
	
	public:
		static void* allocate(size_t bytes);
		static void deallocate(void* ptr, size_t bytes);
		static void* reallocate(void *ptr, size_t old_sz, size_t new_sz);
		
	};//end class alloc
		

}//end namespace ZRainySTL
#endif

