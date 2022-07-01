#ifndef T_TEMPERATURE_CONTROLLER_H_
#define T_TEMPERATURE_CONTROLLER_H_

#include "AlarmManager.h"

/**
 * This class works on an instance of T_TEMPERATURE_SENSOR and switches the
 * cooling actor on, if the temperature rises above the specified level plus
 * a hysteresis value. When it falls below the specified level minus the
 * hysteresis level, it switches the cooling actor off.
 * @tparam T_TEMPERATURE_SENSOR class that is used to retrieve the current
 *   temperature
 * @tparam T_DIGITAL_OUTPUT the digital output driver that controls the
 *   cooling actor
 */
template <class T_TEMPERATURE_SENSOR, class T_DIGITAL_OUTPUT>
class TTemperatureController
{
  public:
    /**
     * Constructor
     * @param[in] temperature the desired temperature
     * @param[in] hysteresis the hysteresis to use when switching the
     *   cooling actor on and off
     * @param[in] alarmTemperature if the temperature rises above this value,
     *   an alarm is issued via AlarmManager
     */
    TTemperatureController( s32 temperature, u32 hysteresis, s32 alarmTemperature ) :
      mTemperatureOn( temperature + static_cast<s32>( hysteresis ) ),
      mTemperatureOff( temperature - static_cast<s32>( hysteresis ) ),
      mTemperatureAlarm( alarmTemperature ),
      mIsCoolingOn( false ),
      mIsOverheat( false )
    {
    }

    /**
     * This method must be called regularly (in the frequency, the
     * temperature shall be controlled). It starts the temperature
     * sampling.
     */
    void runOnce()
    {
      mTemperatureSensor.sampleTemperature();
    }

    /**
     * This method must be called, when the sampling is done.
     */
    void onTemperatureAvailable()
    {
      s32 temp = mTemperatureSensor.getLastSampledTemperature();
      if ( mIsCoolingOn )
      {
        if ( temp < mTemperatureOff )
        {
          mIsCoolingOn = false;
          mIsOverheat = false;
          T_DIGITAL_OUTPUT::setOutput( false );
        }
      }
      else
      {
        if ( temp > mTemperatureOn )
        {
          mIsCoolingOn = true;
          T_DIGITAL_OUTPUT::setOutput( true );
        }
      }
      if ( ( temp > mTemperatureAlarm ) && ( !mIsOverheat ) )
      {
        mIsOverheat = true;
        AlarmManager::raiseAlarm( AlarmManager::ALARM_OVERHEAT );
      }
    }

  private:
    T_TEMPERATURE_SENSOR mTemperatureSensor;
    s32 mTemperatureOn;
    s32 mTemperatureOff;
    s32 mTemperatureAlarm;
    bool mIsCoolingOn;
    bool mIsOverheat;
};


#endif // T_TEMPERATURE_CONTROLLER_H_
