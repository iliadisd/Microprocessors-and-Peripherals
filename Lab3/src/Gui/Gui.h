#ifndef GUI_H_
#define GUI_H_

#include <Core/Base/TGlobalPtrBase.h>
#include <Platform.h>
#include <GUI/TGui.h>
#include "GuiData/Arial20.h"
#include "GuiData/Images.h"

typedef redBlocks::GUI::TGui<Platform::Display> Gui;

struct GuiResources : public redBlocks::Core::TGlobalPtrBase<GuiResources>
{
  GuiResources() :
    fontArial20( GuiData::Arial20::data ),
    bitmaskCoins( GuiData::Images::coins )
  {}

  Gui::Font fontArial20;
  Gui::Bitmask bitmaskCoins;
};

struct GuiColorMap
{
  static const Gui::ColorValueType FOREGROUND = 1;
  static const Gui::ColorValueType BACKGROUND = 0;
};





#endif // GUI_H_
