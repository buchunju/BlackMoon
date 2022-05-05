#include "bios/utils/string.h"
#include "bios/mem/mem.h"

int itoa(const int num, char ostr[])
{
	int quot, i;
	char rem;

	quot = num;
	i = 0;
	do {
		rem = (quot % 10) + '0';
		quot /= 10;
		ostr[i++] = rem;
	} while (quot);

	int start, end;
	char temp;
	for (end = i - 1, start = 0; start < end; end--, start++) {
		temp = ostr[start];
		ostr[start] = ostr[end];
		ostr[end] = temp;
	}

	ostr[i] = '\0';
	return i;
}

int htoa(const int num, char ostr[])
{
	int index = 32;
	int val;
	int cnt = 0;

	do {
		index -= 4;
		val = (num >> index) & 0xf;
	} while (!val && index);

	ostr[cnt++] = '0';
	ostr[cnt++] = 'x';
	do {
		if (val > 9)
			val = 'A' + (val - 10);
		else
			val += '0';
		ostr[cnt++] = val;
		index -= 4;
		val = (num >> index) & 0xf;
	} while (index >= 0);

	ostr[cnt] = '\0';
	return cnt;
}

long int atoh(const char *nptr, int size)
{
        long int val = 0;
        int i;
        for (i = 0; i < size; i++) {
                if (nptr[i] == ' ')
                        continue;
                val <<= 4;
                if (nptr[i] >= '0' && nptr[i] <= '9')
                        val += nptr[i] - '0';
                else if (nptr[i] >= 'a' && nptr[i] <= 'f')
                        val += (10 + (nptr[i] - 'a'));
                else if (nptr[i] >= 'A' && nptr[i] <= 'F')
                        val += 10 + (nptr[i] - 'A');
        }

        return val;
}


void *memcpy(void *dest, const void *src, size_t n)
{
	size_t index = 0;
	unsigned char *t_dest = (unsigned char *) dest;
	unsigned char *t_src = (unsigned char *) src;

	if ((((size_t) dest ^ (size_t) src) & 0x3) == 0) {
		/* We can align src and dest on word boundary */
		int size = (size_t) dest & 0x3;
		while (size-- && index < n) {
			*t_dest++ = *t_src++;
			index++;
		}

		unsigned int *w_dest = (unsigned int *) t_dest;
		unsigned int *w_src = (unsigned int *) t_src;
		while (index < (n - sizeof(unsigned int))) {
			*w_dest++ = *w_src++;
			index += sizeof(unsigned int);
		}
		t_dest = (unsigned char *) w_dest;
		t_src = (unsigned char *) w_src;
	}

	while (index < n) {
		*t_dest++ = *t_src++;
		index++;
	}

	return dest;
}

void *memset(void *s, int c, size_t n)
{
	unsigned char *src = (unsigned char *) s;
	size_t i;

	for (i = 0; i < n; i++)
		src[i] = c;
	return s;
}


size_t strlen(const char *s)
{
	int i;
	for (i = 0; s[i]; i++)
		;

	return i;
}

char *strcpy(char *dest, const char *src)
{
	int i;

	for (i = 0; src[i]; i++)
		dest[i] = src[i];

	dest[i] = '\0';	
	return dest;
}

char *strncpy(char *dest, const char *src, size_t n)
{
	size_t i;

	for (i = 0; i < n && src[i] != '\0'; i++)
		dest[i] = src[i];

	for (; i < n; i++)
		dest[i] = '\0';
	
	return dest;
}

int strcmp(const char *s1, const char *s2)
{
	int i;

	for (i = 0; s1[i]; i++) {
		if (s1[i] != s2[i])
			return s1[i] - s2[i];
	}
	return 0;
}

int strncmp(const char *s1, const char *s2, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++) {
		if (s1[i] != s2[i])
			return s1[i] - s2[i];
		else if (s1[i] == '\0')
			return 0;
	}

	return 0;
}

