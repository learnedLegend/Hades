#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>


#ifdef __linux__ 
#define SEPARATOR ('/')
#elif _WIN32
#define SEPARATOR ('\\')
#else     
#error Platform not supported
#endif

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 256
#endif

int n = 2;
int *writefd = &n; //for different socket support

char isCol = 0;

 static char tag[64] = "";

enum INIT_OPT {
	LOG_INIT = 1,
	LOG_COLOR = 10
};

typedef void (*opt)(int);

void _opt(int opti){
   if(opti == 1){
	isCol = 0;
   }
   if(opti == 11){
	isCol = 1;
   }
}

 opt TAG(const char *tag_name)
 {
   strcpy(tag, tag_name);
   return _opt;
 }

void change_socket(int sockfd){
	*writefd = sockfd;
}

 enum LEVEL {
   CRITICAL,
   PERROR,
   ERROR,
   INFO
 };

typedef void (*color)(const char *);

char *colstr = NULL;

void _color(const char *col){
	printf("%s%s%s\n", col, colstr, COLOR_RESET);
}

color print(const char *msg, ...){
	va_list vl;
	colstr = (char *)malloc(BUFFER_SIZE);
	va_start(vl, msg);
	vsprintf(colstr ,msg, vl);
	va_end(vl);
	return _color;
}

typedef void (*level)(enum LEVEL);

va_list vl;

char * str;

void _level(enum LEVEL log_level){
#ifdef LOG
	if(log_level == INFO)
		printf("%s[!]%s %s: %s\n",isCol == 1?YELLOW:"", isCol == 1?COLOR_RESET:"",tag,str);
	else if(log_level == ERROR){
		fprintf(stderr, "[X] %s: %s\n",tag,str);
	}
	else if(log_level == PERROR){
	}
	else if(log_level == CRITICAL)	
		printf("[CRITICAL] %s: %s\n",tag,str);
#endif
}

level logger(const char *msg, ...){
#ifdef LOG
	str = (char *)malloc(sizeof(msg) + BUFFER_SIZE);
	va_start(vl, msg);
	vsprintf(str,msg,vl);
	va_end(vl);
#endif
	return _level;

}

#endif
