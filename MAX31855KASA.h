/*
 * MAX31855KASA.h
 *
 *  Created on: Oct 28, 2024
 *      Author: chama
 */

#ifndef MAX31855KASA_SPI_DRIVER_H
#define MAX31855KASA_SPI_DRIVER_H

#include "stm32g4xx_hal.h" // Need this for SPI communications via HAL

typedef struct MAXthermo {
	SPI_HandleTypeDef* hspi;

	// Temperature related readings
	float currentTemperature;

	// Raw sensor reading
	uint32_t rawOutput;			// Full 32 bit read, D
	uint16_t rawTemperature; 	// D[31:18] - 14 bit
	uint16_t rawReference;		// D[15:4] - 12 bit

	uint8_t errorBits;			// D[3:0] - 4 bits | D3 = 0, D2 = SCV fault, D1 = SCG fault, D0 = OC fault

} MAXthermo;


/* Data Acquisition functions */
HAL_StatusTypeDef MAXthermo_readThermocouple(MAXthermo* therm_ptr, UART_HandleTypeDef* huart, GPIO_TypeDef* port, uint16_t pin);
float MAXthermo_getTemperature(MAXthermo* therm_ptr);


#endif
