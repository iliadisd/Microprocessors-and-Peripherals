#ifndef PLATFORM_H_
#define PLATFORM_H_

#include "LowLevelPlatform.h"
#include <Core/Log/TStandardLogger.h>
#include <HAL/TBufferedComInterfaceWriter.h>
#include <OS/MainLoop/TMainLoopScheduler.h>
#include <Drivers/TCoinDetectorDriver.h>
#include <Drivers/TGsmModemDriver.h>

/**
 * This class enhances the low layer hardware abstraction with high level (and thus
 * hardware independent) driver functionality.
 */
class Platform : public LowLevelPlatform
{
  public:
    typedef redBlocks::OS::TMainLoopScheduler<LowLevelPlatform::IsrLock, 1, 1, LowLevelPlatform::PowerSaveCallback> OS;
    typedef redBlocks::HAL::TBufferedComInterfaceWriter<Platform::LogComInterface> BufferedLogComInterfaceWriter;
    typedef redBlocks::Core::Log::TGlobalStandardLogger<Platform::BufferedLogComInterfaceWriter> StandardLogger;
    typedef TCoinDetectorDriver<Platform::CoinDetectorComInterface, Platform::OS> CoinDetectorDriver;
    typedef TGsmModemDriver<Platform::GsmModemComInterface> GsmModemDriver;

    static void initHighLevelDrivers()
    {
      new BufferedLogComInterfaceWriter( 400 );
      new StandardLogger();
      new CoinDetectorDriver();
      new GsmModemDriver();
    }
};

// callback routing from low level drivers to high level drivers
RB_CONNECT_ISR_CBK( Platform::SystemTickDriver, Platform::SystemTickDriver::CBK_ON_SYSTEM_TICK, Platform::OS::tick() )
RB_CONNECT_ISR_CBK( Platform::LogComInterface, Platform::LogComInterface::CBK_ON_RECV_DATA, /** Empty */ )
RB_CONNECT_ISR_CBK( Platform::LogComInterface, Platform::LogComInterface::CBK_ON_SEND_READY, Platform::BufferedLogComInterfaceWriter::onSendReadyCallback() )
RB_CONNECT_ISR_CBK( Platform::CoinDetectorComInterface, Platform::CoinDetectorComInterface::CBK_ON_RECV_DATA, Platform::CoinDetectorDriver::getInstanceRef().onCoinDetectorEvent() )
RB_CONNECT_ISR_CBK( Platform::CoinDetectorComInterface, Platform::CoinDetectorComInterface::CBK_ON_SEND_READY, /** Empty */ )
RB_CONNECT_ISR_CBK( Platform::GsmModemComInterface, Platform::GsmModemComInterface::CBK_ON_RECV_DATA, /** Empty */ )
RB_CONNECT_ISR_CBK( Platform::GsmModemComInterface, Platform::GsmModemComInterface::CBK_ON_SEND_READY, Platform::GsmModemDriver::onSendReadyCallback() )
RB_CONNECT_ISR_CBK( Platform::ConfigurationComInterface, Platform::ConfigurationComInterface::CBK_ON_RECV_DATA, /** Empty */ )
RB_CONNECT_ISR_CBK( Platform::ConfigurationComInterface, Platform::ConfigurationComInterface::CBK_ON_SEND_READY, /** Empty */ )

#endif // PLATFORM_H_
