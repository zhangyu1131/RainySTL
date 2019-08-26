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

	void* alloc::refill(size_t bytes){
		size_t nobjs = 20;
		char * chunk = chunk_alloc(bytes, nobjs);
		obj** my_free_list = 0;
		obj* result = 0;
		obj* next = 0, *current = 0;
		
		//如果只获得一个区块，这个区块就分配给调用者用，free list不增加新节点
		if(nobjs == 1){
			return chunk;
		}
		//如果获得的区块超过一个，就准备调整对应的free-list节点
		my_free_list = free_list + FREELIST_INDEX(bytes);//选中要调整的节点
		result = (obj*)chunk;//将第一个节点分给result，准备返回给请求方

		*my_free_list = next = (obj*)(chunk + bytes);
		//从第二个节点开始纳入free-list
		for(size_t i = 1; ; i++){
			current = next;
			next = (obj*)((char*)next + bytes);//TODO 为什么要转成char*？
			                                   //暂时理解：因为obj*是一个union指针，这里强制转成char*表示用obj的第二个字段client_data读取，
											   //表示char数组的首地址，即next的地址，用地址加bytes即得到下一个obj*。
			if(i == nobjs - 1){
				current->next = 0;
				break;
			}else{
				current->next = next;
			}
		}
		return result;
	}


	char* alloc::chunk_alloc(size_t size, size_t& nobjs){
	}



}//end namespace ZRainySTL
