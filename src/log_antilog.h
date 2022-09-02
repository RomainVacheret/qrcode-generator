#include <stdlib.h>

#ifndef LOG_ANTILOG_H
#define LOG_ANTILOG_H


typedef struct {
    int log[256];
    int antilog[256];
    size_t size;
} LogAntilogTable;

void lat_free_table(LogAntilogTable* self);
LogAntilogTable* lat_initialize_table();

#endif // LOG_ANTILOG_H
