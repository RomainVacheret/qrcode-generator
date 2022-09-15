#include <stdio.h>

#ifndef LOGGER_H
#define LOGGER_H

typedef enum {
	DEBUG, INFO, WARNING, 
	ERROR, CRITICAL
} LogMode;

typedef struct {
	LogMode mode; 
	FILE* stream;
} Logger;


Logger* logger_alloc(LogMode mode, FILE* stream);
void logger_write(Logger* self, char* msg, LogMode mode);

extern Logger* LOGGER;

#endif // LOGGER_H