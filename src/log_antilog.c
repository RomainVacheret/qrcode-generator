#include <stdio.h>
#include <stdlib.h>

#include "log_antilog.h"


LogAntilogTable* lat_alloc_table() {
    LogAntilogTable* table = (LogAntilogTable*) malloc(sizeof(LogAntilogTable));
    table->size = 256;

    return table;
}

void lat_free_table(LogAntilogTable* self) {
    free(self);
}

LogAntilogTable* lat_initialize_table() {
    FILE* file = fopen("log-antilog.csv", "r");
    LogAntilogTable* table = lat_alloc_table();
    int a, b, c, d;

    if(!file) {
        fprintf(stderr, "[ERROR] An error occured while opening the log-antilog table");
        exit(1);
    }

    while(fscanf(file, "%d,%d,%d,%d", &a, &b, &c, &d) != EOF) {
        table->antilog[a] = b;
        table->log[c] = d;
    }
    
    return table;
}




// int main() {
//     LogAntilogTable* table = lat_initialize_table();
//     printf("%d %d %d %d", table->log[3], table->log[66], table->antilog[6], table->antilog[234]);
// }
