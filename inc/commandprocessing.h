/*=====[Module Name]===========================================================
 * Copyright 2020 Luis Mariano Campos <lmarianocampos@gmail.com>
 * All rights reserved.
 * License: license text or at least name and link
         (example: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 1.0.0
 * Creation Date: 2023/02/28
 */

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef _COMMANDPROCESSING_H_
#define _COMMANDPROCESSING_H_

/*=====[Inclusions of public function dependencies]==========================*/
#include "sapi.h"
#include "bibliotecasFREERTOS.h"
//#include "interrupttimer.h"

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif


/*=====[Public function-like macros]=========================================*/


extern QueueHandle_t processingCommandQueue;



/*=====[Prototypes (declarations) of public functions]=======================*/
//task
void commandProcessingQueueCreate(void);
uint16_t commandProcessingConverterCaracterToDecimal(char * pointer, uint8_t length);
//esta tarea realiza el procesamiento relacionado a la interpretación del comando se recibio
void commandProcessingTask(void * taskParmPtr);
void commandProcessingExecutionTask(void * taskParmPtr);
// Función para obtener timestamp (en milisegundos), usando xTaskGetTickCount
uint32_t commandProcessingGetTimestampMs(void);


/*=====[Prototypes (declarations) of public interrupt functions]=============*/



/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* _COMMANDPROCESSING_H_ */
