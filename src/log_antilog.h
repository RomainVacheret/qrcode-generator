#include <stdlib.h>

#ifndef LOG_ANTILOG_H
#define LOG_ANTILOG_H


typedef struct {
    int log[256];
    int antilog[256];
    size_t size;
} LogAntilogTable;

LogAntilogTable* lat_initialize();
void lat_free(LogAntilogTable* self);

#endif // LOG_ANTILOG_H
