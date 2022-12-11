/********************************************************************************
* timer.h: Innehåller drivrutiner för timers som kan sättas till att löpa ut
*          efter godtycklig tid. Efter initiering är godtycklig timerkrets
*          aktiverad efter start och det enda som behövs för att kontrollera
*          ifall timern har löpt ut är att anropa funktionen timer_elapsed.
********************************************************************************/
#ifndef TIMER_H_
#define TIMER_H_

/* Inkluderingsdirektiv: */
#include <io.h>      /* Innehåller funktionalitet för läsning/skrivning till I/O-register. */
#include <system.h>  /* Innehåller makrodefinitioner för timerkretsar. */
#include <stdbool.h> /* Innehåller definition av datatypen bool. */
#include <stdint.h>  /* Innehåller typdefinitioner för diverse heltalstyper såsom uint32_t. */

/********************************************************************************
* timer: Strukt för implementering av timerkretsar tillsammans med drivrutiner.
*        En given timer kan sättas till att löpa ut efter valbar tid mätt i
*        millisekunder. Timern är som default aktiverad efter initiering, men
*        kan när som helst stängas av så att den aldrig löper ut.
********************************************************************************/
struct timer
{
   uint32_t time_ms; /* Tiden som timern ska löpa ut efter, mätt i millisekunder. */
   bool enabled;     /* Enable-signal för att kontrollera aktivering av timern. */
};

/********************************************************************************
* timer_init: Initierar och aktiverar refererad timer till att löpa ut efter
*             angiven tid mätt i millisekunder.
*
*             - self   : Pekare till timern som ska initieras.
*             - time_ms: Tiden som timern ska löpa ut efter i millisekunder.
********************************************************************************/
void timer_init(struct timer* self,
                const uint32_t time_ms);

/********************************************************************************
* timer_clear: Nollställer refererad timer så att denna inte kan användas
*              förrän återinitiering genomförs.
*
*             - self: Pekare till timern som ska nollställas.
********************************************************************************/
static inline void timer_clear(struct timer* self)
{
   self->time_ms = 0;
   self->enabled = false;
   return;
}

/********************************************************************************
* timer_on: Aktiverar refererad timerkrets, vilket möjliggör avläsning av ifall
*           timern i fråga har löpt ut.
*
*           - self: Pekare till timern som ska aktiveras.
********************************************************************************/
static inline void timer_on(struct timer* self)
{
   self->enabled = true;
   return;
}

/********************************************************************************
* timer_off: Inaktiverar refererad timerkrets, vilket medför att det inte går
*            att avläsa ifall timern har löpt ut.
*
*            - self: Pekare till timern som ska inaktiveras.
********************************************************************************/
static inline void timer_off(struct timer* self)
{
   self->enabled = false;
   return;
}

/********************************************************************************
* timer_toggle: Togglar aktivering av refererad timerkrets.
*
*               - self: Pekare till timern som ska togglas.
********************************************************************************/
static inline void timer_toggle(struct timer* self)
{
   self->enabled = !self->enabled;
   return;
}

/********************************************************************************
* timer_elapsed: Kontrollerar ifall refererad timer har löpt ut och returnerar
*                i så fall true, annars false.
*
*                - self: Pekare till timern som ska läsas av.
********************************************************************************/
bool timer_elapsed(struct timer* self);

static inline void timer_set_time(struct timer* self,
                                  const uint32_t time_ms)
{
   self->time_ms = time_ms;
   return;
}

/********************************************************************************
* timer_enable_hardware: Aktiverar timerkretsarna på hårdvarnivå, vilket behövs
*                        för att använda timers efter inaktivering via anrop
*                        av funktionen timer_disable_hardware.
********************************************************************************/
void timer_enable_hardware(void);

/********************************************************************************
* timer_disable_hardware: Inaktiverar timerkretsarna på hårdvarnivå, vilket
*                         inaktiverar samtliga timers. Återaktivering genomförs
*                         via anrop av funktionen timer_enable_hardware
********************************************************************************/
void timer_disable_hardware(void);

#endif /* TIMER_H_ */
