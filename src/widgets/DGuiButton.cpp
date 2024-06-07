#include "DGuiButton.h"
//#include "raywui_log.h"

DGuiButton::DGuiButton(int LeftPos, int TopPos, int ControlWidth, int ControlHeight, DGuiWidget *ParentWidget, OnWidgetEventCallback EventCallback) : DGuiWidget(DBUTTON,LeftPos,TopPos,ControlWidth,ControlHeight,ParentWidget,EventCallback)
{
    DEFAULT_SIDE_SIZE=50;
}
        
DGuiButton::DGuiButton(Rectangle WidgetBounds, DGuiWidget *ParentWidget, OnWidgetEventCallback EventCallback) : DGuiWidget(DBUTTON,WidgetBounds,ParentWidget,EventCallback)
{
    DEFAULT_SIDE_SIZE=50;
    DEFAULT_WIDTH=50;
    DEFAULT_HEIGHT=20;
}

/**
 * @brief Draw the button.
 *
 * @return the #Result value of this widget.
 */
void DGuiButton::Draw()
{
    if (GuiButton(Bounds, Text.c_str())) {
        DWidgetEvent Event={DEventCode::BUTTON_PRESS, nullptr};
        SendEvent(Event);
    }
}
