#include "DGuiWidget.h"
#include <DGuiContainer.h>
#include <DGuiButton.h>
#include <DGuiEdit.h>
#include <DGuiLabel.h>
#include <DGuiStatusBar.h>
#include <raygui.h>
#include <sstream>
#include <fstream>
#include "Log.h"
#include <libdpp/DString.h>
#include <libdpp/DPath.h>
#include <libdpp/DCsv.h>

using namespace DTools;

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
    inline static const std::string ITEM_ENABLED="Enabled";
    inline static const std::string ITEM_VISIBLE="Visible";

    inline static const std::string VALUE_BOTTOM="Bottom";
    inline static const std::string VALUE_TOP="Top";
    inline static const std::string VALUE_LEFT="Left";
    inline static const std::string VALUE_RIGHT="Right";
    inline static const std::string VALUE_CENTER="Center";
};

// Rgl layout file stuff
struct DRgl {
    static const uint8_t ControlLineItemsCount=10;
    enum ControlItemsIx{ CTRL_IX_ID=1, CTRL_IX_TYPE, CTRL_IX_NAME, CTRL_IX_X, CTRL_IX_Y, CTRL_IX_WIDTH, CTRL_IX_HEIGHT, CTRL_IX_ANCHOR_ID, CTRL_IX_TEXT };
    //enum RGL_ANCHOR_ITEMS_IX { ANCHOR_IX_ID=1, ANCHOR_IX_NAME, ANCHOR_IX_X, ANCHOR_IX_Y, ANCHOR_IX_ENABLED };
    //enum RGL_WINDOW_ITEMS_IX { REF_WINDOW_IX_X=1, REF_WINDOW_IX_Y, REF_WINDOW_WIDTH, REF_WINDOW_HEIGHT };
};

#define DOT DPreferences::DEFAULT_TRANSLATOR

/**
 * @brief Coordinate constructor.
 * 
 * @param WidgetType 
 * @param LeftPos 
 * @param TopPos 
 * @param WidgetWidth 
 * @param WidgetHeight 
 * @param EventCallback 
 */
DGuiWidget::DGuiWidget(DWidgetType WidgetType, int LeftPos, int TopPos, int WidgetWidth, int WidgetHeight, DGuiWidget *ParentWidget, OnWidgetEventCallback EventCallback)
{
    // Create id
    GenerateId();

    // Init widget
    if (LeftPos < 0) {
        LeftPos=0;
    }
    if (TopPos < 0) {
        TopPos=0;
    }
    Log(DLOG_DEBUG,"ParentWidget=%d",ParentWidget);
    Log(DLOG_DEBUG,"SetParent");
    SetParent(ParentWidget);
    Log(DLOG_DEBUG,"SetBounds");
    SetBounds(LeftPos,TopPos,WidgetWidth,WidgetHeight);
    Log(DLOG_DEBUG,"SetWidgetType");
    SetWidgetType(WidgetType);
    OnWidgetEvent=EventCallback;
};

/**
 * @brief Bounds constructor.
 * 
 * @param WidgetType 
 * @param WidgetBounds 
 * @param EventCallback 
 */
DGuiWidget::DGuiWidget(DWidgetType WidgetType, Rectangle WidgetBounds, DGuiWidget *ParentWidget, OnWidgetEventCallback EventCallback)
{
    // Create id
    GenerateId();

    // Init widget
    if (WidgetBounds.x < 0) {
        WidgetBounds.x=0;
    }
    if (WidgetBounds.y < 0) {
        WidgetBounds.y=0;
    }
    SetBounds(WidgetBounds);
    SetParent(ParentWidget);
    SetWidgetType(WidgetType);
    OnWidgetEvent=EventCallback;
};

void DGuiWidget::GenerateId(void)
{
    // Create id
    std::ostringstream ss;
    ss << (void const *)this;
    Id=ss.str();
}

void DGuiWidget::SetParent(DGuiWidget *ParentContainer) {
    Parent=ParentContainer;
    if (Parent) {
        if (Bounds.width < 0) {
            // Parent width
            SetWidth(Parent->GetWidth());
        }
        if (Bounds.height < 0) {
            // Parent Height
            SetHeight(Parent->GetHeight());
        }
    }
    else {
        // Parent null, force re-set size
        SetWidth(Bounds.width);
        SetHeight(Bounds.height);
    }
}

void DGuiWidget::SetOnWidgetEvent(OnWidgetEventCallback EventCallback)
{
    OnWidgetEvent=EventCallback;
}

void DGuiWidget::SetOnGuiEvent(OnGuiEventCallback EventCallback)
{
    OnGuiEvent=EventCallback;
}

