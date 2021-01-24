/*
    Podstaw¹ pod zaimplementowane funkcje z biblioteki windows.h sta³ siê Microsoft'owy dokument o konsoli
    https://docs.microsoft.com/en-us/windows/console/


    1. program ma umo¿liwiæ wczytanie rozmiarów na samym pocz¹tku
    2. z biblioteki windows.h pobieramy rozmiar okna
    3. muszê wykombinowac sterowanie:   https://en.cppreference.com/w/cpp/header; input/output library
                                        https://en.cppreference.com/w/cpp/io

    TO DO:
        1. napisaæ funkcjê do zarz¹dzania wielkoœci¹ znaku (ustaliæ warunki na bazie wielkoœci pola
        2. napisaæ funkcjê, która ogarniczy poruszanie siê po ekranie (ustali granice)

*/

#include <iostream>

// szereg zaimplementowanych funkcji do zarz¹dzania kursorem, oknem, screen bufforem i klawiatur¹
#include <windows.h>

using namespace std;

void inputChar(char &sign);         // wpisywanie wyœwietlanego znaku ASCII
void screenBufferInfo(HANDLE hOut); // zbieranie informacji o screen buffer
void startScreen ();                // ekran powitalny
void clearScreen(HANDLE hOut);      // czyœci ekran
void tutor();                       // sterowanie wyœwietlane u góry ekranu
COORD GetCursorPosition(HANDLE hOut);   // zwraca pozycjê kursora
COORD heightWidthWindow (HANDLE hOut);  // funkcja do wyliczania wysokoœci i szerokoœci buffor screen w oknie
void cursorPosition(HANDLE hOut);       // ustawia pozycjê kursora
COORD cursorPositionStart(HANDLE hOut);  // ustawia pozycjê kursora po uruchomieniu
int keyEvent(HANDLE hIn);               // funkcja zwraca wybrane virtual-key codes (numery klawiszy)
void eventManager(HANDLE hOut);
void controls(HANDLE hOut, bool &run, int vKeyCode, char sign, short &symbSize);              // funkcja do poruszania siê kursorem
void drawSymbol (HANDLE hOut, char sign, short &symbSize, COORD cursorPosition);     // funkcja do rysowania symbolu
void symbolSizeManager(HANDLE hOut, short &symbSize);
COORD boundaries(HANDLE hOut, COORD cursorPosition, COORD modPosition, short &symbSize);

int main()
{
    setlocale(LC_ALL, "");

    HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);   // ³¹cznik z wyjœciem standardowym z konsoli, z windows.h
    HANDLE hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);   // ³¹cznik z wejœciem standardowym z konsoli
    char letter;                // wyœwietlany znak
    short symbolSize;             // wielkoœæ wyœwietlanego symbolu
    COORD middleWindow;         // œrodek okna
    bool running = true;        // zmienna do przerywania dzia³ania programu
    // zaimplementowana typ zmiennej do zmiany wygl¹du kursora
    CONSOLE_CURSOR_INFO conCurInfo = {100, TRUE};

    SetConsoleTitle("s22051");  // tytu³ okna, z windows.h
    startScreen();              // ekran powitalny
    inputChar(letter);          // wpisywanie znaku
    symbolSize = 3;
    clearScreen(hConsoleOut);   // czyszczenie ekranu
    middleWindow = cursorPositionStart(hConsoleOut);      // ustawia kursor na œrodek ekranu
    SetConsoleCursorInfo(hConsoleOut, &conCurInfo); // z windows.h, zmienia wygl¹d kursora

    drawSymbol(hConsoleOut, letter, symbolSize, middleWindow);        // drukowanie symbolu
    do {
        controls(hConsoleOut, running, keyEvent(hConsoleIn), letter, symbolSize);
    } while (running);



    return 0;
}

