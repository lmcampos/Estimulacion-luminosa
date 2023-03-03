#ifndef	_LED
#define	_LED

#include "sapi.h"
#include <stdlib.h>

struct led {

	gpioMap_t colorLed;
	uint16_t factor;
	uint16_t dutyCycle;

	struct led * sig;
};

typedef struct led led_t;
led_t * led;

// agrego un nodo al final de la lista
void ledAddNodeEnd(led_t * l, gpioMap_t gpio, uint16_t fact, uint16_t dCycle);

#endif
