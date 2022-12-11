/********************************************************************************
* timer.c: Innehåller drivrutiner för timers, vilket i praktiken utgörs av
*          uppräkning av klockpulser via ett 32-bitars register, som i detta
*          sammanhang kallas en hårdvarutimer. En 50 MHz systemklocka på
*          FPGA-kortet används, vilket medför 50 000 klockpulser per sekund.
********************************************************************************/
#include "timer.h"

/********************************************************************************
* TIMER_STOP: Inaktiverar hårdvarutimern, vilket stoppar uppräkning av
*             antalet passerade klockpulser.
********************************************************************************/
#define TIMER_STOP IOWR_32DIRECT(TIMER_HW_IP_0_BASE, 4, 0x00000000)

/********************************************************************************
* TIMER_RESET: Återställer hårdvarutimern till uppräkning av antalet passerade
*              klockpulser från noll.
********************************************************************************/
#define TIMER_RESET IOWR_32DIRECT(TIMER_HW_IP_0_BASE, 4, 0x40000000)

/********************************************************************************
* TIMER_START: Aktiverar hårdvarutimern, vilket startar uppräkning av antalet
*              passerade klockpulser.
********************************************************************************/
#define TIMER_START IOWR_32DIRECT(TIMER_HW_IP_0_BASE, 4, 0x80000000)

/********************************************************************************
* TIMER_READ: Läser av antalet uppräknade klockpulser.
********************************************************************************/
#define TIMER_READ  IORD_32DIRECT(TIMER_HW_IP_0_BASE, 4)

/********************************************************************************
* TIMER_COUNT: Antalet klockpulser som räknas upp per millisekund vid en
*              frekvens på 50 MHz.
********************************************************************************/
#define TIMER_COUNT_1MS 50000

/* Statiska variabler: */
static volatile uint32_t elapsed_time_ms = 0; /* Räknar antalet passerade millisekunder. */

/* Statiska funktioner: */
static inline void timer_update(void);
static inline bool time_has_passed(const uint32_t time_ms);

/********************************************************************************
* timer_init: Initierar och aktiverar timer till att löpa ut efter angiven tid.
*
*             - self   : Pekare till timern som ska initieras.
*             - time_ms: Tiden som timern ska löpa ut efter i millisekunder.
********************************************************************************/
void timer_init(struct timer* self,
                const uint32_t time_ms)
{
   self->time_ms = time_ms;
   self->enabled = true;
   TIMER_START;
   return;
}

/********************************************************************************
* timer_elapsed: Kontrollerar ifall refererad timer har löpt ut och returnerar
*                i så fall true, annars false.
*
*                - self: Pekare till timern som ska läsas av.
********************************************************************************/
bool timer_elapsed(struct timer* self)
{
   if (self->enabled && time_has_passed(self->time_ms))
   {
      return true;
   }
   else
   {
      return false;
   }
}

/********************************************************************************
* timer_enable_hardware: Aktiverar timerkretsarna på hårdvarnivå, vilket behövs
*                        för att använda timers efter inaktivering via anrop
*                        av funktionen timer_disable_hardware.
********************************************************************************/
void timer_enable_hardware(void)
{
   TIMER_RESET;
   TIMER_START;
   return;
}

/********************************************************************************
* timer_disable_hardware: Inaktiverar timerkretsarna på hårdvarnivå, vilket
*                         inaktiverar samtliga timers. Återaktivering genomförs
*                         via anrop av funktionen timer_enable_hardware
********************************************************************************/
void timer_disable_hardware(void)
{
   TIMER_STOP;
   TIMER_RESET;
   elapsed_time_ms = 0;
   return;
}

/********************************************************************************
* timer_update: Läser av hårdvarutimern och räknar upp antalet passerade
*               millisekunder. Efter uppräkning återställs hårdvarutimern.
********************************************************************************/
static inline void timer_update(void)
{
   if (TIMER_READ >= TIMER_COUNT_1MS)
   {
      elapsed_time_ms++;
      TIMER_RESET;
      TIMER_START;
   }
   return;
}

/********************************************************************************
* time_has_passed: Uppdaterar antalet passerade millisekunder och indikerar
*                  ifall angiven tid har passerat. Ifall angiven tid har
*                  passerat returneras true, annars false.
*
*                  - time_ms: Tiden som ska kontrolleras, mätt i millisekunder.
********************************************************************************/
static inline bool time_has_passed(const uint32_t time_ms)
{
   timer_update();

   if (elapsed_time_ms && !(elapsed_time_ms % time_ms))
   {
      return true;
   }
   else
   {
      return false;
   }
}
