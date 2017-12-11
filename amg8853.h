/*
 * amg8853.h
 *
 * Created: 2017/10/1 15:28:59
 *  Author: konar
 */

#ifndef AMG8853_H_
#define AMG8853_H_

#include <stdint.h>

#define AMG8853_DELAY           _delay_us(2)
#define AMG8853_TEM_START_ADDR  0x80
#define AMG8853_SENSOR_ADDR     0b1101000
#define AMG8853_WRITE           (AMG8853_SENSOR_ADDR << 1) | 0
#define AMG8853_READ            (AMG8853_SENSOR_ADDR << 1) | 1

// init
void amg8853_init();

// receive 128 bytes data from amg8853 using twi
void amg8853_twi_read_64_tem(uint8_t buff[]);

/*
 * temperature convert
 * buff				in	data from amg8853 128 bytes
 * snumber			in	sensor index 0~63
 * sign				out	sign of the result 
 *                       1:negative, 0:positive 
 * integer_part		out integer part of the result 0~255
 * decimal_part		out decimal part of the result (multiply by 100) 0~99
 */
void amg8853_tem_convert(uint8_t buff[], uint8_t snumber, 
    uint8_t *sign, uint8_t *integer_part, uint8_t *decimal_part);

#endif /* AMG8853_H_ */
