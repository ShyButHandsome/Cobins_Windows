#include "mousehook.h"
#include "wtermin.h"
#include "wndbase.h"

#pragma data_seg("mousehook_mydata")
 	 HHOOK glhMouseHook = NULL;    //��װ����깴�Ӿ�� 
	 HINSTANCE glhInstance = NULL; //DLLʵ����� 
     HWND  ghHostWnd = NULL;       //host window handle
#pragma data_seg()

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		glhInstance = hInstance; //���뱣��DLLʵ�����
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{

    }

	return 1;   // ok
}

//Hook �ײ���̹���(2009.10.1������ͨ���̹����ڼ���̨ʽ���������Ĺ�������ʧЧ)
static LRESULT WINAPI MouseProc(int nCode,WPARAM wparam,LPARAM lparam)
{
    if (nCode >= 0)
    {
        LPMOUSEHOOKSTRUCT pMouseHook = (MOUSEHOOKSTRUCT FAR *)lparam;
        if (pMouseHook->hwnd == ghHostWnd || IsChild(ghHostWnd, pMouseHook->hwnd))
            return 0;        
        
        if (wparam == WM_MOUSEMOVE || wparam == WM_NCMOUSEMOVE)
        {
            static DWORD dwLastTickCount = 0;
            if (GetTickCount() - dwLastTickCount < 100)  //�ӳ�100ms
                return 0;
            
            dwLastTickCount = GetTickCount();
        }

        PostMessage(ghHostWnd, WSH_MSG_MOUSE, wparam, (LPARAM)pMouseHook->hwnd);
    }

	return CallNextHookEx(glhMouseHook,nCode,wparam,lparam);
}

BOOL Mouse_StartHook(HWND hHostWnd)
{
   	DWORD ThreadId = 0;

    WT_Trace( "Mouse_StartHook: instance = %x\n", glhInstance );

    if (glhMouseHook != NULL)
        return TRUE;

	if (!(glhMouseHook = SetWindowsHookEx(WH_MOUSE, MouseProc, glhInstance, ThreadId)))
	{
		WT_Trace( "Mouse_StartHook: err = %d", GetLastError() );
		return FALSE;
	}

    ghHostWnd = hHostWnd;

	return TRUE; 
}

BOOL Mouse_StopHook()
{
	if (glhMouseHook != NULL)
	{
		if (UnhookWindowsHookEx(glhMouseHook))
		{
			glhMouseHook = NULL;
            ghHostWnd = NULL;
            return TRUE;
		}
	}

	return FALSE;
}
