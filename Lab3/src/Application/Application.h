#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "Platform.h"
#include "CashBox.h"
#include "MaintenanceMode.h"
#include "VendingMode.h"
#include "GlobalState.h"
#include "TTemperatureController.h"
#include "AlarmManager.h"
#include <Gui/Gui.h>
#include <Drivers/TTemperatureSensor.h>
#include <Core/Base/TGlobalPtrBase.h>
#include <Core/Log/Log.h>

/**
 * Example application that provides the functionality: Each time an input
 * changes, the output in 100 ms steps.
 */
class Application: public redBlocks::Core::TGlobalPtrBase<Application>
{
  public:
    typedef TTemperatureController<TTemperatureSensor<Platform::TemperatureSensor>, Platform::CoolingSystemActor> TemperatureController;


    Application();

    void run() const;

    static MaintenanceMode& getMaintenanceMode()
    {
      return getInstanceRef().mMaintenanceMode;
    }

    static VendingMode& getVendingMode()
    {
      return getInstanceRef().mVendingMode;
    }

    static TemperatureController& getTemperatureController()
    {
      return getInstanceRef().mTemperatureController;
    }

    static GlobalState& getGlobalState()
    {
      return getInstanceRef().mGlobalState;
    }

  private:
    /** Convenience definition for our class */
    typedef Application ThisType;

    /** This method is associated with mTemperatureControllerTimer */
    static void onTemperatureControllerTimerEvent( ThisType* t, u8 callCnt );
    /** used to periodically invoke the temperature controller */
    Platform::OS::TEventTimer<ThisType, &onTemperatureControllerTimerEvent> mTemperatureControllerTimer;

    GuiResources mGuiResources;
    AlarmManager mAlarmManager;
    MaintenanceMode mMaintenanceMode;
    TemperatureController mTemperatureController;
    VendingMode mVendingMode;
    GlobalState mGlobalState;
};

#endif // APPLICATION_H_
