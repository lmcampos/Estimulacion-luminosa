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
#include "commandqueue.h"
#include "led.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Doble buffer para almacenar dos comandos parsedos
#define DOUBLE_BUFFER_SIZE 2

static StimCommand doubleBuffer[DOUBLE_BUFFER_SIZE];
// Índice que indica el comando actualmente en ejecución (0 o 1)
static uint8_t currentBufferIndex = 0;

// Semáforos para sincronizar el doble buffering
SemaphoreHandle_t xNextCommandReady = NULL;   // Se da cuando se parsea y almacena el siguiente comando
SemaphoreHandle_t xExecutionDone = NULL;        // Se da cuando termina la ejecución del comando actual


// Función para obtener timestamp (en milisegundos), usando xTaskGetTickCount
uint32_t commandProcessingGetTimestampMs(void) {
    return xTaskGetTickCount() * portTICK_PERIOD_MS;
}


// Tarea que procesa comandos: recibe la cadena, la parsea y la almacena en el doble buffer.
void commandProcessingTask(void * taskParmPtr) {

	char commandToParse[MAX_COMMAND_LENGTH];
	StimCommand stimCmd;
	int nextIndex;
	static bool initialHandshakeDone = false;

	// Crear semáforos
	xNextCommandReady = xSemaphoreCreateBinary();
	xExecutionDone = xSemaphoreCreateBinary();
	if (xNextCommandReady == NULL || xExecutionDone == NULL) {
		printf("Error al crear semáforos\n");
		while (1)
			;
	}

	while (1) {
		while (1) {
		        if (CommandQueue_Receive(commandToParse, portMAX_DELAY)) {
		            printf("Comando recibido para parseo: [%s]\n", commandToParse);

		            // Para el primer comando, si aún no se ha hecho el handshake inicial,
		            // se almacena directamente en el slot actual y se envía READY2.
		            if (!initialHandshakeDone) {
		                if (CommandQueue_parseStimCommand(commandToParse, &stimCmd)) {
		                    doubleBuffer[currentBufferIndex] = stimCmd;
		                    uartWriteString(UART_232, "READY2\n");
		                    initialHandshakeDone = true;
		                    // Notifica que el comando está listo para ejecutarse.
		                    xSemaphoreGive(xNextCommandReady);
		                } else {
		                    printf("Error al parsear el comando: %s\n", commandToParse);
		                }
		            } else {
		                // Para comandos subsiguientes, se almacena en el slot opuesto.
		                nextIndex = currentBufferIndex ^ 1;  // Alterna entre 0 y 1.
		                if (CommandQueue_parseStimCommand(commandToParse, &stimCmd)) {
		                    doubleBuffer[nextIndex] = stimCmd;
		                    // Espera a que la tarea de ejecución termine la ejecución del comando actual.
		                    xSemaphoreTake(xExecutionDone, portMAX_DELAY);
		                    // Ahora cambia el índice para que el nuevo comando se convierta en el activo.
		                    currentBufferIndex ^= 1;
		                    // Notifica a la tarea de ejecución que ya hay un nuevo comando listo.
		                    xSemaphoreGive(xNextCommandReady);
		                } else {
		                    printf("Error al parsear el comando: %s\n", commandToParse);
		                }
		            }
		        }
		    }
		}
}

// Tarea de ejecución: toma el comando actual del doble buffer y lo ejecuta, enviando ACK_DONE al finalizar.
void commandProcessingExecutionTask(void * taskParmPtr) {
	char ackMessage[50];
	StimCommand execCmd;
	uint32_t timestampInicio, timestampACK, latency;
	// Crear el semáforo si aún no existe.

	while (1) {
		// Espera a que se notifique que el siguiente comando ya está listo.
		if (xSemaphoreTake(xNextCommandReady, portMAX_DELAY) == pdTRUE) {
			// Toma el comando actual del doble buffer.
			execCmd = doubleBuffer[currentBufferIndex];
			/*printf("Ejecutando comando: ID=%hu, Duration=%hu, LED Red=%hu, LED Green=%hu, LED Azul=%hu, LED Ambar=%hu, LED Cyan=%hu\n",
			 execCmd.id,
			 execCmd.duration,
			 execCmd.led_red_dc,
			 execCmd.led_green_dc,
			 execCmd.led3_azul_dc,
			 execCmd.led3_ambar_dc,
			 execCmd.led3_cyan_dc);
			 */
			// Registra el timestamp de inicio de ejecución
			timestampInicio = commandProcessingGetTimestampMs();
			// Ejecuta la estimulación. Se simula con un delay.
			//vTaskDelay(pdMS_TO_TICKS(execCmd.duration));

			// Actualiza los PWM según el comando recibido.
			//interrupt_updateDutyCycleForLEDs(execCmd);
			// Configura los PWM según el comando.
			//setPWMForCommand(execCmd);

			// Inicia el temporizador hardware en modo one-shot para la duración de la estimulación.
			interrupt_Timer3Init(execCmd.duration);
			//startHardwareDurationTimer(execCmd.duration);
			//printf("Ejecutando comando: ID=%hu, Duration=%hu\n", execCmd.id,
			//		execCmd.duration);

			// Espera a que la ISR (del timer hardware) libere el semáforo
			if (xSemaphoreTake(xStimDoneSemaphore, portMAX_DELAY) == pdTRUE) {
				timestampACK = commandProcessingGetTimestampMs();
				latency = timestampACK - timestampInicio;
				sprintf(ackMessage, "ACK_DONE,%hu,%lu\n", execCmd.id, latency);
				uartWriteString(UART_232, ackMessage);
				printf("%s enviado - Comando ejecutado\n", ackMessage);
				xSemaphoreGive(xExecutionDone);
			}
		}
	}
}

