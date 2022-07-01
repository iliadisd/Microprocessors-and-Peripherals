#include <Application.h>
#include <Platform.h>
#include <stdbool.h>

static unsigned char ledValue = false;
static unsigned char doBlink = true;

static void onPushButtonChangedCbk()
{
  if (Platform_PushButton_getValue() == true)
  {
    if (doBlink)
    {
      ledValue = true;
      doBlink = false;
    }
    else
    {
      if (ledValue)
      {
        ledValue = false;
      }
      else
      {
        doBlink = true;
      }
    }
    Platform_Led_setValue(ledValue);
  }
}

static void onSystemTick()
{
	static unsigned int i = 0;
  if (doBlink)
  {
    if (0 == i % 500)
    {
      i = 0;
      ledValue = !ledValue;
      Platform_Led_setValue( ledValue );
    }
    ++i;
  }
  else
  {
    i = 0;
  }
}

void Application_run()
{
  Platform_Led_setValue(ledValue);
  Platform_PushButton_setIsrCallback(onPushButtonChangedCbk);
  Platform_PushButton_enableInterrupt();
  Platform_registerSystickCallback(onSystemTick);
  while( 1 );
}