void DGuiWidget::SendEvent(DWidgetEvent WidgetEvent)
{
    bool Handled=false;
    if (OnWidgetEvent) {
        Handled=OnWidgetEvent(WidgetEvent);
    }
    if (OnGuiEvent && !Handled) {
        // Send DGuiEvent only if OnWidgetEvent() return false
        OnGuiEvent(DGuiEvent {Type, Id, WidgetEvent});
    }
}

/**
 * @brief Set the widget position.
 * 
 * @param LeftPos   ->  left position of upper left corner.
 * @param TopPos  ->  right position of upper left corner.
 */
void DGuiWidget::SetPos(int LeftPos, int TopPos)
{
    Bounds.x=LeftPos;
    Bounds.y=TopPos;
    //Changed=true;
};

/**
 * @brief Set the widget size.
 * 
 * @param Width  ->  width of widget.
 * @param Height ->  height of widget.
 */
void DGuiWidget::SetSize(int Width, int Height)
{
    Log(DLOG_DEBUG,"Setwidth");
    SetWidth(Width);
    Log(DLOG_DEBUG,"SetHeight");
    SetHeight(Height);
};

/**
 * @brief Set the widget width.
 * 
 * @param Width  ->  width of widget.
 */
void DGuiWidget::SetWidth(int Width)
{
    if (Width < 0) {
        // Parent width
        if (Parent) {
            Log(DLOG_DEBUG,"Parent->GetWidth()");
            Bounds.width=Parent->GetWidth();
        }
        else {
            Log(DLOG_DEBUG,"GetScreenWidth()");
            Bounds.width=GetScreenWidth();
        }
    }
    else if (Width == 0) {
        // Default width
        Bounds.width=DEFAULT_WIDTH;
    }
    else {
        Bounds.width=Width;
    }
};

/**
 * @brief Set the widget height.
 * 
 * @param Height ->  height of widget.
 */
void DGuiWidget::SetHeight(int Height)
{
    if (Height < 0) {
        // Parent height
        if (Parent) {
            Bounds.height=Parent->GetHeight();
        }
        else {
            Bounds.height=GetScreenHeight();
        }
    }
    else if (Height == 0) {
        // Default height
        Bounds.height=DEFAULT_HEIGHT;
    }
    else {
        Bounds.height=Height;
    }
};

/**
 * @brief Set the widget position and size.
 * 
 * @param LeftPos   ->  left position of upper left corner.
 * @param TopPos    ->  right position of upper left corner.
 * @param Width     ->  width of widget.
 * @param Height    ->  height of widget.
 */
void DGuiWidget::SetBounds(int LeftPos, int TopPos, int Width, int Height)
{
    Log(DLOG_DEBUG,"Setpos");
    SetPos(LeftPos,TopPos);
    Log(DLOG_DEBUG,"Setsize");
    SetSize(Width,Height);
};

/**
 * @brief Set the widget position and size as #Rectange object.
 * 
 * @param WidgetBounds  ->  a #Rectangle object with bounds.
 */
void DGuiWidget::SetBounds(Rectangle WidgetBounds)
{
    Bounds=WidgetBounds;
};

/**
 * @brief Set the size of Text (if widget need text).
 * 
 * @param NewSize   ->  the new text size.
 */
void DGuiWidget::SetTextSize(int NewSize)
{
    Properties.TextSize=NewSize;
}

void DGuiWidget::SetDocking(std::string DockingSideName, int OtherSize) {
    if (DString::CmpNoCase(DockingSideName,DJsonTree::VALUE_BOTTOM)) {
        SetDocking(DGuiWidget::DOCK_BOTTOM,OtherSize);
    }
    else if (DString::CmpNoCase(DockingSideName,DJsonTree::VALUE_TOP)) {
        SetDocking(DGuiWidget::DOCK_TOP,OtherSize);
    }
    else if (DString::CmpNoCase(DockingSideName,DJsonTree::VALUE_LEFT)) {
        SetDocking(DGuiWidget::DOCK_LEFT,OtherSize);
    }
    else if (DString::CmpNoCase(DockingSideName,DJsonTree::VALUE_RIGHT)) {
        SetDocking(DGuiWidget::DOCK_RIGHT,OtherSize);
    }
    else if (DString::CmpNoCase(DockingSideName,DJsonTree::VALUE_CENTER)) {
        SetDocking(DGuiWidget::DOCK_CENTER,OtherSize);
    }
    else {
        Log(DLOG_WARNING,"Docking side unkown: %s",DockingSideName.c_str());
    }
}

/**
 * @brief 
 * 
 * @param DockingPos 
 * @param size:
 * Height if DockingPos is DOCK_LEFT or DOCK_RIGHT.
 * Width if DockingPos is DOCK_BOTTOM or DOCK_TOP.
 */
