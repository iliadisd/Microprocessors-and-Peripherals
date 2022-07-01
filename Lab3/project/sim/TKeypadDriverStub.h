#ifndef T_KEYPAD_DRIVER_STUB_H_
#define T_KEYPAD_DRIVER_STUB_H_

#include <HAL/TDriverIsrCallback.h>
#include <Simulators/IDriverStubCallback.h>
#include <Simulators/SimulationClients/ComChannelSimulationClient.h>


/**
 * Driver stub for a key pad. It works on a communication channel and invokes the callback
 * @ref Callback::CBK_ON_KEY_PRESSED when a byte is received.
 *
 * @tparam T_COM_CHANNEL_INDEX specifies the number of the communication channel, this driver
 * stub uses to exchange data with the simulator
 */
template<unsigned int T_COM_CHANNEL_INDEX>
class TKeypadDriverStub: public redBlocks::Core::TGlobalPtrBase<TKeypadDriverStub<T_COM_CHANNEL_INDEX> >
{
  public:

    enum Callback
    {
      CBK_ON_KEY_PRESSED
    };

    /**
     * This method initializes the driver stub (by adding the underlying logical communication channel).
     * @attention Use it only during the initialization phase, as it dynamically allocates memory.
     */
    static void add()
    {
      TKeypadDriverStub* self = new TKeypadDriverStub();
      redBlocks::Simulators::ComChannelSimulationClient::add( T_COM_CHANNEL_INDEX, 20, 20, &self->mInternal );
      redBlocks::Simulators::ComChannelSimulationClient::enableRecvDataCallback( T_COM_CHANNEL_INDEX );
    }

    /**
     * Should be called from the interrupt callback in order to retrieve the key that was pressed.
     */
    static u8 getKey()
    {
      return TKeypadDriverStub<T_COM_CHANNEL_INDEX>::getInstanceRef().mInternal.mKey;
    }

  private:

    /**
     * Internal class that implements the interface ComChannelSimulationClient::IRecvCallback
     * in order to get notified, when a message is received via the communication channel.
     */
    class Internal: public redBlocks::Simulators::IDriverStubCallback
    {
      public:
        virtual void onDriverStubCallback()
        {
          redBlocks::Simulators::ComChannelSimulationClient::RecvMsg* msg = redBlocks::Simulators::ComChannelSimulationClient::getNextMessage( T_COM_CHANNEL_INDEX );
          if( NULL != msg )
          {
            // only use the first byte of the message, the rest is ignored
            msg->read( &mKey, sizeof( mKey ) );
            // invoke the callback
            RB_INVOKE_ISR_CBK( CBK_ON_KEY_PRESSED );
          }
        }

        /** the key tha was pressed */
        u8 mKey;
    };

    /** private single instance of class Internal */
    Internal mInternal;

    /**
     * Hide the constructor, as this class is meant to be instantiated only via the public method add
     */
    TKeypadDriverStub() {}
};

#endif // T_KEYPAD_DRIVER_STUB_H_
