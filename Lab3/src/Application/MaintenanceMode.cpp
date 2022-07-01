#include "MaintenanceMode.h"
#include "AlarmManager.h"
#include "Platform.h"
#include <Gui/MaintenanceScreen.h>

#include <Core/Log/Log.h>

static const Platform::OS::TimeIntervalType LAMP_TIME_INTERVAL_ALARM = Platform::OS::MilliSec<200>::value;
static const Platform::OS::TimeIntervalType LAMP_TIME_INTERVAL_STEP = Platform::OS::MilliSec<250>::value;
static const Platform::OS::TimeIntervalType LAMP_TIME_INTERVAL_MAX = Platform::OS::MilliSec<1000>::value;
static const Platform::OS::TimeIntervalType ALARM_TIME_INTERVAL = Platform::OS::Sec<5>::value;

static const Platform::OS::TimeIntervalType SOUND_TIME_INTERVAL_CONFIRM_KEY = Platform::OS::MilliSec<300>::value;
static const Platform::OS::TimeIntervalType SOUND_TIME_INTERVAL_ERROR = Platform::OS::MilliSec<200>::value;
static const Platform::OS::TimeIntervalType SOUND_TIME_INTERVAL_CONFIRM_CODE = Platform::OS::MilliSec<1000>::value;


MaintenanceMode::MaintenanceMode() :
  mSoundTimer( this ),
  mCountdownTimer( this ),
  mKeyPressedEvent( this ),
  mKeyPressed( 0 ),
  mAlarmState( ALARM_STATE_IDLE ),
  mAlarmStateCounter( 0 ),
  mKeyCodeState( KEY_CODE_STATE_IGNORE ),
  mKeyCodeStateCount( 0 ),
  mLampFlasher( LAMP_TIME_INTERVAL_MAX )
{
}

void MaintenanceMode::enter()
{
  // this event is only relevant for the state ALARM_STATE_IDLE
  if ( mAlarmState == ALARM_STATE_IDLE )
  {
    mAlarmStateCounter = 0;
    mAlarmState = ALARM_STATE_KEYCODE;
    mCountdownTimer.start( ALARM_TIME_INTERVAL );

    // let the lamp flash
    mLampFlasher.setFlashInterval( LAMP_TIME_INTERVAL_MAX );
    mLampFlasher.startFlashing( true );

    mKeyCodeState = KEY_CODE_STATE_INPUT;
    mKeyCodeStateCount = 0;

    MaintenanceScreen::drawAlarmCodePromptMessage( 0 );
  }
}

bool MaintenanceMode::leave()
{
  bool retval = false;
  if ( mAlarmState == ALARM_STATE_ACCESS_GRANTED )
  {
    mAlarmState = ALARM_STATE_IDLE;
    // switch off the lamp
    mLampFlasher.setOutput( false );

    retval = true;
  }
  return retval;
}

void MaintenanceMode::onCountdownTimerEvent( ThisType* t, u8 callCnt )
{
  redBlocks::unused( callCnt );
  // this event is only relevant for the state ALARM_STATE_KEYCODE
  if ( t->mAlarmState == ALARM_STATE_KEYCODE )
  {
    // the timer may fire three times
    if ( 3 > t->mAlarmStateCounter )
    {
      ++t->mAlarmStateCounter;
      // restart the countdown timer
      t->mCountdownTimer.start( ALARM_TIME_INTERVAL );

      // accelerate the lamp flashing
      t->mLampFlasher.setFlashInterval( t->mLampFlasher.getFlashInterval() - LAMP_TIME_INTERVAL_STEP );
    }
    // when the timer fires for the fourth time, we enter the alarm state
    else
    {
      t->mAlarmState = ALARM_STATE_ALARM;
      // raise an alarm message
      AlarmManager::raiseAlarm( AlarmManager::ALARM_BREAK_IN );

      // let the lamp flash with the alarm flash interval
      t->mLampFlasher.setFlashInterval( LAMP_TIME_INTERVAL_ALARM );
    }
  }
}


void MaintenanceMode::onKeyPressedEvent( ThisType* t, u8 callCnt )
{
  redBlocks::unused( callCnt );
  // ignore the key, if we do not expect it to be pressed
  if ( KEY_CODE_STATE_INPUT == t->mKeyCodeState )
  {
    RB_LOG_DEBUG( "Key pressed: " << RB_LOG_CHAR( '0' + static_cast<char>( t->mKeyPressed ) ) );
    // store the pressed key
    t->mKeyCode[t->mKeyCodeStateCount] = t->mKeyPressed;
    ++t->mKeyCodeStateCount;

    // if the code is complete, we need to check it
    if ( t->mKeyCodeStateCount == sizeof( t->mKeyCode ) )
    {
      if ( t->checkKeyCode() )
      {
        RB_LOG_DEBUG( "Keycode valid" );
        MaintenanceScreen::drawMaintenanceModeMessage();

        t->mAlarmState = ALARM_STATE_ACCESS_GRANTED;
        // switch on the lamp permanently
        t->mLampFlasher.setOutput( true );
        Platform::KeySound::setOutput( true );

        t->mKeyCodeState = KEY_CODE_STATE_CONFIRM_CODE_BEEP;
        t->mSoundTimer.start( SOUND_TIME_INTERVAL_CONFIRM_CODE );
      }
      else
      {
        RB_LOG_DEBUG( "Keycode invalid" );
        MaintenanceScreen::drawAlarmCodePromptMessage( 0 );
        Platform::KeySound::setOutput( true );
        t->mKeyCodeState = KEY_CODE_STATE_ERROR_BEEP;
        t->mKeyCodeStateCount = 0;
        t->mSoundTimer.startPeriodic( SOUND_TIME_INTERVAL_ERROR );
      }
    }
    else
    {
      MaintenanceScreen::drawAlarmCodePromptMessage( t->mKeyCodeStateCount );
      t->mKeyCodeState = KEY_CODE_STATE_CONFIRM_KEY_BEEP;
      t->mSoundTimer.start( SOUND_TIME_INTERVAL_CONFIRM_KEY );
      Platform::KeySound::setOutput( true );
    }
  }
}

void MaintenanceMode::onSoundTimerEvent( ThisType* t )
{
  RB_LOG_DEBUG( "Sound timer event fired" );
  if ( KEY_CODE_STATE_ERROR_BEEP == t->mKeyCodeState )
  {
    ++t->mKeyCodeStateCount;
    Platform::KeySound::setOutput( 0 == ( t->mKeyCodeStateCount % 2 ) );
    if ( 5 == t->mKeyCodeStateCount )
    {
      t->mSoundTimer.stop();
      t->mKeyCodeState = KEY_CODE_STATE_INPUT;
      t->mKeyCodeStateCount = 0;
    }
  }
  else if ( KEY_CODE_STATE_CONFIRM_KEY_BEEP == t->mKeyCodeState )
  {
    Platform::KeySound::setOutput( false );
    t->mKeyCodeState = KEY_CODE_STATE_INPUT;
  }
  else if ( KEY_CODE_STATE_CONFIRM_CODE_BEEP == t->mKeyCodeState )
  {
    Platform::KeySound::setOutput( false );
    t->mKeyCodeState = KEY_CODE_STATE_IGNORE;
  }
}

bool MaintenanceMode::checkKeyCode() const
{
  return 0 == memcmp( mKeyCode, "\001\002\003\004", sizeof( mKeyCode ) );
}
