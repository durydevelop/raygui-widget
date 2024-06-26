#ifndef DGuiWidget_H
#define DGuiWidget_H

#include <raygui.h>
#include <string>
#include <map>
#include <DGuiCommon.h>
#include <dpplib/DPreferences.h>

struct DRglControl {
    DWidgetType WidgetType=DWidgetType::UNKNOWN;
    Rectangle Bounds{0,0,0,0};
    std::string Text;
    std::string Name;
};

/**
 * @brief DGuiWidget base class.
 * This is the base class for all widgets from which all widgets must be descended.
 */
class DGuiWidget
{
    protected:
        inline static const std::map<DWidgetType,std::string> WidgetTypes = {
            {DLABEL,     "Label"},
            {DBUTTON,    "Button"},
            {DEDIT,      "Edit"},
            {DSTATUSBAR, "StatusBar"},
            {DCONTAINER, "Container"},
        };
        struct DJsonTree {
            inline static std::string SEC_CHILDREN="Children";
            inline static const std::string SEC_BOUNDS="Bounds";
            inline static const std::string SEC_STATUSBAR_ITEMS="StatusBarItems";

            inline static const std::string ITEM_NAME="Name";
            //inline static const std::string ITEM_ANCHOR_ID="AnchorId";
            inline static const std::string ITEM_TYPE="Type";
            inline static const std::string ITEM_TEXT="Text";
            inline static const std::string ITEM_TEXT_SIZE="TextSize";
            inline static const std::string ITEM_TEXT_ALIGN_H="TextAlignH";
            inline static const std::string ITEM_TEXT_ALIGN_V="TextAlignV";
            inline static const std::string ITEM_LEFT="Left";
            inline static const std::string ITEM_TOP="Top";
            inline static const std::string ITEM_WIDTH="Width";
            inline static const std::string ITEM_HEIGHT="Height";
            inline static const std::string ITEM_DOCKING="Docking";
            inline static const std::string ITEM_SIDE="Side";
            inline static const std::string ITEM_SIZE="Size";
            inline static const std::string ITEM_BOUNDS="Bounds";
            inline static const std::string ITEM_READ_ONLY="ReadOnly";
            inline static const std::string ITEM_PASSWORD_MODE="PasswordMode";
            inline static const std::string ITEM_MAX_TEXT_LENGHT="MaxTextLenght";
            inline static const std::string ITEM_ENABLED="Enabled";
            inline static const std::string ITEM_VISIBLE="Visible";
            inline static const std::string ITEM_SHOW_BORDER="ShowBorder";
            inline static const std::string ITEM_BORDER_WIDTH="BorderWidth";

            inline static const std::string VALUE_BOTTOM="Bottom";
            inline static const std::string VALUE_TOP="Top";
            inline static const std::string VALUE_LEFT="Left";
            inline static const std::string VALUE_RIGHT="Right";
            inline static const std::string VALUE_CENTER="Center";
        };

    public:
        enum DDocking { DOCK_HCENTER=-7, DOCK_VCENTER=-6, DOCK_CENTER=-5, DOCK_TOP=-4, DOCK_BOTTOM=-3, DOCK_RIGHT=-2, DOCK_LEFT=-1 };
        enum DWidth { WIDTH_AUTO=-1 };
        enum DTextAlignH { TEXT_ALIGN_HLEFT=TEXT_ALIGN_LEFT, TEXT_ALIGN_HCENTER=TEXT_ALIGN_CENTER, TEXT_ALIGN_HRIGHT=TEXT_ALIGN_RIGHT };
        enum DTextAlignV { TEXT_ALIGN_VTOP=TEXT_ALIGN_TOP, TEXT_ALIGN_VCENTER=TEXT_ALIGN_CENTER, TEXT_ALIGN_VBOTTOM=TEXT_ALIGN_BOTTOM };
        typedef struct DTextAlign{
            DTextAlignH Horiz=TEXT_ALIGN_HLEFT;
            DTextAlignV Vert=TEXT_ALIGN_VCENTER;
        } DTextAlign;

        struct DProperties {
            // Text (defaults are set in SetWidgetType())
            unsigned int TextColor;
            int TextPadding;
            DTextAlign TextAlign;
            int TextSize;
            int TextSpacing;

            // Color (defaults are set in SetWidgetType())
            int BorderWidth;
            unsigned int BorderColor;
            unsigned int LineColor;
            unsigned int BackGroundColor;

            // Dinamic behaviours (defaults are set here)
            bool Enabled=true;
            bool Visible=true;
            bool ShowBorder=false;

