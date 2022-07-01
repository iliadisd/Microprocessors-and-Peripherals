#ifndef PLATFORM_CALLBACKS_H_
#define PLATFORM_CALLBACKS_H_

#include <Application/Application.h>
#include <Platform.h>


// callback routing from platform to application modules
RB_CONNECT_ISR_CBK( Platform::TemperatureSensor, Platform::TemperatureSensor::CBK_ON_SAMPLING_FINISHED, Application::getInstanceRef().getTemperatureController().onTemperatureAvailable() )
RB_CONNECT_ISR_CBK( Platform::PowerSupervision, Platform::PowerSupervision::CBK_ON_INPUT_CHANGED, /** Empty */ )
RB_CONNECT_ISR_CBK( Platform::FrontDoorProtection, Platform::FrontDoorProtection::CBK_ON_INPUT_CHANGED, Application::getGlobalState().onFrontDoorContactChange() )
RB_CONNECT_ISR_CBK( Platform::ReturnMoneyButton, Platform::ReturnMoneyButton::CBK_ON_INPUT_CHANGED, Application::getVendingMode().onReleaseMoneyBtnPressed() )
RB_CONNECT_ISR_CBK( Platform::Product1Button, Platform::Product1Button::CBK_ON_INPUT_CHANGED, Application::getVendingMode().onProductBtnPressed( 0 ) )
RB_CONNECT_ISR_CBK( Platform::Product2Button, Platform::Product2Button::CBK_ON_INPUT_CHANGED, Application::getVendingMode().onProductBtnPressed( 1 ) )
RB_CONNECT_ISR_CBK( Platform::Product3Button, Platform::Product3Button::CBK_ON_INPUT_CHANGED, Application::getVendingMode().onProductBtnPressed( 2 ) )
RB_CONNECT_ISR_CBK( Platform::Product4Button, Platform::Product4Button::CBK_ON_INPUT_CHANGED, Application::getVendingMode().onProductBtnPressed( 3 ) )
RB_CONNECT_ISR_CBK( Platform::LightBarrier, Platform::LightBarrier::CBK_ON_INPUT_CHANGED, Application::getVendingMode().onLightBarrierEvent() )
RB_CONNECT_ISR_CBK( Platform::FilledUpButton, Platform::FilledUpButton::CBK_ON_INPUT_CHANGED, Application::getVendingMode().onFillupBtnPressed() )
RB_CONNECT_ISR_CBK( Platform::Keypad, Platform::Keypad::CBK_ON_KEY_PRESSED, Application::getMaintenanceMode().onKeyPressed( Platform::Keypad::getKey() ) )


#endif // PLATFORM_CALLBACKS_H_
