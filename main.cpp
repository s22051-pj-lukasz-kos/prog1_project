/*
    Podstaw� pod zaimplementowane funkcje z biblioteki windows.h sta� si� Microsoft'owy dokument o konsoli
    https://docs.microsoft.com/en-us/windows/console/


    1. program ma umo�liwi� wczytanie rozmiar�w na samym pocz�tku
    2. z biblioteki windows.h pobieramy rozmiar okna
    3. musz� wykombinowac sterowanie:   https://en.cppreference.com/w/cpp/header; input/output library
                                        https://en.cppreference.com/w/cpp/io

    TO DO:
        1. napisa� funkcj� do zarz�dzania wielko�ci� znaku (ustali� warunki na bazie wielko�ci pola
        2. napisa� funkcj�, kt�ra ogarniczy poruszanie si� po ekranie (ustali granice)

*/

#include <iostream>

// szereg zaimplementowanych funkcji do zarz�dzania kursorem, oknem, screen bufforem i klawiatur�
#include <windows.h>

using namespace std;

void inputChar(char &sign);         // wpisywanie wy�wietlanego znaku ASCII
void screenBufferInfo(HANDLE hOut); // zbieranie informacji o screen buffer
void startScreen ();                // ekran powitalny
void clearScreen(HANDLE hOut);      // czy�ci ekran
void tutor();                       // sterowanie wy�wietlane u g�ry ekranu
COORD GetCursorPosition(HANDLE hOut);   // zwraca pozycj� kursora
COORD heightWidthWindow (HANDLE hOut);  // funkcja do wyliczania wysoko�ci i szeroko�ci buffor screen w oknie
void cursorPosition(HANDLE hOut);       // ustawia pozycj� kursora
COORD cursorPositionStart(HANDLE hOut);  // ustawia pozycj� kursora po uruchomieniu
int keyEvent(HANDLE hIn);               // funkcja zwraca wybrane virtual-key codes (numery klawiszy)
void eventManager(HANDLE hOut);
void controls(HANDLE hOut, bool &run, int vKeyCode, char sign, short &symbSize);              // funkcja do poruszania si� kursorem
void drawSymbol (HANDLE hOut, char sign, short &symbSize, COORD cursorPosition);     // funkcja do rysowania symbolu
void symbolSizeManager(HANDLE hOut, short &symbSize);
COORD boundaries(HANDLE hOut, COORD cursorPosition, COORD modPosition, short &symbSize);

int main()
{
    setlocale(LC_ALL, "");

    HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);   // ��cznik z wyj�ciem standardowym z konsoli, z windows.h
    HANDLE hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);   // ��cznik z wej�ciem standardowym z konsoli
    char letter;                // wy�wietlany znak
    short symbolSize;             // wielko�� wy�wietlanego symbolu
    COORD middleWindow;         // �rodek okna
    bool running = true;        // zmienna do przerywania dzia�ania programu
    // zaimplementowana typ zmiennej do zmiany wygl�du kursora
    CONSOLE_CURSOR_INFO conCurInfo = {100, TRUE};

    SetConsoleTitle("s22051");  // tytu� okna, z windows.h
    startScreen();              // ekran powitalny
    inputChar(letter);          // wpisywanie znaku
    symbolSize = 3;
    clearScreen(hConsoleOut);   // czyszczenie ekranu
    middleWindow = cursorPositionStart(hConsoleOut);      // ustawia kursor na �rodek ekranu
    SetConsoleCursorInfo(hConsoleOut, &conCurInfo); // z windows.h, zmienia wygl�d kursora

    drawSymbol(hConsoleOut, letter, symbolSize, middleWindow);        // drukowanie symbolu
    do {
        controls(hConsoleOut, running, keyEvent(hConsoleIn), letter, symbolSize);
    } while (running);



    return 0;
}

// funkcja do wpisywania i zwracania drukowanego znaku
void inputChar(char &sign) {
    cout << "Prosz� wpisa� dowolny znak" << endl;
    cin >> sign;
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
    DWORD charWritten;              // zwracana warto��. Musia�em j� zadeklarowa�, bo inaczej funkcja FillConsoleOutputCharacter przerywa�� dzia�anie programu

    GetConsoleScreenBufferInfo(hOut, &consoleBufferInfo);    // zaimplementowana funkcja, kt�ra zbiera informacje o screen buffer i przekazuje do zmiennej consoleBufferInfo
    screenBufferTotalChar = consoleBufferInfo.dwSize.X * consoleBufferInfo.dwSize.Y;    // opis przy deklaracji zmiennych

    FillConsoleOutputCharacter(hOut, ' ', screenBufferTotalChar, startPoint, &charWritten); // zaimplementowana funkcja do wype�nienia ekranu wybranym znakiem
}

// sterowanie wy�wietlane u g�ry ekranu
void tutor() {

}

// ustawia pozycj� kursora na �rodek ekranu
COORD cursorPositionStart(HANDLE hOut) {
    COORD middleScreen;

    // obliczam �rodek ekranu w osi X
    middleScreen.X = heightWidthWindow(hOut).X / 2;
    // obliczam �rodek ekranu w osi Y
    middleScreen.Y = heightWidthWindow(hOut).Y / 2;

    SetConsoleCursorPosition(hOut, middleScreen);   // zaimplementowana funkcja do ustawiania pozycji kursora
    return middleScreen;
}

