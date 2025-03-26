/* Copyright 2017-2018, Eric Pernia
 * All rights reserved.
 *
 * This file is part of sAPI Library.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
/** @file 	portserie.c
 ** @brief 	Configurción del puerto serie USB y rs232
 **
 **| REV | YYYY.MM.DD | Autor               | Descripción de los cambios                              |
 **|-----|------------|---------------------|---------------------------------------------------------|
 **|   1 | 2022.11.22 | luis Mariano Campos | Version inicial 									      |
 **|     |            |                     | 				 									      |
 ** @addtogroup aplicacion principal
 ** @{ */
#include "serialport.h"
#include "bibliotecasFREERTOS.h"
#include "commandprocessing.h"
#include "commandqueue.h"


static QueueHandle_t receiveQueue;


void serialPortForInterruptInit(void){
			/* Inicializar la UART_USB  */
			uartConfig(UART_232, 115200);
			// Seteo un callback al evento de recepcion y habilito su interrupcion
			uartCallbackSet(UART_232, UART_RECEIVE, serialPortReceiveData, NULL);
			// Habilito todas las interrupciones de UART_232
			uartInterrupt(UART_232, TRUE);
			/* Inicializar la UART_USB  */
			uartConfig(UART_USB, 115200);
	}

void serialPortQueueCreate(void){
 receiveQueue = xQueueCreate(SIZERECEIVEQUEUE,sizeof(char));

 if(receiveQueue == NULL){/*Si devolvio NULL es muy probable que no haya suficiente memoria para crear la cola*/
	 printf("Error en la creación de la cola de recepción de comandos\n");
	 //gpioWrite(LED1, ON); // LED ROJO
	 while(1);/*Se queda bloquedo el sistema hasta que venga el personal de mantenimiento*/
 }
}
//interrupción de recepción de caracteres por el puerto Serial USB
void serialPortReceiveData(void *noUsado){
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	  char data;

	  data = uartRxRead(UART_232);
	  uartWriteByte(UART_USB, data);
	  //uartWriteString(UART_USB, "\n");
	  xQueueSendFromISR(receiveQueue, &data, &xHigherPriorityTaskWoken);
	  if (xHigherPriorityTaskWoken == pdTRUE) {
	  		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	  	}

}

void serialPortTaskStorageCommand(void * taskParmPtr) {

	char command[MAX_COMMAND_LENGTH];
	uint8_t index = 0;
	char data;

	while (TRUE) {
		if (xQueueReceive(receiveQueue, &data, portMAX_DELAY) == pdPASS) {
			command[index] = data;

			//*(pCommand+index) = data;
			if (command[index] == '\n') {
				command[index] = '\0';

				if (CommandQueue_Send(command, portMAX_DELAY)) {
					//printf("Comando encolado desde serialport.c: [%s]\n",
					//		command);
				} else {
					printf("Error al encolar comando.\n");
				}

				index = 0;
			} else {
				index++;
				if (index >= MAX_COMMAND_LENGTH - 1) { // Evitar desbordamiento
					index = 0;
				}
			}
		}
	}
}

