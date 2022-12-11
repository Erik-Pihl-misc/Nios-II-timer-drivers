/********************************************************************************
* timer.c: Inneh�ller drivrutiner f�r timers, vilket i praktiken utg�rs av
*          uppr�kning av klockpulser via ett 32-bitars register, som i detta
*          sammanhang kallas en h�rdvarutimer. En 50 MHz systemklocka p�
*          FPGA-kortet anv�nds, vilket medf�r 50 000 klockpulser per sekund.
********************************************************************************/
#include "timer.h"

/********************************************************************************
* TIMER_STOP: Inaktiverar h�rdvarutimern, vilket stoppar uppr�kning av
*             antalet passerade klockpulser.
********************************************************************************/
#define TIMER_STOP IOWR_32DIRECT(TIMER_HW_IP_0_BASE, 4, 0x00000000)

/********************************************************************************
* TIMER_RESET: �terst�ller h�rdvarutimern till uppr�kning av antalet passerade
*              klockpulser fr�n noll.
********************************************************************************/
#define TIMER_RESET IOWR_32DIRECT(TIMER_HW_IP_0_BASE, 4, 0x40000000)

/********************************************************************************
* TIMER_START: Aktiverar h�rdvarutimern, vilket startar uppr�kning av antalet
*              passerade klockpulser.
********************************************************************************/
#define TIMER_START IOWR_32DIRECT(TIMER_HW_IP_0_BASE, 4, 0x80000000)

/********************************************************************************
* TIMER_READ: L�ser av antalet uppr�knade klockpulser.
********************************************************************************/
#define TIMER_READ  IORD_32DIRECT(TIMER_HW_IP_0_BASE, 4)

/********************************************************************************
* TIMER_COUNT: Antalet klockpulser som r�knas upp per millisekund vid en
*              frekvens p� 50 MHz.
********************************************************************************/
#define TIMER_COUNT_1MS 50000

/* Statiska variabler: */
static volatile uint32_t elapsed_time_ms = 0; /* R�knar antalet passerade millisekunder. */

/* Statiska funktioner: */
static inline void timer_update(void);
static inline bool time_has_passed(const uint32_t time_ms);

/********************************************************************************
* timer_init: Initierar och aktiverar timer till att l�pa ut efter angiven tid.
*
*             - self   : Pekare till timern som ska initieras.
*             - time_ms: Tiden som timern ska l�pa ut efter i millisekunder.
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
* timer_elapsed: Kontrollerar ifall refererad timer har l�pt ut och returnerar
*                i s� fall true, annars false.
*
*                - self: Pekare till timern som ska l�sas av.
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
* timer_enable_hardware: Aktiverar timerkretsarna p� h�rdvarniv�, vilket beh�vs
*                        f�r att anv�nda timers efter inaktivering via anrop
*                        av funktionen timer_disable_hardware.
********************************************************************************/
void timer_enable_hardware(void)
{
   TIMER_RESET;
   TIMER_START;
   return;
}

/********************************************************************************
* timer_disable_hardware: Inaktiverar timerkretsarna p� h�rdvarniv�, vilket
*                         inaktiverar samtliga timers. �teraktivering genomf�rs
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
* timer_update: L�ser av h�rdvarutimern och r�knar upp antalet passerade
*               millisekunder. Efter uppr�kning �terst�lls h�rdvarutimern.
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
*                  - time_ms: Tiden som ska kontrolleras, m�tt i millisekunder.
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