// funkcja do wyliczania wysoko�ci i szeroko�ci buffor screen w oknie
COORD heightWidthWindow (HANDLE hOut) {
    CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo;   // zaimplemmentowana zmienna, kt�ra otrzyma informacje o screen buffer
    COORD heightWidthWindowBuffer;      // zaimplementowany typ zmiennej, kt�ra zwraca wysoko�� i szeroko�� screen buffer w oknie

    GetConsoleScreenBufferInfo(hOut, &consoleBufferInfo);    // zaimplementowana zmienna, kt�ra zbiera informacje o screen buffer i przekazuje do zmiennej consoleBufferInfo
    heightWidthWindowBuffer.X = consoleBufferInfo.srWindow.Right - consoleBufferInfo.srWindow.Left;
    heightWidthWindowBuffer.Y = consoleBufferInfo.srWindow.Bottom - consoleBufferInfo.srWindow.Top;

    return heightWidthWindowBuffer;
}

// zwraca pozycj� kursora
COORD GetCursorPosition(HANDLE hOut) {
    CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo;   // zaimplemmentowana zmienna, kt�ra otrzyma informacje o screen buffer

    GetConsoleScreenBufferInfo(hOut, &consoleBufferInfo);    // zaimplementowana zmienna, kt�ra zbiera informacje o screen buffer i przekazuje do zmiennej consoleBufferInfo
    return consoleBufferInfo.dwCursorPosition;
}

// ustawia pozycj� kursora
void setCursorPosition(HANDLE hOut, COORD posChange) {

}

// funkcja zwraca virtual-key codes (numery klawiszy)
int keyEvent(HANDLE hIn) {
    INPUT_RECORD eventInputArray[128];  // tablica event�w
    DWORD numInput;     // liczba zarejestrowanych event�w
    DWORD keyCode;        // zwracany kod
    ReadConsoleInput(hIn, eventInputArray, 128, &numInput);  // zaimplementowana funkcja
    for (DWORD i=0; i < numInput; i++) {
            if (eventInputArray[i].Event.KeyEvent.bKeyDown) {   // gdy klawisz jest wci�ni�ty (puszczenie klawisza to osobny event)
                keyCode = eventInputArray[i].Event.KeyEvent.wVirtualKeyCode;
            }
    }
    return keyCode;
}

// funkcja do zarz�dzania sterowaniem
void controls(HANDLE hOut, bool &run, int vKeyCode, char sign, short &symbSize) {
    COORD cursorPosition = GetCursorPosition(hOut);
    COORD newPosition = cursorPosition;

    switch (vKeyCode) {
        case 37 : {     // left arrow
            newPosition.X--;
            newPosition = boundaries(hOut, cursorPosition, newPosition, symbSize);
            drawSymbol(hOut, sign, symbSize, newPosition);
            break; }
        case 38 : {     // up arrow
            newPosition.Y--;
            newPosition = boundaries(hOut, cursorPosition, newPosition, symbSize);
            drawSymbol(hOut, sign, symbSize, newPosition);
            break; }
        case 39 : {     // right arrow
            newPosition.X++;
            newPosition = boundaries(hOut, cursorPosition, newPosition, symbSize);
            drawSymbol(hOut, sign, symbSize, newPosition);
            break; }
        case 40 : {     // down arrow
            newPosition.Y++;
            newPosition = boundaries(hOut, cursorPosition, newPosition, symbSize);
            drawSymbol(hOut, sign, symbSize, newPosition);
            break; }
        case 187 : {    // plus
            symbSize++;
            drawSymbol(hOut, sign, symbSize, cursorPosition);
            break; }
        case 189 : {    // minus
            symbSize--;
            drawSymbol(hOut, sign, symbSize, cursorPosition);
            break; }
        case 27 : {     // ESC
            run = false;
            break; }
    }
}

// funkcja do rysowania symbolu
void drawSymbol (HANDLE hOut, char sign, short &symbSize, COORD cursorPosition) {
    DWORD written;
    COORD symbolPartCoord = cursorPosition;

    // czyszczenie ekranu i ustalanie pozycji kursora
    clearScreen(hOut);
    // zaimplementowana funkcja z windows.h
    SetConsoleCursorPosition(hOut, cursorPosition);

    // algorytm do rysowania symbolu
    for (int i=0; i <= 2*symbSize; i++) {
        symbolPartCoord.Y = cursorPosition.Y - i;
        if (i <= symbSize) {
            symbolPartCoord.X = cursorPosition.X + i;
        } else {
            symbolPartCoord.X = cursorPosition.X + (2 * symbSize) % i;
        }
        FillConsoleOutputCharacterA(hOut, sign, 1, symbolPartCoord, &written);
    }
}

// funkcja do zarz�dzania wielko�ci� symbolu
void symbolSizeManager(HANDLE hOut, short &symbSize) {
    COORD heightAndWidth = heightWidthWindow(hOut);


}

// funkcja ustalaj�ca granice w poruszaniu si�
COORD boundaries(HANDLE hOut, COORD cursorPosition, COORD newPosition, short &symbSize) {
    COORD heightAndWidth = heightWidthWindow(hOut);
    COORD returnPosition;

    // warunek okre�laj�cy granice poruszania si�
    if (newPosition.X >= 0 && newPosition.X < heightAndWidth.X - symbSize &&
            newPosition.Y > 2 * symbSize && newPosition.Y < heightAndWidth.Y) {
        returnPosition = newPosition;
    } else
        returnPosition = cursorPosition;

    return returnPosition;
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
