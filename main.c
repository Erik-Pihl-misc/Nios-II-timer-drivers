/********************************************************************************
* main.c: Implementering av timers via strukten timer samt drivrutiner.
********************************************************************************/
#include "timer.h"
#include "sys/alt_stdio.h"

/********************************************************************************
* main: Initierar två timerkretsar t1 - t2 till att löpa ut efter 500 respektive
*       1000 millisekunder. Varje gång respektive timer löper ut skrivs detta
*       ut i terminalen.
********************************************************************************/
int main(void)
{
   struct timer t1, t2;

   timer_init(&t1, 500);
   timer_init(&t2, 1000);

   while (1)
   {
      if (timer_elapsed(&t1))
      {
         alt_printf("Timer 1 elapsed!\n");
      }
      if (timer_elapsed(&t2))
      {
         alt_printf("Timer 2 elapsed!\n");
      }
   }

   return 0;
}
