/*=====[Module Name]===========================================================
 * Copyright 2020 Luis Mariano Campos <lmarianocampos@gmail.com>
 * All rights reserved.
 * License: license text or at least name and link
         (example: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 1.0.0
 * Creation Date: 2022/11/22
 */

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef _SERIALPORT_H_
#define _SERIALPORT_H_

/*=====[Inclusions of public function dependencies]==========================*/
#include "sapi.h"


/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/
#define SIZERECEIVEQUEUE 50
/*=====[Public function-like macros]=========================================*/
/*=====[Definitions of public data types]====================================*/


/*=====[Prototypes (declarations) of public functions]=======================*/
void serialPortForInterruptInit(void);
void serialPortReceiveData(void *noUsado);
void serialPortTaskStorageCommand(void * taskParmPtr);
void serialPortQueueCreate(void);

/*=====[Prototypes (declarations) of public interrupt functions]=============*/



/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* _SERIALPORT_H_ */
