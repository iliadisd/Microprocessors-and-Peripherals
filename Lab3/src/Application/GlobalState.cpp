#include "GlobalState.h"


GlobalState::GlobalState( VendingMode& vendingMode, MaintenanceMode& maintenanceMode ) :
  mDoorOpenEvent( this ),
  mDoorCloseEvent( this ),
  mVendingMode( vendingMode ),
  mMaintenanceMode( maintenanceMode )
{
  if ( Platform::FrontDoorProtection::getValue() )
  {
    mState = &mVendingState;
    mVendingMode.enter();
  }
  else
  {
    mState = &mMaintenanceState;
    mMaintenanceMode.enter();
  }
}
