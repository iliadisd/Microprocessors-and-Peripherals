#include <libredBlocks_sim.h>
#include <Platform.h>

#define ID_LED 0
#define ID_PUSH_BUTTON 0

void Platform_initSimulation( void )
{
  rb_sim_DigitalOutput_add( ID_LED, 0 );
  rb_sim_DigitalInput_add( ID_PUSH_BUTTON, RB_TRIGGER_EDGE_RISING, NULL );
}

void Platform_Led_setValue( unsigned char value )
{
  rb_sim_DigitalOutput_setValue(ID_LED, value);
}

void Platform_PushButton_setIsrCallback(void (*cbk)( void ))
{
  rb_sim_DigitalInput_setCallback(ID_PUSH_BUTTON, cbk);
}

void Platform_PushButton_enableInterrupt()
{
  rb_sim_DigitalInput_enableCallback(ID_PUSH_BUTTON);
}

void Platform_PushButton_disableInterrupt()
{
  rb_sim_DigitalInput_disableCallback(ID_PUSH_BUTTON);
}

unsigned char Platform_PushButton_getValue()
{
  return rb_sim_DigitalInput_getValue(ID_PUSH_BUTTON);
}

void Platform_registerSystickCallback(void (*cbk)( void ))
{
  rb_sim_SystemTick_init(10, 1, cbk);
  rb_sim_SystemTick_enableCallback();
}
