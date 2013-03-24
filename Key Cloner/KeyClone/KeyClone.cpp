#include "resource.h"
#include <windows.h>
#include <stdio.h>
#include <winuser.h>
#include <windowsx.h>
#include <time.h>
#include <thread>


using namespace std;


/*procedures  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
void minimize();
void restore();
void InitNotifyIconData();
void StartKeyClone();


/*variables*/
UINT WM_TASKBAR = 0;
HWND Hwnd;
HMENU Hmenu;
NOTIFYICONDATA notifyIconData;
TCHAR szTIP[64] = TEXT("KeyClone beta1.0");
char szClassName[ ] = "KeyClone 1.0b";
thread start(StartKeyClone);
bool running = true;







int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */
    WM_TASKBAR = RegisterWindowMessageA("TaskbarCreated");
    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (GetModuleHandle(NULL), MAKEINTRESOURCE(ICO1));
    wincl.hIconSm = LoadIcon (GetModuleHandle(NULL), MAKEINTRESOURCE(ICO1));
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    wincl.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(255, 255, 255)));
    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    Hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               szClassName,       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               300,                 /* The programs width */
               150,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
              hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );
    /*Initialize the NOTIFYICONDATA structure only once*/
    InitNotifyIconData();
    /* Make the window visible on the screen */
    ShowWindow (Hwnd, nCmdShow);

	start.detach();
	



    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }
	running = false;
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    if ( message==WM_TASKBAR && !IsWindowVisible( Hwnd ) )
    {
        minimize();
        return 0;
    }

    switch (message)                  /* handle the messages */
    {
    case WM_ACTIVATE:
        Shell_NotifyIcon(NIM_ADD, &notifyIconData);
        break;
    case WM_CREATE:

        ShowWindow(Hwnd, SW_HIDE);
        Hmenu = CreatePopupMenu();
        AppendMenu(Hmenu, MF_STRING, ID_TRAY_EXIT,  TEXT( "Exit KeyClone" ) );

        break;

    case WM_SYSCOMMAND:
        /*In WM_SYSCOMMAND messages, the four low-order bits of the wParam parameter 
		are used internally by the system. To obtain the correct result when testing the value of wParam, 
		an application must combine the value 0xFFF0 with the wParam value by using the bitwise AND operator.*/ 
		
		switch( wParam & 0xFFF0 )  
        {
        case SC_MINIMIZE:
        case SC_CLOSE:  
            minimize() ;
            return 0 ;
            break;
        }
        break;

        
        // Our user defined WM_SYSICON message.
    case WM_SYSICON:
    {

        switch(wParam)
        {
        case ID_TRAY_APP_ICON:
        SetForegroundWindow(Hwnd);

            break;
        }


        if (lParam == WM_LBUTTONUP)
        {

            restore();
        }
        else if (lParam == WM_RBUTTONDOWN) 
        {
            // Get current mouse position.
            POINT curPoint ;
            GetCursorPos( &curPoint ) ;
			SetForegroundWindow(Hwnd);

            // TrackPopupMenu blocks the app until TrackPopupMenu returns

            UINT clicked = TrackPopupMenu(Hmenu,TPM_RETURNCMD | TPM_NONOTIFY,curPoint.x,curPoint.y,0,hwnd,NULL);


            
            SendMessage(hwnd, WM_NULL, 0, 0); // send benign message to window to make sure the menu goes away.
            if (clicked == ID_TRAY_EXIT)
            {
                // quit the application.
                Shell_NotifyIcon(NIM_DELETE, &notifyIconData);
                PostQuitMessage( 0 ) ;
            }
        }
    }
    break;

    // intercept the hittest message..
    case WM_NCHITTEST:
    {
       UINT uHitTest = DefWindowProc(hwnd, WM_NCHITTEST, wParam, lParam);
        if(uHitTest == HTCLIENT)
            return HTCAPTION;
        else
            return uHitTest;
    }

    case WM_CLOSE:

        minimize() ;
        return 0;
        break;

    case WM_DESTROY:

        PostQuitMessage (0);
        break;

    }

    return DefWindowProc( hwnd, message, wParam, lParam ) ;
}


void minimize()
{
    // hide the main window
    ShowWindow(Hwnd, SW_HIDE);
}


void restore()
{
    ShowWindow(Hwnd, SW_SHOW);
}

