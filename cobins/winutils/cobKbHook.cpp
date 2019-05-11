#include "cobKbHook.h"
#include "base/cobUtils.h"
#include "wndbase.h"

NS_COB_BEGIN

static HINSTANCE glhInstance=NULL;   //DLLʵ����� 
static HHOOK glhKeyboardHook = NULL; //��װ����깴�Ӿ��
static HWND  ghHostWnd = NULL;       //host window handle

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	WT_Trace("kbhook: DllMain glhInstance = %x", hInstance);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		glhInstance = hInstance;
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{

	}

	return 1;
}

//Hook �ײ���̹���(2009.10.1������ͨ���̹����ڼ���̨ʽ���������Ĺ�������ʧЧ)
static LRESULT WINAPI KeyboardProc(int nCode,WPARAM wparam,LPARAM lparam)
{
	PKBDLLHOOKSTRUCT pKeyInfo  = (PKBDLLHOOKSTRUCT)lparam;
    if (nCode >= 0)
    {
        LPARAM lParam = 1;
        lParam += pKeyInfo->scanCode << 16;
        lParam += pKeyInfo->flags << 24;
        if (pKeyInfo->flags == 0x80)
        {
            lParam += 1 << 30; //Specifies the previous key state. The value is always 1 for a WM_KEYUP message. (MSDN)
        }

        //WT_Trace("KeyHook: curprocess=%x\n", GetCurrentProcessId());

        if (ghHostWnd != NULL)
        {
            PostMessage(ghHostWnd, WSH_MSG_KEY, (WPARAM)pKeyInfo->vkCode, lParam);
        }

        //WT_Trace("scancode=%x,flags=%x,time=%x, dwMsg=%x\n", pKeyInfo->scanCode, pKeyInfo->flags, pKeyInfo->time, lParam);
    }

	return CallNextHookEx(glhKeyboardHook, nCode, wparam, lparam);
}

BOOL Kb_StartHook(HWND hHostWnd)
{
   	DWORD ThreadId = 0;
    
    if (glhKeyboardHook != NULL)
    {
        WT_Trace( "KB_StartHook: �����Ѵ���\n");
        return FALSE;
    }
    
    if (NULL == (glhKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, glhInstance, ThreadId)))
    {
        WT_Trace( "KB_StartHook: Hook Keyboard error = %d", GetLastError() );
        return FALSE;
    }
    
    ghHostWnd = hHostWnd;
    
    WT_Trace("glhKeyboardHook=%x\n", glhKeyboardHook);
    
    return TRUE; 
}

BOOL Kb_StopHook(HWND hHostWnd)
{
    if(glhKeyboardHook && UnhookWindowsHookEx(glhKeyboardHook))
    {
        glhKeyboardHook = NULL;
        return TRUE;
    }
    else
    {
        WT_Trace("ж�ؼ��̹���ʧ��\n");
    }
    
    return FALSE;
}

NS_COB_END