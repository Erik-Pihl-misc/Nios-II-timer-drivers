/********************************************************************************
* timer.h: Inneh�ller drivrutiner f�r timers som kan s�ttas till att l�pa ut
*          efter godtycklig tid. Efter initiering �r godtycklig timerkrets
*          aktiverad efter start och det enda som beh�vs f�r att kontrollera
*          ifall timern har l�pt ut �r att anropa funktionen timer_elapsed.
********************************************************************************/
#ifndef TIMER_H_
#define TIMER_H_

/* Inkluderingsdirektiv: */
#include <io.h>      /* Inneh�ller funktionalitet f�r l�sning/skrivning till I/O-register. */
#include <system.h>  /* Inneh�ller makrodefinitioner f�r timerkretsar. */
#include <stdbool.h> /* Inneh�ller definition av datatypen bool. */
#include <stdint.h>  /* Inneh�ller typdefinitioner f�r diverse heltalstyper s�som uint32_t. */

/********************************************************************************
* timer: Strukt f�r implementering av timerkretsar tillsammans med drivrutiner.
*        En given timer kan s�ttas till att l�pa ut efter valbar tid m�tt i
*        millisekunder. Timern �r som default aktiverad efter initiering, men
*        kan n�r som helst st�ngas av s� att den aldrig l�per ut.
********************************************************************************/
struct timer
{
   uint32_t time_ms; /* Tiden som timern ska l�pa ut efter, m�tt i millisekunder. */
   bool enabled;     /* Enable-signal f�r att kontrollera aktivering av timern. */
};

/********************************************************************************
* timer_init: Initierar och aktiverar refererad timer till att l�pa ut efter
*             angiven tid m�tt i millisekunder.
*
*             - self   : Pekare till timern som ska initieras.
*             - time_ms: Tiden som timern ska l�pa ut efter i millisekunder.
********************************************************************************/
void timer_init(struct timer* self,
                const uint32_t time_ms);

/********************************************************************************
* timer_clear: Nollst�ller refererad timer s� att denna inte kan anv�ndas
*              f�rr�n �terinitiering genomf�rs.
*
*             - self: Pekare till timern som ska nollst�llas.
********************************************************************************/
static inline void timer_clear(struct timer* self)
{
   self->time_ms = 0;
   self->enabled = false;
   return;
}

/********************************************************************************
* timer_on: Aktiverar refererad timerkrets, vilket m�jligg�r avl�sning av ifall
*           timern i fr�ga har l�pt ut.
*
*           - self: Pekare till timern som ska aktiveras.
********************************************************************************/
static inline void timer_on(struct timer* self)
{
   self->enabled = true;
   return;
}

/********************************************************************************
* timer_off: Inaktiverar refererad timerkrets, vilket medf�r att det inte g�r
*            att avl�sa ifall timern har l�pt ut.
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
* timer_elapsed: Kontrollerar ifall refererad timer har l�pt ut och returnerar
*                i s� fall true, annars false.
*
*                - self: Pekare till timern som ska l�sas av.
********************************************************************************/
bool timer_elapsed(struct timer* self);

static inline void timer_set_time(struct timer* self,
                                  const uint32_t time_ms)
{
   self->time_ms = time_ms;
   return;
}

/********************************************************************************
* timer_enable_hardware: Aktiverar timerkretsarna p� h�rdvarniv�, vilket beh�vs
*                        f�r att anv�nda timers efter inaktivering via anrop
*                        av funktionen timer_disable_hardware.
********************************************************************************/
void timer_enable_hardware(void);

/********************************************************************************
* timer_disable_hardware: Inaktiverar timerkretsarna p� h�rdvarniv�, vilket
*                         inaktiverar samtliga timers. �teraktivering genomf�rs
*                         via anrop av funktionen timer_enable_hardware
********************************************************************************/
void timer_disable_hardware(void);

#endif /* TIMER_H_ */
