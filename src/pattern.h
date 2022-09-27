#include "utils.h"

#ifndef PATTERN_H
#define PATTERN_H

void set_finder_pattern(Array* matrix, size_t initial_idx, bool value);
void set_finder(Array* matrix, size_t initial_idx);
void set_all_finders(Array* matrix);
void set_timings_n_dark(Array* matrix);
void reserve_finder(Array* matrix, size_t initial_idx);
void reserve_all_finders(Array* matrix);
void reserve_all_patterns(Array* matrix);

#endif // PATTERN_H
