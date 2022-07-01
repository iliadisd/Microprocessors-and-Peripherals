#ifndef MAINTENANCE_MODE_H_
#define MAINTENANCE_MODE_H_

#include "Platform.h"

#include <Core/Base/NoCopyBase.h>
#include <HAL/TDigitalOutputFlasher.h>

/**
 * This class handles the alarm mechanism when entering the maintenance mode.
 * Upon entering the maintenance mode, the service personnel has a certain time period to identify
 * itself by inserting the right code via the keypad. Each key event is acknowledged via a short
 * acoustic beep. The inserted code is checked after 4 digits have been entered. In case
 * the correct code was inserted, a long beep, in case of a wrong code, three short
 * beeps are issued. While the time period elapses, the "Alarm" LED flashes (the flashing accelerates
 * every few seconds). When the correct code is inserted, the LED is continuously switched on.
 * If the time expires without the right code being inserted, an alarm is raised via the AlarmManager.
 */
class MaintenanceMode : public redBlocks::Core::NoCopyBase
{
  public:
    /**
     * Constructor
     */
    MaintenanceMode();

    /**
     * Enters the maintenance mode (when the door is opened)
     */
    void enter();

    /**
     * Tries to leave the maintenance mode (leaving the maintenance mode is only allowed,
     * when the right code was entered)
     */
    bool leave();

    /**
     * This method must be called, when a button was pressed. It can be
     * called from interrupt context, as it only schedules an event and returns immediately.
     * @param[in] key the digit (0..9) that was inserted
     */
    void onKeyPressed( u8 key )
    {
      mKeyPressed = key;
      Platform::OS::scheduleEvent( mKeyPressedEvent );
    }

  private:
    /** Convenience definition */
    typedef MaintenanceMode ThisType;

    enum AlarmState
    {
      ALARM_STATE_IDLE = 0,
      ALARM_STATE_KEYCODE,
      ALARM_STATE_ACCESS_GRANTED,
      ALARM_STATE_ALARM
    };

    enum KeyCodeState
    {
      KEY_CODE_STATE_IGNORE = 0,
      KEY_CODE_STATE_INPUT,
      KEY_CODE_STATE_CONFIRM_KEY_BEEP,
      KEY_CODE_STATE_CONFIRM_CODE_BEEP,
      KEY_CODE_STATE_ERROR_BEEP
    };

    /** Checks whether the inserted key code is correct */
    bool checkKeyCode() const;

    /** This method is called from mSoundTimer */
    static void onSoundTimerEvent( ThisType* t );

    /** This method is called from mCountdownTimer */
    static void onCountdownTimerEvent( ThisType* t, u8 callCnt );

    /** This method is called, when a key pressed event has been generated */
    static void onKeyPressedEvent( ThisType* t, u8 callCnt );

    /** timer that is used to give feedback, when a button was pressed */
    Platform::OS::TIsrTimer<ThisType, &onSoundTimerEvent> mSoundTimer;

    /** timer that is used for the countdown after the door was opened */
    Platform::OS::TEventTimer<ThisType, &onCountdownTimerEvent> mCountdownTimer;

    /** event that is scheduled, when a button was pressed */
    Platform::OS::TEvent<ThisType, &onKeyPressedEvent> mKeyPressedEvent;

    /** the key that has been pressed */
    u8 mKeyPressed;

    /** the current alarm state */
    AlarmState mAlarmState;

    /** the counter used in certain alarm states */
    u8 mAlarmStateCounter;

    /** the current keycode state */
    KeyCodeState mKeyCodeState;

    /** the counter used in certain keycode states */
    u8 mKeyCodeStateCount;

    /** the keycode that has been input */
    u8 mKeyCode[4];

    /** the helper class that is used to let the lamp flash */
    redBlocks::HAL::TDigitalOutputFlasher<Platform::KeyPadLamp, Platform::OS> mLampFlasher;

};

#endif
