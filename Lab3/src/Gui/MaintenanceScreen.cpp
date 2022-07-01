#include <Core/Error/Error.h>
#include "MaintenanceScreen.h"
#include "Gui.h"


static void drawTwoLineMessage( const char* line1, const char* line2 )
{
  Gui::clear( GuiColorMap::BACKGROUND );
  Gui::paint( GuiResources::getInstanceRef().fontArial20.text( line1 ), Gui::CenterHorizontal(), Gui::BottomAlign( Gui::Display::getHeight() / 2 ), GuiColorMap::FOREGROUND );
  Gui::paint( GuiResources::getInstanceRef().fontArial20.text( line2 ), Gui::CenterHorizontal(), Gui::TopAlign( Gui::Display::getHeight() / 2 ), GuiColorMap::FOREGROUND );
  Gui::update();
}


void MaintenanceScreen::drawAlarmCodePromptMessage( u8 numDigits )
{
  RB_FATAL_ASSERT( numDigits <= 4 );
  const char codeStr[] = "####";
  char code[sizeof( codeStr )];
  ::memcpy( code, codeStr, sizeof( codeStr ) );

  Gui::CoordinateType width = GuiResources::getInstanceRef().fontArial20.text( code ).getWidth();
  Gui::CoordinateType x = 0;
  if ( width < Gui::Display::getWidth() )
  {
    x = ( Gui::Display::getWidth() - width ) / 2;
  }
  Gui::clear( GuiColorMap::BACKGROUND );
  Gui::paint( GuiResources::getInstanceRef().fontArial20.text( "Enter code!" ), Gui::CenterHorizontal(), Gui::BottomAlign( Gui::Display::getHeight() / 2 ), GuiColorMap::FOREGROUND );
  if ( 0 != numDigits )
  {
    code[numDigits] = 0;
    Gui::paint( GuiResources::getInstanceRef().fontArial20.text( code ), Gui::LeftAlign( x ), Gui::TopAlign( Gui::Display::getHeight() / 2 ), GuiColorMap::FOREGROUND );
  }
  Gui::update();
}

void MaintenanceScreen::drawMaintenanceModeMessage()
{
  drawTwoLineMessage( "Maintenance", "Mode" );
}
