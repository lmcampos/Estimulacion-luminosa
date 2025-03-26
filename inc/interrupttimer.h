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
#include "bibliotecasFREERTOS.h"
#include "commandqueue.h"

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/
//#define NOVENTAPORCIENTO 450000  //

#define VEINTICINCOPORCIENTO   250 		//0.25mseg
#define CINCUENTAPORCIENTO     500 		//0.5mseg
#define SETENTAYCINCOPORCIENTO 750		//0.75 mseg
#define COMPLETECYCLE_PERIODO  1000 	//1 mseg
#define NOVENTAYNUEVEPORCIENTO 999      //????
#define NOVENTAPORCIENTO       900
#define NOVENTAYCINCOPORCIENTO 950
#define NOVENTAYOCHOPORCIENTO  980
#define UNOPORCIENTO            1

#define INTERRUPCION 100 //para llegar

#define LED_RED   GPIO6
#define LED_AMBAR GPIO7
#define LED_BLUE  GPIO5
#define LED_GREEN GPIO4
#define LED_CYAN  GPIO3
#define LED_GPIO


/*=====[Public function-like macros]=========================================*/
/*=====[Definitions of public data types]====================================*/
//char colores[][10] = {"ROJO", "AMARILLO", "VERDE", "AZUL", "VIOLETA"," CELESTE"};
//gpioMap_t gpio[] = { GPIO0, GPIO1, GPIO2, GPIO3, GPIO4, GPIO5};

// Declaración del semáforo que indica la finalización del estímulo.
extern SemaphoreHandle_t xStimDoneSemaphore;

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
//TIMER 2
void timer2Periodo(void* ptr);
void timer2CompareMatch1(void *ptr);
void timer2CompareMatch2(void *ptr);
//TIMER3
void interrupt_Timer3Init(uint16_t);
void interrupt_timer3Periodo(void *ptr);

void Semaphore_Init(void);
void interrupt_updateDutyCycleForLEDs(StimCommand cmd);
void interrupt_pwmInitForLEDs(void);



/*=====[Prototypes (declarations) of public interrupt functions]=============*/



/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* _INTERRUPTTIMER_H_ */
