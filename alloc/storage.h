#ifndef ALLOC_STORAGE_H
#define ALLOC_STORAGE_H

#include "alloc/types.h"

/** @brief Reads byte where address points to
 *
 * Given an address @p addr, this function reads the byte value where addr
 * points to
 *
 * @param[in] addr An address
 * @return Value of byte where address points to
 *
 */
uint8_t *read_byte(const uint8_t *addr);

/** @brief Sets byte where address points to
 *
 * Given an address @p addr, this function sets the byte where @p addr points to
 * to @p v
 *
 * @param[in] addr An address
 * @param[in] value A byte value
 *
 */
void set_byte(uint8_t *addr, uint8_t v);
#endif
