#pragma once
#include "include/helper.h"
#include "include/Console.hpp"
#include "include/Game.hpp"
#include "include/D3D11Window.hpp"
#include "include/Hooking.hpp"
#include <detours.h>

DWORD WINAPI MainThread_Initialize()
{
    CG::InitSdk();
    using namespace DX11_Base;
    g_Console = std::make_unique<Console>();

#if DEBUG
    g_Console->InitializeConsole("DX11 Doorstop Interal - ArilisDev");
    g_Console->printdbg("Operation: Harsh Doorstop ImGui Hook - Initializing . . .\n\n", g_Console->color.DEFAULT);
#endif

    ///  ESTABLISH GAME DATA   
    g_GameData = std::make_unique<GameData>();
    g_GameVariables = std::make_unique<GameVariables>();

    ///  CREATE WINDOW AND ESTABLISH HOOKS
    g_D3D11Window = std::make_unique<D3D11Window>();
    g_Hooking = std::make_unique<Hooking>();
    g_Menu = std::make_unique<Menu>();
    g_Hooking->Hook();
   
#if DEBUG
    g_Console->printdbg("Main::Initialized\n", g_Console->color.green);
#endif

    ///  RENDER LOOP
    g_Running = TRUE;
    while (g_Running)
    {
        g_Menu->Loops();
        if (GetAsyncKeyState(VK_INSERT) & 1) g_GameVariables->m_ShowMenu = !g_GameVariables->m_ShowMenu;
    }
   
    ///  EXIT
    FreeLibraryAndExitThread(g_hModule, EXIT_SUCCESS);
    return EXIT_SUCCESS;
}

int post_index = 0x61;
using fn = void(__thiscall*)(CG::UObject*, CG::UObject*);
fn original = 0;
void posthook(CG::UObject* vp_client, CG::UObject* canvas) {
    try {
        using namespace DX11_Base;
        g_Menu = std::make_unique<Menu>();
        g_Running = TRUE;
        while (g_Running) {
            g_Menu->Loops();
            std::this_thread::sleep_for(0ms);
            std::this_thread::yield();
        }
    }
    catch (std::exception& e) {
        return original(vp_client, canvas);
    }
    original(vp_client, canvas);
}
void MyThread()
{
    CG::UWorld** p_uworld = reinterpret_cast<CG::UWorld**>(CG::UWorld::GWorld);
    auto gameinstance = (*p_uworld)->OwningGameInstance;
    auto localplayer = gameinstance->LocalPlayers[0];
    CG::UGameEngine* Engine = nullptr;
    auto EngineClass = CG::UGameEngine::StaticClass();
    for (int i = 0; i < CG::UObject::GObjects->Count(); ++i) {
        if (auto elem = CG::UObject::GObjects->GetByIndex(i)) {
            if (elem->IsA(EngineClass)) {
                Engine = static_cast<CG::UGameEngine*>(elem);
            }
        }
    }
    auto Viewport = Engine->GameViewport;
    auto oPost = reinterpret_cast<void**>(Viewport->VfTable)[0x61];
    DWORD protect = 0;
    VirtualProtect(&oPost, 8, PAGE_EXECUTE_READWRITE, &protect);
    original = reinterpret_cast<decltype(original)>(oPost);
    oPost = &posthook;
    VirtualProtect(&oPost, 8, protect, 0);

}