#include "DGuiStatusBar.h"
//#define RAYGUI_IMPLEMENTATION
//#include <raygui.h>
#include "Log.h"
#include <DGuiContainer.h>
#include <libdpp/DPreferences.h>

DGuiStatusBar::DGuiStatusBar(int LeftPos, int TopPos, int ControlWidth, int ControlHeight, DGuiWidget *ParentWidget) : DGuiWidget(DSTATUSBAR,LeftPos,TopPos,ControlWidth,ControlHeight,ParentWidget) {
    DEFAULT_SIDE_SIZE=16;
    if (LeftPos <= 0 || TopPos <= 0 || ControlHeight <= 0 || ControlWidth <= 0) {
        // Default position
        SetDocking(DOCK_BOTTOM,DEFAULT_SIDE_SIZE);
    }
}

DGuiStatusBar::DGuiStatusBar(Rectangle WidgetBounds, DGuiWidget *ParentWidget) : DGuiWidget(DSTATUSBAR,WidgetBounds,ParentWidget) {
    DEFAULT_SIDE_SIZE=16;
    if (WidgetBounds.x <= 0 || WidgetBounds.y <= 0 || WidgetBounds.width <= 0 || WidgetBounds.height <= 0) {
        // Default position
        SetDocking(DOCK_BOTTOM,DEFAULT_SIDE_SIZE);
    }
}
/*
DGuiStatusBar::DGuiStatusBar(DDocking DockingPos, int SideSize, DGuiWidget *ParentWidget) : DGuiWidget(DSTATUSBAR,0,0,0,SideSize,ParentWidget)
{
    DEFAULT_SIDE_SIZE=16;
    SetDocking(DockingPos,SideSize);
}
*/

DGuiStatusBar* DGuiStatusBar::LoadFromJson(std::string JsonFilename, DGuiWidget *Parent) {
    DTools::DPreferences Json(JsonFilename);
    if (!Json.IsReady()) {
        Log(DLOG_ERROR,"%s cannot be opened: %s",JsonFilename.c_str(),Json.GetLastStatus().c_str());
        return nullptr;
    }
    
    DGuiWidget *Widget=DGuiWidget::New(Json.GetRootTree(),Parent);
    if (Widget->GetWidgetType() != DWidgetType::DSTATUSBAR) {
        Log(DLOG_ERROR,"Not a StatusBar widget: %s",Widget->GetWidgetTypeName().c_str());
        return nullptr;
    }

    return (DGuiStatusBar *) Widget;
}
/*
void DGuiStatusBar::SetParent(DGuiWidget *ParentContainer) {
    Parent=ParentContainer;
    if (Parent) {
        if (Bounds.width <= 0) {
            Bounds.width=Parent->GetWidth();
        }
        if (Bounds.height <= 0) {
            Bounds.height=Parent->GetHeight();
        }
    }
}
*/
/**
 * @brief 
 * 
 * @param ItemName 
 * @param LeftPos ->    -1 = FROM_LEFT -2 FROM_RIGHT
 * @param Width 
 * @param ItemText 
 */
void DGuiStatusBar::AddItem(std::string ItemName, int LeftPos, int Width, std::string ItemText) {
    if (Width == WIDTH_AUTO) {
        // Auto width
        Width=MeasureText(ItemText.c_str(),Properties.TextSize);
        //Width=(ItemText.size()*Properties.TextSize); //+((ItemText.size()-1)*Properties.TextSpacing);
        if (Width == 0) {
            // Default width
            Width=100;
        }
        else {
            // Add Padding space
            Width+=Properties.TextSize;
        }
    }
    
    switch (LeftPos) {
        case DOCK_LEFT:
            LeftPos=0;
            break;
        case DOCK_RIGHT:
            LeftPos=Bounds.x+Bounds.width-Width;
            break;
        case DOCK_CENTER:
            LeftPos=(Bounds.width/2)-(Width/2);
            break;
        default:
            break;
    }

    DStatusBarItem Item(Rectangle{Bounds.x+LeftPos, Bounds.y, (float) Width, Bounds.height},this);
    if (!ItemText.empty()) {
        Item.SetText(ItemText.c_str());
    }
    Item.Properties.TextSize=Properties.TextSize;
    Item.Properties.TextSpacing=Properties.TextSpacing;
    Item.Properties.TextPadding=2;
    Items.emplace(ItemName,std::move(Item));
}

DGuiStatusBar::DStatusBarItem* DGuiStatusBar::GetItem(std::string ItemName) {
    if (Items.contains(ItemName)) {
        return &Items.at(ItemName);
    }
    else {
        return nullptr;
    }
}

void DGuiStatusBar::SetItemText(std::string ItemName, std::string ItemText) {
    if (Items.contains(ItemName)) {
        Items.at(ItemName).SetText(ItemText);
    }
}

/**
 * @brief Draw the status bar.
 */
void DGuiStatusBar::Draw()
{
    GuiStatusBar(Bounds, "");
    for (auto [Name,Item] : Items) {
        Item.Draws();
        DrawLineEx(Vector2{Item.Bounds.x,Item.Bounds.y},Vector2{Item.Bounds.x,Item.Bounds.y+Item.Bounds.height},Properties.BorderWidth,GetColor(Properties.BorderColor));
        DrawLineEx(Vector2{Item.Bounds.x+Item.Bounds.width,Item.Bounds.y},Vector2{Item.Bounds.x+Item.Bounds.width,Item.Bounds.y+Item.Bounds.height},Properties.BorderWidth,GetColor(Properties.BorderColor));
    }
}