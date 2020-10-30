#ifndef ADC_H
#define ADC_H
#include <stdint.h>

/**
 * @brief Initialized a periodic sampling of the external
 * MAX118 ADC; sampling all channels once every 10 ms.
 *
 * Resources used:
 * - Timer/Counter2 (peripheral)
 *
 * @warning The ADC will not be sampled while global
 * interrupts are disabled.
 */
void adc_init();

/**
 * @brief Returns the last sampled value for the ADC
 * channel determined by @p adc_channel.
 *
 * @param channel The channel that a sample is requested for.
 *
 * @return Last sample of channel @p channel.
 */
uint8_t adc_read(uint8_t adc_channel);

#endif
