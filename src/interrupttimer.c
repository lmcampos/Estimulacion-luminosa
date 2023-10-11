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
 **|   1 | 2023.03.03 | luis Mariano Campos | Version inicial 									      |
 **|     |            |                     | 				 									      |
 ** @addtogroup aplicacion principal
 ** @{ */

/*==================[inlcusiones]============================================*/
#include "interrupttimer.h"
#include "led.h"

static struct led * pAux = NULL;
//char miTextoLis[]="\n la lista se encuentra vacía...";

void interruptTimerInit(void) {
//********* Configuración para la interrupnción del timer*******************
	// esta función inicializa la función que se disparará al completar el periodo de la señal PWM
	//TIMER 0
	Timer_Init(TIMER0, Timer_microsecondsToTicks(COMPLETECYCLE_PERIODO),
		timer0Periodo);
	//esta función habilita el modo compare
	Timer_EnableCompareMatch(TIMER0, TIMERCOMPAREMATCH1,
			Timer_microsecondsToTicks(NOVENTAPORCIENTO), timer0CompareMatch1);

/*
	Timer_EnableCompareMatch(TIMER0, TIMERCOMPAREMATCH2,
				Timer_microsecondsToTicks(VEINTICINCOPORCIENTO), timer0CompareMatch2);
	Timer_EnableCompareMatch(TIMER0, TIMERCOMPAREMATCH3,
					Timer_microsecondsToTicks(CINCUENTAPORCIENTO), timer0CompareMatch3);
	Timer_EnableCompareMatch(TIMER0, TIMERCOMPAREMATCH3,
					Timer_microsecondsToTicks(NOVENTAPORCIENTO), timer0CompareMatch3);*/
	//TIMER1
	Timer_Init(TIMER1, Timer_microsecondsToTicks(COMPLETECYCLE_PERIODO),
				timer1Periodo);

	Timer_EnableCompareMatch(TIMER1, TIMERCOMPAREMATCH1,
				Timer_microsecondsToTicks(VEINTICINCOPORCIENTO), timer1CompareMatch1);

	/*
	Timer_EnableCompareMatch(TIMER1, TIMERCOMPAREMATCH2,
					Timer_microsecondsToTicks(CINCUENTAPORCIENTO), timer1CompareMatch2);

	Timer_EnableCompareMatch(TIMER1, TIMERCOMPAREMATCH3,
					Timer_microsecondsToTicks(SETENTAYCINCOPORCIENTO), timer1CompareMatch3);*/

}
void interruptTimerDiseable(void) {
	Timer_DisableCompareMatch(TIMER0, TIMERCOMPAREMATCH1);
	Timer_DeInit(TIMER0);
}


void timer0Periodo(void* ptr) {
	/*======== Interrupción de periodica de COMPLETECYCLE_PERIODO============*/
	/*======== Con esta interrupciòn generamos una señal PWM con periodo de 1[seg]=========*/
	static uint8_t i = 0;

		if (i == 0) {
			pAux = pLed; //Error---> cada vez que ingresa a la interrupción me asigna nuevamente la direcciòn del primer nodo
			i = 1;
		}
		if (pAux != NULL) {
		gpioWrite(pAux->colorLed, ON);

		pAux->dutyCycle = pAux->dutyCycle + pAux->factor;
		if (pAux->dutyCycle <= COMPLETECYCLE_PERIODO) {
			Timer_SetCompareMatch(TIMER0, TIMERCOMPAREMATCH1,
					Timer_microsecondsToTicks(pAux->dutyCycle));
		} else
			Timer_DisableCompareMatch(TIMER0, TIMERCOMPAREMATCH1);

		if (pAux->dutyCycle > COMPLETECYCLE_PERIODO) {
			gpioWrite(pAux->colorLed, OFF);
			pAux->dutyCycle = pAux->factor;
			Timer_EnableCompareMatch(TIMER0, TIMERCOMPAREMATCH1,
					Timer_microsecondsToTicks(pAux->dutyCycle),
					timer0CompareMatch1);

			if (pAux != NULL) {
				pAux = pAux->sig;
			}
			if (pAux == NULL) {
				pAux = pLed;
			}

			gpioWrite(pAux->colorLed, ON);

		}
	} //else uartWriteString(UART_USB,miTextoLis);

}
void timer0CompareMatch1(void *ptr) {

	gpioWrite(pAux->colorLed, OFF);
	//gpioToggle( GPIO3 );

}

void timer0CompareMatch2(void *ptr){
	gpioToggle( GPIO1 );

}

void timer0CompareMatch3(void *ptr){

	gpioToggle( GPIO2 );
}
void timer1Periodo(void* ptr)
{
	//gpioToggle( GPIO3 );
	gpioWrite(GPIO3, ON);
	gpioWrite(GPIO5, ON);

}
void timer1CompareMatch1(void *ptr)
{
	gpioWrite(GPIO3, OFF);
}
void timer1CompareMatch2(void *ptr)
{
	gpioWrite(GPIO5, OFF);
}
void timer1CompareMatch3(void *ptr)
{
	gpioToggle( GPIO6 );
}
/*
}
void timer0CompareMatch3(void *ptr){
	gpioToggle( GPIO3 );

}*/
