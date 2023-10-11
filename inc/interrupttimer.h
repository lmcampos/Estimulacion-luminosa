/*=====[Module Name]===========================================================
 * Copyright 2020 Luis Mariano Campos <lmarianocampos@gmail.com>
 * All rights reserved.
 * License: license text or at least name and link
         (example: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Version: 1.0.0
 * Creation Date: 2023/03/03
 */

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef _INTERRUPTTIMER_H_
#define _INTERRUPTTIMER_H_

/*=====[Inclusions of public function dependencies]==========================*/
#include "sapi.h"
#include "sapi_timer.h"


/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/
#define NOVENTAPORCIENTO 450000  //

#define VEINTICINCOPORCIENTO   250 		//0.25mseg
#define CINCUENTAPORCIENTO     500 		//0.5mseg
#define SETENTAYCINCOPORCIENTO 750		//0.75 mseg
#define COMPLETECYCLE_PERIODO  1000 	//1 mseg

#define INTERRUPCION 100 //para llegar



/*=====[Public function-like macros]=========================================*/
/*=====[Definitions of public data types]====================================*/
//char colores[][10] = {"ROJO", "AMARILLO", "VERDE", "AZUL", "VIOLETA"," CELESTE"};
//gpioMap_t gpio[] = { GPIO0, GPIO1, GPIO2, GPIO3, GPIO4, GPIO5};


/*=====[Prototypes (declarations) of public functions]=======================*/
void interruptTimerInit(void);
void interruptTimerDiseable(void);
//TIMER0
void timer0Periodo(void* ptr);
//void timer0CompareMatch0(void *ptr);
void timer0CompareMatch1(void *ptr);
void timer0CompareMatch2(void *ptr);
void timer0CompareMatch3(void *ptr);
//TIMER1
void timer1Periodo(void* ptr);
void timer1CompareMatch1(void *ptr);
void timer1CompareMatch2(void *ptr);
void timer1CompareMatch3(void *ptr);


/*=====[Prototypes (declarations) of public interrupt functions]=============*/



/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* _INTERRUPTTIMER_H_ */
