// TODO
// When changing the device in the project settings, adjust the following include accordingly and
// set the heap and stack size in the startup file to reasonable values (as this project includes
// a redBlocks heap manager, which maintains its memory in the global data section, the heap size
// in the startup file can be set to 0).
#include <ARMCM0.h>

#include <Platform.h>
#include <PlatformCallbacks.h>
#include <Application/Application.h>
#include <Core/Log/Log.h>
#include <Memory/HeapManager/HeapManager.h>

#include <HAL/Drivers/Cortex/CortexMSystemTickDriver.h>
#include <Simulators/TargetConnection/UVisionClientTargetConnection.h>
#include <Simulators/TSystemTickSimulationClientEnvironment.h>
#include <libredBlocks_sim.h> //Lab code - needed library


typedef redBlocks::Simulators::UVisionClientTargetConnection TargetConnectionType;

typedef redBlocks::HAL::Drivers::CortexMSystemTickDriver SystemTickDriver;
RB_CONNECT_ISR_CBK( SystemTickDriver, SystemTickDriver::CBK_ON_SYSTEM_TICK, redBlocks::Simulators::SimulationClient::tick() );

static const u32 SYSTEM_TICKS_PER_SEC = 100;

extern "C"
{
  void SysTick_Handler( void )
  {
    SystemTickDriver::onIsr();
  }
}

int main( void )
{
  // For correct timing, you need to make sure this value corresponds to the setting
  // "Xtal (MHz)" in the Keil uVision target option settings (e. g. 8.0 MHz => 8000000)
  SystemCoreClock = 8000000;

  typedef redBlocks::Memory::HeapManager::THeapManagerStaticAlloc<6000> HeapManager;
  HeapManager::init();

  // The data with the redBlocks Simulator is exchanged via a target memory area, the
  // redBlocks Simulator accesses via the uVision UVSOCK interface. The memory region
  // must be exclusively reserved for this purpose (e. g. via the Keil uVision target
  // option settings, tab Debug -> Initialization File.
  // With the following line, the memory at address 0x30000000 and a default size of
  // 1052 bytes is used (52 bytes for control information plus a send and receive
  // buffer with 500 bytes each).
  TargetConnectionType simulatorConnection( 0x30000000 );
  simulatorConnection.connect();

  // Setup the simulation client environment.
  redBlocks::Simulators::TSystemTickSimulationClientEnvironment<SystemTickDriver> simClientEnv;

  // Enable system tick.
  SystemTickDriver::configure( SYSTEM_TICKS_PER_SEC );
  SystemTickDriver::enableCallback();

  // Initialize the simulation client.
  // The system tick needs to be enabled before entering this method, otherwise this method will never return.
  redBlocks::Simulators::SimulationClient simulationClient( simulatorConnection, simClientEnv );

  // Driver initialization.
  Platform::initLowLevelDrivers();
  Platform::initHighLevelDrivers();

  // Initialization of all simulation stubs finished.
  simulationClient.initDone();
	rb_sim_DigitalInput_add( 9, RB_TRIGGER_EDGE_RISING, NULL ); //Lab Code - Adds a new digital input interface with index 9 (as indicated in the simulator)

  // Application initialization.
  Application app;

  // Prevent dynamic memory allocation after this point.
  HeapManager::disableAllocation();

  RB_LOG_DEBUG( "Application initialized, starting event processing" );
  RB_LOG_DEBUG( "Heap memory used:" << HeapManager::getUsed() );

		
	app.run();
}
