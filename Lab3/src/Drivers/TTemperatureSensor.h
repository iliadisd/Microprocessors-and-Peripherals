#ifndef T_TEMPERATURE_SENSOR_H_
#define T_TEMPERATURE_SENSOR_H_

#include <HAL/TLinearInterpolator.h>

/**
 * This class works on an analog input, and maps the values from there to a
 * temperature value in the range from -10 to +40 degrees centigrade.
 */
template <class T_ANALOG_INPUT>
class TTemperatureSensor
{
  public:
    /** Constructor */
    TTemperatureSensor();
    /**
     * Returns the last sampled value in degrees * 1000, i. e. -5000 is returned for
     *   -5 degrees
     * @return the last sampled value
     */
    s32 getLastSampledTemperature()
    {
      // uses the interpolator to map the values according to the sensor curve
      return mInterpolator.interpolate( T_ANALOG_INPUT::getValue() );
    }

    void sampleTemperature()
    {
      T_ANALOG_INPUT::sampleOnce();
    }

  private:
    /** the interpolator type that is used */
    typedef redBlocks::HAL::TLinearInterpolator<u8, s32> InterpolatorType;
    /** the curve data */
    static const InterpolatorType::Point mSensorCurve[];
    /** the interpolator instance */
    InterpolatorType mInterpolator;
};

// compile time initialization of the sensor curve
template <class T_ANALOG_INPUT>
const typename TTemperatureSensor<T_ANALOG_INPUT>::InterpolatorType::Point
TTemperatureSensor<T_ANALOG_INPUT>::mSensorCurve[3] =
{
  {0x00, -10000},
  {0x80, +10000},
  {0xFF, +40000}
};

template <class T_ANALOG_INPUT>
TTemperatureSensor<T_ANALOG_INPUT>::TTemperatureSensor()
  : mInterpolator( mSensorCurve, 3 )
{
}

#endif // T_TEMPERATURE_SENSOR_H_
