/*
    1. program ma umo�liwi� wczytanie rozmiar�w na samym pocz�tku
    2. z biblioteki windows.h pobieramy rozmiar okna
    3. musz� wykombinowac sterowanie:   https://en.cppreference.com/w/cpp/header; input/output library
                                        https://en.cppreference.com/w/cpp/io
*/

#include <iostream>
#include <windows.h> // przesuwanie i pobieranie rozmiar�w okna

using namespace std;

char inputChar(char sign);          // wpisywanie wy�wietlanego znaku ASCII
void screenBufferInfo(HANDLE hOut); // zbieranie informacji o screen buffer
void startScreen ();                // ekran powitalny
void clearScreen(HANDLE hOut);      // czy�ci ekran
void tutor();                       // sterowanie wy�wietlane u g�ry ekranu
void cursorPosition();              // ustawia pozycj� kursora
void cursorPositionStart();         // ustawia pozycj� kursora po uruchomieniu
void cursorMovement();              // funkcja do poruszania si� kursorem

int main()
{
    setlocale(LC_ALL, "");

    HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);   // ��cznik z wyj�ciem standardowym z konsoli
    char letter;                // wy�wietlany znak

    SetConsoleTitle("s22051");  // tytu� okna
    startScreen();              // ekran powitalny
    letter = inputChar(letter); // wpisywanie znaku
    clearScreen(hConsoleOut);   // czyszczenie ekranu

    screenBufferInfo(hConsoleOut);  // zbieranie informacji o screen buffer



    return 0;
}

// funkcja do wpisywania i zwracania drukowanego znaku
char inputChar(char sign) {
    cout << "Prosz� wpisa� dowolny znak" << endl;
    cin >> sign;
    return sign;
}

// funkcja testowa do testowania GetConsoleScreenBufferInfo()
void screenBufferInfo(HANDLE hOut) {
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;  // zmienna zawieraj�ca dane z konsoli
    char dalej;

    do {
        GetConsoleScreenBufferInfo(hOut, &consoleInfo);   // funkcja zbiera informacje o oknie konsoli
        cout << "\nSize of the console screen buffer. X = " << consoleInfo.dwSize.X << " & Y = " << consoleInfo.dwSize.Y << endl;

        /* Poni�ej najwa�niejsza warto�� w dobieraniu obszaru roboczego do wykorzystania. Dostosowuje si� do wielko�ci okna i scrollingu */
        cout << "Console screen buffer coordinates. Top = " << consoleInfo.srWindow.Top << ", Left = " << consoleInfo.srWindow.Left <<
            ", Bottom = " << consoleInfo.srWindow.Bottom << ", Right = " << consoleInfo.srWindow.Right << endl;
        cout << "Maximum size of console window. X = " << consoleInfo.dwMaximumWindowSize.X << " & Y = " << consoleInfo.dwMaximumWindowSize.Y << endl;
        cout << "Cursor position. X = " << consoleInfo.dwCursorPosition.X << " & Y = " << consoleInfo.dwCursorPosition.Y << endl;
        cout << "Attributes of the characters. " << consoleInfo.wAttributes << endl;

        // TEST


        cout << "\nNajwi�ksza mo�liwa wielko�� okna bazuj�ca na funkcji zewn�trznej. X = " << GetLargestConsoleWindowSize(hOut).X << " & Y = " << GetLargestConsoleWindowSize(hOut).Y << endl;

        cout << "\nczy kontynuowa�?" << endl;
        cin >> dalej;
    } while (dalej == 't');
}

// ekran powitalny
void startScreen () {
    cout << "Ekran powitalny.\n" << endl;
}

// czy�ci ekran
void clearScreen(HANDLE hOut) {
    CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo;   // zaimplemmentowana zmienna, kt�ra otrzyma informacje o screen buffer
    DWORD screenBufferTotalChar;    // sumaryczna liczba znak�w screen buffer
    COORD startPoint = {0, 0};      // punkt startowy czyszczenia ekranu

    GetConsoleScreenBufferInfo(hOut, &consoleBufferInfo);    // zaimplementowana zmienna, kt�ra zbiera informacje o screen buffer i przekazuje do zmiennej consoleBufferInfo
    screenBufferTotalChar = consoleBufferInfo.dwSize.X * consoleBufferInfo.dwSize.Y;    // opis przy deklaracji zmiennych

    FillConsoleOutputCharacter(hOut, ' ', screenBufferTotalChar, startPoint, NULL); // zaimplementowana funkcja do wype�nienia ekranu wybranym znakiem
}

// sterowanie wy�wietlane u g�ry ekranu
void tutor() {

}

// ustawia pozycj� kursora
void cursorPosition() {

}

// ustawia pozycj� kursora po uruchomieniu
void cursorPositionStart() {

}

// funkcja do poruszania si� kursorem
void cursorMovement() {

}
