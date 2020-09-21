#ifndef ADC_H
#define ADC_H
#include <stdint.h>

typedef enum {
    ADC_CHANNEL_0,
    ADC_CHANNEL_1,
    ADC_CHANNEL_2,
    ADC_CHANNEL_3
} ADCChannel;

/**
 * @brief Initializes the ATmega to sample the external ADC
 * with a 10 ms period.
 *
 * Resources used:
 * - Timer/Counter0 is used to generate an external clock signal
 * for the ADC module.
 * - Timer/Counter2 is used to generate interrupts that each
 * sample the ADC every 10 ms.
 * - PORT B pin 0 for ADC clock.
 * - PORT B pin 1 for ADC ~busy signal.
 *
 * @warning Global interrupts must be enabled outside of
 * this function.
 *
 * @warning If @c adc_read is called before the first sample
 * interrupt has been processed, the returned value will
 * contain invalid data.
 */
void adc_init();

/**
 * @brief Returns the last sampled value for the ADC
 * channel determined by @p channel.
 *
 * @param channel The channel that a sample is requested for.
 *
 * @return Last sample of channel @p channel.
 *
 * @warning If this function is invoked before the first
 * ADC sampling interrupt has been processed, the returned
 * value will contain invalid data.
 */
uint8_t adc_read(ADCChannel channel);

#endif
