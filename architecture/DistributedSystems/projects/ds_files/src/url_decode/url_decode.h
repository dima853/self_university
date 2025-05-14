#ifndef URL_DECODE_H  
#define URL_DECODE_H

#include <stdlib.h>

char* url_decode(const char* src);

int url_decode_safe(const char* src, char* dest, size_t dest_size);

#endif