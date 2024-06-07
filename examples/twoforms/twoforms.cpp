/*******************************************************************************************
*
*   DGuiApp v1.0.0
*
*   LICENSE:
*
*   2023 Dury Corp.
*
**********************************************************************************************/

#include <DGuiApp.h>
#include <DGuiButton.h>
#include <DGuiLabel.h>
#include <DGuiStatusBar.h>
#include <DGuiContainer.h>

#define VERSION "0.0.1"
#define FORM_MAIN "FormMain"
#define FORM_TWO "FormTwo"
/*
bool ButtonDxMainClick(DWidgetEvent Event) {
    //DLog::debug("FormMain->ButtonSx WidgetEvent %d from callback outside App context",Event.EventCode);
    return false;
};
*/
int main()
{
    // Create App context
    //DLog::debug("Create DGuiApp");
    DGuiApp App(800,480,"Goldlake Frontend Ver. " VERSION);

    // Add MainForm
    //DLog::debug("Create Container1");
    DGuiContainer *Container1=App.AddContainer(FORM_MAIN);

    // Add Button and set callback
    //DLog::debug("Create FormMainButtonFormTwo");
    DGuiButton *FormMainButtonFormTwo=(DGuiButton*) Container1->AddWidget(DWidgetType::DBUTTON,50,10,100,100,"FormTwo");
    FormMainButtonFormTwo->SetOnWidgetEvent([&] (DWidgetEvent Event) {
        //DLog::debug("FormMainButtonFormTwo WidgetEvent %d",Event.EventCode);
        App.SetActiveContainer(FORM_TWO);
        return true;
    });

    // Add Label
    //DLog::debug("Create FormMainLabel");
    DGuiLabel *FormMainLabel=(DGuiLabel*) Container1->AddWidget(DWidgetType::DLABEL,100,200,100,20,"Label1");
    FormMainLabel->SetText("Press ESC to quit");

    // Add StatusBar with 2 items
    //DLog::debug("Create FormMainStatusBar");
    DGuiStatusBar *FormMainStatusBar=(DGuiStatusBar*) Container1->AddWidget(DWidgetType::DSTATUSBAR,0,DGuiWidget::DOCK_BOTTOM,-1,20,"Statusbar1");
    if (FormMainStatusBar) {
        //DLog::debug("AddItem: 0,50");
        FormMainStatusBar->AddItem("Item1",0,100,"Status: normal");
        //DLog::debug("AddItem: 100,50");
        FormMainStatusBar->AddItem("Item2",FormMainStatusBar->GetWidth()-100,100,"Version 0.0.0.0");
        FormMainStatusBar->SetBorderWidth(2);
    }
    else {
        //DLog::debug("FormMainStatusBar nullptr");
    }
    
    // Add second form
    //DLog::debug("Create Container2");
    DGuiContainer *Container2=App.AddContainer(FORM_TWO);

    // Add Label
    DGuiLabel *LabelFormTwo=(DGuiLabel*) Container2->AddWidget(DWidgetType::DLABEL,100,200,100,20,"Label2");
    LabelFormTwo->SetText("I am FormSx");

    // Add button for change label text
    DGuiButton* FormTwoButtonLabel=(DGuiButton*) Container2->AddWidget(DWidgetType::DBUTTON,50,50,100,100,"ButtonLabel");
    FormTwoButtonLabel->SetText("Change Label");
    FormTwoButtonLabel->SetOnWidgetEvent([&] (DWidgetEvent Event) {
        //DLog::debug("FormTwoButtonLabel WidgetEvent %d",Event.EventCode);
        LabelFormTwo->SetText("New text");
        return false;
    });

    // Add button for back to main form
    DGuiButton* FormTwoButtonBack=(DGuiButton*) Container2->AddWidget(DBUTTON,200,50,100,100,"ButtonBack");
    FormTwoButtonBack->SetText("Back");
    FormTwoButtonBack->SetOnWidgetEvent([&] (DWidgetEvent Event) {
        //DLog::debug("FormTwoButtonBack WidgetEvent %d",Event.EventCode);
        App.SetActiveContainer(FORM_MAIN);
        return true;
    });

    // Set global gui event handler that receive all events except the ones
    // that widget has been hadled by it self by returning true from OnWidgetEvent callback
    App.SetOnGuiEvent([&] (DGuiEvent GuiEvent) {
        //DLog::debug("Global GuiEvent from %s",GuiEvent.WidgetId.c_str());
        //if (GuiEvent.WidgetId == ButtonDxMain->GetId()) {
        //    LabelMain->SeText("ButtonDx pressed");
        //}
    });

    // Now run all and wait for ESC or App quit
    DResult res=App.Run();

    // print out result message
    //DLog::debug("Result=%s",res.Message.c_str());
}
