/*
 * MAX31855KASA.c
 *
 *  Created on: Oct 28, 2024
 *      Author: chama
 */
#include "MAX31855KASA.h"


HAL_StatusTypeDef MAXthermo_readThermocouple(MAXthermo* therm_ptr, UART_HandleTypeDef* huart, GPIO_TypeDef* port, uint16_t pin) {
	/*
		Reads thermocouple IC and stores temperature

		Inputs:
			- therm_ptr: 	Pointer to struct instance
			- huart: 		Pointer to UART Handle. Only used if VERBOSE is defined in this .c file
			- port: 		Pointer to the CS pin port connecting to the IC
			- pin:			Pin number matching to the CS pin

		Output:
			None
	*/

	// Initialise variables
	uint8_t rxBuffer[4] = {2,3,4,5};

#ifdef VERBOSE
	float MSG[100] = {'\0'};
	sprintf(MSG, "Original rxBuffer: %d, %d, %d, %d \n",
			rxBuffer[3], rxBuffer[2], rxBuffer[1], rxBuffer[0]);
	HAL_UART_Transmit(huart, (uint8_t*) MSG, sizeof(MSG), HAL_MAX_DELAY);
#endif

	// Drive CS pin low
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);

	// Read 32 bits
	HAL_StatusTypeDef result = HAL_SPI_Receive(
							therm_ptr->hspi,
							rxBuffer,
							4,
							HAL_MAX_DELAY);

	// Drive CS pin high
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);


	// Store 32 bits into the ptr
	therm_ptr->rawOutput = (rxBuffer[0] << 24) | (rxBuffer[1] << 16) | (rxBuffer[2] << 8) | (rxBuffer[3]);

	// Grab first 14 bits for temperature reading, then divide by 4 to get decimal readings
	therm_ptr->currentTemperature = (therm_ptr->rawOutput >> 18)/4.0;
	therm_ptr->errorBits = rxBuffer[3] & 0x7;

#ifdef VERBOSE
	sprintf(MSG,
			"rawOutput in reverse order: %02X, %02X, %02X, %02X \nCurrent Temperature = %.2f\nError Bits: %d\n",
			rxBuffer[3], rxBuffer[2], rxBuffer[1], rxBuffer[0],
			therm_ptr->currentTemperature,
			therm_ptr->errorBits);
	HAL_UART_Transmit(huart, (uint8_t*) MSG, sizeof(MSG), HAL_MAX_DELAY);
#endif

	return result;
}

float MAXthermo_getTemperature(MAXthermo* therm_ptr) {
	return therm_ptr->currentTemperature;
}