void DGuiWidget::SetDocking(DDocking DockingSide, int OtherSize) {
    int ParentHeight=0;
    int ParentWidth=0;
    if (!Parent) {
        Log(DLOG_WARNING,"Parent Container not set for widget <%s>: assume screen",Name.c_str());
        ParentHeight=GetScreenHeight();
        ParentWidth=GetScreenWidth();
    }
    else {
        ParentHeight=Parent->GetHeight();
        ParentWidth=Parent->GetWidth();
    }


    // Calculate bounds due to the screen position
    switch (DockingSide) {
        case DOCK_LEFT:
            Bounds.height=ParentHeight;
            Bounds.width=OtherSize <= 0 ? DEFAULT_SIDE_SIZE : OtherSize;
            Bounds.x=0;
            Bounds.y=0;
            break;
        case DOCK_RIGHT:
            Bounds.height=ParentHeight;
            Bounds.width=OtherSize <= 0 ? DEFAULT_SIDE_SIZE : OtherSize;
            Bounds.x=ParentWidth-Bounds.height;
            Bounds.y=0;
            break;
        case DOCK_BOTTOM:
            Bounds.height=OtherSize <= 0 ? DEFAULT_SIDE_SIZE : OtherSize;
            Bounds.width=ParentWidth;
            Bounds.x=0;
            Bounds.y=ParentHeight-Bounds.height;
            break;
        case DOCK_TOP:
            Bounds.height=OtherSize <= 0 ? DEFAULT_SIDE_SIZE : OtherSize;
            Bounds.width=ParentWidth;
            Bounds.x=0;
            Bounds.y=0;
            break;
        default:
            break;
    }
}

void DGuiWidget::SetBorderWidth(uint8_t NewWidth) {
    if (NewWidth == Properties.BorderWidth) {
        return;
    }
    Properties.BorderWidth=NewWidth;
}

DWidgetType DGuiWidget::GetWidgetType(void) {
    return Type;
}

std::string DGuiWidget::GetWidgetTypeName(void) {
    if (WidgetTypeToName.contains(Type)) {
        return WidgetTypeToName.at(Type);
    }
    return std::string();
}

/**
 * @return current text size.
 */
int DGuiWidget::GetTextSize(void)
{
    return Properties.TextSize;
}

/**
 * @return widget ID as string.
 */
std::string DGuiWidget::GetId(void) {
    return Id;
}

size_t DGuiWidget::GetWidth(void) {
    return Bounds.width;
}

size_t DGuiWidget::GetHeight(void) {
    return Bounds.height;
}

DGuiWidget* DGuiWidget::GetParent(void) {
    return Parent;
}

void DGuiWidget::SetText(std::string NewText) {
    if (NewText == Text) {
        return;
    }
    Text=NewText;
}

const std::string& DGuiWidget::GetText(void) {
    return std::ref(Text);
}

/**
 * @brief Fill widget bounds with default backgound color.
 */
