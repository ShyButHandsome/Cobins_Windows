﻿#include "windows.h"
#include "winutils/BindPipe.h"
#include "cobins.h"
#include "AppDelegate.h"

extern "C" 
{
    static HHOOK     g_hHook = NULL;	    //the handle to the hook procedure
    static HINSTANCE g_hinstDll = NULL;     //the handle to DLL module
    static BOOL g_hostFlag = FALSE;
    static BOOL g_hooked = FALSE;
    static CBindPipe g_pipe;
    static AppDelegate gAppDelegate;
    static HWND g_mainWnd = NULL;

    //消息回调函数
    LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
    {
        //COBLOG("---GetMsgProc-----\n");
        if (!g_hooked)
        {
            WT_Trace("OnHooked: process=%x,hinstDLL=%x,g_hostFlag=%d\n", GetCurrentProcessId(), g_hinstDll, g_hostFlag);
            g_hooked = true;
            if (!g_hostFlag)
            {
                MSG *lpMsg;
                lpMsg = (MSG*)lParam;

                gAppDelegate.create(lpMsg->hwnd, "embed123");
            }
        }
        return CallNextHookEx(g_hHook, nCode, wParam, lParam);
    }

    _declspec(dllexport) void triggerAppliation()
    {
        if (!g_hooked)
        {
            WT_Trace("OnHooked: process=%x,hinstDLL=%x,g_hostFlag=%d\n", GetCurrentProcessId(), g_hinstDll, g_hostFlag);
            g_hooked = true;
            if (!g_hostFlag)
            {
                gAppDelegate.create(g_mainWnd, "embed123");
            }
        }
    }

    _declspec(dllexport) BOOL HookThread(DWORD ThreadId)
    {
        if (g_hHook == NULL)
        {
            COBLOG("Target window thread = 0x%08x, g_hinstDll=%x\r\n", ThreadId, g_hinstDll);

            g_hHook = SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, g_hinstDll, ThreadId);
            if (g_hHook == NULL)
            {
                COBLOG("Hook error!\n");
                return FALSE;
            }
            else
            {
                COBLOG("Hook target thread Successfully\r\n");
            }
        }

        return TRUE;
    }

    _declspec(dllexport) HHOOK HookWnd(HWND hDestWnd)
    {
        HHOOK hHook = NULL;
        DWORD dwProcessId = NULL;
        DWORD dwThreadId = NULL;

        if (g_hHook != NULL || hDestWnd == NULL)
            return NULL;

        dwThreadId = GetWindowThreadProcessId(hDestWnd, &dwProcessId);
        if (dwThreadId == NULL)
            return NULL;

        COBLOG("Target window thread = 0x%08x, g_hinstDll=%x\r\n", dwThreadId, g_hinstDll);

        g_hostFlag = TRUE;
        g_mainWnd = hDestWnd;
        hHook = SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, g_hinstDll, dwThreadId);
        if (hHook == NULL)
        {
            COBLOG("Hook error!\n");
            return NULL;
        }
        else
        {
            COBLOG("Hook target thread Successfully\r\n");
        }

        g_hHook = hHook;
        return hHook;
    }
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
    {
        COBLOG("***** DLL_PROCESS_ATTACH: process=%x, hinstDLL=%x\n", GetCurrentProcessId(), hinstDLL);

        g_hinstDll = hinstDLL;
    }
    break;

    case DLL_THREAD_ATTACH:
        break;

    case DLL_THREAD_DETACH:
    {

    }
        break;

    case DLL_PROCESS_DETACH:
    {
        COBLOG("DLL_PROCESS_DETACH111: process=%x, hinstDLL=%x\n", GetCurrentProcessId(), hinstDLL);
        if (!g_hostFlag)
        {
            //gAppDelegate.destroy();
        }
        COBLOG("DLL_PROCESS_DETACH222: process=%x, hinstDLL=%x\n", GetCurrentProcessId(), hinstDLL);
    }
    break;
    }

    return TRUE;
}