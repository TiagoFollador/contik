#include "contiki.h"
#include <stdio.h>
#include <random.h>
#include <string.h>


// Minimos e maximos para gerar os valores
#define BATIMENTO_MIN 20
#define BATIMENTO_MAX 140

#define OXIGENIO_MAX 100
#define OXIGENIO_MIN 80

#define TEMPERATURA_MAX 41
#define TEMPERATURA_MIN 34

// definindo os processos
PROCESS(batimentoCardiaco, "Batimento Cardiaco");
PROCESS(oxigenio, "Saturação do Oxigenio");
PROCESS(temperatura, "Temperatura Corporal");
PROCESS(evento, "Evento");

// inicializacao automatica dos processos
AUTOSTART_PROCESSES(&batimentoCardiaco, &oxigenio, &temperatura, &evento);

// Thread responsavel por gerar o batimento cardiaco
PROCESS_THREAD(batimentoCardiaco, ev, data) 
{
    static struct etimer timer;
    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 3);

    int batimento = 0;
    static char msg[50];

    while(true) {

        batimento =   (random_rand() % (BATIMENTO_MAX - BATIMENTO_MIN)) + BATIMENTO_MIN;
        sprintf(msg, "Batimento cardiaco: %d\n", batimento);
        printf("%s", msg);

        if (batimento < 50) {
            process_post(&evento, PROCESS_EVENT_MSG, (void *)"BATIMENTO_BAIXO");
        } else if (batimento > 90) {
            process_post(&evento, PROCESS_EVENT_MSG, (void *)"BATIMENTO_ALTO");
        }

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        etimer_reset(&timer);
    }

    PROCESS_END();
}

// Thread responsavel por gerar o oxigenio
PROCESS_THREAD(oxigenio, ev, data)
{
    static struct etimer timer;
    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 3);
    int oxigenio = 0;

    static char msg[50];

    while(true) {

        oxigenio =   (random_rand() % (OXIGENIO_MAX - OXIGENIO_MIN)) + OXIGENIO_MIN;
        sprintf(msg, "Saturação do oxigenio: %d%%\n", oxigenio);
        printf("%s", msg);

        if (oxigenio < 90) {
            process_post(&evento, PROCESS_EVENT_MSG, (void *)"OXIGENIO_BAIXO");
        }

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        etimer_reset(&timer);
    }

    PROCESS_END();
}

// Thread  responsavel por gerar a temperatura
PROCESS_THREAD(temperatura, ev, data)
{
    static struct etimer timer;
    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 3);
    int temperatura = 0;

    static char msg[50];

    while(true) {

        temperatura =   (random_rand() % (TEMPERATURA_MAX - TEMPERATURA_MIN)) + TEMPERATURA_MIN; 
        sprintf(msg, "Temperatura: %d\n", temperatura);
        printf("%s", msg);

        if (temperatura < 35) {
            process_post(&evento, PROCESS_EVENT_MSG, (void *) "HIPOTERMIA");
        } else if (temperatura > 39) {
            process_post(&evento, PROCESS_EVENT_MSG, (void *) "FEBRE");
        }

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        etimer_reset(&timer);
    }

    PROCESS_END();
}

// Thread responsavel por receber os eventos 
PROCESS_THREAD(evento, ev, data)
{
    PROCESS_BEGIN();

    while(true) {
        PROCESS_WAIT_EVENT();
        if (ev == PROCESS_EVENT_MSG) {
            char *eventoAlerta = (char *)data;

            if (strcmp(eventoAlerta, "BATIMENTO_BAIXO") == 0) {
                printf("ALERTA: Batimento cardíaco baixo!\n");
            } else if (strcmp(eventoAlerta, "BATIMENTO_ALTO") == 0) {
                printf("ALERTA: Batimento cardíaco alto!\n");
            } else if (strcmp(eventoAlerta, "OXIGENIO_BAIXO") == 0) {
                printf("ALERTA: Saturação de oxigênio baixa!\n");
            } else if (strcmp(eventoAlerta, "HIPOTERMIA") == 0) {
                printf("ALERTA: Hipotermia!\n");
            } else if (strcmp(eventoAlerta, "FEBRE") == 0) {
                printf("ALERTA: Febre!\n");
            }

        }
    }

    PROCESS_END();
}