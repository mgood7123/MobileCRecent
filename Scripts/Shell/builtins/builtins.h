﻿//Author: mgood7123 (Matthew James Good) http://github.com/mgood7123

#ifndef shell__builtins
#define shell__builtins
#ifndef SHELL
#include "getline.h"
#include "colors.h"
#include "printfmacro.h"
#include "usage.h"

char * chartostring(char c) {
	char cc[2];
	cc[0] = c;
	cc[1] = '\0';
	return strdup(cc);
}

#define DEBUG if (shell.debug)
#define DEBUG2 if (shell.debug_calls)

#ifndef DEBUG_SLEEP
#define DEBUG_SLEEP /* printf("sleeping for 2 seconds\n") ; sleep(2) ; */
#endif


#ifndef PASSED
#define PASSED DEBUG2 DEBUG_SLEEP fprintf(stderr, "passed %s() at line %d from %s\n", __func__, __LINE__, __FILE__);
#endif

#ifndef PASSED_COLORS
#define PASSED_COLORS
#define PASSED_B DEBUG2 DEBUG_SLEEP fprintf_b(stderr, "passed %s() at line %d from %s\n", __func__, __LINE__, __FILE__);
#define PASSED_R DEBUG2 DEBUG_SLEEP fprintf_r(stderr, "passed %s() at line %d from %s\n", __func__, __LINE__, __FILE__);
#define PASSED_M DEBUGw DEBUG_SLEEP fprintf_m(stderr, "passed %s() at line %d from %s\n", __func__, __LINE__, __FILE__);
#endif

#ifndef CURRENT_FUNCTION
#define CURRENT_FUNCTION DEBUG2 printf("->called %s() at line %d from %s\n", __func__, __LINE__, __FILE__);
#endif


#define dothis(what, times) {\
	for(int i = 0;i<times;i++) { what }; \
}


#define SHELL

struct shell {
	char * name;
	char * version;
	int exit;
	int debug;
	int debug_calls;
	int exe;
	int exebackground;
	int internal;
	int builtin;
} shell = { "Shell", "1.1" };
#endif

//#include "find.h"

#include "libstring.h"
#include <SDL.h>
#include <inttypes.h>
#include "regex.h"
#include "argv.h"
#define itta(x) list_array(x, #x)
#include "grep.h" // not needed but included if it ever is
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <libgen.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef __CCR__
#include <ccr.h>
#endif
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include "clock.h"

ssize_t getline_stdin(char ** dest) {
	ssize_t nu = 0;
	getline_include_delimiter = false;
	return getline(dest, &nu, stdin);
}

#define ift if (has_time == true)

#define iftime(x) { ift { timefunc(x); } else { x; } }
#define iftimeret(x) { ift { timefuncret(x); } else { return x; } }
#define iftimeretc(x, ret) { ift { timefuncretc(x, ret); } else { x; return ret; } }


char * quote (char * str) { 
	char * p = malloc(strlen(str)+3); 
	sprintf(p, "\"%s\"", str); 
	return p;
}

#define mq(a, b) char * a = quote(b)

int builtin__CPU_Info(const int argc, const char * argv[]) {
	Uint64 PerformanceCounter = SDL_GetPerformanceCounter();
	Uint64 PerformanceFrequency = SDL_GetPerformanceFrequency();
	int CPUCount = SDL_GetCPUCount();
	int CPUCacheLineSize = SDL_GetCPUCacheLineSize();
	int RAM = SDL_GetSystemRAM();
	int seconds, pct;
	SDL_GetPowerInfo(&seconds, &pct);
	printf("System Information:\n    CPU Count: %d\n    CPU Cache Line Size: %d\n    Performance Counter: %zd\n    Performance Frequency: %zd\n    RAM: %d\n",
	CPUCount,
	CPUCacheLineSize,
	PerformanceCounter,
	PerformanceFrequency,
	RAM
	);
	return 0;
}

#include "bom.h"

void builtin__coliru(char * mode, char * file, char * cmd, char * out)
{
	if(access(file, F_OK) == -1) {
		printf(file);
		puts(" must be accessable");
		return;
	}
	if ((strcmp(mode, "compile") != 0 && strcmp(mode, "share") == 0) || (strcmp(mode, "compile") == 0 && strcmp(mode, "share") != 0));
	else {
		puts("must specify either \"compile\" or \"share\"");
		return;
	}
	if (file == NULL) {
		puts("must specify a file");
		return;
	}
	if (cmd == NULL) {
		puts("must specify a command (\" \" is allowed)");
		return;
	}
	char ** argv = env__add(env__add(env__new(), "default"), file);
	int argc = 2;
	char * s4 = builtin__json(argc, argv);
	free(argv);
	char * s1 = "curl 'http://coliru.stacked-crooked.com/";
	char * s2 = "' --data-binary '{\"cmd\":\"";
	char * s3 = "\
\",\"src\":\"";
	char * s5;
	if (out == NULL) {
		s5 = "\"}'";
		out = "";
	}
	else {
		s5 = "\"}' -o ";
	}
	char * line = malloc(
	strlen(s1)+
	strlen(mode)+
	strlen(s2)+
	strlen(cmd)+
	strlen(s3)+
	strlen(s4)+
	strlen(s5)+
	strlen(out)
	);
	sprintf(line, "%s%s%s%s%s%s%s%s", s1, mode, s2, cmd, s3, s4, s5, out);
	//puts(line);
	if (strcmp(mode, "share") == 0) printf("http://coliru.stacked-crooked.com/a/");
	system(line);
	puts("");
}

char * __find_unknown(int type) {
	char unknown[10+16];
	sprintf(unknown, "unknown (%d)", type);
	return unknown;
}

char * __find_gettype (int type) {
	if (type==8) return "file";
	else if (type==4) return "folder";
	else return __find_unknown(type);
}

int builtin__ls(const int argc, const char * argv[]) {
	DIR *pDir;
	struct dirent *ent;
	char * path = argc==1?"./":argv[1];
	pDir = opendir(path);
	if (pDir)
	{
		while ((ent = readdir(pDir)) != NULL) printf("%s : %s\n", ent->d_name, __find_gettype(ent->d_type));
		closedir(pDir);
	}
	else {
		printf("Access to %s was denied\n", path);
		return -1;
	}
	return 0;
}

#include "gcc.h"

#endif
 
