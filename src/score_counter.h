/**
 * @file score_counter.h
 *
 * @brief Score counter module.
 */
#ifndef SCORE_COUNTER_H
#define SCORE_COUNTER_H
#include <stdint.h>

/**
 * @brief Initialize the Score Counter module.
 *
 * Resources used:
 * - Timer/Counter1 (peripheral)
 *
 * @warning The Score Counter is only capable of
 * tracking time periods of 2 hours, 16 minutes, and
 * 32 seconds. If more time than this elapses between
 * calls to @p score_counter_start and
 * @p score_counter_end, the internal Score Counter
 * value will overflow and yield an aliased score.
 */
void score_counter_init();

/**
 * @brief Begin counting elapsed time as score.
 */
void score_counter_start();

/**
 * @brief Return the score that has accumulated since
 * @p score_counter_start was invoked.
 *
 * @return Score built up since @p score_counter_start
 * was called.
 */
uint16_t score_counter_end();

#endif
