#include "contiki.h"
#include <stdio.h>
#include <random.h>

#define BATIMENTO_MIN 20
#define BATIMENTO_MAX 140

#define OXIGENIO_MAX 100;
#define OXIGENIO_MIN 80;

#define TEMPERATURA_MAX 41;
#define TEMPERATURA_MIN 34;

PROCESS(batimentoCardiaco, "Batimento Cardiaco");
PROCESS(oxigenio, "Saturação do Oxigenio");
PROCESS(temperatura, "Temperatura Corporal");
PROCESS(evento, "Evento");

AUTOSTART_PROCESSES(&batimentoCardiaco, &oxigenio, &temperatura, &evento);

PROCESS_THREAD(batimentoCardiaco, ev, data)
{
    static struct etimer timer;
    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 3);

    int batimento = 0;
    static char msg[50];

    while(true) {

        batimento =   random_rand() % (BATIMENTO_MIN - BATIMENTO_MAX) + BATIMENTO_MAX;
        if (batimento < 50) {
            sprintf(msg, "Alerta, batimento cardiaco baixo: %d\n", batimento);
            printf("%s", msg);
            process_post(&evento, PROCESS_EVENT_MSG, (void *)"BATIMENTO_BAIXO");
        } else if (batimento > 90) {
            sprintf(msg, "Alerta, batimento cardiaco alto: %d\n", batimento);
            printf("%s", msg);
            process_post(&evento, PROCESS_EVENT_MSG, (void *)"BATIMENTO_ALTO");
        } else {
            sprintf(msg, "Batimento cardiaco normal: %d\n", batimento);
            printf("%s", msg);
        }

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        etimer_reset(&timer);
    }

    PROCESS_END();
}

PROCESS_THREAD(oxigenio, ev, data)
{
    static struct etimer timer;
    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 3);
    int oxigenio = 0;

    static char msg[50];

    while(true) {

        oxigenio =   random_rand() % (OXIGENIO_MAX - OXIGENIO_MIN) + OXIGENIO_MIN;
        if (oxigenio < 90) {
            sprintf(msg, "Alerta, saturacao de oxigenio baixa: %d%%\n", oxigenio);
            printf("%s", msg);
            process_post(&evento, PROCESS_EVENT_MSG, (void *)"OXIGENIO_BAIXO");
        } else {
            sprintf(msg, "Saturação do oxigenio normal: %d%%\n", oxigenio);
            printf("%s", msg);
        }

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        etimer_reset(&timer);
    }

    PROCESS_END();
}

PROCESS_THREAD(temperatura, ev, data)
{
    static struct etimer timer;
    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 3);
    int temperatura = 0;

    static char msg[50];

    while(true) {

        temperatura =   random_rand() % (TEMPERATURA_MAX - TEMPERATURA_MIN) + TEMPERATURA_MIN; 
        if (temperatura < 35) {
            sprintf(msg, "Alerta, Hipotermia: %d\n", temperatura); 
            printf("%s", msg);
            process_post(&evento, PROCESS_EVENT_MSG, (void *) "HIPOTERMIA");
        } else if (temperatura > 39) {
            sprintf(msg, "Alerta, Febre: %d\n", temperatura);
            printf("%s", msg);
            process_post(&evento, PROCESS_EVENT_MSG, (void *) "FEBRE");
        } else {
            sprintf(msg, "Temperatura normal: %d\n", temperatura);
            printf("%s", msg);
        }

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        etimer_reset(&timer);
    }

    PROCESS_END();
}


PROCESS_THREAD(evento, ev, data)
{
    PROCESS_BEGIN();

    while(true) {
        PROCESS_WAIT_EVENT();
        if (ev == PROCESS_EVENT_MSG) {
            char *evento = (char *)data;

            if (evento == "BATIMENTO_BAIXO") {
                printf("ALERTA: Batimento cardíaco abaixo do normal!\n");
            } else if (evento == "BATIMENTO_ALTO") {
                printf("ALERTA: Batimento cardíaco acima do normal!\n");
            } else if (evento == "OXIGENIO_BAIXO") {
                printf("ALERTA: Saturação de oxigênio abaixo do normal!\n");
            } else if (evento == "HIPOTERMIA") {
                printf("ALERTA: Temperatura corporal abaixo do normal!\n");
            } else if (evento == "FEBRE") {
                printf("ALERTA: Temperatura corporal acima do normal!\n");
            }
        }
    }

    PROCESS_END();
}