#ifndef COMMANDQUEUE_H_
#define COMMANDQUEUE_H_

/*=====[Inclusions of public function dependencies]==========================*/
#include "sapi.h"
#include "bibliotecasFREERTOS.h"
#include <stdint.h>
#include <stdbool.h>

// Tamaño de la cola y tamaño máximo de cada comando
#define COMMAND_QUEUE_SIZE 4

/*=====[Definition macros of public constants]===============================*/

#define COMMAND_QUEUE_SIZE 4   // Tamaño de la cola
#define MAX_COMMAND_LENGTH 50  // Tamaño máximo de cada comando

/*=====[Definitions of public data types]====================================*/
// Estructura que representa un comando de estimulación.
// Todos los campos se expresan como uint16_t (por ejemplo, duración y duty cycles en unidades enteras).
typedef struct {
    uint16_t id;
    uint16_t duration;
    uint16_t led_red_dc;
    uint16_t led_green_dc;
    uint16_t led_azul_dc;
    uint16_t led_ambar_dc;
    uint16_t led_cyan_dc;
} StimCommand;

/* Interfaz pública de la cola:
   - CommandQueue_Init: inicializa la cola de comandos.
   - CommandQueue_Send: envía un comando (cadena completa) a la cola.
   - CommandQueue_Receive: recibe un comando (cadena completa) de la cola.
   - parseStimCommand: parsea la cadena y llena una estructura StimCommand.
*/
void CommandQueue_Init(void);
bool CommandQueue_Send(const char * cmd, TickType_t ticksToWait);
bool CommandQueue_Receive(char *cmd, TickType_t ticksToWait);
/* Prototipo para parsear una cadena de comando y llenar una estructura StimCommand.
   Se espera el formato:
   "STIM,<ID>,<DURATION>,<LED_RED_DC>,<LED_GREEN_DC>,<LED3_AZUL_DC>,<LED3_AMBAR_DC>,<LED3_CYAN_DC>"
   Ejemplo: "STIM,1,50,75,65,80,90,85"
*/
int CommandQueue_parseStimCommand(const char * commandStr, StimCommand * cmd);

#endif /* COMMANDQUEUE_H_ */
