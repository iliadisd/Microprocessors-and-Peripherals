#ifndef ALARM_MANAGER_H_
#define ALARM_MANAGER_H_

#include "Platform.h"
#include <Core/Base/TGlobalPtrBase.h>

/**
 * This class deals with all alarm messages, sending an SMS via
 * the GSM modem.
 */
class AlarmManager : public redBlocks::Core::TGlobalPtrBase<AlarmManager>
{
  public:
    enum Alarm
    {
      ALARM_BREAK_IN = 0,
      ALARM_OVERHEAT,
      ALARM_EMPTY_SLOT,
      ALARM_SLOT_BLOCKED
    };

    /**
     * Constructor
     */
    AlarmManager();

    /**
     * Raises the specified alarm. This method can be called from interrupt
     * context, as is only sets a flag and schedules an Event
     * @param alarm the alarm to raise
     */
    static void raiseAlarm( Alarm alarm );

  private:
    /** Convenience definition for our class */
    typedef AlarmManager ThisType;
    /**
     * This method is associated with mAlarmEvent
     */
    static void onAlarmEvent( ThisType* t, u8 callCnt );
    /** this event is scheduled by raiseAlarm() */
    Platform::OS::TEvent<ThisType, &onAlarmEvent> mAlarmEvent;
    /** bit field of active alarms */
    u32 mAlarms;
};

#endif
