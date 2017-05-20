#ifndef ENABLE_LOG_H
#define ENABLE_LOG_H

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#ifdef __linux__ //TODO add MacOSX and Unix and Solaris
#include <unistd.h>
#include <pthread.h>
#define SEPARATOR ('/')
#elif _WIN32
#include <Windows.h>
#define SEPARATOR ('\\')
#else
#error Platform not supprted
#endif

#ifdef ENABLE_LOG_COLOR
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"
#else
#define COLOR_RED     ""
#define COLOR_GREEN   ""
#define COLOR_YELLOW  ""
#define COLOR_BLUE    ""
#define COLOR_MAGENTA ""
#define COLOR_CYAN    ""
#define COLOR_RESET   ""
#endif

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 256
#endif

int n = 2;
int *writefd = &n; //for different socket support

#ifdef __linux__ //TODO siblings
pthread_mutex_t lock;
pthread_mutex_t plock;
#else
//windows locks
#endif

void change_out_socket(int *sockfd) {
	writefd = sockfd;
}

void init_logger() {

#ifdef __linux__
	pthread_mutex_init(&lock, NULL);
	pthread_mutex_init(&plock, NULL);
#else
	//windows mutex init
#endif
}

void log_inf(const char *tag, const char *msg, ...) {
#ifdef ENABLE_LOG
#ifdef __linux__
	pthread_mutex_lock(&lock);
#endif
	char *str = (char *)malloc(BUFFER_SIZE);
	va_list vl;
	va_start(vl, msg);
	vsprintf(str, msg, vl);
	va_end(vl);
	fprintf(stderr, "%s[!]%s %s: %s\n", COLOR_YELLOW, COLOR_RESET, tag, str);
#ifdef __linux__
	pthread_mutex_unlock(&lock);
#endif
#endif
}

void log_err(const char *tag, const char *msg, ...) {
#ifdef ENABLE_LOG
#ifdef __linux__
	pthread_mutex_lock(&lock);
#endif
	char *str = (char *)malloc(BUFFER_SIZE);
	va_list vl;
	va_start(vl, msg);
	vsprintf(str, msg, vl);
	va_end(vl);
	fprintf(stderr, "[X] %s: %s\n", tag, str);
#ifdef __linux__
	pthread_mutex_unlock(&lock);
#endif
#endif
}

void log_per(const char *tag, const char *msg, ...) {
#ifdef ENABLE_LOG
#ifdef __linux__
	pthread_mutex_lock(&lock);
#endif
	char *str = (char *)malloc(BUFFER_SIZE);
	va_list vl;
	va_start(vl, msg);
	vsprintf(str, msg, vl);
	va_end(vl);
	perror(str);
#ifdef __linux__
	pthread_mutex_unlock(&lock);
#endif
#endif
}

void log_fat(const char *tag, const char *msg, ...) {
#ifdef ENABLE_LOG
#ifdef __linux__
	pthread_mutex_lock(&lock);
#endif
	char *str = (char *)malloc(BUFFER_SIZE);
	va_list vl;
	va_start(vl, msg);
	vsprintf(str, msg, vl);
	va_end(vl);
	fprintf(stderr, "[FATAL] %s: %s\n", tag, str);
#ifdef __linux__
	pthread_mutex_unlock(&lock);
#endif
#endif
}
#endif
