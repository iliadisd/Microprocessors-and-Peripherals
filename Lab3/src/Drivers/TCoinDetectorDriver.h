#ifndef T_COIN_DETECTOR_DRIVER_H_
#define T_COIN_DETECTOR_DRIVER_H_

#include <Core/Base/IBase.h>
#include <Core/Base/TGlobalPtrBase.h>

/**
 * High level driver whose method @ref onCoinDetectorEvent() must be called from
 * the receive ISR routine, when a byte is received from the communication interface
 * via which the coin detector is connected
 * @tparam T_COM_INTERFACE the communication interface class via which the coin detector
 *   messages are received
 * @tparam T_EVENT_SCHEDULER the class that provides the methods to schedule
 *   mainloop events
 */
template <class T_COM_INTERFACE, class T_EVENT_SCHEDULER>
class TCoinDetectorDriver : public redBlocks::Core::TGlobalPtrBase<TCoinDetectorDriver<T_COM_INTERFACE, T_EVENT_SCHEDULER> >
{
  private:
    /** More convenient definition of the own type. */
    typedef TCoinDetectorDriver<T_COM_INTERFACE, T_EVENT_SCHEDULER> ThisType;

  public:
    /**
     * An instance implementing this class must be associated with this driver instance
     * and gets called, when one or more coins were inserted.
     */
    class ICoinsInsertedCallback : public redBlocks::Core::IBase
    {
      public:
        virtual void onCoinsInserted( u16 money ) = 0;
    };


    /**
     * Constructor
     */
    TCoinDetectorDriver() :
      mMoneyChangedEvent( this ),
      mCallback( NULL ),
      mMoney( 0 )
    {
      T_COM_INTERFACE::enableRecvDataCallback();
    }

    /**
     * Associates the callback with this driver.
     * @param callback reference to the ICoinsInsertedCallback that is invoked from a
     *   synchronous event, after one ore more coins have been inserted
     */
    void setCallback( ICoinsInsertedCallback& callback )
    {
      mCallback = &callback;
    }

    /**
     * This method must be called from the ISR callback, when a data byte is received
     * from the coin detector.
     */
    void onCoinDetectorEvent()
    {
      u16 amount = 0;
      u8 coin;
      // prevent another interrupt to interfere with this call
      T_COM_INTERFACE::disableRecvDataCallback();
      if ( 1 == T_COM_INTERFACE::getRecvData( &coin, 1 ) )
      {
        switch( coin )
        {
          case 0x30:
            amount = 10;
            break;
          case 0x31:
            amount = 20;
            break;
          case 0x32:
            amount = 50;
            break;
          case 0x33:
            amount = 100;
            break;
          case 0x34:
            amount = 200;
            break;
        }
        mMoney += amount;
      }
      T_COM_INTERFACE::enableRecvDataCallback();
      if ( amount != 0 )
      {
        mMoneyChangedEvent.schedule();
      }
    }

  private:
    /**
     * This method is associated with mMoneyChangedEvent in order to be called from the
     * mainloop context after onCoinDetectorEvent() has been called from ISR context
     */
    static void onMoneyChangedEvent( ThisType* t, u8 callCnt )
    {
      T_COM_INTERFACE::disableRecvDataCallback();
      u16 money = t->mMoney;
      t->mMoney = 0;
      T_COM_INTERFACE::enableRecvDataCallback();

      if ( NULL != t->mCallback )
      {
        t->mCallback->onCoinsInserted( money );
      }
    }

    /** event instance that is scheduled from the ISR routine onCoinDetectorEvent() */
    typename T_EVENT_SCHEDULER::template TEvent<ThisType, &ThisType::onMoneyChangedEvent> mMoneyChangedEvent;

    /** the callback that wants to be informed, when the money has changed */
    ICoinsInsertedCallback* mCallback;

    /** the current money amount in cent */
    u16 mMoney;
};

#endif // T_COIN_DETECTOR_DRIVER_H_
