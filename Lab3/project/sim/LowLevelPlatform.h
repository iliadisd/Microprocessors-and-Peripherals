#ifndef LOW_LEVEL_PLATFORM_H_
#define LOW_LEVEL_PLATFORM_H_

#include <Core/Base/NoInstanceBase.h>
#include <SimulationEnvironment.h>
#include "TKeypadDriverStub.h"
#include <OS/MainLoop/DefaultSimulationPowerSaveCallback.h>

/**
 * This class provides a low layer hardware abstraction, making the
 * layers above (high level drivers and application) independent of the
 * platform they are running on.
 * It defines the low level drivers for the simulation environment.
 */
template< unsigned int T = 0 >
class TLowLevelPlatform : public redBlocks::Core::NoInstanceBase
{
  public:

    typedef redBlocks::HAL::Drivers::TDigitalInputDriverStub<0> PowerSupervision;
    typedef redBlocks::HAL::Drivers::TDigitalInputDriverStub<1> LightBarrier;
    typedef redBlocks::HAL::Drivers::TDigitalInputDriverStub<2> FrontDoorProtection;
    typedef redBlocks::HAL::Drivers::TDigitalInputDriverStub<3> ReturnMoneyButton;
    typedef redBlocks::HAL::Drivers::TDigitalInputDriverStub<4> FilledUpButton;
    typedef redBlocks::HAL::Drivers::TDigitalInputDriverStub<5> Product1Button;
    typedef redBlocks::HAL::Drivers::TDigitalInputDriverStub<6> Product2Button;
    typedef redBlocks::HAL::Drivers::TDigitalInputDriverStub<7> Product3Button;
    typedef redBlocks::HAL::Drivers::TDigitalInputDriverStub<8> Product4Button;

    typedef redBlocks::HAL::Drivers::TDigitalOutputDriverStub<0> CashBoxReturnActor;
    typedef redBlocks::HAL::Drivers::TDigitalOutputDriverStub<1> OpenCashBoxActor;
    typedef redBlocks::HAL::Drivers::TDigitalOutputDriverStub<2> CoolingSystemActor;
    typedef redBlocks::HAL::Drivers::TDigitalOutputDriverStub<3> PowerDisabledAlarmOutput;
    typedef redBlocks::HAL::Drivers::TDigitalOutputDriverStub<4> KeyPadLamp;
    typedef redBlocks::HAL::Drivers::TDigitalOutputDriverStub<5> KeySound;
    typedef redBlocks::HAL::Drivers::TDigitalOutputDriverStub<6> ChangeBayLamp;
    typedef redBlocks::HAL::Drivers::TDigitalOutputDriverStub<7> DispenseBayLamp;
    typedef redBlocks::HAL::Drivers::TDigitalOutputDriverStub<8> Product1ReleaseGear;
    typedef redBlocks::HAL::Drivers::TDigitalOutputDriverStub<9> Product2ReleaseGear;
    typedef redBlocks::HAL::Drivers::TDigitalOutputDriverStub<10> Product3ReleaseGear;
    typedef redBlocks::HAL::Drivers::TDigitalOutputDriverStub<11> Product4ReleaseGear;
    typedef redBlocks::HAL::Drivers::TDigitalOutputDriverStub<12> Product1Lamp;
    typedef redBlocks::HAL::Drivers::TDigitalOutputDriverStub<13> Product2Lamp;
    typedef redBlocks::HAL::Drivers::TDigitalOutputDriverStub<14> Product3Lamp;
    typedef redBlocks::HAL::Drivers::TDigitalOutputDriverStub<15> Product4Lamp;
    typedef redBlocks::HAL::Drivers::TDigitalOutputDriverStub<16> DisplayBacklight;

    typedef redBlocks::HAL::Drivers::TAnalogInputDriverStub<0> TemperatureSensor;

    typedef redBlocks::HAL::Drivers::TComInterfaceDriverStub<0> LogComInterface;
    typedef redBlocks::HAL::Drivers::TComInterfaceDriverStub<1> GsmModemComInterface;
    typedef redBlocks::HAL::Drivers::TComInterfaceDriverStub<2> CoinDetectorComInterface;
    typedef redBlocks::HAL::Drivers::TComInterfaceDriverStub<3> ConfigurationComInterface;

