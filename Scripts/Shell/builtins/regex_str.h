﻿//Author: mgood7123 (Matthew James Good) http://github.com/mgood7123

int getbitgroupcount(int bit) {
	return snprintf(stdout, 0, "%d", bit);
}

struct regex_string
{
	char * string;
	int index;
	int len;
	int malloced;
};

#define str_malloc(y, z) \
	struct regex_string y; \
	y.string = malloc(z); \
	memset(y.string, 0, z); \
	y.malloced = z; 
	
#define str_mallocr(y, z) \
	y.string = malloc(z); \
	memset(y.string, 0, z); \
	y.malloced = z; 
	
#define str_new(str) \
	str_malloc(str, 1) \
	str.len = 0; \
	str.index = 0;
	
#define str_free(y) \
	{ \
		free(y.string); \
		y.malloced = 0; \
		y.len = 0; \
		y.index = 0; \
	}
	
#define str_reset(str) { \
	str_free(str) \
	str_mallocr(str, 1) \
	str.len = 0; \
	str.index = 0; \
}

#define str_realloc(y, z) \
	y.string = realloc(y.string, z); \
	if (y.malloced < z) { \
		memset(y.string+y.malloced, 0, y.malloced-z); \
	} \
	y.malloced = z;
	
#define str_info(str) \
	{ \
		printf("%s.index = %d\n", #str, str.index); \
		printf("%s.len = %d\n", #str, str.len); \
		printf("%s.malloced = %d\n", #str, str.malloced); \
		printf("%s.string = %s\n", #str, str.string); \
	}


#define str_insert_char(str, index, ch) { \
	str_realloc(str, str.malloced+1); \
	str.string[index] = ch; \
	index++; \
	str.len = strlen(str.string); \
}

#define str_insert_string(str, index, string) { \
	char * s = string; \
	for(int i = 0; s[i]; i++) \
		str_insert_char(str, index, s[i]); \
}

#define str_int2string(x,y) \
	char * x = malloc(getbitgroupcount(y)); \
	sprintf(x, "%d", y);
	
#define str_insert_int(str, index, integer) { \
	str_int2string(j, integer); \
	str_insert_string(str, index, j); \
	free(j); \
}

int reverseBool(int val) {
	if (val == 1 || val == 0) return val^1;
	else return val;
}

void str_output(struct regex_string * str, const char * file);

void str_output_append(struct regex_string * str, const char * file);

#define str_output(str, file) { \
	FILE * stream = stdout; \
	int isfile = 0; \
	if (strcmp("stdin" file) == 0) stream = stdin; \
	else if(strcmp("stdout" file) == 0) stream = stdout; \
	else if(strcmp("stderr" file) == 0) stream = stderr; \
	else isfile = 1; \
	if (isfile) stream = fopen(file, "w"); \
	fwrite(str.string,str.len, 1, stream); \
	if (isfile) fclose(stream); \
}

#define str_output_append(str, file) { \
	FILE * stream = stdout; \
	int isfile = 0; \
	if (strcmp("stdin" file) == 0) stream = stdin; \
	else if(strcmp("stdout" file) == 0) stream = stdout; \
	else if(strcmp("stderr" file) == 0) stream = stderr; \
	else isfile = 1; \
	if (isfile) stream = fopen(file, "a"); \
	fwrite(str.string,str.len, 1, stream); \
	if (isfile) fclose(stream); \
}
