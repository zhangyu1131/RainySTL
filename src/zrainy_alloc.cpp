/* 
 * 空间配置器实现
 * 用SGI STL的话，这里实习的只是二级配置器，没有采用第一级配置器
 * 第一级配置器其实也是使用malloc来分配内存，但有oom处理机制，可能会释放出
 * 其他内存供使用，如果还不行会发出bad_alloc错误。
 * 这里暂时只实现简易配置器，所以未使用第一级配置器，具体可自行查资料。
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
		obj* volatile * my_free_list = 0;
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
		char* result = 0;
		size_t total_bytes = size * nobjs;//需要分配的总内存
		size_t total_left = end_free - start_free;//内存剩余空间

		if(total_left >= total_bytes){
			//内存池剩余空间满足分配要求
			//返回内存池空闲起点，并调整新起点
			result = start_free;
			start_free += total_bytes;
			return result;
		}else if(total_left >= size){
			//内存池剩余空间不能满足分配要求，但还可以分配一个以上所需对象
			//调整最大分配个数，然后返回对应数据
			nobjs = total_left / size;
			total_bytes = size * nobjs;
			result = start_free;
			start_free += total_bytes;
			return result;
		}else{
			//内存池剩余空间连一个对象都满足不了
			//会先尝试释放空间扩充内存池再分配
			if(total_left > 0){
				//如果内存池还有一些剩余空间，就发挥余热，把剩余空间
				//连到合适的free-list上
				obj * volatile * my_free_list = free_list + FREELIST_INDEX(total_left);
				((obj*)start_free)->next = *my_free_list;
				*my_free_list = (obj*)start_free;
			}
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			start_free = (char*)malloc(bytes_to_get);
			if(!start_free){
				//malloc分配失败
				//这里的策略是当malloc失败的时候回头去检查我们现在已经拥有的
				//free-list，试图将目前的free-list中的空闲区块拿过来使用。
				int i = 0;
				obj * volatile * my_free_list, *p;
				for(i = size; i <= ZMaxBytes::__MAX_BYTES; i += ZAlign::__ALIGN){
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if(p != 0){
						//该大小的链表尚有区块未分配
						//则调整free-list来释放出该区块
						*my_free_list = p->next;
						start_free = (char*)p;
						end_free = start_free + i;
						return chunk_alloc(size, nobjs);
					}
				}
				end_free = 0;//真的没内存了，TODO 感觉这句好像没什么必要，现在能
							 //想到的解释就是为了不让end_free指针变成野指针，所
							 //以设置为空指针，因为走到这里的时候start_free绝对已经是空指针了
							 //如果真是这个原因，那确实是想得周到，换我肯定忘记了，内存泄漏就来了……
			}
			//malloc分配成功
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			return chunk_alloc(size, nobjs);//这里和malloc失败中的return语句都是递归调用本身，目的是得到内存后重新进行分配，修正nobjs
		}
	}
}//end namespace ZRainySTL
