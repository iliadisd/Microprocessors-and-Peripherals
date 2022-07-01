#include "CashBox.h"
#include <Core/Log/Log.h>

CashBox::CashBox() :
  mTimer( this )
{
  mIsActive = false;
  mMoneyInIntermediateCash = 0;
  mState = STATE_IDLE;
  mStateCounter = 0;
}

void CashBox::setActivationState( bool isActivated )
{
  if ( !isActivated )
  {
    mTimer.stop();
    Platform::CashBoxReturnActor::setOutput( false );
    Platform::ChangeBayLamp::setOutput( false );
    mMoneyInIntermediateCash = 0;
    mState = STATE_IDLE;
  }
  mIsActive = isActivated;
  Platform::OpenCashBoxActor::setOutput( isActivated );
}

void CashBox::onMoneyInserted( u16 money )
{
  if ( mIsActive && ( mState == STATE_IDLE ) )
  {
    mMoneyInIntermediateCash += money;
    RB_LOG_DEBUG( "Money inserted: " << money << " cent" );
  }
}

void CashBox::releaseMoney()
{
  if ( mIsActive )
  {
    startReleaseMoney();
  }
}

void CashBox::boxMoney()
{
  if ( mIsActive )
  {
    startBoxMoney();
  }
}


void CashBox::onTimerEvent( ThisType* t, u8 callCnt )
{
  redBlocks::unused( callCnt );
  if ( t->mIsActive )
  {
    switch ( t->mState )
    {
      case STATE_RELEASE_MONEY:
        ++t->mStateCounter;
        Platform::ChangeBayLamp::setOutput( 0 == t->mStateCounter % 2 );
        if ( 9 == t->mStateCounter )
        {
          t->mTimer.stop();
          Platform::OpenCashBoxActor::setOutput( true );
          t->mState = STATE_IDLE;
        }
        break;
      case STATE_BOX_MONEY1:
        Platform::OpenCashBoxActor::setOutput( false );
        t->mTimer.start( Platform::OS::Sec<3>::value );
        t->mState = STATE_BOX_MONEY2;
        break;
      case STATE_BOX_MONEY2:
        Platform::OpenCashBoxActor::setOutput( true );
        Platform::CashBoxReturnActor::setOutput( false );
        t->mState = STATE_IDLE;
        break;
      case STATE_IDLE:
      default:
        break;
    }
  }
}

void CashBox::startReleaseMoney()
{
  RB_LOG_DEBUG( "Releasing money" );
  mMoneyInIntermediateCash = 0;

  Platform::OpenCashBoxActor::setOutput( false );
  Platform::ChangeBayLamp::setOutput( true );
  mTimer.startPeriodic( Platform::OS::MilliSec<500>::value );
  mState = STATE_RELEASE_MONEY;
  mStateCounter = 0;
}

void CashBox::startBoxMoney()
{
  RB_LOG_DEBUG( "Boxing money: " << mMoneyInIntermediateCash << " cent" )
  mMoneyInIntermediateCash = 0;

  Platform::CashBoxReturnActor::setOutput( true );
  mTimer.start( Platform::OS::Sec<1>::value );
  mState = STATE_BOX_MONEY1;
}
