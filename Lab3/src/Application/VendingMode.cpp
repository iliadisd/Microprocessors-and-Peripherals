#include "VendingMode.h"
#include "AlarmManager.h"
#include <Gui/VendingScreen.h>

#include <Core/Log/Log.h>//Lab code - added the logger to output its value



VendingMode::VendingMode() :
  onFillupBtnPressed( &mInner ),
  onReleaseMoneyBtnPressed( &mInner )
{
}

void VendingMode::enter()
{
  mInner.mCashBox.setActivationState( true );
  mInner.mVendingState = Inner::STATE_WAITING;
  VendingScreen::drawPrompt();
}

void VendingMode::leaveImmediate()
{
  mInner.mCashBox.setActivationState( false );
  if ( mInner.mVendingState == Inner::STATE_PRODUCT_SELECTED )
  {
    mInner.mSlotActors[mInner.mSelectedProduct].switchLamp( false );
  }

  mInner.mTimer.stop();
  mInner.mVendingState = Inner::STATE_INACTIVE;
  for ( u8 i = 0; i < NUM_SLOTS; ++i )
  {
    mInner.mSlotActors[i].switchLamp( false );
    mInner.mSlotActors[i].switchFlap( false );
  }
  Platform::DispenseBayLamp::setOutput( false );
  Platform::DisplayBacklight::setOutput( true );
}


VendingMode::Inner::Inner() :
  mTimer( this ),
  mProductButtonPressedEvent( this )
{
  mVendingState = STATE_INACTIVE;
  mProductSlots[0].configure( 10, 240 );
  mProductSlots[1].configure( 10, 220 );
  mProductSlots[2].configure( 10, 180 );
  mProductSlots[3].configure( 10, 200 );

  mSlotActors[0].switchLamp = Platform::Product1Lamp::setOutput;
  mSlotActors[1].switchLamp = Platform::Product2Lamp::setOutput;
  mSlotActors[2].switchLamp = Platform::Product3Lamp::setOutput;
  mSlotActors[3].switchLamp = Platform::Product4Lamp::setOutput;

  mSlotActors[0].switchFlap = Platform::Product1ReleaseGear::setOutput;
  mSlotActors[1].switchFlap = Platform::Product2ReleaseGear::setOutput;
  mSlotActors[2].switchFlap = Platform::Product3ReleaseGear::setOutput;
  mSlotActors[3].switchFlap = Platform::Product4ReleaseGear::setOutput;

  for ( u8 i = 0; i < NUM_SLOTS; ++i )
  {
    mProductSlots[i].setActive( true );
  }

  mCashBox.setActivationState( false );

  Platform::CoinDetectorDriver::getInstanceRef().setCallback( *this );
}

void VendingMode::Inner::onCoinsInserted( u16 money )
{
  mCashBox.onMoneyInserted( money );

  bool isProductValid =
    mVendingState == STATE_PRODUCT_SELECTED &&
    mProductSlots[mSelectedProduct].getIsActive() &&
    ( 0 < mProductSlots[mSelectedProduct].getItemCount() );

  if ( isProductValid || ( mVendingState == STATE_WAITING ) )
  {
    u16 price = 0;
    if ( mVendingState == STATE_PRODUCT_SELECTED )
    {
      price = mProductSlots[mSelectedProduct].getPricePerItem();
    }

    if ( isProductValid && ( mProductSlots[mSelectedProduct].getPricePerItem() <= mCashBox.getMoneyInIntermediateCash() ) )
    {
      startReleaseWorkflow();
			getChange(mProductSlots[mSelectedProduct].getPricePerItem(), mCashBox.getMoneyInIntermediateCash()); //Lab code

    }
    else
    {
      VendingScreen::drawPriceInfo( price, mCashBox.getMoneyInIntermediateCash() );
    }
  }
}


void VendingMode::Inner::onFillupButtonPressedEvent( ThisType* t, u8 callCnt )
{
  for ( u8 i = 0; i < NUM_SLOTS; ++i )
  {
    t->mProductSlots[i].setActive( true );
  }
}

void VendingMode::Inner::onProductButtonPressedEvent( ThisType* t, u8 callCnt )
{
  if ( ( t->mVendingState == STATE_WAITING ) || ( t->mVendingState == STATE_PRODUCT_SELECTED ) )
  {
    for ( u8 i = 0; i < NUM_SLOTS; ++i )
    {
      t->mSlotActors[i].switchLamp( false );
    }

    t->mVendingState = STATE_PRODUCT_SELECTED;
    t->mSlotActors[t->mSelectedProduct].switchLamp( true );
    if ( ( !t->mProductSlots[t->mSelectedProduct].getIsActive() ) || ( 0 == t->mProductSlots[t->mSelectedProduct].getItemCount() ) )
    {
      VendingScreen::drawProductNotAvailableMessage();
    }
    else
    {
      if ( t->mProductSlots[t->mSelectedProduct].getPricePerItem() <= t->mCashBox.getMoneyInIntermediateCash() )
      {
        t->startReleaseWorkflow();
				getChange(t->mProductSlots[t->mSelectedProduct].getPricePerItem(), t->mCashBox.getMoneyInIntermediateCash()); //Lab code
      }
      else
      {
        VendingScreen::drawPriceInfo( t->mProductSlots[t->mSelectedProduct].getPricePerItem(), t->mCashBox.getMoneyInIntermediateCash() );
      }
    }
  }
}

