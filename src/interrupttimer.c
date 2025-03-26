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
/** @file 	interrupttimer.c
 ** @brief 	Implementacioón de los servicios de interrupción correspondiente a los Timer y sus canales
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

// Definición del semáforo: se crea aquí en la inicialización del módulo.
SemaphoreHandle_t xStimDoneSemaphore = NULL;
void Semaphore_Init(void){
	// Crea el semáforo que usará la ISR para notificar el fin del estímulo.
	    xStimDoneSemaphore = xSemaphoreCreateBinary();
	    if(xStimDoneSemaphore == NULL) {
	        printf("Error al crear xStimDoneSemaphore\n");
	        while(1);
	    }

}
/*
void interruptTimerInit(void) {
//********* Configuración para la interrupnción del timer*******************
	// esta función inicializa la función que se disparará al completar el periodo de la señal PWM
	//TIMER 0
	Timer_Init(TIMER0, Timer_microsecondsToTicks(COMPLETECYCLE_PERIODO),
		timer0Periodo);
	//esta función habilita el modo compare
	Timer_EnableCompareMatch(TIMER0, TIMERCOMPAREMATCH1,
			Timer_microsecondsToTicks(NOVENTAPORCIENTO), timer0CompareMatch1);


	Timer_EnableCompareMatch(TIMER0, TIMERCOMPAREMATCH2,
				Timer_microsecondsToTicks(VEINTICINCOPORCIENTO), timer0CompareMatch2);
	Timer_EnableCompareMatch(TIMER0, TIMERCOMPAREMATCH3,
					Timer_microsecondsToTicks(CINCUENTAPORCIENTO), timer0CompareMatch3);
	Timer_EnableCompareMatch(TIMER0, TIMERCOMPAREMATCH3,
					Timer_microsecondsToTicks(NOVENTAPORCIENTO), timer0CompareMatch3);
	//TIMER1
	Timer_Init(TIMER1, Timer_microsecondsToTicks(COMPLETECYCLE_PERIODO),
				timer1Periodo);

	Timer_EnableCompareMatch(TIMER1, TIMERCOMPAREMATCH1,
				Timer_microsecondsToTicks(VEINTICINCOPORCIENTO), timer1CompareMatch1);


	Timer_EnableCompareMatch(TIMER1, TIMERCOMPAREMATCH2,
					Timer_microsecondsToTicks(NOVENTAYOCHOPORCIENTO), timer1CompareMatch2);

	Timer_EnableCompareMatch(TIMER1, TIMERCOMPAREMATCH3,
					Timer_microsecondsToTicks(NOVENTAYOCHOPORCIENTO), timer1CompareMatch3);

	Timer_Init(TIMER2, Timer_microsecondsToTicks(COMPLETECYCLE_PERIODO),
					timer2Periodo);
	Timer_EnableCompareMatch(TIMER2, TIMERCOMPAREMATCH1,
						Timer_microsecondsToTicks(NOVENTAYOCHOPORCIENTO), timer2CompareMatch1);
	Timer_EnableCompareMatch(TIMER2, TIMERCOMPAREMATCH2,
							Timer_microsecondsToTicks(NOVENTAYOCHOPORCIENTO), timer2CompareMatch2);


}*/

// Función para inicializar los PWM de los 5 LEDs.
void interrupt_pwmInitForLEDs(void) {
    // Timer1: configura 4 canales para 4 LEDs.
    Timer_Init(TIMER1, Timer_microsecondsToTicks(COMPLETECYCLE_PERIODO), timer1Periodo);
    // Inicialmente, duty cycle 0% (compare match = 0) o un valor predeterminado.
    Timer_EnableCompareMatch(TIMER1, TIMERCOMPAREMATCH1, Timer_microsecondsToTicks(0), timer1CompareMatch1);
    Timer_EnableCompareMatch(TIMER1, TIMERCOMPAREMATCH2, Timer_microsecondsToTicks(0), timer1CompareMatch2);
    Timer_EnableCompareMatch(TIMER1, TIMERCOMPAREMATCH3, Timer_microsecondsToTicks(0), timer1CompareMatch3);

    // Timer2: configura el canal 1 para LED Cian.
    Timer_Init(TIMER2, Timer_microsecondsToTicks(COMPLETECYCLE_PERIODO), timer2Periodo);

    Timer_EnableCompareMatch(TIMER2, TIMERCOMPAREMATCH1, Timer_microsecondsToTicks(0), timer2CompareMatch1);
    Timer_EnableCompareMatch(TIMER2, TIMERCOMPAREMATCH2, Timer_microsecondsToTicks(0), timer2CompareMatch2);
}



