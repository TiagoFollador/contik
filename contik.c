#include "contiki.h"
#include <stdio.h>
#include <random.h>

#define MIN_VAL 20
#define MAX_VAL 140

PROCESS(batimentoCardiaco, "Batimento Cardiaco");
PROCESS(oxigenio, "Saturação do Oxigenio");
PROCESS(temperatura, "Temperatura Corporal");

AUTOSTART_PROCESSES(&batimentoCardiaco, &oxigenio, &temperatura);

PROCESS_THREAD(batimentoCardiaco, ev, data)
{
    static struct etimer timer;
    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 3);

    int batimento = 0;
    static char msg[50];

    while(true) {

        batimento =   random_rand() % (MAX_VAL - MIN_VAL) + MIN_VAL;
        if (batimento < 50) {
            sprintf(msg, "Alerta, batimento cardiaco baixo: %d\n", batimento);
            printf("%s", msg);
        } else if (batimento > 90) {
            sprintf(msg, "Alerta, batimento cardiaco alto: %d\n", batimento);
            printf("%s", msg);
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
    int max = 100;
    int min = 80;
    static char msg[50];

    while(true) {

        oxigenio =   random_rand() % (max - min) + min;
        if (oxigenio < 90) {
            sprintf(msg, "Alerta, saturacao de oxigenio baixa: %d%%\n", oxigenio);
            printf("%s", msg);
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
    int max = 41;
    int min = 34;
    static char msg[50];

    while(true) {

        temperatura =   random_rand() % (max - min) + min; 
        if (temperatura < 35) {
            sprintf(msg, "Alerta, Hipotermia: %d\n", temperatura);
            printf("%s", msg);
        } else if (temperatura > 39) {
            sprintf(msg, "Alerta, Febre: %d\n", temperatura);
            printf("%s", msg);
        } else {
            sprintf(msg, "Temperatura normal: %d\n", temperatura);
            printf("%s", msg);
        }

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        etimer_reset(&timer);
    }

    PROCESS_END();
}
