#include <stdlib.h>

#include "logger.h"

Logger* LOGGER;

const static char* MODE_STRING[5] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITITAL"};

Logger* logger_alloc(LogMode mode, FILE* stream) {
	Logger* self = (Logger*) malloc(sizeof(Logger));
	self->mode = mode;
	self->stream = stream;

	return self;
}

void logger_write(Logger* self, char* msg, LogMode mode) {
	if(self->mode <= mode) {
		fprintf(self->stream, "[%s] %s\n", MODE_STRING[mode], msg);
	}
}