    typedef redBlocks::HAL::Drivers::TDisplayDriverStub<0> Display;

    typedef TKeypadDriverStub<10> Keypad;

    typedef redBlocks::Simulators::SimulationStubs::SystemTick SystemTickDriver;
    typedef redBlocks::Simulators::SimulationStubs::IsrLock IsrLock;
    typedef redBlocks::OS::MainLoopScheduler::DefaultSimulationPowerSaveCallback PowerSaveCallback;

    /**
     * Entering the sleep mode is delegated to the simulation client
     */
    static void enterSleepMode()
    {
      redBlocks::Simulators::SimulationStubs::SleepMode::enter();
    }

    static void initLowLevelDrivers()
    {
      typedef redBlocks::Simulators::SimulationStubs SimulationStubs;

      // configure system ticks: per simulation tick we generate 10 system ticks
      SimulationStubs::SystemTick::configure( 10, 1 );

      SimulationStubs::DigitalInput::add<PowerSupervision>();
      SimulationStubs::DigitalInput::add<LightBarrier>( SimulationStubs::DigitalInput::TRIGGER_EDGE_BOTH );
      SimulationStubs::DigitalInput::add<FrontDoorProtection>( SimulationStubs::DigitalInput::TRIGGER_EDGE_BOTH );
      SimulationStubs::DigitalInput::add<ReturnMoneyButton>( SimulationStubs::DigitalInput::TRIGGER_EDGE_RISING );
      SimulationStubs::DigitalInput::add<FilledUpButton>( SimulationStubs::DigitalInput::TRIGGER_EDGE_RISING );
      SimulationStubs::DigitalInput::add<Product1Button>( SimulationStubs::DigitalInput::TRIGGER_EDGE_RISING );
      SimulationStubs::DigitalInput::add<Product2Button>( SimulationStubs::DigitalInput::TRIGGER_EDGE_RISING );
      SimulationStubs::DigitalInput::add<Product3Button>( SimulationStubs::DigitalInput::TRIGGER_EDGE_RISING );
      SimulationStubs::DigitalInput::add<Product4Button>( SimulationStubs::DigitalInput::TRIGGER_EDGE_RISING );

      SimulationStubs::DigitalOutput::add<CashBoxReturnActor>( false );
      SimulationStubs::DigitalOutput::add<OpenCashBoxActor>( false );
      SimulationStubs::DigitalOutput::add<CoolingSystemActor>( false );
      SimulationStubs::DigitalOutput::add<PowerDisabledAlarmOutput>( false );
      SimulationStubs::DigitalOutput::add<KeyPadLamp>( false );
      SimulationStubs::DigitalOutput::add<KeySound>( false );
      SimulationStubs::DigitalOutput::add<ChangeBayLamp>( false );
      SimulationStubs::DigitalOutput::add<DispenseBayLamp>( false );
      SimulationStubs::DigitalOutput::add<Product1ReleaseGear>( false );
      SimulationStubs::DigitalOutput::add<Product1ReleaseGear>( false );
      SimulationStubs::DigitalOutput::add<Product2ReleaseGear>( false );
      SimulationStubs::DigitalOutput::add<Product3ReleaseGear>( false );
      SimulationStubs::DigitalOutput::add<Product4ReleaseGear>( false );
      SimulationStubs::DigitalOutput::add<Product1Lamp>( false );
      SimulationStubs::DigitalOutput::add<Product2Lamp>( false );
      SimulationStubs::DigitalOutput::add<Product3Lamp>( false );
      SimulationStubs::DigitalOutput::add<Product4Lamp>( false );
      SimulationStubs::DigitalOutput::add<DisplayBacklight>( true );

      SimulationStubs::AnalogInput::add<TemperatureSensor> ( 255 );

      SimulationStubs::ComInterface::add<LogComInterface> ( 1, 1 );
      SimulationStubs::ComInterface::add<GsmModemComInterface> ( 1, 1 );
      SimulationStubs::ComInterface::add<CoinDetectorComInterface> ( 1, 1 );

      SimulationStubs::ComInterface::add<ConfigurationComInterface> ( 1, 1 );

      SimulationStubs::Display::add<Display>( 128, 64, 1 );

      Keypad::add();
    }
};

typedef TLowLevelPlatform<> LowLevelPlatform;

#endif