void DGuiWidget::Clear(void) {
    DrawRectangle(Bounds.x,Bounds.y,Bounds.width,Bounds.height,GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
}

/**
 * @brief Set text align value from align string.
 * 
 * @param AlignHoriz 
 * @param AlignVert 
 */
void DGuiWidget::SetTextAlign(std::string AlignHoriz,std::string AlignVert) {
    if (DString::StartsWith(AlignHoriz,DJsonTree::VALUE_LEFT)) {
        Properties.TextAlign.Horiz=DTextAlignH::TEXT_ALIGN_HLEFT;
    }
    else if (DString::StartsWith(AlignHoriz,DJsonTree::VALUE_CENTER)) {
        Properties.TextAlign.Horiz=DTextAlignH::TEXT_ALIGN_HCENTER;
    }
    else if (DString::StartsWith(AlignHoriz,DJsonTree::VALUE_RIGHT)) {
        Properties.TextAlign.Horiz=DTextAlignH::TEXT_ALIGN_HRIGHT;
    }

    if (DString::StartsWith(AlignVert,DJsonTree::VALUE_TOP)) {
        Properties.TextAlign.Vert=DTextAlignV::TEXT_ALIGN_VTOP;
    }
    else if (DString::StartsWith(AlignVert,DJsonTree::VALUE_CENTER)) {
        Properties.TextAlign.Vert=DTextAlignV::TEXT_ALIGN_VCENTER;
    }
    else if (DString::StartsWith(AlignVert,DJsonTree::VALUE_BOTTOM)) {
        Properties.TextAlign.Vert=DTextAlignV::TEXT_ALIGN_VBOTTOM;
    }
}

void DGuiWidget::SetEnabled(bool Enabled) {
    Properties.Enabled=Enabled;
}

void DGuiWidget::SetVisible(bool Visible) {
    Properties.Visible=Visible;
}

/**
 * @brief Set all common proprerties to its current values.
 */
void DGuiWidget::SetWidgetType(DWidgetType WidgetType)
{
    Type=WidgetType;
    // Text
    Properties.TextColor=GuiGetStyle(Type,TEXT_COLOR_NORMAL);
    Properties.TextPadding=GuiGetStyle(Type, TEXT_PADDING);
    //int align=GuiGetStyle(Type,TEXT_ALIGNMENT);
    Properties.TextAlign.Horiz=(DTextAlignH) GuiGetStyle(Type,TEXT_ALIGNMENT);
    // from global (DEFAULT) control
    Properties.TextAlign.Vert=(DTextAlignV) GuiGetStyle(DEFAULT,TEXT_ALIGNMENT_VERTICAL);
    Properties.TextSize=GuiGetStyle(DEFAULT,TEXT_SIZE);
    Properties.TextSpacing=GuiGetStyle(DEFAULT,TEXT_SPACING);
    
    // Borders
    Properties.BorderWidth=GuiGetStyle(Type,BORDER_WIDTH);
    Properties.BorderColor=GuiGetStyle(Type,BORDER_COLOR_NORMAL);

    // Other Colors
    Properties.LineColor=GuiGetStyle(Type,LINE_COLOR);
    Properties.BackGroundColor=GuiGetStyle(Type,BACKGROUND_COLOR);
}

// ********************** Methods used during draw **********************
void DGuiWidget::BackupCurrentGuiStyle(void) {
    // Text
    TempStyle.TextColor=GuiGetStyle(Type,TEXT_COLOR_NORMAL);
    TempStyle.TextAlign.Horiz=(DTextAlignH) GuiGetStyle(Type,TEXT_ALIGNMENT);
    TempStyle.TextPadding=GuiGetStyle(Type,TEXT_PADDING);
    
    // Borders
    TempStyle.BorderWidth=GuiGetStyle(Type,BORDER_WIDTH);
    TempStyle.BorderColor=GuiGetStyle(Type,BORDER_COLOR_NORMAL);

    // Other Colors 
    TempStyle.LineColor=GuiGetStyle(Type,LINE_COLOR);
    TempStyle.BackGroundColor=GuiGetStyle(Type,BACKGROUND_COLOR);

    // Properties from global (DEFAULT) control
    TempStyle.TextSize=GuiGetStyle(DEFAULT,TEXT_SIZE);
    TempStyle.TextSpacing=GuiGetStyle(DEFAULT,TEXT_SPACING);
    TempStyle.TextAlign.Vert=(DTextAlignV) GuiGetStyle(DEFAULT,TEXT_ALIGNMENT_VERTICAL);
}

void DGuiWidget::SetWidgetGuiStyle(void) {
    // Text
    GuiSetStyle(Type,TEXT_COLOR_NORMAL,Properties.TextColor);
    GuiSetStyle(Type,TEXT_ALIGNMENT,Properties.TextAlign.Horiz);
    GuiSetStyle(Type,TEXT_PADDING,Properties.TextPadding);

    // Borders
    GuiSetStyle(Type,BORDER_WIDTH,Properties.BorderWidth);
    GuiSetStyle(Type,BORDER_COLOR_NORMAL,Properties.BorderColor);

    // Other colors
    GuiSetStyle(Type,LINE_COLOR,Properties.LineColor);
    GuiSetStyle(Type,BACKGROUND_COLOR,Properties.BackGroundColor);

    // Properties from global (DEFAULT) control
    GuiSetStyle(DEFAULT,TEXT_SIZE,Properties.TextSize);
    GuiSetStyle(DEFAULT,TEXT_SPACING,Properties.TextSpacing);
    GuiSetStyle(DEFAULT,TEXT_ALIGNMENT_VERTICAL,Properties.TextAlign.Vert);

    Properties.Enabled ? GuiEnable() : GuiDisable();
    
    /* TODO: Global style
        TEXT_WRAP_WORD
        TEXT_LINE_SPACING
    */
}

void DGuiWidget::RestoreCurrentGuiStyle(void) {
    // Text
    GuiSetStyle(Type,TEXT_COLOR_NORMAL,TempStyle.TextColor);
    GuiSetStyle(Type,TEXT_ALIGNMENT,TempStyle.TextAlign.Horiz);
    GuiSetStyle(Type,TEXT_PADDING,TempStyle.TextPadding);
    
    // Borders
    GuiSetStyle(Type,BORDER_WIDTH,TempStyle.BorderWidth);
    GuiSetStyle(Type,BORDER_COLOR_NORMAL,TempStyle.BorderColor);

    // Other colors
    GuiSetStyle(Type,LINE_COLOR,TempStyle.LineColor);
    GuiSetStyle(Type,BACKGROUND_COLOR,TempStyle.BackGroundColor);

    // Properties from global (DEFAULT) control
    GuiSetStyle(DEFAULT,TEXT_SIZE,TempStyle.TextSize);
    GuiSetStyle(DEFAULT,TEXT_SPACING,TempStyle.TextSpacing);
    GuiSetStyle(DEFAULT,TEXT_ALIGNMENT_VERTICAL,TempStyle.TextAlign.Vert);

    TempStyle.Enabled ? GuiEnable() : GuiDisable();
}

void DGuiWidget::Draws(void) {
    BackupCurrentGuiStyle();
    SetWidgetGuiStyle();

    Draw();

    RestoreCurrentGuiStyle();
}

/**
 * @brief Load all gui widgets from file, can be a json formatted or rgl.
 * If file is an rgl saved from rGui app it will be converted in json.
 * @param Filename  ->  file to load.
 * @return true if all widget are loaded.
 * @return false if an error occours or not all widgets are loaded.
 * 
 */
DGuiWidget* DGuiWidget::New(const std::string& Filename, DGuiWidget *Parent) {
    Log(DLOG_DEBUG,"Loading %s",Filename.c_str());
    std::string JsonFilename;
    if (DString::CmpNoCase(DPath::GetExt(Filename),"rgl")) {
        // Raygui rgl format, convert to json
        JsonFilename=DPath::ChangeExt(Filename,"json",false);
        if (DPath::Exists_StdFs(JsonFilename.c_str())) {
            Log(DLOG_WARNING,"found a Json version of same layout, assume to use it");
        }
        else {
            if (!DPath::Exists(Filename)) {
                Log(DLOG_ERROR,"Layout file does not exist: %s",Filename.c_str());
                return nullptr;
            }
            Log(DLOG_DEBUG,"rgl file need to be converted to json");
            JsonFilename=RglToJson(Filename);
        }
    }
    else {
        JsonFilename=Filename;
    }

    if (JsonFilename.empty()) {
        Log(DLOG_ERROR,"Not valid layout file...");
        return nullptr;
    }

    DPreferences Json(JsonFilename,false);
    if (!Json.IsReady()) {
        Log(DLOG_ERROR,"Error parsing %s: %s",JsonFilename.c_str(), Json.GetLastStatus().c_str());
        return nullptr;
    }

    DTree dt=Json.GetTree();
    return DGuiWidget::New(std::move(dt), Parent);
}

DGuiWidget* DGuiWidget::New(DTree WidgetTree, DGuiWidget* Parent)
{
    // ** Load base info to instantiate a DWidget **
    // Type
    DWidgetType WidgetType=DWidgetType::UNKNOWN;
    std::string TypeName=WidgetTree.ReadString(DJsonTree::ITEM_TYPE,"");
    if (WidgetNameToType.contains(TypeName)) {
        WidgetType=WidgetNameToType.at(TypeName);
    }

    // Bounds
    Rectangle Bounds;
    Bounds.x=WidgetTree.ReadInteger(DJsonTree::ITEM_BOUNDS,DJsonTree::ITEM_LEFT,-1);
    Bounds.y=WidgetTree.ReadInteger(DJsonTree::ITEM_BOUNDS,DJsonTree::ITEM_TOP,-1);
    Bounds.width=WidgetTree.ReadInteger(DJsonTree::ITEM_BOUNDS,DJsonTree::ITEM_WIDTH,-1);
    Bounds.height=WidgetTree.ReadInteger(DJsonTree::ITEM_BOUNDS,DJsonTree::ITEM_HEIGHT,-1);

    // Text
    std::string Text=WidgetTree.ReadString(DJsonTree::ITEM_TEXT,"");

    // Create widget
    auto Widget=DGuiWidget::New(WidgetType,Bounds,Text, Parent);
    if (!Widget) {
        Log(DLOG_ERROR,"Cannot create widget from tree");
        return nullptr;
    }

    // ** Common widget properties **
    // Text size
    Widget->Properties.TextSize=WidgetTree.ReadInteger(DJsonTree::ITEM_TEXT_SIZE,-1);
    // Text align
    std::string AlignHoriz=WidgetTree.ReadString(DJsonTree::ITEM_TEXT_ALIGN_H,"");
    std::string AlignVert=WidgetTree.ReadString(DJsonTree::ITEM_TEXT_ALIGN_V,"");
    Widget->SetTextAlign(AlignHoriz,AlignVert);
    
// TODO (sono già gestite da Widget->Draw)
//    Properties.TextColor=GuiGetStyle(Type,TEXT_COLOR_NORMAL);
//    Properties.TextSpacing=GuiGetStyle(Type,TEXT_SPACING);
    
    // Borders
//    Properties.BorderWidth=GuiGetStyle(Type,BORDER_WIDTH);
//   Properties.BorderColor=GuiGetStyle(Type,BORDER_COLOR_NORMAL);

    // Other Colors
//    Properties.LineColor=GuiGetStyle(Type,LINE_COLOR);
//    Properties.BackGroundColor=GuiGetStyle(Type,BACKGROUND_COLOR);


    // AnchorId
    // Widget->Properties.AnchorId=WidgetTree.ReadString(DJsonTree::ITEM_ANCHOR_ID,""); // deprecated

    // Possible docking position
    std::string DockingSide=WidgetTree.ReadString(DJsonTree::ITEM_DOCKING,DJsonTree::ITEM_SIDE,"");
    int DockingSize=WidgetTree.ReadInteger(DJsonTree::ITEM_DOCKING,DJsonTree::ITEM_SIZE,-1);
    if (!DockingSide.empty()) {
        Widget->SetDocking(DockingSide,DockingSize);
    }

    // Name
    Widget->Name=WidgetTree.ReadString(DJsonTree::ITEM_NAME,"");
    if (Widget->Name.empty()) {
        Log(DLOG_WARNING,"Container %s has no name",Widget->Id.c_str());
    }

    // Enabled
    Widget->Properties.Enabled=WidgetTree.ReadBool(DJsonTree::ITEM_ENABLED,true);

    // Visible
    Widget->Properties.Visible=WidgetTree.ReadBool(DJsonTree::ITEM_VISIBLE,true);

    // ** Specific widget properties **
    if (Widget->GetWidgetType() == DWidgetType::DCONTAINER) {
        DGuiContainer* Container=(DGuiContainer *) Widget;
        // Widgets
        std::vector<DTree> Children=WidgetTree.ReadArrayTrees(DJsonTree::SEC_CHILDREN);
        Log(DLOG_DEBUG,"Container have %d widgets",Children.size());
        for (auto Child : Children) {
            DGuiWidget *SubWidget=DGuiWidget::New(Child,Widget);
            if (SubWidget) {
                Container->AddWidget(SubWidget);
            }
        }
    }
    if (Widget->GetWidgetType() == DWidgetType::DSTATUSBAR) {
        DGuiStatusBar *StatusBar=(DGuiStatusBar *) Widget;
        // StatusBar items        
        DTree SubItems=WidgetTree.GetTree(DJsonTree::SEC_STATUSBAR_ITEMS);
        std::vector<std::string> Items;
        SubItems.ReadNames(Items);
        Log(DLOG_DEBUG,"StatusBar have %d items",Items.size());
        // Add Items
        for (std::string& ItemName : Items) {
            int Left=0;
            int Width=-1; // auto
            std::string DockingSide=SubItems.ReadString(ItemName+"."+DJsonTree::ITEM_DOCKING,DJsonTree::ITEM_SIDE,"");
            if (DockingSide == DJsonTree::VALUE_LEFT) {
                Log(DLOG_DEBUG,"Statusbar item docked to the left");
                Left=DGuiWidget::DOCK_LEFT;
                Width=SubItems.ReadInteger(ItemName+"."+DJsonTree::ITEM_DOCKING,DJsonTree::ITEM_SIZE,DGuiWidget::WIDTH_AUTO);
            }
            else if (DockingSide == DJsonTree::VALUE_RIGHT) {
                Log(DLOG_DEBUG,"Statusbar item docked to the right");
                Left=DGuiWidget::DOCK_RIGHT;
                Width=SubItems.ReadInteger(ItemName+"."+DJsonTree::ITEM_DOCKING,DJsonTree::ITEM_SIZE,DGuiWidget::WIDTH_AUTO);
            }
            else if (DockingSide == DJsonTree::VALUE_CENTER) {
                Log(DLOG_DEBUG,"Statusbar item docked to center");
                Left=DGuiWidget::DOCK_CENTER;
                Width=SubItems.ReadInteger(ItemName+"."+DJsonTree::ITEM_DOCKING,DJsonTree::ITEM_SIZE,DGuiWidget::WIDTH_AUTO);
            }
            else {
                Left=SubItems.ReadInteger(ItemName,DJsonTree::ITEM_LEFT,DGuiWidget::DOCK_LEFT);
                if (Left < DGuiWidget::DOCK_CENTER) {
                    Log(DLOG_WARNING,"Left value of %d is not supported, set to 0");
                    Left=0;
                }
                Width=SubItems.ReadInteger(ItemName,DJsonTree::ITEM_WIDTH,DGuiWidget::WIDTH_AUTO);
            }
            // Text
            std::string Text=SubItems.ReadString(ItemName,DJsonTree::ITEM_TEXT,"");
            // Text align
            std::string AlHoriz=SubItems.ReadString(ItemName,DJsonTree::ITEM_TEXT_ALIGN_H,"");
            std::string AlVert=SubItems.ReadString(ItemName,DJsonTree::ITEM_TEXT_ALIGN_V,"");
            

            // Add Item
            Log(DLOG_DEBUG,"%s: Left=%d, Width=%d, Text=<%s>",ItemName.c_str(),Left,Width,Text.c_str());
            StatusBar->AddItem(ItemName,Left,Width,Text);
            // Set align
            DGuiStatusBar::DStatusBarItem *Item=StatusBar->GetItem(ItemName);
            if (Item) {
                Item->SetTextAlign(AlHoriz,AlVert);
            }
        }
    }
    else if (Widget->GetWidgetType() == DWidgetType::DEDIT) {
        DGuiEdit *Edit=(DGuiEdit *) Widget;
        // ReadOnly
        bool ro=WidgetTree.ReadBool(DJsonTree::ITEM_READ_ONLY,false);
        Edit->SetReadOnly(ro);
        // PasswordMode
        bool pm=WidgetTree.ReadBool(DJsonTree::ITEM_PASSWORD_MODE,false);
        Edit->SetPasswordMode(pm);
    }

    return Widget;
}

DGuiWidget* DGuiWidget::New(DWidgetType WidgetType, int LeftPos, int TopPos, int WidgetWidth, int WidgetHeight, std::string Text, DGuiWidget *Parent)
{
    if (WidgetType == DCONTAINER) {
        Log(DLOG_DEBUG,"New Container");
        DGuiContainer *Container=new DGuiContainer(LeftPos,TopPos,WidgetWidth,WidgetHeight,Parent);
        //Container->SetParent(Parent);
        Container->SetText(Text);
        return Container;
    }
    if (WidgetType == DLABEL) {
        Log(DLOG_DEBUG,"New label");
        DGuiLabel *Label=new DGuiLabel(LeftPos,TopPos,WidgetWidth,WidgetHeight,Parent);
        //Label->SetParent(Parent);
        Label->SetText(Text);
        return Label;
    }
    else if (WidgetType == DBUTTON) {
        Log(DLOG_DEBUG,"New button");
        DGuiButton *Button=new DGuiButton(LeftPos,TopPos,WidgetWidth,WidgetHeight,Parent);
        //Button->SetParent(Parent);
        Button->SetText(Text);
        return Button;
    }
    else if (WidgetType == DEDIT) {
        Log(DLOG_DEBUG,"New edit");
        DGuiEdit *Edit=new DGuiEdit(LeftPos,TopPos,WidgetWidth,WidgetHeight,Parent);
        //Edit->SetParent(Parent);
        Edit->SetText(Text);
        return Edit;
    }
    if (WidgetType == DSTATUSBAR) {
        Log(DLOG_DEBUG,"New StatusBar");
        DGuiStatusBar *StatusBar=new DGuiStatusBar(LeftPos,TopPos,WidgetWidth,WidgetHeight,Parent);
        //StatusBar->SetParent(Parent);
        StatusBar->SetText(Text);
        return StatusBar;
    }

    if (WidgetTypeToName.contains(WidgetType)) {
        Log(DLOG_ERROR,"Widget type %s not implemented",WidgetTypeToName.at(WidgetType).c_str());
    }
    else {
        Log(DLOG_ERROR,"Unknown Widget Type");
    }
    return nullptr;
}

DGuiWidget* DGuiWidget::New(DWidgetType WidgetType, Rectangle WidgetBounds, std::string Text, DGuiWidget *Parent)
{
    return DGuiWidget::New(WidgetType,WidgetBounds.x,WidgetBounds.y,WidgetBounds.width,WidgetBounds.height,Text, Parent);
}

/**
 * @brief Convert an rgl layout file in its json version.
 * Result file have same name but json extension.
 * 
 * @param Filename  ->  file to convert.
 * @return new filename on succesfully conversion otherwise an empty string.
 */
std::string DGuiWidget::RglToJson(std::string Filename) {
    Log(DLOG_DEBUG,"Converting %s",Filename.c_str());

    // Load rgl file
    auto FileStream=std::ifstream(Filename,std::ios::in);
    if (!FileStream.is_open()) {
        Log(DLOG_ERROR,"%s cannot be opened.",Filename.c_str());
        return std::string();
    }

    // Create a new empty json
    DPreferences JsonFile(DPath::ChangeExt(Filename,"json",false));
    if (!JsonFile.IsReady()) {
        Log(DLOG_ERROR,"%s cannot be created.",Filename.c_str());
        return std::string();
    }

    // Set to Container type
    JsonFile.WriteString(DJsonTree::ITEM_TYPE,WidgetTypeToName.at(DCONTAINER));
    JsonFile.WriteString(DJsonTree::ITEM_NAME,fs::path(Filename).stem());

    // Add Widgets
    std::vector<DGuiWidget *> WidgetsList;
    for (std::string Line; std::getline(FileStream,Line);) {
        // Ignore all lines that not starts with 'r', 'a' or 'c'
        if (Line.empty()) {
            continue;
        }
        else if (Line[0] == 'r') {
            // Ref. window:    r <x> <y> <width> <height>
            // TODO
        }
        else if (Line[0] == 'a') {
            // Anchor info:    a <id> <name> <posx> <posy> <enabled>
            // TODO
        }
        else if (Line[0] == 'c') {
            // Control
            DGuiWidget *Widget=RglLineToWidget(Line);
            if (!Widget) {
                return std::string();
            }
            WidgetsList.emplace_back(Widget);
        }
    }

    for (size_t ixC=0; ixC<WidgetsList.size(); ixC++) {
        auto *Widget=WidgetsList[ixC];
        std::string TreeName=DJsonTree::SEC_CHILDREN+"."+Widget->Name;
        //JsonFile.WriteString(TreeName,DJsonTree::ITEM_ANCHOR_ID,Widget->Properties.AnchorId); // deprecated
        JsonFile.WriteString(TreeName,DJsonTree::ITEM_TYPE,Widget->GetWidgetTypeName());
        JsonFile.WriteString(TreeName,DJsonTree::ITEM_TEXT,Widget->Text);
        TreeName=TreeName+DOT+DJsonTree::SEC_BOUNDS;
        JsonFile.WriteInteger(TreeName,DJsonTree::ITEM_LEFT,Widget->Bounds.x);
        JsonFile.WriteInteger(TreeName,DJsonTree::ITEM_TOP,Widget->Bounds.y);
        JsonFile.WriteInteger(TreeName,DJsonTree::ITEM_WIDTH,Widget->Bounds.width);
        JsonFile.WriteInteger(TreeName,DJsonTree::ITEM_HEIGHT,Widget->Bounds.height);
    }
    if (!JsonFile.Save()) {
        Log(DLOG_ERROR,"Error saving %s file: %s",JsonFile.GetFilename().c_str(),JsonFile.GetLastStatus().c_str());
        return std::string();
    }

    return(JsonFile.GetFilename());
}

/**
 * @brief Generate a DLayoutControl from text line ans save it to json.
 * 
 * @param Line      ->  Text line to decode.
 * @param JsonFile  ->  A DPreferences instance reference.
 * @return true on success.
 * @return false if some error occours.
 */
DGuiWidget* DGuiWidget::RglLineToWidget(std::string Line) {
    std::vector<std::string> LineItems;
    DCsv::ReadCSVRow(LineItems,Line,' ');
    if (LineItems.size() < DRgl::ControlLineItemsCount-1) {
        Log(DLOG_ERROR,"Line fields nr %d wrong",LineItems.size());
        return nullptr;
    }

    if (LineItems.size() == DRgl::ControlLineItemsCount-1) {
        LineItems.emplace_back(std::string());
    }

    // ** Load base info to instantiate a DWidget **
    // Type
    int WidgetType=DString::ToInt(LineItems[DRgl::CTRL_IX_TYPE]);
    WidgetType-=3; // TODO: For now it is a trick to match with RayGui type
    if (!WidgetTypeToName.contains((DWidgetType) WidgetType)) {
        Log(DLOG_ERROR,"Control type unkown: %d",WidgetType);
        return nullptr;
    }

    // Bounds
    Rectangle Bounds;
    Bounds.x=DString::ToInt(LineItems[DRgl::CTRL_IX_X]);
    Bounds.y=DString::ToInt(LineItems[DRgl::CTRL_IX_Y]);
    Bounds.width=DString::ToInt(LineItems[DRgl::CTRL_IX_WIDTH]);
    Bounds.height=DString::ToInt(LineItems[DRgl::CTRL_IX_HEIGHT]);

    // Text
    std::string Text;
    for (size_t ixV=DRgl::ControlLineItemsCount-1; ixV<LineItems.size(); ixV++) {
        Text.append(LineItems[ixV]);
        if (ixV < LineItems.size()-1) {
            Text.append(" ");
        }
    }

    // Create widget
    auto Widget=DGuiWidget::New((DWidgetType) WidgetType,Bounds,Text,nullptr);
    if (!Widget) {
        Log(DLOG_ERROR,"Cannot create widget from rgl line");
        return nullptr;
    }

    Widget->Name=LineItems[DRgl::CTRL_IX_NAME];
    //Widget->Properties.AnchorId=LineItems[CTRL_IX_ANCHOR_ID]; // deprecated

    return (Widget);
}

/**
 * @brief Draw text directly into widget
 * 
 * @param TextMsg 
 * @param TextBounds 
 */
/*
void DGuiWidget::DrawText(std::string TextMsg, Rectangle TextBounds, bool clearBefore) { //, DTextAlign TextAlign, Color TextColor) {
    BackupCurrentGuiStyle();
    SetWidgetGuiStyle();
    // Draw text
    if (clearBefore) {
        DrawRectangle(Bounds.x,Bounds.y,Bounds.width,Bounds.height,GetColor(Properties.BackGroundColor));
    }
    GuiDrawText(TextMsg.c_str(),TextBounds,Properties.TextAlign.Horiz,GetColor(Properties.TextColor));
    RestoreCurrentGuiStyle();
}
*/