#ifndef CASH_BOX_H_
#define CASH_BOX_H_

#include <Platform.h>
#include <Core/Base/IBase.h>
#include <Core/Base/NoCopyBase.h>

/**
 * This class maintains the cash box, i. e. it controls the cash box flaps
 * and it must be informed when a coin is inserted.
 */
class CashBox : public redBlocks::Core::NoCopyBase
{
  public:

    /**
     * Constructor
     */
    CashBox();

    /**
     * Activate / deactivate the cash box (if deactivated, the release flap is opened, so
     * all money is falling through).
     * @param isActivated true, if the cash box is active
     */
    void setActivationState( bool isActivated );

    /**
     * Needs to be called, when money was inserted
     */
    void onMoneyInserted( u16 money );

    /**
     * Opens the cash box release flap and switches on the return bay lamp
     */
    void releaseMoney();

    /**
     * Boxes the money
     */
    void boxMoney();

    /**
     * Returns the information, if a workflow is currently in progress
     */
    bool isMoneyWorkflowInProgress() const
    {
      return mState != STATE_IDLE;
    }

    /**
     * Returns the information, how much money is in the intermediate cash
     */
    u16 getMoneyInIntermediateCash() const
    {
      return mMoneyInIntermediateCash;
    }

  private:
    /** Convenience definition for our class */
    typedef CashBox ThisType;
    /**
     * The internal workflow state
     */
    enum State
    {
      STATE_IDLE = 0,
      STATE_RELEASE_MONEY,
      STATE_BOX_MONEY1,
      STATE_BOX_MONEY2
    };
    /**
     * This method gets called, when the timer fires.
     */
    static void onTimerEvent( ThisType* t, u8 callCnt );

    /**
     * Releases the money
     */
    void startReleaseMoney();

    /**
     * Boxes the money
     */
    void startBoxMoney();

    /** used to invoke the method onTimerEvent after certain time periods */
    Platform::OS::TEventTimer<ThisType, &onTimerEvent> mTimer;
    /** true, if the cash box is active */
    bool mIsActive;
    /** the money that is in the intermediate cash box in cent */
    u16 mMoneyInIntermediateCash;
    /** the current workflow state */
    State mState;
    /** the counter that is used in certain substates */
    u8 mStateCounter;
};

#endif // CASH_BOX_H_