void VendingMode::Inner::onReleaseMoneyButtonPressedEvent( ThisType* t, u8 callCnt )
{
  if ( ( t->mVendingState == STATE_WAITING ) || ( t->mVendingState == STATE_PRODUCT_SELECTED ) )
  {
    if ( t->mVendingState == STATE_PRODUCT_SELECTED )
    {
      t->mSlotActors[t->mSelectedProduct].switchLamp( false );
    }
    t->mCashBox.releaseMoney();
    VendingScreen::drawReturningMoneyMessage();
    t->mVendingState = STATE_RETURN_MONEY;
    t->mTimer.start( Platform::OS::Sec<3>::value );
  }
}

void VendingMode::Inner::onTimerEvent( ThisType* t, u8 callCnt )
{
  redBlocks::unused( callCnt );
  switch ( t->mVendingState )
  {
    case STATE_FLAP_OPEN:
      t->mSlotActors[t->mSelectedProduct].switchFlap( false );
      if ( 2 == t->mReleaseLightBarrierEventCnt )
      {
        t->mCashBox.boxMoney();
        ( void )t->mProductSlots[t->mSelectedProduct].itemSold();
        t->mVendingState = STATE_LAMP_ON;
        t->mReleaseStateCounter = 0;
        VendingScreen::drawTakeProductMessage();
        t->mTimer.start( Platform::OS::Sec<1>::value );
      }
      else
      {
        ++t->mReleaseStateCounter;
        if ( t->mReleaseStateCounter > 3 )
        {
          VendingScreen::drawProductErrorMessage();
          t->mCashBox.releaseMoney();
          t->mProductSlots[t->mSelectedProduct].setActive( false );
          AlarmManager::raiseAlarm( AlarmManager::ALARM_SLOT_BLOCKED );
          t->mVendingState = STATE_RETURN_MONEY;
          t->mTimer.start( Platform::OS::Sec<3>::value );
        }
        else
        {
          t->mVendingState = STATE_FLAP_RETRY;
          t->mTimer.start( Platform::OS::MilliSec<500>::value );
        }
      }
      break;

    case STATE_FLAP_RETRY:
      t->mSlotActors[t->mSelectedProduct].switchFlap( true );
      t->mVendingState = STATE_FLAP_OPEN;
      t->mTimer.start( Platform::OS::Sec<1>::value );
      break;

    case STATE_LAMP_ON:
      Platform::DispenseBayLamp::setOutput( false );
      Platform::DisplayBacklight::setOutput( true );
      t->mVendingState = STATE_LAMP_OFF;
      t->mTimer.start( Platform::OS::MilliSec<500>::value );
      break;

    case STATE_LAMP_OFF:
      ++t->mReleaseStateCounter;
      if ( t->mReleaseStateCounter > 5 )
      {
        t->mVendingState = STATE_WAITING;
        VendingScreen::drawPrompt();
      }
      else
      {
        Platform::DispenseBayLamp::setOutput( true );
        Platform::DisplayBacklight::setOutput( false );
        t->mVendingState = STATE_LAMP_ON;
        t->mTimer.start( Platform::OS::MilliSec<500>::value );
      }
      break;
    case STATE_RETURN_MONEY:
      t->mVendingState = STATE_WAITING;
      VendingScreen::drawPrompt();
    case STATE_WAITING:
    default:
      // do nothing
      break;
  }
}

void VendingMode::Inner::startReleaseWorkflow()
{
  mSlotActors[mSelectedProduct].switchLamp( false );
  VendingScreen::drawReleaseMessage();
  mReleaseLightBarrierEventCnt = 0;
  mVendingState = STATE_FLAP_OPEN;
  mReleaseStateCounter = 0;
  mSlotActors[mSelectedProduct].switchFlap( true );
  mTimer.start( Platform::OS::Sec<1>::value );
}

//Lab code - gets price and money as inputs, outputs their difference if money-price>0 and outputs it in the logger
extern "C"
{	
	void getChange(u16 price, u16 money)
	{
		u16 change = 0;
		if((money-price)>0){
			change = money - price;
			RB_LOG_DEBUG( change );

			}
	}
}