            //std::string AnchorId; // deprecated
        }Properties;
        
        DGuiWidget(DWidgetType WidgetType, int LeftPos, int TopPos, int WidgetWidth, int WidgetHeight, DGuiWidget *ParentWidget, OnWidgetEventCallback EventCallback = nullptr);
        DGuiWidget(DWidgetType WidgetType, Rectangle WidgetBounds, DGuiWidget *ParentWidget, OnWidgetEventCallback EventCallback = nullptr);
        DGuiWidget(DWidgetType WidgetType, DDocking DockingPos, int OtherSize, DGuiWidget *ParentWidget, OnWidgetEventCallback EventCallback = nullptr);
        DGuiWidget(DTools::DTree WidgetTree, DGuiWidget* ParentWidget, OnWidgetEventCallback EventCallback = nullptr);
        
        static DGuiWidget* New(DTools::DTree& WidgetTree, DGuiWidget* ParentWidget = nullptr, OnWidgetEventCallback EventCallback = nullptr);
        static DGuiWidget* New(const std::string& LayoutFilename, DGuiWidget* ParentWidget = nullptr, OnWidgetEventCallback EventCallback = nullptr);

        /// Virtual method that MUST be implemented by sub-class
        virtual void Draw() = 0;
        
        /// Virtual methods that can be reimplemented
        virtual void SetText(std::string NewText);
        virtual const std::string& GetText(void);
        virtual void SetOnGuiEvent(OnGuiEventCallback Callback);

        /// Static methods
//        static DGuiWidget* New(const std::string& Filename, DGuiWidget *Parent);
//        static DGuiWidget* New(DTools::DTree WidgetTree, DGuiWidget* Parent);
//        static DGuiWidget* New(DWidgetType WidgetType, int LeftPos, int TopPos, int WidgetWidth, int WidgetHeight, std::string Text, DGuiWidget *Parent);
//        static DGuiWidget* New(DWidgetType WidgetType, Rectangle WidgetBounds, std::string Text, DGuiWidget *Parent);
        static DTools::DTree ExtractDTree(const std::string& Filename);
        static std::string RglToJson(std::string Filename);
        static DRglControl DecodeRglLine(std::string Line);
        static DWidgetType NameToType(const std::string& WidgetTypeName);
        static std::string TypeToName(DWidgetType WidgetType);

        void GenerateId(void);
        void Clear(void);
        void Draws(void);

        void SetWidgetType(DWidgetType WidgetType);
        void SetTextSize(int NewSize);
        void SetWidth(int Width);
        void SetHeight(int Height);
        void SetParent(DGuiWidget *Parent);
        void SetTextAlign(std::string AlignHoriz, std::string AlignVert);
        void SetOnWidgetEvent(OnWidgetEventCallback Callback);
        void SendEvent(DWidgetEvent WidgetEvent);
        void SetPos(int LeftPos, int TopPos);
        void SetSize(int Width, int Height);
        void SetDocking(DDocking DockingPos, int OtherSize);
        void SetDocking(std::string DockingSideName, int OtherSize);
        void SetBounds(int LeftPos, int TopPos, int Width, int Height);
        void SetBounds(Rectangle WidgetBounds);
        void SetBorderWidth(uint8_t NewWidth);
        void SetEnabled(bool Enabled);
        void SetVisible(bool Visible);

        int GetTextSize(void);
        size_t GetWidth(void);
        size_t GetHeight(void);
        DGuiWidget* GetParent(void);
        DWidgetType GetWidgetType(void);
        std::string GetWidgetTypeName(void);
        std::string GetId(void);
        std::string GetLastError(void);
        bool IsReady(void);

        std::string Name;
        Rectangle Bounds;
        std::string Text;

    protected:
        int DEFAULT_SIDE_SIZE=20;   //! Used for set docking position, will be overrided by widget subclass
        int DEFAULT_WIDTH=50;       //! Used for set default witdh, will be overrided by widget subclass
        int DEFAULT_HEIGHT=50;      //! Used for set default height, will be overrided by widget subclass

        DWidgetType Type;
        DGuiWidget *Parent;
        
        OnGuiEventCallback OnGuiEvent;
        OnWidgetEventCallback OnWidgetEvent;
        std::string LastError;
        bool Ready;

        void BackupCurrentGuiStyle(void);
        void SetWidgetGuiStyle(void);
        void RestoreCurrentGuiStyle(void);

    private:
        std::string Id;
        DProperties TempStyle;

        bool InitFromTree(DTools::DTree& WidgetTree);
};
#endif