#define UNICODE
#define ID_SAVE 1001
#include <windows.h>
#include <CommCtrl.h>

const TCHAR CLASSNAME[] = TEXT("window");

LRESULT CALLBACK EditWProcedure(
    HWND hwnd,
    UINT wm,
    WPARAM wp,
    LPARAM lp,
    UINT_PTR uIdSubclass,
    DWORD_PTR dwRefData
)
{
    if(wm == WM_KEYDOWN && wp == 'A' && (GetKeyState(VK_CONTROL) & 0x8000)){
        SendMessage(hwnd, EM_SETSEL, 0, -1);
    }
    return DefSubclassProc(hwnd, wm, wp, lp);
}

LRESULT CALLBACK winprocedure(HWND hwnd, UINT wm, WPARAM wp, LPARAM lp)
{
    static HWND edit;
    static HWND save;
    switch(wm)
    {
        case WM_CREATE:
            edit = CreateWindowEx(
                WS_EX_CLIENTEDGE,
                TEXT("EDIT"),
                TEXT("NOTEPAD"),
                WS_CHILD | WS_BORDER | WS_VISIBLE | WS_EX_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN,
                20, 40, 1280, 720,
                hwnd,
                NULL,
                ((LPCREATESTRUCT)lp)->hInstance,
                NULL
            );
            save = CreateWindowEx(
                WS_EX_CLIENTEDGE,
                TEXT("BUTTON"),
                TEXT("Save"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                20, 5, 
                40, //w
                26, //h
                hwnd,
                (HMENU)ID_SAVE,
                ((LPCREATESTRUCT)lp)->hInstance,
                NULL
            );
            return 0;
        case WM_COMMAND:
            if(LOWORD(wp) == ID_SAVE){
                wchar_t filename_size[MAX_PATH] = L"";
                OPENFILENAMEW openfilename = {0};
                openfilename.lStructSize = sizeof(openfilename);
                openfilename.lpstrFile = filename_size;
                openfilename.hwndOwner = hwnd;
                openfilename.nMaxFile = MAX_PATH;
                openfilename.lpstrFilter = 
                    L"Text Files (*.txt)\0*.txt\0"
                    L"All Files (*.*)\0*.*\0";
                openfilename.lpstrDefExt = L"txt";
                openfilename.Flags = OFN_OVERWRITEPROMPT;
                if(GetSaveFileName(&openfilename))
                {
                    HANDLE file = CreateFileW(
                        openfilename.lpstrFile,
                        GENERIC_WRITE, //normal write
                        0, //don't share the file
                        NULL, //default security attributes
                        CREATE_ALWAYS, //always create/overwrite if any
                        FILE_ATTRIBUTE_NORMAL,
                        NULL //no template for file
                    );
                }
            }

            SetWindowSubclass(edit, EditWProcedure, 0, 0);

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