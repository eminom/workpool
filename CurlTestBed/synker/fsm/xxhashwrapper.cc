

#include "xxhashwrapper.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "deps/xxhash/xxhash.h"

//
unsigned int calculateFileXXHASH(const char *path) {
	FILE *fin = fopen(path, "rb");
	if(!fin){
		fprintf(stderr, "Failed to open %s\n", path);
		return 0;
	}
	fseek(fin, 0, SEEK_END);
	int length = ftell(fin);		//~ Warnings
	rewind(fin);
	char *buffer = (char*)malloc(length+1);
	fread(buffer, length, 1, fin);
	fclose(fin);
	void* handle = XXH32_init(10241024);
	XXH32_update(handle, buffer, length);
	unsigned int hex = XXH32_digest(handle);
	free(buffer);
	return hex;
}