long int strtox(const char *nptr, int size)
{
        long int val = 0;
        int i;
        for (i = 0; i < size; i++) {
                if (nptr[i] == ' ')
                        continue;
                val <<= 4;
                if (nptr[i] >= '0' && nptr[i] <= '9')
                        val += nptr[i] - '0';
                else if (nptr[i] >= 'a' && nptr[i] <= 'f')
                        val += (10 + (nptr[i] - 'a'));
                else if (nptr[i] >= 'A' && nptr[i] <= 'F')
                        val += 10 + (nptr[i] - 'A');
        }

        return val;
}

char * strtok_r(char * str, const char * delim, char ** saveptr) {
	char * token;
	if (str == NULL) {
		str = *saveptr;
	}
	str += strspn(str, delim);
	if (*str == '\0') {
		*saveptr = str;
		return NULL;
	}
	token = str;
	str = strpbrk(token, delim);
	if (str == NULL) {
		*saveptr = (char *)lfind(token, '\0');
	} else {
		*str = '\0';
		*saveptr = str + 1;
	}
	return token;
}

size_t lfind(const char * str, const char accept) {
	size_t i = 0;
	while ( str[i] != accept) {
		i++;
	}
	return (size_t)(str) + i;
}

size_t rfind(const char * str, const char accept) {
	size_t i = strlen(str) - 1;
	while (str[i] != accept) {
		if (i == 0) return -1;
		i--;
	}
	return (size_t)(str) + i;
}

char * strstr(const char * haystack, const char * needle) {
	const char * out = NULL;
	const char * ptr;
	const char * acc;
	const char * p;
	size_t s = strlen(needle);
	for (ptr = haystack; *ptr != '\0'; ++ptr) {
		size_t accept = 0;
		out = ptr;
		p = ptr;
		for (acc = needle; (*acc != '\0') && (*p != '\0'); ++acc) {
			if (*p == *acc) {
				accept++;
				p++;
			} else {
				break;
			}
		}
		if (accept == s) {
			return (char *)out;
		}
	}
	return NULL;
}

uint8_t startswith(const char * str, const char * accept) {
	size_t s = strlen(accept);
	for (size_t i = 0; i < s; ++i) {
		if (*str != *accept) return 0;
		str++;
		accept++;
	}
	return 1;
}

char *strdup(const char *s) {
    char *news = kmalloc(strlen(s) + 1);
    char *temp = news;
    while(*temp++ = *s++);
    return news;
}

size_t strspn(const char * str, const char * accept) {
	const char * ptr = str;
	const char * acc;

	while (*str) {
		for (acc = accept; *acc; ++acc) {
			if (*str == *acc) {
				break;
			}
		}
		if (*acc == '\0') {
			break;
		}

		str++;
	}

	return str - ptr;
}

char * strpbrk(const char * str, const char * accept) {
	const char *acc = accept;

	if (!*str) {
		return NULL;
	}

	while (*str) {
		for (acc = accept; *acc; ++acc) {
			if (*str == *acc) {
				break;
			}
		}
		if (*acc) {
			break;
		}
		++str;
	}

	if (*acc == '\0') {
		return NULL;
	}

	return (char *)str;
}

int toupper(int c) {
    if(c >= 97 && c <= 122) {
        return c - 32;
    }
    return c;
}
int tolower(int c) {
    if(c >=65 && c <= 90) {
        return c + 32;
    }
    return c;
}


char * itos(uint32_t myint, char buffer[], int bufflen)
{
    int i = bufflen - 2;
    buffer[bufflen-1] = 0;

    if(myint == 0) {
        buffer[i--] = '0';
    }

    while(myint > 0 && i >= 0)
    {
        buffer[i--] = (myint % 10) + '0';
        myint/=10;
    }

    return &buffer[i+1];
}

uint8_t hex_char(uint8_t byte)
{
    byte = byte & 0x0F;
    if(byte < 0xA)
    {
        char buff[2];
        itos(byte, buff, 2);
        return buff[0];
    }
    else
    {
        switch(byte)
        {
        case 0x0A:
            return 'A';
            break;
        case 0x0B:
            return 'B';
            break;
        case 0x0C:
            return 'C';
            break;
        case 0x0D:
            return 'D';
            break;
        case 0x0E:
            return 'E';
            break;
        case 0x0F:
            return 'F';
            break;
        }
    }
    return 0;
}
