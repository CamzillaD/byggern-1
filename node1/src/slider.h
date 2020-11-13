/**
 * @file slider.h
 *
 * @brief Slider module.
 */
#ifndef SLIDER_H
#define SLIDER_H
#include <stdint.h>

/**
 * @brief Read the positions of the two on board sliders
 * and update the values pointed to by @p p_left and
 * @p p_right accordingly. 0 is taken to mean all the way
 * to the left, while 255 (0xff) is taken to mean all the
 * way to the right.
 * 
 * @warning The ADC module must be enabled before this
 * function may be used.
 */
void slider_read(uint8_t * p_left, uint8_t * p_right);

#endif