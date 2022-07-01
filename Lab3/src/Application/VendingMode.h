#ifndef VENDING_MODE_H_
#define VENDING_MODE_H_

#include "CashBox.h"
#include "ProductSlot.h"
#include <Drivers/TCoinDetectorDriver.h>

#include <Platform.h>
#include <Core/Base/IBase.h>
#include <Core/Base/NoCopyBase.h>

extern "C" void getChange(u16 price, u16 money);//Lab code

/**
 * This class controls all product slots and the associated buttons
 */
class VendingMode: public redBlocks::Core::NoCopyBase
{
  public:
    /** the number of slots this product controller supports */
    static const u8 NUM_SLOTS = 4;

  private:
    /**
     * This class implements the logic of class ProductController. The callback interface
     * CashBox::IMoneyChangedCallback is implemented by this inner class, in order to keep
     * the interface of the outer class clean.
     */
    class Inner : public Platform::CoinDetectorDriver::ICoinsInsertedCallback
    {
      public:
        /** More convenient definition of the own type. */
        typedef Inner ThisType;
        /**
         * The states during the vending workflow
         */
        enum State
        {
          STATE_INACTIVE = 0,
          STATE_WAITING,
          STATE_PRODUCT_SELECTED,
          STATE_FLAP_OPEN,
          STATE_FLAP_RETRY,
          STATE_LAMP_ON,
          STATE_LAMP_OFF,
          STATE_RETURN_MONEY
        };

        /**
         * Constructor
         */
        Inner();

        /**
         * This method is called by Platform::CoinDetectorDiriver when coins were inserted.
         * @see ICoinsInsertedCallback
         */
        virtual void onCoinsInserted( u16 money );

        /**
         * Starts the workflow to release the currently selected product
         */
        void startReleaseWorkflow();

        /**
         * Structure that holds pointers to the methods that allow to switch the lamp
         * and product release flap that correspond to a product slot
         */
        struct SlotActors
        {
          void ( *switchLamp )( bool on );
          void ( *switchFlap )( bool open );
        };

        /**
         * This event handler method is associated with mTimer.
         */
        static void onTimerEvent( ThisType*, u8 callCnt );

        /**
         * This event handler method is associated with mFillupButtonPressedEvent
         */
        static void onFillupButtonPressedEvent( ThisType* t, u8 callCnt );

        /**
         * This event handler method is associated with mProductButtonPressedEvent
         */
        static void onProductButtonPressedEvent( ThisType* t, u8 callCnt );

        /**
         * This event handler method is associated with mReleaseMoneyButtonPressedEvent
         */
        static void onReleaseMoneyButtonPressedEvent( ThisType* t, u8 callCnt );


        /** used to invoke the method onTimerEvent after certain time periods */
        Platform::OS::TEventTimer<ThisType, &onTimerEvent> mTimer;
        /** event that is scheduled, when a product button is pressed */
        Platform::OS::TEvent<ThisType, &onProductButtonPressedEvent> mProductButtonPressedEvent;
        /** the instances representing a product slot */
        ProductSlot mProductSlots[NUM_SLOTS];
        /** array of function pointers via which the slot actors can be controlled */
        SlotActors mSlotActors[NUM_SLOTS];
        /** the cash box instance associated with this product controller */
        CashBox mCashBox;
        /** the currently selected product */
        u8 mSelectedProduct;
        /** the state the released item is in */
        State mVendingState;
        /** the counter that is used within a state */
        u8 mReleaseStateCounter;
        /** will be incremented, when the light barrier level changes during the release workflow */
        u8 mReleaseLightBarrierEventCnt;
    };

    Inner mInner;

  public:
    /**
     * Constructor
     */
    VendingMode();

    /**
     * Called when the vending mode is entered.
     */
    void enter();

    /**
     * Called when the vending mode needs to be terminated (e. g. because
     * the door was opened. It opens the cash box and does no longer react on
     * button events.
     */
    void leaveImmediate();

    /**
     * Called, when a product release button is pressed. It can be called from ISR
     * context, as it does not perform any lenghty operations.
     * @param btnIndex index of the button that was pressed
     */
    void onProductBtnPressed( u8 btnIndex )
    {
      mInner.mSelectedProduct = btnIndex;
      Platform::OS::scheduleEvent( mInner.mProductButtonPressedEvent );
    }

    /**
     * Called when the light barrier input level changes.
     */
    void onLightBarrierEvent()
    {
      ++mInner.mReleaseLightBarrierEventCnt;
    }

    /**
     * Functor that must be called, when the fillup buttion is pressed. It can be
     * called from ISR context, as it only schedules an event and returns immediately.
     */
    Platform::OS::TEventFunc<Inner, &Inner::onFillupButtonPressedEvent> onFillupBtnPressed;

    /**
     * Functor that must be called, when the release money button was pressed. It can
     * be called from ISR context, as it only schedules an event and returns immediately.
     */
    Platform::OS::TEventFunc<Inner, &Inner::onReleaseMoneyButtonPressedEvent> onReleaseMoneyBtnPressed;
};

#endif // VENDING_MODE_H_
