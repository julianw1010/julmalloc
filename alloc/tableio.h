#ifndef ALLOC_TABLEIO_H
#define ALLOC_TABLEIO_H

#include "alloc/types.h"

/** @brief Sets map entry given a memory address
 *
 * This function takes a memory address, converts it to a map address and sets
 * the corresponding half-byte in the map entry to @p v
 *
 * @param[in] m_addr A valid memory address
 * @param[in] v Value of the corresponding map entry to be set to
 * @return  SUCCESS (0) on success, ERROR (-1) on error
 *
 */
int set_map_value(const uint8_t *m_addr, uint8_t v);

/** @brief Read map entry given a memory address
 *
 * This function takes a memory address, converts it to a map address and ready
 * the corresponding half-byte value in the map table
 *
 * @attention This function will exit on invalid address. Make sure to handle
 * the validity of the address in the calling function.
 *
 * @param[in] m_addr Valid memory address
 * @param[in, out] ok Indicator which is false iff. an error while
 * reading occured.
 * @return Value of map entry corresponding to @p addr
 *
 */
uint8_t read_map_value(const uint8_t *m_addr, bool *ok);

/** @brief Sets a memory byte
 *
 * Given a memory address, this function sets the byte where @p m_addr points to
 * to @p v
 *
 * @param[in] m_addr A valid memory address
 * @param[in] v Byte which the byte where @p addr points to should be set to
 *
 * @return SUCCESS (0) on success, ERROR (-1) else
 */
int set_mem_value(uint8_t *m_addr, uint8_t v);

/**
 * @brief Read byte where an address points to
 *
 * This function reads an uint8_t value of a corresponding address
 *
 * @attention Function will exit on invalid address. Make sure to handle the
 * validity of the address in the calling function.
 * @param[in] m_addr A valid memory address
 * @param[in, out] ok Indicator which is false iff. an error while
 * reading occured.
 * @return The stored uint8_t value
 *
 */
uint8_t read_mem_value(uint8_t *m_addr, bool *ok);

/** @brief Get the gap size, that is free space between two allocated segments
 *
 * Given a memory address, this function converts it into a map address, checks
 * if the map address points to the beginning of a free space, and then
 * counts the number of free map entries until a map entry with ALLOCATED_START
 * (allocated) occurs.
 *
 * @param[in] m_addr Valid memory address
 * @param[in] target To avoid unnecessary calculations, specify a gap size
 * cutoff value on which the function preemtively returns
 * @return A value less or equal than @p target in any case, 0 if addr doesn't
 * point to the beginning of free space
 *
 */
size_t get_gap_size(const uint8_t *m_addr, size_t target);

/** @brief Get the segment size of an allocated space
 *
 * Given a memory address, this function converts it into a map address, checks
 * if the map address points to the beginning of an allocated space, and then
 * counts the number of ALLOCATED_CONSECUTIVE map entries until a map entry with
 * UNALLLOCATED or ALLOCATED_START occurs.
 *
 * @param[in] m_addr Valid memory address pointing to beginning of a segment
 * @return 0 if @p m_addr is invalid address, segment size otherwise
 *
 */
size_t get_segment_size(const uint8_t *m_addr);

/** @brief Checks if address points to allocated segment beginning
 *
 * Given a memory address, this function converts it into a map address p_addr,
 * then checks if:
 * The byte where p_addr points to is set to ALLOCATED_START. If yes, return
 * true, otherwise return false
 *
 * @param[in] addr Valid memory address
 * @param[in,out] ok Indicator variable which is false iff. processing errors
 * occured.
 * @return true if addr points to a beginning of unallocated space, false
 * otherwise
 */
bool is_segment_beginning(const uint8_t *m_addr, bool *ok);

/** @brief Checks if address points to free segment beginning
 *
 * Given a memory address, this function converts it into a map address p_addr,
 * then checks if:
 * a) The byte where p_addr points to is set to UNALLLOCATED. If yes,
 * continue, otherwise return false
 * b) p_addr is equal to g_map_start, that is
 * p_addr points to beginning of map table. If yes, return true
 * c) (p_addr-1) points to a byte which is *not* UNALLLOCATED. If yes, return
 * true, otherwise return false
 *
 * @param[in] m_addr Valid memory address
 * @return true if addr points to a beginning of unallocated space, false
 * otherwise
 */
bool is_gap_beginning(const uint8_t *m_addr);

/** @brief Checks if address is valid map address
 *
 * Checks if p_addr>= g_map_start and p_addr< g_map_end
 *
 * @param[in] p_addr address to check
 * @return true if p_addr is map address, false otherwise
 *
 */
bool is_map_addr(const uint8_t *p_addr);

/** @brief Checks if address is valid mem address
 *
 * Checks if m_addr>= g_mem_start and m_addr< g_mem_end
 *
 * @param[in] m_addr address to check
 * @return true if m_addr is mem address, false otherwise
 */
bool is_mem_addr(const uint8_t *m_addr);

#endif
