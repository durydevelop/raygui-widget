#include "DGuiApp.h"
#include <raylib.h>
//#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <DGuiContainer.h>
#include "Log.h"

DGuiApp::DGuiApp(size_t ScreenWidth, size_t ScreenHeight, std::string AppTitle)
{
    Width=ScreenWidth;
    Height=ScreenHeight;
    Title=AppTitle;
    ActiveContainer=nullptr;
    Running=false;
    OnAppStartedCallback=nullptr;

    InitWindow(Width,Height,Title.c_str());
    /*
    auto cm=GetCurrentMonitor();
    Width=GetMonitorWidth(cm);
    Height=GetMonitorHeight(cm);
    SetWindowSize(Width,Height);
    */
};

DGuiApp::~DGuiApp()
{
    // TODO: Free Containers
    // TODO: Free StaticWidgets
    // Close window and OpenGL context
    CloseWindow();
};

bool DGuiApp::IsReady(void) {
    return IsWindowReady();
}

void DGuiApp::SetTitle(std::string AppTitle) {
    Title=AppTitle;
}

DGuiContainer* DGuiApp::SetActiveContainer(std::string ContainerName)
{
    auto Container=GetContainerFromName(ContainerName);
    if (!Container) {
        Log(DLOG_ERROR,"Container %s not found",ContainerName);
        return nullptr;
    }
    ActiveContainer=Container;
    return ActiveContainer;
}

void DGuiApp::OnGuiEvent(OnGuiEventCallback Callback)
{
    for (auto &[Id,Container] : Containers) {
        Container->SetOnGuiEvent(Callback);
    }
}

void DGuiApp::OnAppStarted(std::function<void (void)> Callback) {
    OnAppStartedCallback=Callback;
}

void DGuiApp::ClearScreen(void)
{
    if (ActiveContainer) {
        ClearBackground(GetColor(ActiveContainer->Properties.BackGroundColor));
        EndDrawing();
    }
    else {
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
    }
}

DGuiContainer* DGuiApp::AddContainer(std::string ContainerName) {
    if (ContainerName.empty()) {
        ContainerName="Container"+std::to_string(Containers.size());
    }

    DGuiContainer *NewContainer=new DGuiContainer(0,0,Width,Height,nullptr);
    return (AddContainer(NewContainer));
}

DGuiContainer* DGuiApp::AddContainer(DGuiContainer *NewContainer) {
    if (!NewContainer) {
        return nullptr;
    }
    Containers.emplace(NewContainer->GetId(),NewContainer);
    if (Containers.size() == 1) {
        // Set it to active one if it is alone
        ActiveContainer=NewContainer;
    }
    return NewContainer;
}

DGuiContainer* DGuiApp::AddContainerFromFile(std::string JsonFilename) {
    DGuiContainer *NewContainer=(DGuiContainer *) DGuiWidget::New(JsonFilename,nullptr);

    if (!NewContainer) {
        Log(DLOG_ERROR,"%s layout possibile incorrect",JsonFilename.c_str());
        return nullptr;
    }

    AddContainer(NewContainer);
    return NewContainer;
}

DGuiWidget* DGuiApp::AddStaticWidgetFromFile(std::string JsonFilename) {
    DGuiWidget* Widget=DGuiWidget::New(JsonFilename,nullptr);
    if (!Widget) {
        Log(DLOG_ERROR,"%s layout possibile incorrect",JsonFilename.c_str());
        return nullptr;
    }

    StaticWidgets.emplace(Widget->Name,Widget);
    return Widget;
}

DGuiContainer* DGuiApp::GetContainerFromName(std::string ContainerName) {
    for (auto [Id,Container] : Containers) {
        if (Container->Name == ContainerName) {
            return Container;
        }
    }
    return nullptr;
}

DResult DGuiApp::Run(void)
{
    DResult Result;
    #ifdef NET_DRIVER_EM
        if (Width == 0 || Height == 0) {
            Result.SetError("In WASM context, Width or Height cannot be 0");
            return Result;    
        }
    #endif

    if (!IsWindowReady()) {
        Result.SetError("Cannot run, OpenGL context was not initialized correctly");
        return Result;
    }
    
    SetTargetFPS(60);
    Running=true;
    if (OnAppStartedCallback) {
        OnAppStartedCallback();
    }

    // Main loop (detect window close button or ESC key)
    while (!WindowShouldClose()) {
        if (ActiveContainer) {
            //Log(DLOG_DEBUG,"ActiveContainer=%s",ActiveContainer->Name.c_str());
            ClearBackground(GetColor(ActiveContainer->Properties.BackGroundColor));
            // Draw container
            ActiveContainer->Draw();
            // Draw Static widgets
            for (auto [Name,Widget] : StaticWidgets) {
                Widget->Draws();
            }
            EndDrawing();
        }
    }

    Running=false;
    return Result;
};
