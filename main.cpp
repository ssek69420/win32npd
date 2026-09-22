#define UNICODE

#include <windows.h>

const TCHAR CLASSNAME[] = TEXT("window");

LRESULT CALLBACK winprocedure(HWND hwnd, UINT wm, WPARAM wp, LPARAM lp)
{
    static HWND edit;
    switch(wm)
    {
        case WM_CREATE:
            edit = CreateWindowEx(
                WS_EX_CLIENTEDGE,
                TEXT("EDIT"),
                TEXT("NOTEPAD"),
                WS_CHILD | WS_BORDER | WS_VISIBLE | WS_EX_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN,
                20, 20, 1280, 720,
                hwnd,
                NULL,
                ((LPCREATESTRUCT)lp)->hInstance,
                NULL
            );
            return 0;
            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;
    }
    return DefWindowProc(hwnd, wm, wp, lp);
};

int WINAPI wWinMain(HINSTANCE hinst, HINSTANCE hprevinst, LPTSTR cmdline, int cmdshow)
{
    WNDCLASSEX windowclass = {};
    MSG msg;
    HWND hwnd;
    windowclass.cbSize = sizeof(windowclass);
    windowclass.style = 0;
    windowclass.lpfnWndProc = winprocedure;
    windowclass.cbClsExtra = 0;
    windowclass.hInstance = hinst;
    windowclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    windowclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    windowclass.lpszMenuName = NULL;
    windowclass.lpszClassName = CLASSNAME;
    windowclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassEx(&windowclass);

    hwnd = CreateWindowEx(
        WS_EX_LEFT,
        CLASSNAME,
        NULL,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hinst,
        NULL
    );
    ShowWindow(hwnd, cmdshow);
    UpdateWindow(hwnd);
    while(GetMessage(&msg, NULL, 0, 0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}