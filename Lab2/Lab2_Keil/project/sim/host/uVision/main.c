// TODO
// When changing the device in the project settings, this include has to be adjusted accordingly.
// Also make sure to adjust the heap and stack size in startup file.
#include <ARMCM0.h>

#include <Application.h>
#include <libredBlocks_sim_uvision.h>

extern void Platform_initSimulation( void );

void SysTick_Handler()
{
  rb_sim_tick();
}

void SysTick_control(u8 disable)
{
  if (disable)
  {
    NVIC_DisableIRQ ( SysTick_IRQn );
  }
  else
  {
    NVIC_EnableIRQ ( SysTick_IRQn );
  }
}
static const u32 SYSTEM_TICKS_PER_SEC = 100;

/**
 * Main routine of our simulator application. It sets up the single components,
 * associates them with each other and starts the application.
 */
int main()
{
  SystemCoreClock = 8000000UL;
  SysTick_Config( SystemCoreClock / SYSTEM_TICKS_PER_SEC );

  rb_sim_uvision_init( 0x30000000, 500, 500, &SysTick_control);
  Platform_initSimulation();
  rb_sim_initDone();

  Application_run();

  while (true);
}
