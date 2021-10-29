#ifdef __cplusplus
extern "C" {
#endif

#include"memallocator.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

	typedef struct descriptor {
		int size;
		struct descriptor* next;
		int isFree;
	}descriptor;

	typedef struct {
		descriptor* desc;
		int size;
	}memAllocator;

	memAllocator Allocator = { (descriptor*)NULL };

	int memgetminimumsize() {
		return sizeof(descriptor);
	}

	int memgetblocksize() {
		return sizeof(descriptor);
	}

	int meminit(void* InitMem, int size) {
		if (!InitMem || size <= memgetminimumsize())
			return 1;
		descriptor desc;
		Allocator.size = size;
		Allocator.desc = (descriptor*)InitMem;
		Allocator.desc->size = size - memgetminimumsize();
		Allocator.desc->next = NULL;
		Allocator.desc->isFree = 1;
		return 0;
	}

	void* memalloc(int size) {
		if (size <= 0 || !Allocator.desc)
			return NULL;

		void* res = NULL;
		descriptor* cur = Allocator.desc;
		descriptor* addnext = NULL;
		descriptor* pointerBestFit = NULL;
		int blockSize = memgetblocksize();

		while (cur) {
			if ((cur->size >= size) && (pointerBestFit == NULL || (cur->size < pointerBestFit->size)) && (cur->isFree))
				pointerBestFit = cur;
			else cur = cur->next;
		}

		if (pointerBestFit) {
			if (pointerBestFit->size <= size + memgetminimumsize())
				pointerBestFit->isFree = 0;
			else {
				//ѕроверка на размер между текущим диск и следущим
				if (pointerBestFit->next && pointerBestFit->next - (pointerBestFit + size) < memgetminimumsize())
				{
					pointerBestFit->isFree = 0;
					pointerBestFit->size = size;
				}
				else {
					addnext = (descriptor*)((char*)pointerBestFit + size + blockSize);
					addnext->next = pointerBestFit->next;
					addnext->isFree = 1;
					pointerBestFit->next = addnext;
					pointerBestFit->isFree = 0;
					addnext->size = pointerBestFit->size - size - blockSize;
					pointerBestFit->size = size;
				}
			}
			res = (void*)((char*)pointerBestFit + blockSize);
		}
		else {
			res = NULL;
		}
		return res;
	}

	void memfree(void* memory) {
		int blockSize = memgetblocksize();
		descriptor* cur = Allocator.desc;
		descriptor* prev = NULL;
		descriptor* next = NULL;
		while (cur) {
			if ((char*)memory != (char*)(cur + 1)) {
				prev = cur;
				cur = cur->next;
			}
			else if ((char*)(cur + 1) == (char*)memory)
				break;
			else return;
		}

		if (!cur || cur->isFree)
			return;

		if (cur->next && cur->next->isFree) {
			cur->size += cur->next->size + blockSize;
			cur->next = cur->next->next;
		}
			
		if (prev && prev->isFree) {
			prev->size += blockSize + cur->size;
			prev->next = cur->next;
		}
		
		cur->isFree = 1;
		return;
	}

	void memdone() {
		Allocator.size = 0;
		Allocator.desc = NULL;
	}

#ifdef __cplusplus
}
#endif 