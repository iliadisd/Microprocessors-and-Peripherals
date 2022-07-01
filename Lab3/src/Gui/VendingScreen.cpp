#include <Core/Utils/AsciiConverter.h>

#include "VendingScreen.h"
#include "Gui.h"


static void drawMoneyValue( Gui::Font& font, Gui::CoordinateType posX, Gui::CoordinateType posY, u16 value )
{
  Gui::Font::Text komma = GuiResources::getInstanceRef().fontArial20.text( "," );

  redBlocks::Core::AsciiConverter::Decimal::StringRepresentation<u8> value1Str( value / 100 );
  redBlocks::Core::AsciiConverter::Decimal::StringRepresentation<u8> value2Str( value % 100 );
  // fill with leading 0s and skip the first 0, as we only need 2 digits
  value2Str.fill( '0' );
  Gui::Font::Text value1 = GuiResources::getInstanceRef().fontArial20.text( value1Str.getData() );
  Gui::Font::Text value2 = GuiResources::getInstanceRef().fontArial20.text( value2Str.getData() + 1 );

  Gui::CoordinateType width = value1.getWidth() + komma.getWidth() + value2.getWidth();
  if ( posX > width )
  {
    posX -= width;
  }
  else
  {
    posX = 0;
  }

  Gui::paint( value1, Gui::LeftAlign( posX ), Gui::CenterVertical( posY ), GuiColorMap::FOREGROUND );
  posX += value1.getWidth();
  Gui::paint( komma, Gui::LeftAlign( posX ), Gui::CenterVertical( posY ), GuiColorMap::FOREGROUND );
  posX += komma.getWidth();
  Gui::paint( value2, Gui::LeftAlign( posX ), Gui::CenterVertical( posY ), GuiColorMap::FOREGROUND );
}

static void drawTwoLineMessage( const char* line1, const char* line2 )
{
  Gui::clear( GuiColorMap::BACKGROUND );
  Gui::paint( GuiResources::getInstanceRef().fontArial20.text( line1 ), Gui::CenterHorizontal(), Gui::BottomAlign( Gui::Display::getHeight() / 2 ), GuiColorMap::FOREGROUND );
  Gui::paint( GuiResources::getInstanceRef().fontArial20.text( line2 ), Gui::CenterHorizontal(), Gui::TopAlign( Gui::Display::getHeight() / 2 ), GuiColorMap::FOREGROUND );
  Gui::update();
}


void VendingScreen::drawPriceInfo( u16 price, u16 money )
{
  const Gui::CoordinateType left = 15;
  const Gui::CoordinateType right = Gui::Display::getWidth() - 15;
  const Gui::CoordinateType firstLine = 5 + ( Gui::Display::getHeight() - 10 ) / 4;
  const Gui::CoordinateType secondLine = 5 + ( ( Gui::Display::getHeight() - 10 ) / 4 ) * 3;

  Gui::clear( GuiColorMap::BACKGROUND );

  if ( 0 == price )
  {
    Gui::paint(
      GuiResources::getInstanceRef().fontArial20.text( "Select product" ),
      Gui::CenterHorizontal(),
      Gui::CenterVertical( firstLine ),
      GuiColorMap::FOREGROUND
    );
  }
  else
  {
    Gui::paint(
      GuiResources::getInstanceRef().fontArial20.text( "Price:" ),
      Gui::LeftAlign( left ),
      Gui::CenterVertical( firstLine ),
      GuiColorMap::FOREGROUND
    );

    drawMoneyValue( GuiResources::getInstanceRef().fontArial20, right, firstLine, price );
  }

  Gui::paint(
    GuiResources::getInstanceRef().bitmaskCoins,
    Gui::LeftAlign( left + 5 ),
    Gui::CenterVertical( secondLine ),
    GuiColorMap::FOREGROUND
  );
  drawMoneyValue( GuiResources::getInstanceRef().fontArial20, right, secondLine, money );


  Gui::update();
}

void VendingScreen::drawProductNotAvailableMessage()
{
  drawTwoLineMessage( "Not available!", "Select other" );
}


void VendingScreen::drawProductErrorMessage()
{
  drawTwoLineMessage( "Error!", "Coins returned" );
}

void VendingScreen::drawReturningMoneyMessage()
{
  drawTwoLineMessage( "Your money", "is returned" );
}

void VendingScreen::drawReleaseMessage()
{
  drawTwoLineMessage( "Your product", "is released!" );
}

void VendingScreen::drawTakeProductMessage()
{
  drawTwoLineMessage( "Remove your", "product!" );
}

void VendingScreen::drawPrompt()
{
  drawTwoLineMessage( "Select a", "product!" );
}
