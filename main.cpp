/*
    1. program ma umo¿liwiæ wczytanie rozmiarów na samym pocz¹tku
    2. z biblioteki windows.h pobieramy rozmiar okna
    3. muszê wykombinowac sterowanie:   https://en.cppreference.com/w/cpp/header; input/output library
                                        https://en.cppreference.com/w/cpp/io
*/

#include <iostream>
#include <windows.h> // przesuwanie i pobieranie rozmiarów okna

using namespace std;

void inputChar(char &sign);          // wpisywanie wyœwietlanego znaku ASCII
void screenBufferInfo(HANDLE hOut); // zbieranie informacji o screen buffer
void startScreen ();                // ekran powitalny
void clearScreen(HANDLE hOut);      // czyœci ekran
void tutor();                       // sterowanie wyœwietlane u góry ekranu
void cursorPosition(HANDLE hOut);              // ustawia pozycjê kursora
void cursorPositionStart(HANDLE hOut);         // ustawia pozycjê kursora po uruchomieniu
void cursorMovement();              // funkcja do poruszania siê kursorem

int main()
{
    setlocale(LC_ALL, "");

    HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);   // ³¹cznik z wyjœciem standardowym z konsoli
    HANDLE hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);   // ³¹cznik z wejœciem standardowym z konsoli
    char letter;                // wyœwietlany znak

    SetConsoleTitle("s22051");  // tytu³ okna
    startScreen();              // ekran powitalny
    inputChar(letter);          // wpisywanie znaku
    clearScreen(hConsoleOut);   // czyszczenie ekranu
    cursorPositionStart(hConsoleOut);      // ustawia kursor na œrodek ekranu
    cout << letter;

//    screenBufferInfo(hConsoleOut);  // zbieranie informacji o screen buffer

    INPUT_RECORD eventInputArray[128];
    DWORD numInput;
    int escChecker;
    do {
        ReadConsoleInput(hConsoleIn, eventInputArray, 128, &numInput);
        for (int i=0; i<numInput; i++) {
                if (eventInputArray[i].Event.KeyEvent.bKeyDown) {
                    escChecker = eventInputArray[i].Event.KeyEvent.wVirtualKeyCode;
                    cout << "wVirtualKeyCode: " << eventInputArray[i].Event.KeyEvent.wVirtualKeyCode << endl;
                    cout << "wVirtualScanCode: " << eventInputArray[i].Event.KeyEvent.wVirtualScanCode << endl;
                }
           }
    } while (escChecker != 27); // ESC




    return 0;
}

// funkcja do wpisywania i zwracania drukowanego znaku
void inputChar(char &sign) {
    cout << "Proszê wpisaæ dowolny znak" << endl;
    cin >> sign;
}

// funkcja testowa do testowania GetConsoleScreenBufferInfo()
void screenBufferInfo(HANDLE hOut) {
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;  // zmienna zawieraj¹ca dane z konsoli
    char dalej;

    do {
        GetConsoleScreenBufferInfo(hOut, &consoleInfo);   // funkcja zbiera informacje o oknie konsoli
        cout << "\nSize of the console screen buffer. X = " << consoleInfo.dwSize.X << " & Y = " << consoleInfo.dwSize.Y << endl;

        /* Poni¿ej najwa¿niejsza wartoœæ w dobieraniu obszaru roboczego do wykorzystania. Dostosowuje siê do wielkoœci okna i scrollingu */
        cout << "Console screen buffer coordinates. Top = " << consoleInfo.srWindow.Top << ", Left = " << consoleInfo.srWindow.Left <<
            ", Bottom = " << consoleInfo.srWindow.Bottom << ", Right = " << consoleInfo.srWindow.Right << endl;
        cout << "Maximum size of console window. X = " << consoleInfo.dwMaximumWindowSize.X << " & Y = " << consoleInfo.dwMaximumWindowSize.Y << endl;
        cout << "Cursor position. X = " << consoleInfo.dwCursorPosition.X << " & Y = " << consoleInfo.dwCursorPosition.Y << endl;
        cout << "Attributes of the characters. " << consoleInfo.wAttributes << endl;

        // TEST


        cout << "\nNajwiêksza mo¿liwa wielkoœæ okna bazuj¹ca na funkcji zewnêtrznej. X = " << GetLargestConsoleWindowSize(hOut).X << " & Y = " << GetLargestConsoleWindowSize(hOut).Y << endl;

        cout << "\nczy kontynuowaæ?" << endl;
        cin >> dalej;
    } while (dalej == 't');
}

// ekran powitalny
void startScreen () {
    cout << "Ekran powitalny.\n" << endl;
}

// czyœci ekran
void clearScreen(HANDLE hOut) {
    CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo;   // zaimplemmentowana zmienna, która otrzyma informacje o screen buffer
    DWORD screenBufferTotalChar;    // sumaryczna liczba znaków screen buffer
    COORD startPoint = {0, 0};      // punkt startowy czyszczenia ekranu
    DWORD charWritten;              // zwracana wartoœæ. Musia³em j¹ zadeklarowaæ, bo inaczej funkcja FillConsoleOutputCharacter przerywa³¹ dzia³anie programu

    GetConsoleScreenBufferInfo(hOut, &consoleBufferInfo);    // zaimplementowana zmienna, która zbiera informacje o screen buffer i przekazuje do zmiennej consoleBufferInfo
    screenBufferTotalChar = consoleBufferInfo.dwSize.X * consoleBufferInfo.dwSize.Y;    // opis przy deklaracji zmiennych

    FillConsoleOutputCharacter(hOut, ' ', screenBufferTotalChar, startPoint, &charWritten); // zaimplementowana funkcja do wype³nienia ekranu wybranym znakiem

    SetConsoleCursorPosition(hOut, startPoint); // resetuje pozycjê kursora na pocz¹tek
}

// sterowanie wyœwietlane u góry ekranu
void tutor() {

}

// ustawia pozycjê kursora
void cursorPosition(HANDLE hOut) {

}

// ustawia pozycjê kursora na œrodek ekranu
void cursorPositionStart(HANDLE hOut) {
    CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo;
    COORD middleScreen;

    GetConsoleScreenBufferInfo(hOut, &consoleBufferInfo);

    // obliczam œrodek ekranu w osi X
    middleScreen.X = (consoleBufferInfo.srWindow.Right - consoleBufferInfo.srWindow.Left) / 2;
    // obliczam œrodek ekranu w osi Y
    middleScreen.Y = (consoleBufferInfo.srWindow.Bottom - consoleBufferInfo.srWindow.Top) / 2;

    SetConsoleCursorPosition(hOut, middleScreen);   // zaimplementowana funkcja do ustawiania pozycji kursora
}

// funkcja do poruszania siê kursorem
void cursorMovement() {

}
