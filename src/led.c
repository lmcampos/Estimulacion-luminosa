#include "led.h"


void ledAddNodeEnd(struct led **l, gpioMap_t gpio, uint16_t fact,
		uint16_t dCycle) {

	struct led * aux = NULL, *lis = NULL;
	lis = *l;
	char miTexto[] = "\n Error en la asignacion de memoria a estructura LED";

	aux = (struct led *) malloc(sizeof(struct led));
	if (aux == NULL)
		uartWriteString(UART_USB, miTexto);

	aux->colorLed = gpio;
	aux->factor = fact;
	aux->dutyCycle = dCycle;


	if (*l == NULL) {
		aux->sig = *l;
		*l = aux;

	} else {
		while (lis->sig != NULL) {
			lis = lis->sig;
		}
		aux->sig = lis->sig;
		lis->sig = aux;
	}
}

