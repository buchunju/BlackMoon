#ifndef __STRING_H_
#define __STRING_H_

#include "bios/utils/stdint.h"
#include "bios/utils/stddef.h"


int itoa(const int num, char ostr[]);
int htoa(const int num, char ostr[]);

char * itos(uint32_t myint, char buffer[], int bufflen);

long int atoh(const char *nptr, int size);
uint8_t hex_char(uint8_t byte);

int    strcmp(const char *s1, const char *s2);
size_t strlen(const char *s);
char *strdup(const char *s);
char  *strcpy(char *dest, const char *src);
char  *strncpy(char *dest, const char *src, size_t n);
int    strncmp(const char *s1, const char *s2, size_t n);
char * strstr(const char * haystack, const char * needle);
uint8_t startswith(const char * str, const char * accept);
char * strtok_r(char * str, const char * delim, char ** saveptr);
char * strpbrk(const char * str, const char * accept);
size_t strspn(const char * str, const char * accept);

size_t lfind(const char * str, const char accept);
size_t rfind(const char * str, const char accept);

void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);

int toupper(int c);
int tolower(int c);

#endif //__STRING_H_