#include "commandqueue.h"
#include <stdio.h>

#include <stdlib.h>

// Se reserva un bloque de memoria estático para la cola: cada ítem es un array de MAX_COMMAND_LENGTH bytes.
static uint8_t queueBuffer[ COMMAND_QUEUE_SIZE * MAX_COMMAND_LENGTH ];
static StaticQueue_t xStaticQueue;

// Variable global privada de la cola. No se expone en el header.
static QueueHandle_t xCommandQueue = NULL;

void CommandQueue_Init(void) {
    xCommandQueue = xQueueCreateStatic(COMMAND_QUEUE_SIZE,
                                        MAX_COMMAND_LENGTH,
                                        queueBuffer,
                                        &xStaticQueue);
    if(xCommandQueue == NULL) {
        printf("Error al crear la cola de comandos\n");
        while(1);
    }
}

bool CommandQueue_Send(const char * commandStr, TickType_t ticksToWait) {
    // La función xQueueSend copiará MAX_COMMAND_LENGTH bytes desde el buffer "commandStr"
    BaseType_t result = xQueueSend(xCommandQueue, commandStr, ticksToWait);
    return (result == pdPASS);
}

bool CommandQueue_Receive(char * commandStr, TickType_t ticksToWait) {
    BaseType_t result = xQueueReceive(xCommandQueue, commandStr, ticksToWait);
    return (result == pdPASS);
}


/*
 * parseStimCommand:
 * Se espera que commandStr tenga el formato:
 * "STIM,<ID>,<DURATION>,<LED_RED_DC>,<LED_GREEN_DC>,<LED3_AZUL_DC>,<LED3_AMBAR_DC>,<LED3_CYAN_DC>"
 * Ejemplo: "STIM,1,50,75,65,80,90,85"
 */
int CommandQueue_parseStimCommand(const char * commandStr, StimCommand * cmd) {
    int ret = sscanf(commandStr, "STIM,%hu,%hu,%hu,%hu,%hu,%hu,%hu",
                     &cmd->id,
                     &cmd->duration,
                     &cmd->led_red_dc,
                     &cmd->led_green_dc,
                     &cmd->led_azul_dc,
                     &cmd->led_ambar_dc,
                     &cmd->led_cyan_dc);
    //printf("sscanf retornó %d\n", ret);
    return (ret == 7) ? 1 : 0;
}
