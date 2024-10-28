/*
 * MAX31855KASA.c
 *
 *  Created on: Oct 28, 2024
 *      Author: chama
 */
#include "MAX31855KASA.h"

MAXthermo* MAXthermo_init(MAXthermo* therm_ptr,
						  SPI_HandleTypeDef* hspi,
	                      GPIO_TypeDef* therm_CS_port,
	                      uint16_t therm_CS_pin) {
	therm_ptr->hspi = hspi;
	therm_ptr->CS_port = therm_CS_port;
	therm_ptr->CS_pin = therm_CS_pin;

	therm_ptr->currentTemperature = 0.0;	// Measured in Celsius
	therm_ptr->rawOutput = 0;
	therm_ptr->rawReference = 0;
	therm_ptr->rawTemperature = 0;

	therm_ptr->errorBits = 0;

	return therm_ptr;
}

HAL_StatusTypeDef MAXthermo_readTemperature(MAXthermo* therm_ptr) {

	// Initialise variables
	uint8_t rxBuffer[4];


	// Drive CS pin low
	HAL_GPIO_WritePin(therm_ptr->CS_port, therm_ptr->CS_pin, GPIO_PIN_RESET);

	// Read 32 bits
	HAL_StatusTypeDef result = HAL_SPI_Receive(therm_ptr->hspi, rxBuffer, 4, HAL_MAX_DELAY);

	// Store 32 bits into the ptr
//	therm_ptr->rawOutput = rxBuffer;
	therm_ptr->rawOutput = (rxBuffer[0] << 24) | (rxBuffer[1]<< 16) | (rxBuffer[2] << 8) | (rxBuffer[3]);

	// Grab first 14 bits for temperature reading, then divide by 4 to get decimal readings
	therm_ptr->currentTemperature = (therm_ptr->rawOutput >> 18)/4.0;

	return result;
}






