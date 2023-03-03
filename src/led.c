#include "led.h"

void ledAddNodeEnd(led_t * l, gpioMap_t gpio, uint16_t fact, uint16_t dCycle) {

	led_t * aux;
	aux = (led_t *) malloc(sizeof(led_t));

	aux->colorLed = gpio;
	aux->factor = fact;
	aux->dutyCycle = dCycle;

	if (led == NULL) {

		aux->sig = l;
		l = aux;
		free(aux);

	} else {
		while (l->sig != NULL) {
			l = l->sig;
		}
		aux->sig = l->sig;
		l->sig= aux;
		free(aux);


	}

}

