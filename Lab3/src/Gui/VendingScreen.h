#ifndef VENDING_SCREEN_H_
#define VENDING_SCREEN_H_

#include <Core/Base/NoInstanceBase.h>

/**
 * This class paints price and payment information to the graphical display.
 */
class VendingScreen : public redBlocks::Core::NoInstanceBase
{
  public:
    static void drawPriceInfo( u16 price, u16 money );
    static void drawProductNotAvailableMessage();
    static void drawProductErrorMessage();
    static void drawReturningMoneyMessage();
    static void drawReleaseMessage();
    static void drawTakeProductMessage();
    static void drawPrompt();
};

#endif // VENDING_SCREEN_H_
