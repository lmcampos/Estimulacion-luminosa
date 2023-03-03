/*=====[Module Name]===========================================================
 * Copyright 2022 Author Campos, Luis Mariano <lmcampos@herrera.unt.edu.ar>
 * All rights reserved.
 * License: license text or at least name and link
         (example: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 1.0.0
 * Creation Date: 2022/11/22
 */

/*=====[Inclusion of own header]=============================================*/

#include "bibliotecasFREERTOS.h"
#include "heartbeat.h"
#include "serialport.h"
#include "commandprocessing.h"


/*=====[Inclusions of private function dependencies]=========================*/

/*=====[Definition macros of private constants]==============================*/

/*=====[Private function-like macros]========================================*/

/*=====[Definitions of private data types]===================================*/

/*=====[Definitions of external public global variables]=====================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/

/*=====[Implementations of public functions]=================================*/

/*=====[Implementations of interrupt functions]==============================*/


/*=====[Implementations of private functions]================================*/
int main(void) {
	boardConfig();
	gpioConfig(GPIO0, GPIO_OUTPUT);
	serialPortForInterruptInit();
	// creación de las colas
	serialPortQueueCreate();
	commandProcessingQueueCreate();
	printf("Estimulacion Luminosa con freeRTOS y sAPI.\n");
	//printf( "Imgrese un caracter:\n" );

	//uartWriteByte(UART_USB,"\n**Seleccione una opcion**");
	//uartWriteByte(UART_USB, "\n ROJO(R0) AMARILLO(A1) VERDE(V2) AZUL(A3) VIOLETA(VI4) CELESTE(C5):");

	//printf("\n ingrese el factor ejemplo: f100");


	xTaskCreate(heartBeat,                     // Funcion de la tarea a ejecutar
			(const char *) "heartbeat", // Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE * 2, // Cantidad de stack de la tarea
			0,                          // Parametros de tarea
			tskIDLE_PRIORITY + 1,         // Prioridad de la tarea
			0                         // Puntero a la tarea creada en el sistema
			);

	xTaskCreate(serialPortTaskStorageCommand,  // Funcion de la tarea a ejecutar
			(const char *) "Tarea que almacena el comando", // Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE * 2, // Cantidad de stack de la tarea
			0,                          // Parametros de tarea
			tskIDLE_PRIORITY + 3,         // Prioridad de la tarea
			0                         // Puntero a la tarea creada en el sistema
			);

	xTaskCreate(commandProcessingTask,         // Funcion de la tarea a ejecutar
			(const char *) "Tarea que procesa el comando recibido", // Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE * 2, // Cantidad de stack de la tarea
			0,                          // Parametros de tarea
			tskIDLE_PRIORITY + 3,         // Prioridad de la tarea
			0                         // Puntero a la tarea creada en el sistema
			);

	vTaskStartScheduler();
	while (TRUE) {

	}
	return (0);
}