// Función para actualizar los duty cycle de cada LED según el comando recibido.
void interrupt_updateDutyCycleForLEDs(StimCommand cmd) {
    uint32_t red_ticks   = Timer_microsecondsToTicks( cmd.led_red_dc);
    uint32_t green_ticks = Timer_microsecondsToTicks(cmd.led_green_dc);
    uint32_t azul_ticks  = Timer_microsecondsToTicks(cmd.led3_azul_dc);
    uint32_t ambar_ticks = Timer_microsecondsToTicks(cmd.led3_ambar_dc);
    uint32_t cyan_ticks  = Timer_microsecondsToTicks(cmd.led3_cyan_dc);

    Timer_SetCompareMatch(TIMER1, TIMERCOMPAREMATCH1, red_ticks);
    Timer_SetCompareMatch(TIMER1, TIMERCOMPAREMATCH2, green_ticks);
    Timer_SetCompareMatch(TIMER1, TIMERCOMPAREMATCH3, azul_ticks);
    Timer_SetCompareMatch(TIMER2, TIMERCOMPAREMATCH1, ambar_ticks);
    Timer_SetCompareMatch(TIMER2, TIMERCOMPAREMATCH2, cyan_ticks);
}


void interrupt_Timer3Init(uint16_t duration_ms) {

    // Convierte la duración a microsegundos si es necesario.
    uint32_t duration_us = duration_ms * 1000;
    // Aquí se pueden configurar los temporizadores de hardware para PWM, etc.
    // Por ejemplo:
    Timer_Init(TIMER3, Timer_microsecondsToTicks(duration_us), interrupt_timer3Periodo);
    // ... otras configuraciones de timer ...
}

// ISR del temporizador TIMER3: se invoca cuando expira la duración del estímulo.
void interrupt_timer3Periodo(void *ptr) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    //deshabilito el timer
    Timer_DeInit(TIMER3);

    // Apagar los PWM: se pueden apagar actualizando los compare match a 0.
    Timer_SetCompareMatch(TIMER1, TIMERCOMPAREMATCH1, Timer_microsecondsToTicks(0));
    Timer_SetCompareMatch(TIMER1, TIMERCOMPAREMATCH2, Timer_microsecondsToTicks(0));
    Timer_SetCompareMatch(TIMER1, TIMERCOMPAREMATCH3, Timer_microsecondsToTicks(0));
    Timer_SetCompareMatch(TIMER2, TIMERCOMPAREMATCH1, Timer_microsecondsToTicks(0));
    Timer_SetCompareMatch(TIMER2, TIMERCOMPAREMATCH2, Timer_microsecondsToTicks(0));

    // Libera el semáforo (xStimDoneSemaphore) para notificar que el estímulo terminó.
    xSemaphoreGiveFromISR(xStimDoneSemaphore, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}




void interruptTimerDiseable(void) {
	Timer_DisableCompareMatch(TIMER0, TIMERCOMPAREMATCH1);
	Timer_DeInit(TIMER0);
}


void timer0Periodo(void* ptr) {
	/*======== Interrupción periodica con periodo de COMPLETECYCLE_PERIODO= 1 mseg ============*/
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

void timer1Periodo(void* ptr) {
	/*
	//gpioToggle( GPIO3 );
	static uint16_t count_interrup = 0; // cuenta la cantidad de interrupciones para incrementar el ciclo de trabajo
	static uint16_t dutyCycle = 0;

	gpioWrite(GPIO3, ON);
	gpioWrite(GPIO4, ON);
	gpioWrite(GPIO5, ON);

	//esta porción de código aumenta el ancho de pulso por el GPIO01 con el tiempo automáticamente.
	count_interrup = count_interrup + 1;

	if (count_interrup >= 30) {
		count_interrup = 0;
		dutyCycle = dutyCycle + 1;
		if (dutyCycle <= 1000) {
			Timer_SetCompareMatch(TIMER1, TIMERCOMPAREMATCH1,
					Timer_microsecondsToTicks(dutyCycle));
		} else
			dutyCycle = 0;
	}
*/
}
void timer1CompareMatch1(void *ptr)
{
	gpioWrite(GPIO3, OFF);
}
void timer1CompareMatch2(void *ptr)
{
	gpioWrite(GPIO4, OFF);
}
void timer1CompareMatch3(void *ptr)
{	gpioWrite(GPIO5, OFF);

}

void timer2Periodo(void* ptr){
	gpioWrite(GPIO6, ON);
	gpioWrite(GPIO7, ON);
}
void timer2CompareMatch1(void *ptr){
	gpioWrite(GPIO6, OFF);
}
void timer2CompareMatch2(void *ptr){
	gpioWrite(GPIO7, OFF);
}
/*
}
void timer0CompareMatch3(void *ptr){
	gpioToggle( GPIO3 );

}*/
