#include "Application.h"
#include "Gui/Gui.h"
#include <libredBlocks_sim.h> //Lab code - needed library



Application::Application() :
  mTemperatureControllerTimer( this ),
  // cooling temperature 8 +/- 2 degrees, alarm at 14 degrees
  mTemperatureController( 8000, 2000, 14000 ),
  mVendingMode(),
  mGlobalState( mVendingMode, mMaintenanceMode )
{
  Platform::PowerSupervision::enableCallback();
  Platform::LightBarrier::enableCallback();
  Platform::FrontDoorProtection::enableCallback();
  Platform::ReturnMoneyButton::enableCallback();
  Platform::FilledUpButton::enableCallback();
  Platform::Product1Button::enableCallback();
  Platform::Product2Button::enableCallback();
  Platform::Product3Button::enableCallback();
  Platform::Product4Button::enableCallback();
  Platform::TemperatureSensor::enableCallback();
  mTemperatureControllerTimer.startPeriodic( Platform::OS::MilliSec<100>::value );
}

void Application::run() const
{
	for( ; ; )
	{
			if(rb_sim_DigitalInput_getValue(9) == 0){ //Lab code - Returns the current value of the digital output interface - the Push button we add is the 9th digital interface in the simulator
				rb_sim_DigitalInput_enableCallback(9);//Lab code - Enables the callback invocation of the digital input interface - the Push button we add is the 9th digital interface in the simulator
			}
			else{ //Lab code - if the digital input has any other value the vending machine can start
				Platform::OS::processEvents();
				Platform::enterSleepMode();
			}
	}
}

void Application::onTemperatureControllerTimerEvent( ThisType* t, u8 callCnt )
{
  redBlocks::unused( callCnt );
  t->mTemperatureController.runOnce();
}

