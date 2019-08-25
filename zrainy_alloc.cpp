/* 
 * 空间配置器实现
 *
 * by Zrainy
 *
 */

#include "../include/zrainy_alloc.h"

namespace ZRainySTL{
	char* alloc::start_free = 0;
	char* alloc::end_free = 0;
	size_t alloc::heap_size = 0;

	alloc::obj* alloc::free_list[alloc::ZNewFreeLists::__NFREELISTS] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	void* alloc::allocate(size_t bytes){
		if(bytes > alloc::ZMaxBytes::__MAX_BYTES){
			return malloc(bytes);
		}
		size_t index = FREELIST_INDEX(bytes);
		obj* listNode = free_list[index];
		if(listNode){//该节点下还有可用空间，就直接分配
			free_list[index] = listNode->next;
			return listNode;
		}else{//该节点下没有可用空间，调用refill函数进行处理
			return refill(ROUND_UP(bytes));
		}
	}

	void alloc::deallocate(void* ptr, size_t bytes){
		if(bytes > alloc::ZMaxBytes::__MAX_BYTES){
			return free(ptr);
		}else{
			size_t index = FREELIST_INDEX(bytes);
			obj* listNode = static_cast<obj*>(ptr);
			listNode->next = free_list[index];
			free_list[index] = listNode;
		}
	}

	void* alloc::reallocate(void* ptr, size_t old_sz, size_t new_sz){
		deallocate(ptr,old_sz);
		ptr = allocate(new_sz);
		return ptr;
	}



}//end namespace ZRainySTL
