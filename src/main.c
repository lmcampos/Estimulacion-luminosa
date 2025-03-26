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
#include "commandqueue.h"
#include "interrupttimer.h"



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
	gpioConfig(GPIO1, GPIO_OUTPUT);
	gpioConfig(GPIO2, GPIO_OUTPUT);
	gpioConfig(GPIO3, GPIO_OUTPUT);
	gpioConfig(GPIO4, GPIO_OUTPUT);
	gpioConfig(GPIO5, GPIO_OUTPUT);
	gpioConfig(GPIO6, GPIO_OUTPUT);
	gpioConfig(GPIO7, GPIO_OUTPUT);

	// configuración del puerto serial para la recepción de datos con interrupción
	serialPortForInterruptInit();
	// creación de las colas y semaforos
	serialPortQueueCreate();
	CommandQueue_Init();
	Semaphore_Init();
	//incializo los temporizadores
	//interrupt_pwmInitForLEDs();
	printf("Estimulacion Luminosa con freeRTOS y sAPI.\n");



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
			configMINIMAL_STACK_SIZE * 4, // Cantidad de stack de la tarea
			0,                          // Parametros de tarea
			tskIDLE_PRIORITY + 3,         // Prioridad de la tarea
			0                         // Puntero a la tarea creada en el sistema
			);
	// Crea la tarea de ejecución (ejecución de comandos y envío de ACK_DONE).
	 xTaskCreate(commandProcessingExecutionTask,
			 (const char *) "CmdExec",
			 configMINIMAL_STACK_SIZE *2 ,
			 0,
			 tskIDLE_PRIORITY + 3,
			 0
			 );

	 printf("LLego a crear las tareas.\n");
	vTaskStartScheduler();
	while (TRUE) {

	}
	return (0);
}


