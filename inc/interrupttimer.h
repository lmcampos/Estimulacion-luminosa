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
#define COMPLETECYCLE_PERIODO_MAS  1010
#define NOVENTAYNUEVEPORCIENTO 999      //????
#define NOVENTAPORCIENTO       900
#define NOVENTAYCINCOPORCIENTO 950
#define NOVENTAYOCHOPORCIENTO  980
#define UNOPORCIENTO            1

#define INTERRUPCION 100 //para llegar

// Asociación de los LEDs con los GPIOs
#define PIN_TRIGGER            GPIO0
#define LED_INFRARROJO_STIM_1  GPIO1 // para el led infrarrojo
#define LED_INFRARROJO_STIM_2  GPIO2 // para el led infrarrojo
#define LED_CYAN_STIM          GPIO3
#define LED_GREEN_STIM         GPIO4
#define LED_BLUE_STIM          GPIO5
#define LED_RED_STIM           GPIO6
#define LED_AMBAR_STIM         GPIO7
#define LED_INFRARROJO_STIM_3  GPIO8 // para el led infrarrojo


/*=====[Public function-like macros]=========================================*/
/*=====[Definitions of public data types]====================================*/

// Variables globales para el modo estímulo:
extern volatile bool stimActive;
extern volatile bool firstCycle;
extern volatile uint32_t red_ticks;
extern volatile uint32_t gree_ticks;
extern volatile uint32_t azul_ticks;
extern volatile uint32_t ambar_ticks;
extern volatile uint32_t cyan_ticks;

  // Define el pin de aviso
// Declaración del semáforo que indica la finalización del estímulo.
extern SemaphoreHandle_t xStimDoneSemaphore;

/*=====[Prototypes (declarations) of public functions]=======================*/
void interruptTimerInit(void);
void interruptTimerDiseable(void);


void interrupt_Timer3Init(uint16_t);

void Semaphore_Init(void);
void interrupt_updateDutyCycleForLEDs(StimCommand cmd);
void interrupt_pwmInitForLEDs(void);



/*=====[Prototypes (declarations) of public interrupt functions]=============*/

//TIMER1
void interrupt_timer1Periodo(void* ptr);
void interrupt_timer1CompareMatch1(void *ptr);
void interrupt_timer1CompareMatch2(void *ptr);
void interrupt_timer1CompareMatch3(void *ptr);
//TIMER 2
void interrupt_timer2Periodo(void* ptr);
void interrupt_timer2CompareMatch1(void *ptr);
void interrupt_timer2CompareMatch2(void *ptr);
//TIMER3
void interrupt_timer3Periodo(void *ptr);
/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* _INTERRUPTTIMER_H_ */
