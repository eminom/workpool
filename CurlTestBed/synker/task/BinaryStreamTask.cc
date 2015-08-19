
#include "BinaryStreamTask.h"
#include <cstdlib>

BinaryStreamTask::BinaryStreamTask(const char *url, int capacity)
	:begin_ptr_(nullptr),
	end_ptr_(nullptr),
	write_ptr_(nullptr),
	_taskUrl(url) {
	realloc(capacity);
}

BinaryStreamTask::~BinaryStreamTask() {
	cleanup();
}

size_t BinaryStreamTask::writeFunc(void *ptr, size_t sz, size_t nmemb) {
	size_t tot = sz * nmemb;
	int left = end_ptr_ - write_ptr_;
	if(left < tot) {
		size_t now = end_ptr_ - begin_ptr_;
		size_t req = tot - left + now;
		realloc(req * 2);  // realloc(req) for debug
	}
	memcpy(write_ptr_, ptr, tot);
	write_ptr_ += tot;
	return tot;
}

const char* BinaryStreamTask::taskUrl() {
	return _taskUrl.c_str();
}

void BinaryStreamTask::realloc(int size) {
	char *begin = (char*)malloc(size);
	char *end   = begin + size;
	char *write = begin;
	if (begin_ptr_) {
		int org_size = write_ptr_ - begin_ptr_;
		int wsize = size;
		if(org_size < wsize ) {
			wsize = org_size;
		}
		memcpy(begin, begin_ptr_, wsize);
		write = begin + wsize;
		free(begin_ptr_); //~ Free the olds
	}
	begin_ptr_ = begin;
	write_ptr_ = write;
	end_ptr_   = end;
}

void BinaryStreamTask::cleanup() {
	free(begin_ptr_);
	begin_ptr_ = end_ptr_ = write_ptr_ = nullptr;
}


int BinaryStreamTask::length()const{
	return write_ptr_ - begin_ptr_;
}

const char* BinaryStreamTask::data()const{
	return begin_ptr_;
}