// funkcja do wpisywania i zwracania drukowanego znaku
void inputChar(char &sign) {
    cout << "Proszê wpisaæ dowolny znak" << endl;
    cin >> sign;
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

    GetConsoleScreenBufferInfo(hOut, &consoleBufferInfo);    // zaimplementowana funkcja, która zbiera informacje o screen buffer i przekazuje do zmiennej consoleBufferInfo
    screenBufferTotalChar = consoleBufferInfo.dwSize.X * consoleBufferInfo.dwSize.Y;    // opis przy deklaracji zmiennych

    FillConsoleOutputCharacter(hOut, ' ', screenBufferTotalChar, startPoint, &charWritten); // zaimplementowana funkcja do wype³nienia ekranu wybranym znakiem
}

// sterowanie wyœwietlane u góry ekranu
void tutor() {

}

// ustawia pozycjê kursora na œrodek ekranu
COORD cursorPositionStart(HANDLE hOut) {
    COORD middleScreen;

    // obliczam œrodek ekranu w osi X
    middleScreen.X = heightWidthWindow(hOut).X / 2;
    // obliczam œrodek ekranu w osi Y
    middleScreen.Y = heightWidthWindow(hOut).Y / 2;

    SetConsoleCursorPosition(hOut, middleScreen);   // zaimplementowana funkcja do ustawiania pozycji kursora
    return middleScreen;
}

// funkcja do wyliczania wysokoœci i szerokoœci buffor screen w oknie
COORD heightWidthWindow (HANDLE hOut) {
    CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo;   // zaimplemmentowana zmienna, która otrzyma informacje o screen buffer
    COORD heightWidthWindowBuffer;      // zaimplementowany typ zmiennej, która zwraca wysokoœæ i szerokoœæ screen buffer w oknie

    GetConsoleScreenBufferInfo(hOut, &consoleBufferInfo);    // zaimplementowana zmienna, która zbiera informacje o screen buffer i przekazuje do zmiennej consoleBufferInfo
    heightWidthWindowBuffer.X = consoleBufferInfo.srWindow.Right - consoleBufferInfo.srWindow.Left;
    heightWidthWindowBuffer.Y = consoleBufferInfo.srWindow.Bottom - consoleBufferInfo.srWindow.Top;

    return heightWidthWindowBuffer;
}

// zwraca pozycjê kursora
COORD GetCursorPosition(HANDLE hOut) {
    CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo;   // zaimplemmentowana zmienna, która otrzyma informacje o screen buffer

    GetConsoleScreenBufferInfo(hOut, &consoleBufferInfo);    // zaimplementowana zmienna, która zbiera informacje o screen buffer i przekazuje do zmiennej consoleBufferInfo
    return consoleBufferInfo.dwCursorPosition;
}

// ustawia pozycjê kursora
void setCursorPosition(HANDLE hOut, COORD posChange) {

}

// funkcja zwraca virtual-key codes (numery klawiszy)
int keyEvent(HANDLE hIn) {
    INPUT_RECORD eventInputArray[128];  // tablica eventów
    DWORD numInput;     // liczba zarejestrowanych eventów
    DWORD keyCode;        // zwracany kod
    ReadConsoleInput(hIn, eventInputArray, 128, &numInput);  // zaimplementowana funkcja
    for (DWORD i=0; i < numInput; i++) {
            if (eventInputArray[i].Event.KeyEvent.bKeyDown) {   // gdy klawisz jest wciœniêty (puszczenie klawisza to osobny event)
                keyCode = eventInputArray[i].Event.KeyEvent.wVirtualKeyCode;
            }
    }
    return keyCode;
}

// funkcja do zarz¹dzania sterowaniem
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

// funkcja do zarz¹dzania wielkoœci¹ symbolu
void symbolSizeManager(HANDLE hOut, short &symbSize) {
    COORD heightAndWidth = heightWidthWindow(hOut);


}

// funkcja ustalaj¹ca granice w poruszaniu siê
COORD boundaries(HANDLE hOut, COORD cursorPosition, COORD newPosition, short &symbSize) {
    COORD heightAndWidth = heightWidthWindow(hOut);
    COORD returnPosition;

    // warunek okreœlaj¹cy granice poruszania siê
    if (newPosition.X >= 0 && newPosition.X < heightAndWidth.X - symbSize &&
            newPosition.Y > 2 * symbSize && newPosition.Y < heightAndWidth.Y) {
        returnPosition = newPosition;
    } else
        returnPosition = cursorPosition;

    return returnPosition;
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
