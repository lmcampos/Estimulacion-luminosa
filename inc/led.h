#ifndef	_LED
#define	_LED

#include "sapi.h"


struct led {

	gpioMap_t colorLed;  //es un ping de salida GPIO
	uint16_t factor;     // factor en el que se va ir incrementando el ciclo de trabajo hasta el valor establecido en la variable
						 //dutyCycle
	uint16_t dutyCycle;  // valor que debe alcanzar el ciclo de trabajo de la señal
	struct led * sig;
};
struct led * pLed;

//pAux = pLed;

// agrego un nodo al final de la lista
void ledAddNodeEnd(struct led ** pLed, gpioMap_t gpio, uint16_t fact, uint16_t dCycle);
//borra por completo la lista
void ledDeleteNodeInit(struct led **pLed);

#endif
