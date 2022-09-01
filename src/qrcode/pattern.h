#include "utils.h"

#ifndef PATTERN_H
#define PATTERN_H

void pattern_set_finder_pattern(Array* matrix, size_t initial_idx, bool value);
void pattern_set_finder(Array* matrix, size_t initial_idx);
void pattern_set_all_finders(Array* matrix);
void pattern_set_timings_n_dark(Array* matrix);
void pattern_reserve_finder(Array* matrix, size_t initial_idx);
void pattern_reserve_all_finders(Array* matrix);
void pattern_reserve_all_patterns(Array* matrix);

#endif // PATTERN_H
