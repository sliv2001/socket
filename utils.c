#include "utils.h"

char* stralloc(int* len){
	char* res=(char*)malloc(*len);
	if (res==NULL){
		*len=0;
		return NULL;
	}
	return res;
}

char* strrealloc(char* str, int* len){
	char* res;
	if (*len==0)
		return str;
	res=(char*)realloc(str, sizeof(str)+*len);
	if (res==NULL){
		len=0;
		return str;
	}
	return res;
}