void InitNotifyIconData()
{
    memset( &notifyIconData, 0, sizeof( NOTIFYICONDATA ) ) ;

    notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
    notifyIconData.hWnd = Hwnd;
    notifyIconData.uID = ID_TRAY_APP_ICON;
    notifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    notifyIconData.uCallbackMessage = WM_SYSICON; //Set up our invented Windows Message
    notifyIconData.hIcon = (HICON)LoadIcon( GetModuleHandle(NULL),      MAKEINTRESOURCE(ICO1) ) ;
    strncpy(notifyIconData.szTip, szTIP, sizeof(szTIP));
}
void StartKeyClone(){
	HWND hwnd = GetConsoleWindow();
	//ShowWindow(hwnd,SW_HIDE);
	ShowWindow(hwnd,SW_MINIMIZE);
	printf("Now Running KeyClone 1.0Beta. This software is free to use with permission of Leland Peterson.\n\n\nIf you think you can help add Superduck247 on skype.\n");
	short character;
	while(running)
	{
		Sleep(10);
		for(character=8;character<=222;character++)
		{
			if(GetAsyncKeyState(character)==-32767)
			{
				switch(character)
				{
				case 49:{
					printf("Cloned The '1' Key to all World of Warcraft windows.\n");
					HWND GameWindow = FindWindow(0, "World of Warcraft");
					SendMessage(GameWindow, WM_KEYDOWN, character, 0);
					Sleep(10);
					SendMessage(GameWindow, WM_KEYUP, character, 0);
					HWND GameWindow2 = FindWindowEx(NULL,GameWindow,0, "World of Warcraft");
					SendMessage(GameWindow2, WM_KEYDOWN, character, 0);
					Sleep(10);
					SendMessage(GameWindow2, WM_KEYUP, character, 0);
					break;
						}
				case 50:{
					printf("Cloned The '2' Key to all World of Warcraft windows.\n");
					HWND GameWindow = FindWindow(0, "World of Warcraft");
					SendMessage(GameWindow, WM_KEYDOWN, character, 0);
					Sleep(10);
					SendMessage(GameWindow, WM_KEYUP, character, 0);
					HWND GameWindow2 = FindWindowEx(NULL,GameWindow,0, "World of Warcraft");
					SendMessage(GameWindow2, WM_KEYDOWN, character, 0);
					Sleep(10);
					SendMessage(GameWindow2, WM_KEYUP, character, 0);
					break;
				}
				case 51:{
					printf("Cloned The '3' Key to all World of Warcraft windows.\n");
					HWND GameWindow = FindWindow(0, "World of Warcraft");
					SendMessage(GameWindow, WM_KEYDOWN, character, 0);
					Sleep(10);
					SendMessage(GameWindow, WM_KEYUP, character, 0);
					HWND GameWindow2 = FindWindowEx(NULL,GameWindow,0, "World of Warcraft");
					SendMessage(GameWindow2, WM_KEYDOWN, character, 0);
					Sleep(10);
					SendMessage(GameWindow2, WM_KEYUP, character, 0);
					break;
				}
				case 52:{
					printf("Cloned The '4' Key to all World of Warcraft windows.\n");
					HWND GameWindow = FindWindow(0, "World of Warcraft");
					SendMessage(GameWindow, WM_KEYDOWN, character, 0);
					Sleep(10);
					SendMessage(GameWindow, WM_KEYUP, character, 0);
					HWND GameWindow2 = FindWindowEx(NULL,GameWindow,0, "World of Warcraft");
					SendMessage(GameWindow2, WM_KEYDOWN, character, 0);
					Sleep(10);
					SendMessage(GameWindow2, WM_KEYUP, character, 0);
					break;
				}
				case 53:{
					printf("Cloned The '5' Key to all World of Warcraft windows.\n");
					HWND GameWindow = FindWindow(0, "World of Warcraft");
					SendMessage(GameWindow, WM_KEYDOWN, character, 0);
					Sleep(10);
					SendMessage(GameWindow, WM_KEYUP, character, 0);
					HWND GameWindow2 = FindWindowEx(NULL,GameWindow,0, "World of Warcraft");
					SendMessage(GameWindow2, WM_KEYDOWN, character, 0);
					Sleep(10);
					SendMessage(GameWindow2, WM_KEYUP, character, 0);
					break;
				}
				case 54:{
					printf("Cloned The '6' Key to all World of Warcraft windows.\n");
					HWND GameWindow = FindWindow(0, "World of Warcraft");
					SendMessage(GameWindow, WM_KEYDOWN, character, 0);
					Sleep(10);
					SendMessage(GameWindow, WM_KEYUP, character, 0);
					HWND GameWindow2 = FindWindowEx(NULL,GameWindow,0, "World of Warcraft");
					SendMessage(GameWindow2, WM_KEYDOWN, character, 0);
					Sleep(10);
					SendMessage(GameWindow2, WM_KEYUP, character, 0);
					break;
				}
				case 55:{
					printf("Cloned The '7' Key to all World of Warcraft windows.\n");
					HWND GameWindow = FindWindow(0, "World of Warcraft");
					SendMessage(GameWindow, WM_KEYDOWN, character, 0);
					Sleep(10);
					SendMessage(GameWindow, WM_KEYUP, character, 0);
					HWND GameWindow2 = FindWindowEx(NULL,GameWindow,0, "World of Warcraft");
					SendMessage(GameWindow2, WM_KEYDOWN, character, 0);
					Sleep(10);
					SendMessage(GameWindow2, WM_KEYUP, character, 0);
					break;
				}
				case 56:{
					printf("Cloned The '8' Key to all World of Warcraft windows.\n");
					HWND GameWindow = FindWindow(0, "World of Warcraft");
					SendMessage(GameWindow, WM_KEYDOWN, character, 0);
					Sleep(10);
					SendMessage(GameWindow, WM_KEYUP, character, 0);
					HWND GameWindow2 = FindWindowEx(NULL,GameWindow,0, "World of Warcraft");
					SendMessage(GameWindow2, WM_KEYDOWN, character, 0);
					Sleep(10);
					SendMessage(GameWindow2, WM_KEYUP, character, 0);
					break;
				}
				case 57:{
					printf("Cloned The '9' Key to all World of Warcraft windows.\n");
					HWND GameWindow = FindWindow(0, "World of Warcraft");
					SendMessage(GameWindow, WM_KEYDOWN, character, 0);
					Sleep(10);
					SendMessage(GameWindow, WM_KEYUP, character, 0);
					HWND GameWindow2 = FindWindowEx(NULL,GameWindow,0, "World of Warcraft");
					SendMessage(GameWindow2, WM_KEYDOWN, character, 0);
					Sleep(10);
					SendMessage(GameWindow2, WM_KEYUP, character, 0);
					break;
				}
				case 48:{
					printf("Cloned The '0' Key to all World of Warcraft windows.\n");
					HWND GameWindow = FindWindow(0, "World of Warcraft");
					SendMessage(GameWindow, WM_KEYDOWN, character, 0);
					Sleep(10);
					SendMessage(GameWindow, WM_KEYUP, character, 0);
					HWND GameWindow2 = FindWindowEx(NULL,GameWindow,0, "World of Warcraft");
					SendMessage(GameWindow2, WM_KEYDOWN, character, 0);
					Sleep(10);
					SendMessage(GameWindow2, WM_KEYUP, character, 0);
					break;
				}
				case 189:{
					printf("Cloned The '-' Key to all World of Warcraft windows.\n");
					HWND GameWindow = FindWindow(0, "World of Warcraft");
					SendMessage(GameWindow, WM_KEYDOWN, character, 0);
					Sleep(10);
					SendMessage(GameWindow, WM_KEYUP, character, 0);
					HWND GameWindow2 = FindWindowEx(NULL,GameWindow,0, "World of Warcraft");
					SendMessage(GameWindow2, WM_KEYDOWN, character, 0);
					Sleep(10);
					SendMessage(GameWindow2, WM_KEYUP, character, 0);
					break;
				}
				case 187:{
					printf("Cloned The '=' Key to all World of Warcraft windows.\n");
					HWND GameWindow = FindWindow(0, "World of Warcraft");
					SendMessage(GameWindow, WM_KEYDOWN, character, 0);
					Sleep(10);
					SendMessage(GameWindow, WM_KEYUP, character, 0);
					HWND GameWindow2 = FindWindowEx(NULL,GameWindow,0, "World of Warcraft");
					SendMessage(GameWindow2, WM_KEYDOWN, character, 0);
					Sleep(10);
					SendMessage(GameWindow2, WM_KEYUP, character, 0);
					break;
				}
				case 32:{
					printf("Cloned The 'SPACE' Key to all World of Warcraft windows.\n");
					HWND GameWindow = FindWindow(0, "World of Warcraft");
					SendMessage(GameWindow, WM_KEYDOWN, character, 0);
					Sleep(10);
					SendMessage(GameWindow, WM_KEYUP, character, 0);
					HWND GameWindow2 = FindWindowEx(NULL,GameWindow,0, "World of Warcraft");
					SendMessage(GameWindow2, WM_KEYDOWN, character, 0);
					Sleep(10);
					SendMessage(GameWindow2, WM_KEYUP, character, 0);
					break;
				}
				case 70:{
					printf("Cloned The 'f' Key to all World of Warcraft windows.\n");
					HWND GameWindow = FindWindow(0, "World of Warcraft");
					SendMessage(GameWindow, WM_KEYDOWN, character, 0);
					Sleep(10);
					SendMessage(GameWindow, WM_KEYUP, character, 0);
					HWND GameWindow2 = FindWindowEx(NULL,GameWindow,0, "World of Warcraft");
					SendMessage(GameWindow2, WM_KEYDOWN, character, 0);
					Sleep(10);
					SendMessage(GameWindow2, WM_KEYUP, character, 0);
					break;
				}
				default:{
					printf("The %d keycode is not in the Clone list.\n",character);
						}
				}
			}
		}
	}
}

