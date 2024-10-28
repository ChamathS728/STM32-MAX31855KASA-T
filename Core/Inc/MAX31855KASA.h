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
	GPIO_TypeDef* CS_port;
	uint16_t CS_pin;

	// Temperature related readings
	float currentTemperature;

	// Raw sensor reading
	uint32_t rawOutput;			// Full 32 bit read, D
	uint16_t rawTemperature; 	// D[31:18] - 14 bit
	uint16_t rawReference;		// D[15:4] - 12 bit

	uint8_t errorBits;			// D[3:0] - 4 bits | D3 = 0, D2 = SCV fault, D1 = SCG fault, D0 = OC fault

} MAXthermo;


/* Initialisation functions */
MAXthermo* MAXthermo_init(MAXthermo* therm_ptr,
						  SPI_HandleTypeDef* hspi,
	                      GPIO_TypeDef* therm_CS_port,
	                      uint16_t therm_CS_pin);

/* Data Acquisition functions */
HAL_StatusTypeDef MAXthermo_readTemperature(MAXthermo* therm_ptr);
HAL_StatusTypeDef MAXthermo_readTemperatureAndReference(MAXthermo* therm_ptr);
HAL_StatusTypeDef MAXthermo_getTemperature(MAXthermo* therm_ptr);
HAL_StatusTypeDef MAXthermo_getReference(MAXthermo* therm_ptr);


#endif
