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
/** @file 	commandprocessing.c.c
 ** @brief 	Funciones relacionadas a la interpretación de los comandos recebidos por parte de los usuarios
 **
 **| REV | YYYY.MM.DD | Autor               | Descripción de los cambios                              |
 **|-----|------------|---------------------|---------------------------------------------------------|
 **|   1 | 2023.02.28 | luis Mariano Campos | Version inicial 									      |
 **|     |            |                     | 				 									      |
 ** @addtogroup aplicacion principal
 ** @{ */

/*==================[inlcusiones]============================================*/
#include "commandprocessing.h"
#include "interrupttimer.h"
#include "led.h"

//struct led * pled = NULL;

gpioMap_t gpio[] = { LED1, LED2, LED3, GPIO3, GPIO4, GPIO5};
                   //ROJO(R0) AMARILLO(A1) VERDE(V2) AZUL(A3) VIOLETA(VI4) CELESTE(C5):
void commandProcessingQueueCreate(void) {
	processingComandQueue = xQueueCreate(SIZECOMMANDQUEUE, sizeof(char*));

	if (processingComandQueue == NULL) {/*Si devolvio NULL es muy probable que no haya suficiente memoria para crear la cola*/
		printf("Error al crear la cola del procesamiento de comandos\n");
		//gpioWrite(LED1, ON);
		while (1)
			;/*Se queda bloquedo el sistema hasta que venga el personal de mantenimiento*/
	}
}

void commandProcessingTask(void * taskParmPtr) {

	char *pCommandToProcess = NULL;
	uint16_t factor;
	gpioMap_t cLed;
	uint8_t j;

	while (TRUE) {
		if (xQueueReceive(processingComandQueue, &pCommandToProcess,
		portMAX_DELAY) == pdTRUE) {
			j = *(pCommandToProcess + 1) - '0';
			switch (*(pCommandToProcess)) {
			case 'r':
				if (*(pCommandToProcess + 1) == '0') {
					// debo asignar el gpio cnectado al led rojo
					cLed = gpio[j];
				} else {
					// comando invalido
				}
				break;
			case 'a':
				if (*(pCommandToProcess + 1) == '1') {
					// debo asignar el gpio cnectado al led amarillo
					cLed = gpio[j];
				} else {
					// comando invalido
				}
				break;
			case 'v':
				if (*(pCommandToProcess + 1) == '2') {
					// debo asignar el gpio cnectado al led verde
					cLed = gpio[j];
				} else {
					// comando invalido
				}
				break;
			case 'A':
				if (*(pCommandToProcess + 1) == '3') {
					// debo asignar el gpio cnectado al led azul
					cLed = gpio[j];

				} else {
					// comando invalido
				}
				break;
			case 'V':
				if (*(pCommandToProcess + 1) == '4') {
					// debo asignar el gpio cnectado al led violeta
					cLed = gpio[j];
				} else {
					// comando invalido
				}
				break;

			case 'c':

				if (*(pCommandToProcess + 1) == '5') {
					// debo asignar el gpio cnectado al celeste
					cLed = gpio[j];
				} else {
					// comando invalido
				}
				break;
			case 'f':
				factor = commandProcessingConverterCaracterToDecimal((pCommandToProcess+1), 4);
				//uartWriteString(UART_USB, "\n El factor es:");
			    printf("\n el factor es:%d",factor);
			    ledAddNodeEnd(&pLed, cLed, factor, 0);
			    cLed=110;// poner un valor invalido porque queda grabado para el proxímo ingreso de datos
			    break;
			case 'b':
				ledDeleteNodeInit(&pLed);
				break;
			case 'i':
				interruptTimerInit();
				break;
			case 'd':
				interruptTimerDiseable();
				break;
			}

		}
	}

}

uint16_t commandProcessingConverterCaracterToDecimal(char * pointer, uint8_t length) {
			uint32_t num=0;
		    uint32_t digito = 0;
		    uint32_t tam = length;
		    uint32_t expo = 0;
		    uint32_t pot=1;
		    uint32_t i;

		    for(i=tam ;i>0;--i)
		    {
		        digito=(uint32_t) (*(pointer+(i-1))-48);

		        expo = tam-i;
		        pot=1;
		        while( expo > 0)//para sacar la potencia
		        {
		                pot  = pot*10  ;
		                expo = expo-1 ;
		        }
		    num = num+digito*pot;
		    }
		    return num;
	}
