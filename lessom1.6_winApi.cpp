#include <windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL FileExists(LPCTSTR szPath);


int WINAPI WinMain(HINSTANCE hInst, 
    HINSTANCE hPrevInst, 
    LPSTR lpCmdLine, 
    int nCmdShow) 
{
    TCHAR szClassName[] = L"Мой класс"; 
    HWND hMainWnd; 
    MSG msg; 
    WNDCLASSEX wc; 
    wc.cbSize = sizeof(wc); 
    wc.style = CS_HREDRAW | CS_VREDRAW; 
    wc.lpfnWndProc = WndProc; 
    wc.lpszMenuName = NULL; 
    wc.lpszClassName = szClassName; 
    wc.cbWndExtra = NULL; 
    wc.cbClsExtra = NULL; 
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO); 
    wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO); 
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); 
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.hInstance = hInst; 
    if (!RegisterClassEx(&wc)) {
    
        MessageBox(NULL, L"Не получилось зарегистрировать класс!", L"Ошибка", MB_OK);
        return NULL; 
    }
   
    hMainWnd = CreateWindow(
        szClassName, 
        L"Сохранение данных в файл", 
        WS_OVERLAPPEDWINDOW, 
        0, 
        0, 
        640,
        480, 
        (HWND)NULL, 
        NULL, 
        HINSTANCE(hInst), 
        NULL); 
    if (!hMainWnd) {
     
        MessageBox(NULL, L"Не получилось создать окно!", L"Ошибка", MB_OK);
        return NULL;
    }
    ShowWindow(hMainWnd, nCmdShow); 
    UpdateWindow(hMainWnd); 
    while (GetMessage(&msg, NULL, NULL, NULL)) { 
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
    }
    return msg.wParam; 
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND hBtn1, hBtn2, hStatic1, hEdit1, hEdit2, hStatic2;
    switch (uMsg) {
    case WM_CREATE:
         hBtn1 = CreateWindow(
            L"BUTTON",
            L"Сохранить",
            WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
            10,400,150,35,
            hWnd, reinterpret_cast<HMENU>(1), NULL, NULL);

         hStatic1 = CreateWindow(
             L"static",
             L"Введите текст в поле:",
             WS_CHILD | WS_VISIBLE,
             10, 0, 150, 20,
             hWnd, NULL, NULL, NULL);
         hStatic2 = CreateWindow(
             L"static",
             L"Введите название файла:",
             WS_CHILD | WS_VISIBLE,
             10, 240, 200, 20,
             hWnd, NULL, NULL, NULL);
         
         hEdit1 = CreateWindow(
             L"eDit",
             L"Не более 1000 символов",
             WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL| ES_LEFT,
             10,25,600,200,
             hWnd, NULL, NULL, NULL
         );

         hEdit2 = CreateWindow(
             L"eDit",
             L"Название",
             WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_LEFT,
             200, 240, 100, 20,
             hWnd, NULL, NULL, NULL
         );
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 1:
            wchar_t buf[1010];
            wchar_t fileName[250];
            GetWindowText(hEdit1, buf,1001);
            GetWindowText(hEdit2, fileName, 250);
            HANDLE hFile;
            if(wcslen(fileName))
            {

                if (FileExists(fileName)) {
                    MessageBox(NULL, L"Такой файл уже существует", L"Ошибка",  MB_ICONERROR);
                }
                else
                {
                    hFile = CreateFile(
                        fileName,
                        GENERIC_READ | GENERIC_WRITE,
                        FILE_SHARE_READ,
                        NULL,
                        OPEN_ALWAYS,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL);

                    DWORD iNumWrite = 0;
                    if (WriteFile(hFile, buf, wcslen(buf) * 2, &iNumWrite, NULL)) 
                    {
                        MessageBox(NULL, L"Файл успешно создан", L"Файл успешно создан", MB_OK);
                    }
                    else
                    {
                        MessageBox(NULL, L"Что то пошло не так. (((", L"Ошибка", MB_ICONERROR);
                    }

                    CloseHandle(hFile);
                }

            }
            else
            {
                MessageBox(NULL, L"Введите название файла!", L"Введите название файла", MB_ICONWARNING);
            }

            //MessageBox(NULL, L"Файл успешно создан", L"App", MB_OK);
            break;
        }


        break;
    case WM_DESTROY: 
        PostQuitMessage(NULL); 
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return NULL; 
}

BOOL FileExists(LPCTSTR szPath)
{
    DWORD dwAttrib = GetFileAttributes(szPath);

    return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
        !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}
