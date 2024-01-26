#include "framework.h"
#include "Passports.h"
#include <fstream>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <deque>

#define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    ENTER(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    ADMIN(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    USER(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    INSERT(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    EDIT(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    DELETEP(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Search1(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    SEARCHBYAGE(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    SEARCHBYEYES(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    SEARCHBYCITYANDCOUNTRY(HWND, UINT, WPARAM, LPARAM);

void SaveToFile(HWND hDlg);
void SaveEditedDataToTextFile(int personID, HWND hDlg);
bool LoadDataForID(int personID, HWND hDlg);
void TrimString(std::string& str);
int GetControlIDFromAttribute(const std::string& attribute);
void LoadDataFromFileAndFillControls(HWND hDlg);
void SaveChangesToFile(HWND hDlg);
void DeletePersonById(int personID, HWND hDlg);
void SearchPeopleByAgeAndWriteToFile(int targetAge);
void SearchPeopleByAgeAndWriteToFile(int targetAge);
int ExtractAgeFromPersonInfo(const std::vector<std::string>& personInfo);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PASSPORTS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PASSPORTS));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PASSPORTS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PASSPORTS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case ID_ENTER_LOG:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, ENTER);
                break;
                
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;
        

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
        case WM_INITDIALOG:
            return (INT_PTR)TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
            {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK ENTER(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    int mID = LOWORD(wParam);
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }

        if (mID == IDC_BUTTON1)
        {
            char username[20], password[20];
            int usernameLength = GetWindowTextLength(GetDlgItem(hDlg, IDC_EDIT1));
            int passwordLength = GetWindowTextLength(GetDlgItem(hDlg, IDC_EDIT2));
            if (usernameLength > 0 && passwordLength > 0)
            {
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG3), hDlg, USER);
            }
            else
            {
                MessageBox(hDlg, "Not entered password or username", "Wrong password", MB_OK | MB_ICONERROR);
            }

        }
        
        if (mID == IDC_BUTTON2)
        {
            char username[20], password[20];
            int usernameLength = GetWindowTextLength(GetDlgItem(hDlg, IDC_EDIT1));
            if (usernameLength > 0)
            {
                GetDlgItemText(hDlg, IDC_EDIT1, username, 20);
                GetDlgItemText(hDlg, IDC_EDIT2, password, 20);
                


                if (strcmp(password, "PASSWORD") == 0)
                {
                    
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hDlg, ADMIN);
                }
                else
                {
                    MessageBox(hDlg, "Wrong password for admin", "Wrong password", MB_OK | MB_ICONERROR);
                    SetDlgItemText(hDlg, IDC_EDIT2, "");
                }
            }
            else
            {
                MessageBox(hDlg, "Please enter a username", "Username missing", MB_OK | MB_ICONERROR);
            }
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK ADMIN(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    int mID = LOWORD(wParam);
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
        case WM_INITDIALOG:
            return (INT_PTR)TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL){
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            if (mID == IDC_BUTTON1){
                DialogBox(hInst, MAKEINTRESOURCE(IDD_INSERT), hDlg, INSERT);
            }
            if (mID == IDC_BUTTON2){
                DialogBox(hInst, MAKEINTRESOURCE(IDD_EDIT), hDlg, EDIT);
            }
            if (mID == IDC_BUTTON3){
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DELETE), hDlg, DELETEP);
            }

            break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK USER(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    int mID = LOWORD(wParam);
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
        case WM_INITDIALOG:
            return (INT_PTR)TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
            {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            if (mID == IDC_BUTTON1) {
                DialogBox(hInst, MAKEINTRESOURCE(IDD_SEARCHBYNAME), hDlg, Search1);
            }
            if (mID == IDC_BUTTON2) {
                DialogBox(hInst, MAKEINTRESOURCE(IDD_SEARCHBYAGE), hDlg, SEARCHBYAGE);
            }

            if (mID == IDC_BUTTON3) {
                DialogBox(hInst, MAKEINTRESOURCE(IDD_SEARCHBYEYES), hDlg, SEARCHBYEYES);
            }
            if (mID == IDC_BUTTON4) {
                DialogBox(hInst, MAKEINTRESOURCE(IDD_SEARCHBYCITYANDCOUNTRY), hDlg, SEARCHBYCITYANDCOUNTRY);
            }
            
            break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK INSERT(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    int c_index;
    int mID = LOWORD(wParam);
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
        case WM_INITDIALOG:
            c_index = SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"Male");
            c_index = SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"Female");
            c_index = SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"Other");
       
            return (INT_PTR)TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
            {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            else if (mID == IDC_BUTTON1)
            {
                SaveToFile(hDlg);
            }
            break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK EDIT(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    int c_index;
    static int personID;

    switch (message)
    {
        case WM_INITDIALOG:
            c_index = SendDlgItemMessage(hDlg, IDC_COMBO2, CB_ADDSTRING, 0, (LPARAM)"Male");
            c_index = SendDlgItemMessage(hDlg, IDC_COMBO2, CB_ADDSTRING, 0, (LPARAM)"Female");
            c_index = SendDlgItemMessage(hDlg, IDC_COMBO2, CB_ADDSTRING, 0, (LPARAM)"Other");

            return (INT_PTR)TRUE;

        case WM_COMMAND:
        {
            int mID = LOWORD(wParam);
            switch (mID)
            {
            case IDOK:
            {
                char idBuffer[256];
                GetDlgItemTextA(hDlg, IDC_EDIT1, idBuffer, sizeof(idBuffer));
                personID = std::stoi(idBuffer);

                if (LoadDataForID(personID, hDlg))
                {
                    SaveEditedDataToTextFile(personID, hDlg);
                    EndDialog(hDlg, IDOK);
                }
                else
                {
                    MessageBox(hDlg, "ID not found!", "Error", MB_OK | MB_ICONERROR);
                }
                
                return (INT_PTR)TRUE;
            }

            case IDCANCEL:
                EndDialog(hDlg, IDCANCEL);
                return (INT_PTR)TRUE;

            case IDC_BUTTONLOAD:
                LoadDataFromFileAndFillControls(hDlg);
                return (INT_PTR)TRUE;

            case IDC_BUTTONSAVE:
                SaveChangesToFile(hDlg);
                return (INT_PTR)TRUE;
            }
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK DELETEP(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    switch (message)
    {
        case WM_INITDIALOG:
            return (INT_PTR)TRUE;

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
            case IDOK:
            {
                char idBuffer[256];
                GetDlgItemTextA(hDlg, IDC_EDIT1, idBuffer, sizeof(idBuffer));
                int personID = std::stoi(idBuffer);

                DeletePersonById(personID, hDlg);

                EndDialog(hDlg, IDOK);
                return (INT_PTR)TRUE;
            }

            case IDCANCEL:
                EndDialog(hDlg, IDCANCEL);
                return (INT_PTR)TRUE;
            }
            break;
    }

    return (INT_PTR)FALSE;
}
bool VerifyInput(const std::string& input) {
    if (input.empty()) {
        return false;
    }

    if (!std::isupper(input[0])) {
        return false;
    }

    for (size_t i = 0; i < input.length(); ++i) {
        if (!std::isalpha(input[i])) {
            return false;
        }
    }

    return true;
}


bool VerifyEGN(const std::string& egn) {
    if (egn.length() != 10) {
        return false;
    }

    for (char c : egn) {
        if (!std::isdigit(c)) {
            return false;
        }
    }

    return true;
}
bool VerifyAlphabetic(const std::string& input) {
    if (input.empty()) {
        return false;
    }

    for (char c : input) {
        if (!std::isalpha(c)) {
            return false;
        }
    }

    return true;
}


bool VerifyNumeric(const std::string& input) {
    if (input.empty()) {
        return false;
    }

    for (char c : input) {
        if (!std::isdigit(c)) {
            return false;
        }
    }

    return true;
}

void SaveToFile(HWND hDlg) {
    static int idCounter = 1;

    std::ifstream counterFile("counter.txt");
    if (counterFile.is_open()) {
        counterFile >> idCounter;
        counterFile.close();
    }
    char buffer[256];
    GetDlgItemTextA(hDlg, IDC_EDIT1, buffer, sizeof(buffer));
    if (!VerifyInput(buffer))
    {
        MessageBox(hDlg, "Invalid input for Name field!", "Error", MB_OK | MB_ICONERROR);
        return;
    }
    GetDlgItemTextA(hDlg, IDC_EDIT2, buffer, sizeof(buffer));
    if (!VerifyInput(buffer))
    {
        MessageBox(hDlg, "Invalid input for MiddleName field!", "Error", MB_OK | MB_ICONERROR);
        return;
    }
    GetDlgItemTextA(hDlg, IDC_EDIT3, buffer, sizeof(buffer));
    if (!VerifyInput(buffer))
    {
        MessageBox(hDlg, "Invalid input for SurName field!", "Error", MB_OK | MB_ICONERROR);
        return;
    }
    GetDlgItemTextA(hDlg, IDC_EDIT4, buffer, sizeof(buffer));
    if(!VerifyEGN(buffer))
    {
        MessageBox(hDlg, "Invalid input for EGN field!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    GetDlgItemTextA(hDlg, IDC_EDIT5, buffer, sizeof(buffer));
    if (!VerifyInput(buffer))
    {
        MessageBox(hDlg, "Invalid input for city field!", "Error", MB_OK | MB_ICONERROR);
        return;
    }
    GetDlgItemTextA(hDlg, IDC_EDIT6, buffer, sizeof(buffer));
    if (!VerifyInput(buffer))
    {
        MessageBox(hDlg, "Invalid input for country field!", "Error", MB_OK | MB_ICONERROR);
        return;
    }
   
    GetDlgItemTextA(hDlg, IDC_EDIT8, buffer, sizeof(buffer));
    if (!VerifyAlphabetic(buffer))
    {
        MessageBox(hDlg, "Invalid input for color of eyes field!", "Error", MB_OK | MB_ICONERROR);
        return;
    }


    std::ofstream file("data.txt", std::ios_base::app);
    if (file.is_open()) {
        char buffer[256];
       
        file << "ID: " << idCounter << std::endl;
        
        GetDlgItemTextA(hDlg, IDC_EDIT1, buffer, sizeof(buffer));
        file << "Name: " << buffer << std::endl;

        GetDlgItemTextA(hDlg, IDC_EDIT2, buffer, sizeof(buffer));
        file << "Middle Name: " << buffer << std::endl;

        GetDlgItemTextA(hDlg, IDC_EDIT3, buffer, sizeof(buffer));
        file << "Last Name: " << buffer << std::endl;

        GetDlgItemTextA(hDlg, IDC_EDIT4, buffer, sizeof(buffer));
        file << "EGN: " << buffer << std::endl;

        GetDlgItemTextA(hDlg, IDC_EDIT5, buffer, sizeof(buffer));
        file << "City: " << buffer << std::endl;

        GetDlgItemTextA(hDlg, IDC_EDIT6, buffer, sizeof(buffer));
        file << "Country: " << buffer << std::endl;

        int selectedIndex = SendDlgItemMessage(hDlg, IDC_COMBO1, CB_GETCURSEL, 0, 0);

        if (selectedIndex != CB_ERR) {
            SendDlgItemMessage(hDlg, IDC_COMBO1, CB_GETLBTEXT, selectedIndex, (LPARAM)buffer);
            file << "Gender: " << buffer << std::endl;
        }
        else
        {
            file << "Gender: Unknown" << std::endl;
        }

        GetDlgItemTextA(hDlg, IDC_EDIT9, buffer, sizeof(buffer));
        file << "Color of eyes: " << buffer << std::endl;

        file << "-------------------------" << std::endl;

        std::ofstream counterOut("counter.txt");
        counterOut << ++idCounter;
        counterOut.close();

        file.close();
        MessageBox(hDlg, "Data saved to file!", "Success", MB_OK | MB_ICONINFORMATION);
    }
    else
    {
        MessageBox(hDlg, "Error opening file for writing!", "Error", MB_OK | MB_ICONERROR);
    }
}

void SaveEditedDataToTextFile(int personID, HWND hDlg)
{
    std::ifstream inputFile("data.txt");
    std::vector<std::string> fileData;
    std::string line;

    while (std::getline(inputFile, line)) {
        fileData.push_back(line);
    }

    inputFile.close();

    std::ofstream outputFile("data_temp.txt");

    for (size_t i = 0; i < fileData.size(); ++i) {
        if (fileData[i].find("ID: " + std::to_string(personID)) != std::string::npos) {
            char buffer[256];

            GetDlgItemTextA(hDlg, IDC_EDITNAME, buffer, sizeof(buffer));
            fileData[i + 1] = "Name: " + std::string(buffer);

            GetDlgItemTextA(hDlg, IDC_EDITMIDDLENAME, buffer, sizeof(buffer));
            fileData[i + 2] = "Middle Name: " + std::string(buffer);

            GetDlgItemTextA(hDlg, IDC_EDITLASTNAME, buffer, sizeof(buffer));
            fileData[i + 3] = "Last Name: " + std::string(buffer);

            GetDlgItemTextA(hDlg, IDC_EDITEGN, buffer, sizeof(buffer));
            fileData[i + 4] = "EGN: " + std::string(buffer);

            GetDlgItemTextA(hDlg, IDC_EDITCITY, buffer, sizeof(buffer));
            fileData[i + 5] = "City: " + std::string(buffer);

            GetDlgItemTextA(hDlg, IDC_EDITCOUNTRY, buffer, sizeof(buffer));
            fileData[i + 6] = "Country: " + std::string(buffer);

            int genderIndex = SendDlgItemMessage(hDlg, IDC_COMBOGENDER, CB_GETCURSEL, 0, 0);

            if (genderIndex != CB_ERR) {
                SendDlgItemMessage(hDlg, IDC_COMBOGENDER, CB_GETLBTEXT, genderIndex, (LPARAM)buffer);
                fileData[i + 7] = "Gender: " + std::string(buffer);
            }
            else {
                fileData[i + 7] = "Gender: Unknown";
            }

            GetDlgItemTextA(hDlg, IDC_EDIT10, buffer, sizeof(buffer));
            fileData[i + 8] = "Color of eyes: " + std::string(buffer);

            break;
        }
    }

    for (const auto& data : fileData) {
        outputFile << data << std::endl;
    }

    outputFile.close();

    std::remove("data.txt");
    std::rename("data_temp.txt", "data.txt");
}

bool LoadDataForID(int personID, HWND hDlg)
{
    std::ifstream inputFile("data.txt");
    std::string line;

    while (std::getline(inputFile, line))
    {
        if (line.find("ID: " + std::to_string(personID)) != std::string::npos)
        {
            char buffer[256];

            for (int i = 1; i <= 8; ++i)
            {
                std::getline(inputFile, line);
                size_t pos = line.find(":");
                if (pos != std::string::npos)
                {
                    std::string attribute = line.substr(0, pos);
                    std::string value = line.substr(pos + 2);
                    TrimString(value);

                    int controlID = GetControlIDFromAttribute(attribute);
                    if (controlID != -1)
                    {
                        SetDlgItemTextA(hDlg, controlID, value.c_str());
                        OutputDebugStringA(("Loaded: " + attribute + " - " + value + "\n").c_str());
                    }
                    else
                    {
                        OutputDebugStringA(("Unknown attribute: " + attribute + "\n").c_str());
                    }
                }
            }

            std::getline(inputFile, line);
            size_t pos = line.find(":");
            if (pos != std::string::npos)
            {
                std::string attribute = line.substr(0, pos);
                std::string value = line.substr(pos + 2);
                TrimString(value);

                int index = SendDlgItemMessageA(hDlg, IDC_COMBO2, CB_FINDSTRINGEXACT, -1, (LPARAM)value.c_str());
                SendDlgItemMessageA(hDlg, IDC_COMBO2, CB_SETCURSEL, index, 0);
                OutputDebugStringA(("Loaded: Gender - " + value + "\n").c_str());
            }

            inputFile.close();
            return true;
        }
    }

    inputFile.close();
    return false;
}

void TrimString(std::string& str)
{
    size_t first = str.find_first_not_of(" \t\r\n");
    size_t last = str.find_last_not_of(" \t\r\n");

    if (first != std::string::npos && last != std::string::npos)
    {
        str = str.substr(first, last - first + 1);
    }
    else
    {
        str.clear();
    }
}

int GetControlIDFromAttribute(const std::string& attribute)
{
    if (attribute == "Name") return IDC_EDITNAME;
    else if (attribute == "Middle Name") return IDC_EDITMIDDLENAME;
    else if (attribute == "Last Name") return IDC_EDITLASTNAME;
    else if (attribute == "EGN") return IDC_EDITEGN;
    else if (attribute == "City") return IDC_EDITCITY;
    else if (attribute == "Country") return IDC_EDITCOUNTRY;
    else if (attribute == "Gender") return IDC_COMBOGENDER;
    else if (attribute == "Color of eyes") return IDC_EDIT10;

    return -1;
}

void LoadDataFromFileAndFillControls(HWND hDlg)
{
    char idBuffer[256];
    GetDlgItemTextA(hDlg, IDC_EDIT1, idBuffer, sizeof(idBuffer));
    
    
    int personID = std::stoi(idBuffer);

    if (LoadDataForID(personID, hDlg))
    {
        MessageBox(hDlg, "Data loaded successfully!", "Success", MB_OK | MB_ICONINFORMATION);
    }
    else
    {
        MessageBox(hDlg, "ID not found!", "Error", MB_OK | MB_ICONERROR);
    }
}

void SaveChangesToFile(HWND hDlg)
{
    char idBuffer[256];
    GetDlgItemTextA(hDlg, IDC_EDIT1, idBuffer, sizeof(idBuffer));
    int personID = std::stoi(idBuffer);

    SaveEditedDataToTextFile(personID, hDlg);

    MessageBox(hDlg, "Changes saved successfully!", "Success", MB_OK | MB_ICONINFORMATION);
}

void DeletePersonById(int personID, HWND hDlg)
{
    std::ifstream inputFile("data.txt");
    std::vector<std::string> fileData;
    std::string line;

    bool personFound = false;
    bool skipRecord = false;

    while (std::getline(inputFile, line)) {
        if (!personFound && line.find("ID: " + std::to_string(personID)) != std::string::npos) {
            personFound = true;
            skipRecord = true;
            continue;
        }

        if (skipRecord) {
            if (line.find("-------------------------") != std::string::npos) {
                skipRecord = false;
            }
            continue;
        }

        fileData.push_back(line);
    }

    inputFile.close();

    std::ofstream outputFile("data_temp.txt");

    for (const auto& data : fileData) {
        outputFile << data << std::endl;
    }

    outputFile.close();

    std::remove("data.txt");
    std::rename("data_temp.txt", "data.txt");

    if (personFound) {
        MessageBox(hDlg, "Person deleted successfully!", "Success", MB_OK | MB_ICONINFORMATION);
    }
    else {
        MessageBox(hDlg, "Person not found!", "Error", MB_OK | MB_ICONERROR);
    }
}

void SaveRecordsWithGivenNameToFile(const std::string& desiredName) {
    std::ifstream inputFile("data.txt");
    std::ofstream outputFile("result.txt", std::ios::trunc);

    if (inputFile.is_open() && outputFile.is_open()) {
        std::string line;
        bool foundRecord = false;
        while (std::getline(inputFile, line)) {
            if (line.find("Name: " + desiredName) != std::string::npos) {
                foundRecord = true;
                outputFile << line << std::endl;
                for (int i = 1; i <=8; ++i) {
                    std::getline(inputFile, line);
                    outputFile << line << std::endl;
                }
                outputFile << "-------------------------" << std::endl;
            }
        }

        if (!foundRecord) {
            outputFile << "No records found for the given name." << std::endl;
        }

        inputFile.close();
        outputFile.close();
    }
    else 
    {
        std::cerr << "Error opening one or both files." << std::endl;
    }
}


INT_PTR CALLBACK Search1(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    int mID = LOWORD(wParam);
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        if (mID == IDC_BUTTON1)
        {
            char nameBuffer[256];
            GetDlgItemTextA(hDlg, IDC_EDIT1, nameBuffer, sizeof(nameBuffer));
            if (!VerifyInput(nameBuffer))
            {
                MessageBox(hDlg, "Invalid input for Name field!", "Error", MB_OK | MB_ICONERROR);
                return (INT_PTR)TRUE;
            }
            else

            {
                SaveRecordsWithGivenNameToFile(nameBuffer);
                MessageBox(hDlg, "Records with given name saved to file!", "Success", MB_OK | MB_ICONINFORMATION);
            }
        }
        break;
    }
    return (INT_PTR)FALSE;
}
void SaveRecordsWithGivenColorOfEyesToFile(const std::string& desiredColorOfEyes) {
    std::ifstream inputFile("data.txt");
    std::ofstream outputFile("result.txt", std::ios::trunc);

    if (inputFile.is_open() && outputFile.is_open()) {
        std::string line;
        bool foundRecord = false;
        std::deque<std::string> previousLines;

        while (std::getline(inputFile, line)) {
            previousLines.push_back(line);

            if (previousLines.size() > 9) {
                previousLines.pop_front();
            }

            if (line.find("Color of eyes: " + desiredColorOfEyes) != std::string::npos) {
                foundRecord = true;
                outputFile << "-------------------------" << std::endl;

                for (const auto& prevLine : previousLines) {
                    outputFile << prevLine << std::endl;
                }

                outputFile << "-------------------------" << std::endl;
            }
        }

        if (!foundRecord) {
            outputFile << "No records found for the given eye colour." << std::endl;
        }

        inputFile.close();
        outputFile.close();
    }
    else {
        std::cerr << "Error opening one or both files." << std::endl;
    }
}


INT_PTR CALLBACK SEARCHBYEYES(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    int mID = LOWORD(wParam);
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (mID == IDC_BUTTON1)
        {
            char colorBuffer[256];
            GetDlgItemTextA(hDlg, IDC_EDIT1, colorBuffer, sizeof(colorBuffer));
            if (!VerifyAlphabetic(colorBuffer))
            {
                MessageBox(hDlg, "Invalid input for color of eyes field!", "Error", MB_OK | MB_ICONERROR);
                return (INT_PTR)TRUE;
            }
            else

            {
                SaveRecordsWithGivenColorOfEyesToFile(colorBuffer);
                MessageBox(hDlg, "Records with this color of eyes saved to file!", "Success", MB_OK | MB_ICONINFORMATION);
            }
           
            
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK SEARCHBYAGE(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    int mID = LOWORD(wParam);
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (mID == IDC_BUTTON1) {
            char ageBuffer[256], * stopstring;
            GetDlgItemText(hDlg, IDC_EDIT1, ageBuffer, 256);

            try {
                int targetAge = std::stoi(ageBuffer);
                SearchPeopleByAgeAndWriteToFile(targetAge);
                MessageBox(hDlg, "Records with this age saved to file!", "Success", MB_OK | MB_ICONINFORMATION);
            }
            catch (const std::exception& e) {
                MessageBox(hDlg, "Invalid age input!", "Error", MB_OK | MB_ICONERROR);
            }
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void SearchPeopleByAgeAndWriteToFile(int targetAge) {
    std::ifstream inputFile("data.txt");
    std::ofstream outputFile("result.txt", std::ios::trunc);

    if (inputFile.is_open() && outputFile.is_open()) {
        std::string line;
        std::vector<std::string> personInfo;

        while (std::getline(inputFile, line)) {
            if (line.find_first_not_of(' ') == std::string::npos) {
                continue;
            }

            personInfo.push_back(line);

            if (line.find("-------------------------") != std::string::npos) {
                int age = ExtractAgeFromPersonInfo(personInfo);

                
                if (age == targetAge) {
                    for (const auto& info : personInfo) {
                        outputFile << info << std::endl;
                    }
                    outputFile << "-------------------------" << std::endl;
                }

                personInfo.clear();
            }
        }

        int age = ExtractAgeFromPersonInfo(personInfo);
        if (!personInfo.empty() && age == targetAge) {
            for (const auto& info : personInfo) {
                outputFile << info << std::endl;
            }
            outputFile << "-------------------------" << std::endl;
        }

        inputFile.close();
        outputFile.close();
    }
    else {
        std::cerr << "Error opening one or both files." << std::endl;
    }
}

int ExtractAgeFromPersonInfo(const std::vector<std::string>& personInfo) {
    int age = 0;
    for (const auto& info : personInfo) {
        size_t pos = info.find("EGN: ");
        if (pos != std::string::npos) {
            std::string egn = info.substr(pos + 5, 10);

            int year = std::stoi(egn.substr(0, 2));
            int month = std::stoi(egn.substr(2, 2));
            int day = std::stoi(egn.substr(4, 2));

            int century;
            if (month > 0 && month <= 12) {
                century = 1900;
            }
            else if (month > 20 && month <= 32) {
                century = 1800;
                month -= 20;
            }
            else {
                century = 2000;
                month -= 40;
            }

            struct tm birthDate = { 0 };
            birthDate.tm_year = century + year - 1900;
            birthDate.tm_mon = month - 1;
            birthDate.tm_mday = day;

            time_t birthTime = mktime(&birthDate);
            time_t currentTime = time(nullptr);
            double secondsPerYear = 60 * 60 * 24 * 365.25;
            age = static_cast<int>((currentTime - birthTime) / secondsPerYear);

            break;
        }
    }
    return age;
}

void SearchByCityAndCountry(const std::string& desiredCity, const std::string& desiredCountry) {
    std::ifstream inputFile("data.txt");
    std::ofstream outputFile("result.txt", std::ios::trunc);

    if (inputFile.is_open() && outputFile.is_open()) {
        std::string line;
        bool foundRecord = false;
        std::deque<std::string> previousLines;

        while (std::getline(inputFile, line)) {
            previousLines.push_back(line);

            if (previousLines.size() >=7) {
                previousLines.pop_front();
            }

            if (line.find("City: " + desiredCity) != std::string::npos) {
                std::getline(inputFile, line);
                if (line.find("Country: " + desiredCountry) != std::string::npos) {
                    foundRecord = true;

                    for (const auto& prevLine : previousLines) {
                        outputFile << prevLine << std::endl;
                    }

                    for (int i = 0; i <= 2; ++i) {
                        std::getline(inputFile, line);
                        outputFile << line << std::endl;
                    }
                }
            }
        }

        if (!foundRecord) {
            outputFile << "No records found for the given city and country." << std::endl;
        }

        inputFile.close();
        outputFile.close();
    }
    else {
        std::cerr << "Error opening one or both files." << std::endl;
    }
}

INT_PTR CALLBACK SEARCHBYCITYANDCOUNTRY(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    int mID = LOWORD(wParam);

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (mID == IDC_BUTTON1)
        {
            
            char nameBuffer[256];
            char nameBuffer2[256];

            GetDlgItemTextA(hDlg, IDC_EDIT1, nameBuffer, sizeof(nameBuffer));
            GetDlgItemTextA(hDlg, IDC_EDIT2, nameBuffer2, sizeof(nameBuffer2));
           
            if ((!VerifyInput(nameBuffer))||(!VerifyInput(nameBuffer2)))
            {
                MessageBox(hDlg, "Invalid input for city or country field!", "Error", MB_OK | MB_ICONERROR);
                return (INT_PTR)TRUE;
            }
            else

            {
                SearchByCityAndCountry(nameBuffer, nameBuffer2);
                MessageBox(hDlg, "Records with given city and country saved to file!", "Success", MB_OK | MB_ICONINFORMATION);
            }
        }
        break;
    }
    return (INT_PTR)FALSE;
}
