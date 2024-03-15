#ifndef DGuiStatusBar_H
#define DGuiStatusBar_H

#include <raygui.h>
#include <DGuiWidget.h>
#include <DGuiLabel.h>
#include <map>

class DGuiStatusBar : public DGuiWidget
{
    public:
        typedef DGuiLabel DStatusBarItem;

        DGuiStatusBar(int LeftPos, int TopPos, int ControlWidth, int ControlHeight, DGuiWidget *ParentWidget);
        DGuiStatusBar(Rectangle WidgetBounds, DGuiWidget *ParentWidget);
        //DGuiStatusBar(DDocking DockPos = DOCK_BOTTOM, int SideSize , DGuiWidget *ParentWidget = nullptr);
        //TODO? DGuiStatusBar(std::string JsonFilename);

        static DGuiStatusBar* LoadFromJson(std::string JsonFilename, DGuiWidget *Parent);

        //void SetParent(DGuiWidget *Parent) override;

        void AddItem(std::string ItemName, int Left, int Width, std::string ItemText);
        DStatusBarItem* GetItem(std::string ItemName);

        void SetItemText(std::string ItemName, std::string ItemText);

        void Draw() override;

        std::map<std::string,DStatusBarItem> Items;
};

#endif