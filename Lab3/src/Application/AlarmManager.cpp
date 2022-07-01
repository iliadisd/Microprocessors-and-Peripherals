#include "AlarmManager.h"
#include "Platform.h"


static const char* const alarmMessages[] =
{
  "Break-in attempt",
  "Overheat",
  "Slot empty",
  "Slot blocked"
};

static const char* const number = "+4915233767667";


AlarmManager::AlarmManager() :
  mAlarmEvent( this ),
  mAlarms( 0 )
{
}

void AlarmManager::raiseAlarm( Alarm alarm )
{
  AlarmManager& instance = getInstanceRef();
  instance.mAlarms |= 1u << static_cast<u8>( alarm );
  instance.mAlarmEvent.schedule();
}

void AlarmManager::onAlarmEvent( ThisType* t, u8 callCnt )
{
  redBlocks::unused( callCnt );
  for ( u8 i = 0; i < sizeof( alarmMessages ) / sizeof( const char* ); ++i )
  {
    if ( 0 != ( t->mAlarms & ( 1u << i ) ) )
    {
      Platform::GsmModemDriver::sendSms( number, alarmMessages[i] );
    }
  }
  t->mAlarms = 0;
}
