#include <stdio.h>
#include <stdlib.h>

#include "log_antilog.h"


static LogAntilogTable* lat_alloc() {
    LogAntilogTable* table = (LogAntilogTable*) malloc(sizeof(LogAntilogTable));
    table->size = 256;

    return table;
}

void lat_free(LogAntilogTable* self) {
    free(self);
}

LogAntilogTable* lat_initialize() {
    FILE* file = fopen("log-antilog.csv", "r");
    LogAntilogTable* table = lat_alloc();
    int a, b, c, d;

    if(!file) {
        fprintf(stderr, "[ERROR] An error occured while opening the log-antilog table");
        exit(1);
    }

    while(fscanf(file, "%d,%d,%d,%d", &a, &b, &c, &d) != EOF) {
        table->antilog[a] = b;
        table->log[c] = d;
    }
    
    fclose(file);
    return table;
}
