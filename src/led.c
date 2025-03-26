#include "led.h"
#include <stdlib.h>


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
//borra por completo la lista
void ledDeleteNodeInit(struct led **l) {
	struct led * aux, *ant, *lis = *l;
	char miTexto2[] = "\n La lista se encuentra vacia";

	if (*l == NULL) {
		uartWriteString(UART_USB, miTexto2);
	} else {
		while (*l != NULL) {
			aux = *l;
			*l = aux->sig;
			gpioWrite(aux->colorLed, OFF);
			free(aux);
		}

	}
}
