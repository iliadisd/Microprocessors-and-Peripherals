#ifndef MAINTENANCE_SCREEN_H_
#define MAINTENANCE_SCREEN_H_

#include <Core/Base/NoInstanceBase.h>

/**
 * This class paints maintenance messages to the screen.
 */
class MaintenanceScreen : public redBlocks::Core::NoInstanceBase
{
  public:
    static void drawAlarmCodePromptMessage( u8 numDigits );
    static void drawMaintenanceModeMessage();
    static void drawFillupConfirmationMessage();
};

#endif // MAINTENANCE_SCREEN_H_
