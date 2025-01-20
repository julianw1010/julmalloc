#ifndef ALLOC_TABLEOPTS_H
#define ALLOC_TABLEOPTS_H

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Initializes storage lookup table consisting of map and mem entries,
 * each half byte of a mem entry corresponding to a map entry
 */
void init_table();

/**
 * @brief Adds a map entry for a given memory address.
 *
 * Adds mem entry for a given memory address by setting the
 * corresponding map entry to ALLOCATED_START and size-1 consecutive entries to
 * ALLOCATED_CONSECUTIVE
 * @param[in] addr A memory address
 * @param[in] size Size of the segment to be allocated in the map table
 * @return      SUCCESS (0) on success, ERROR (-1) on error.
 */
int add_map_entry(const uint8_t *m_addr, size_t size);

/**
 * @brief Removes a map entry for a given memory address
 *
 * Removes a map entry for a given memory address by setting all
 * corresponding map entries to UNALLLOCATED. This function only works if @p
 * m_addr points to a valid beginning of a memory segment, otherwise ERROR will
 * be returned.
 * @param[in] m_addr Memory address to a beginning of a segment
 * @return SUCCESS (0) if @p m_addr pointed to a beginning of an allocated
 * memory segment and its map entries got successfully removed, ERROR (-1) in
 * any other case
 */
int remove_map_entry(const uint8_t *m_addr);

/** @brief Sets memory of allocated space to zero
 *
 * Given a memory address pointing to a beginning of a segment, which also has a
 * valid map entry indicating it is the beginning of the segment, this function
 * sets all memory bytes of the allocated space to zero
 * @param[in] start Memory address of the beginning of an allocated segment
 * @return SUCCESS (0) on success, ERROR (-1) on error (if start doesn't point
 * to a valid beginning of a segment, for example)
 */
int memset_zero(uint8_t *m_addr);

/** @brief Copies memory from one segment to another
 *
 * This function copies @p size bytes beginning from the old memory address @p
 * m_addr_old upwards towards the new memory address @p m_addr_new .
 *
 * @attention map entries in this function are not regarded, so use this
 * function with caution as it might cause undefined behaviour.
 *
 * @param[in] m_addr_old Memory address of the beginning of a segment to be
 * copied from
 * @param[in] m_addr_new Destination Memory address where entries of @p
 * m_addr_old should be copied to
 * @param[in] size The number of bytes to be copied from @p m_addr_old to @p
 * m_addr_new
 *
 * @return SUCCESS (0) on success, ERROR (-1) on error
 *
 */
int copy_mem(uint8_t *m_addr_old, uint8_t *m_addr_new, size_t size);

/** @brief Checks the integrity of the heap used for allocation
 *
 * This function checks that map entries of allocated space begins with
 * ALLOCATED_START only, and returns false if and only if a segment begins with
 * ALLOCATED_CONSECUTIVE
 *
 * @return true if every segment of allocated space begins with ALLOCATED_START,
 * false otherwise.
 */
bool check_heap_integrity();

/** @brief Gets size of used space
 *
 * This function calculates the sum of used space by iterating over the map
 * table and counting ALLOCATED_START and ALLOCATED_CONSECUTIVE entries. If heap
 * integrity is not okay, 0 will be returned.
 *
 * @return The size of allocated space if cheap_heap_integrity is okay, 0
 * otherwise.
 *
 */
size_t get_heap_used_space();

#endif
