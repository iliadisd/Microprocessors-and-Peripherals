#ifndef GLOBAL_STATE_H_
#define GLOBAL_STATE_H_

#include "Platform.h"
#include <Core/Base/NoCopyBase.h>
#include "VendingMode.h"
#include "MaintenanceMode.h"

/**
 * This class keeps track of the application's global state.
 * It is an example of the state design pattern implementation.
 */
class GlobalState : public redBlocks::Core::NoCopyBase
{
  private:
    /** Convenience definition for our class */
    typedef GlobalState ThisType;


    /**
     * Base class for all states: It defines a method for each trigger and
     * provides default implementations for all these methods that do nothing.
     */
    class BaseState : public redBlocks::Core::NoCopyBase
    {
      public:
        virtual void onDoorOpen( GlobalState& context )
        {
          // do nothing per default
        }
        virtual void onDoorClose( GlobalState& context )
        {
          // do nothing per default
        }
    };


    /**
     * State class that overwrites the trigger's default behavior (i. e. do nothing)
     * for the vending state.
     */
    class VendingState : public BaseState
    {
      public:
        virtual void onDoorOpen( GlobalState& context )
        {
          context.mVendingMode.leaveImmediate();
          context.mState = &context.mMaintenanceState;
          context.mMaintenanceMode.enter();
        }
    };
    friend class VendingState;

    /**
     * State class that overwrites the trigger's default behavior (i. e. do nothing)
     * for the maintenance state.
     */
    class MaintenanceState : public BaseState
    {
      public:
        virtual void onDoorClose( GlobalState& context )
        {
          if ( context.mMaintenanceMode.leave() )
          {
            context.mState = &context.mVendingState;
            context.mVendingMode.enter();
          }
        }
    };
    friend class MaintenanceState;

  public:
    /**
     * Constructor
     */
    GlobalState( VendingMode& vendingMode, MaintenanceMode& maintenanceMode );

    /**
     * This method must be called, when the front door changes.
     * It generates an event
     */
    void onFrontDoorContactChange()
    {
      if ( Platform::FrontDoorProtection::getValue() )
      {
        mDoorCloseEvent.schedule();
      }
      else
      {
        mDoorOpenEvent.schedule();
      }
    }

  private:
    /** This method is called from mDoorOpenEvent */
    static void onDoorOpenEvent( ThisType* t, u8 callCnt )
    {
      t->mState->onDoorOpen( *t );
    }

    /** This method is called from mDoorCloseEvent */
    static void onDoorCloseEvent( ThisType* t, u8 callCnt )
    {
      t->mState->onDoorClose( *t );
    }

    /** event that is scheduled, when the door is opened */
    Platform::OS::TEvent<ThisType, &onDoorOpenEvent> mDoorOpenEvent;
    /** event that is scheduled, when the door is closed */
    Platform::OS::TEvent<ThisType, &onDoorCloseEvent> mDoorCloseEvent;
    /** the current state */
    BaseState* mState;
    /** the state object for the vending state */
    VendingState mVendingState;
    /** the state object for the vending state */
    MaintenanceState mMaintenanceState;
    /** reference to the vending mode state machine */
    VendingMode& mVendingMode;
    /** reference to the maintenance mode state machine */
    MaintenanceMode& mMaintenanceMode;
};

#endif
