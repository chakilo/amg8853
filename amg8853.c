/*
 * amg8853.c
 *
 * Created: 2017/10/1 15:30:44
 *  Author: konar
 */ 
 
 #include "amg8853.h"

 #include "twi.h"

 uint8_t *target_buff;

 // init
 void amg8853_init() {
	twi_init();
 }

 // read from twi buffer
 void amg8853_read_callback(uint8_t address, uint8_t *data) {
	 uint8_t i;
	 for (i=0; i<128; i++) {
		 target_buff[i] = data[i];
	 }
 }

 // receive 128 bytes data from amg8853 using twi
 void amg8853_twi_read_64_tem(uint8_t buff[]) {
	target_buff = buff;
	uint8_t data = AMG8853_TEM_START_ADDR;
	twi_write(AMG8853_SENSOR_ADDR, &data, 1, NULL);
	twi_wait();
	twi_read(AMG8853_SENSOR_ADDR, 128, amg8853_read_callback);
	twi_wait();
 }

/* 
 * temperature convert
 * buff				in	data from amg8853 128 bytes
 * snumber			in	sensor index 0~63
 * sign				out	sign of the result 1:negative, 0:positive
 * integer_part		out integer part of the result 0~255
 * decimal_part		out decimal part of the result (multiply by 100) 0~99
 */
 void amg8853_tem_convert(uint8_t buff[], uint8_t snumber, uint8_t *sign, uint8_t *integer_part, uint8_t *decimal_part) {

	// read data
	uint8_t lowByte = buff[snumber << 1];
	uint8_t highByte = buff[(snumber << 1) + 1];

	int16_t rst;

	// charge sign
	if (highByte & 0x08) {
		// negative
		*sign = 1;
		// negative, two's complement, set bit for high-order bits
		rst = (((((uint16_t)(highByte | 0xf0)) << 8) & 0xff00) | (((uint16_t)(lowByte)) & 0x00ff));
		// to positive number
		rst = -rst;
	} else {
		// positive
		*sign = 0;
		rst = (((((uint16_t)(highByte)) << 8) & 0xff00) | (((uint16_t)(lowByte)) & 0x00ff));
	}

	// decimal part
	switch (rst & 0x03) {
		case 0x00:
			*decimal_part = 0;
			break;

		case 0x01:
			*decimal_part = 25;
			break;

		case 0x02:
			*decimal_part = 50;
			break;

		case 0x03:
			*decimal_part = 75;
			break;

		default:
			break;
	}

	// integer part
	*integer_part = (uint8_t)((rst >> 2) & 0x00ff);

 }
