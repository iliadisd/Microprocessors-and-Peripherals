#include <Platform.h>
#include <PlatformCallbacks.h>
#include <Application/Application.h>
#include <Core/Log/Log.h>
#include <Memory/HeapManager/HeapManager.h>

#include <Simulators/ClientPosixTcpSimulatorConnection.h>
#include <Simulators/TPosixSimulationClientEnvironmentThread.h>
#include <OS/Posix/Posix.h>

#include <stdlib.h>

/**
 * Main routine of our simulator application. It sets up the single components,
 * associates them with each other and starts the application.
 *
 * Arguments:
 * binary.exe [port]
 */
int main( int argc, char* argv[] )
{
  const u16 port = ( argc >= 2 ) ? atoi( argv[1] ) : 10000;

  typedef redBlocks::Memory::HeapManager::THeapManagerStaticAlloc<100000> HeapManager;
  HeapManager::init();

  // Connect to the simulator via TCP connection.
  redBlocks::Simulators::ClientPosixTcpSimulatorConnection simulatorConnection;
  simulatorConnection.connect( "127.0.0.1", port );

  // Setup the simulation client environment.
  // We choose the thread based version, as we are working on top of an operating system.
  redBlocks::Simulators::TPosixSimulationClientEnvironmentThread<> simClientEnv;

  // Initialize the simulation client.
  // The system tick needs to be enabled before entering this method, otherwise this method will never return.
  redBlocks::Simulators::SimulationClient simulationClient( simulatorConnection, simClientEnv );

  // Driver initialization.
  Platform::initLowLevelDrivers();
  Platform::initHighLevelDrivers();

  // Initialization of all simulation stubs finished.
  simulationClient.initDone();

  // Initialize application.
  Application app;

  // Prevent dynamic memory allocation after this point.
  HeapManager::disableAllocation();

  RB_LOG_DEBUG( "Application initialized, starting event processing" );
  RB_LOG_DEBUG( "Heap memory used: " << HeapManager::getUsed() );

  app.run();
}

