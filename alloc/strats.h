/**
 * @file
 * @brief Module to change the currently used memory algorithm
 */
#ifndef ALLOC_STRATS_H
#define ALLOC_STRATS_H

#include "alloc/types.h"

/** @brief Sets alloc algorithm
 *
 * This function sets the global memory algorithm function pointer
 *
 * @param[in] strat An alloc algorithm. See alloc_strat_e for a list.
 *
 */
void set_alloc_function(alloc_strat_e strat);

#endif
