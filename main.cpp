/*
    Podstaw� pod zaimplementowane funkcje z biblioteki windows.h sta� si� Microsoft'owy dokument o konsoli
    https://docs.microsoft.com/en-us/windows/console/


    TO DO:
        1. problem ze zmian� wielko�ci okna - gdy okno zostaje zmniejszone do momentu gdy ekran przesuwa si� w d� to
            granice s� ustawione poza ekranem (u g�ry). Znale�� spos�b na zresetowanie granic lub u�y� innej
            funkcji windowsowej.

*/

#include <iostream>
// szereg zaimplementowanych funkcji do zarz�dzania kursorem, oknem, screen bufforem i klawiatur�
#include <windows.h>

using namespace std;

COORD heightWidthWindow(HANDLE hOut);  // funkcja do wyliczania wysoko�ci i szeroko�ci screen buffor w oknie
COORD startCursorPosition(HANDLE hOut, int &symbSize); // ustawia pozycj� kursora po uruchomieniu
void startScreen(HANDLE hOut, int &symbSize); // ekran powitalny
void inputChar(char &sign); // wpisywanie wy�wietlanego znaku ASCII
void inputSize(HANDLE hOut, int &symbSize); // wpisanie wielko�ci symbolu
void clearScreen(HANDLE hOut, bool all);  // czy�ci ekran w ca�o�ci lub od 2-go wiersza
void tutor(HANDLE hOut);    // sterowanie wy�wietlane u g�ry ekranu
COORD GetCursorPosition(HANDLE hOut);   // zwraca pozycj� kursora
// funkcja do rysowania symbolu
void drawSymbol(HANDLE hOut, char sign, int symbSize, COORD cursorPosition);
// funkcja ograniczaj�ca rozmiar symbolu
void symbolSizeManager(HANDLE hOut, int &symbSize, int mod, COORD cursorPosition);
// funkcja ograniczaj�ca poruszanie si�
COORD boundaries(HANDLE hOut, COORD cursorPosition, COORD newPosition, int &symbSize);
DWORD keyEvent(HANDLE hIn);   // funkcja zwraca wybrane virtual-key codes (numery klawiszy)
// funkcja do poruszania si� kursorem
void controls(HANDLE hOut, bool &run, DWORD vKeyCode, char sign, int &symbSize);


int main()
{
    setlocale(LC_ALL, "");

    // windows.h, handle ze standardowym wyj�ciem i wej�ciem konsoli
    HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
    // zaimplementowany typ zmiennej do zmiany wygl�du kursora
    CONSOLE_CURSOR_INFO conCurInfo = {100, TRUE};
    char letter;            // wy�wietlany znak
    int symbolSize = 2;       // wielko�� wy�wietlanego symbolu
    COORD startPosition;    // �rodek okna
    bool running = true;    // zmienna do przerywania dzia�ania programu

    // windows.h, tytu� okna
    SetConsoleTitle("Grupa 115, Lukasz Kos, s22051");
    startScreen(hConsoleOut, symbolSize);   // ekran powitalny
    inputChar(letter);                      // wpisywanie znaku
    inputSize(hConsoleOut, symbolSize);     // wpisywanie pocz�tkowego rozmiaru
    clearScreen(hConsoleOut, true);               // czyszczenie ekranu
    tutor(hConsoleOut);                     // sterowanie wy�wietlane u g�ry ekranu

    // ustawia kursor na �rodek ekranu
    startPosition = startCursorPosition(hConsoleOut, symbolSize);
    // windows.h, zmienia wygl�d kursora
    SetConsoleCursorInfo(hConsoleOut, &conCurInfo);
    // drukowanie symbolu
    drawSymbol(hConsoleOut, letter, symbolSize, startPosition);

    // sterowanie, ustalanie wielko�ci i przerywanie programu
    do {
        controls(hConsoleOut, running, keyEvent(hConsoleIn), letter, symbolSize);
    } while (running);

    return 0;
}

// funkcja do wyliczania wysoko�ci i szeroko�ci buffor screen w oknie
COORD heightWidthWindow (HANDLE hOut) {
    // zaimplementowana zmienna, kt�ra otrzyma informacje o screen buffer
    CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo;
    // zaimplementowana zmienna, kt�ra zwraca wysoko�� i szeroko�� screen buffer w oknie
    COORD maxSize;

    // windows.h, funkcja zbiera informacje o screen buffer i przekazuje do zmiennej consoleBufferInfo
    GetConsoleScreenBufferInfo(hOut, &consoleBufferInfo);
    maxSize.X = consoleBufferInfo.srWindow.Right - consoleBufferInfo.srWindow.Left;
    maxSize.Y = consoleBufferInfo.srWindow.Bottom - consoleBufferInfo.srWindow.Top;

    return maxSize;
}

// funkcja ustawia pozycj� kursora na �rodek ekranu
COORD startCursorPosition(HANDLE hOut, int &symbSize) {
    COORD middleScreen;

    // obliczam �rodek ekranu w osi X
    middleScreen.X = heightWidthWindow(hOut).X / 2;
    // obliczam pozycj� kursora w osi Y
    middleScreen.Y = heightWidthWindow(hOut).Y / 2 + symbSize;

    // windows.h, funkcja do ustawiania pozycji kursora
    SetConsoleCursorPosition(hOut, middleScreen);

    return middleScreen;
}

// ekran powitalny
void startScreen (HANDLE hOut, int &symbSize) {
    cout << "Program wy�wietla poni�szy symbol i umo�liwa poruszanie si� nim " <<
         "po ekranie oraz zmian� jego rozmiaru.\n" << endl;

    // cz�� wy�wietlaj�ca symbol na �rodku ekranu
    COORD cursorPosition = startCursorPosition(hOut, symbSize);
    cursorPosition.Y /= 2;
    drawSymbol (hOut, 'x', symbSize, cursorPosition);

    // resetowanie pozycji kursora pod symbol
    cursorPosition.X = 0;
    cursorPosition.Y++;
    SetConsoleCursorPosition(hOut, cursorPosition);
}

// funkcja do wpisywania drukowanego znaku
void inputChar(char &sign) {
    cout << "Prosz� wpisa� dowolny znak" << endl;
    cin >> sign;
}

// funkcja do wpisywania rozmiaru symbolu
void inputSize(HANDLE hOut, int &symbSize) {
    // pobieranie rozmiaru okna
    COORD maxSize = heightWidthWindow(hOut);
    // obliczenie maksymalnej warto�ci
    int mSize = maxSize.Y / 2 - 1;

    cout << "\nProsz� wpisa� rozmiar symbolu (2 - " << mSize << ")" << endl;
    cin >> symbSize;

    // warunek do sprawdzenia czy rozmiar mie�ci si� w wymaganym zakresie
    while (symbSize < 2 || symbSize > mSize) {
        cout << "Wpisa�e� rozmiar poza zakresem. Wpisz warto�� od 2 do " << mSize << endl;
        cin >> symbSize;
    }
}

// czy�ci ekran w ca�o�ci
void clearScreen(HANDLE hOut, bool all) {
    // zaimplemmentowana zmienna, kt�ra otrzyma informacje o screen buffer
    CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo;
    DWORD screenBufferTotalChar;    // sumaryczna liczba znak�w screen buffer
    COORD startPoint;   // punkt startowy czyszczenia ekranu
    /* zwracana warto��. Musia�em j� zadeklarowa�, bo inaczej funkcja
        FillConsoleOutputCharacter przerywa�� dzia�anie programu */
    DWORD charWritten;

    // windows.h, funkcja zbiera informacje o screen buffer i przekazuje do zmiennej consoleBufferInfo
    GetConsoleScreenBufferInfo(hOut, &consoleBufferInfo);
    // obliczam ilo�� znak�w (miejsc) na ekranie
    screenBufferTotalChar = consoleBufferInfo.dwSize.X * (consoleBufferInfo.dwSize.Y);

    startPoint.X = 0;
    if (all) {
        startPoint.Y = 0;
    } else {
        startPoint.Y = 1;
    }
    // windows.h, funkcja do wype�nienia ekranu ci�giem znak�w od wybranej pozycji
    FillConsoleOutputCharacter(hOut, ' ', screenBufferTotalChar, startPoint, &charWritten);
}

// sterowanie wy�wietlane u g�ry ekranu
void tutor(HANDLE hOut) {
    COORD cursorPosition = {0, 0};

    SetConsoleCursorPosition(hOut, cursorPosition);
    cout << "Strza�ki : sterowanie \t+/- : zmiana rozmiaru \t ESC : wyj�cie z programu" << endl;
}

// zwraca pozycj� kursora
COORD GetCursorPosition(HANDLE hOut) {
    // zaimplemmentowana zmienna, kt�ra otrzyma informacje o screen buffer
    CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo;

    // windows.h, funkcja zbiera informacje o screen buffer i przekazuje do zmiennej consoleBufferInfo
    GetConsoleScreenBufferInfo(hOut, &consoleBufferInfo);

    return consoleBufferInfo.dwCursorPosition;
}

// funkcja do rysowania symbolu
void drawSymbol (HANDLE hOut, char sign, int symbSize, COORD cursorPosition) {
    DWORD written;
    // zmienna do poruszania si� po fragmentach symbolu
    COORD symbolPartCoord = cursorPosition;

    // czyszczenie ekranu
    clearScreen(hOut, false);

    // zaimplementowana funkcja z windows.h do ustalania pozycji kursora
    SetConsoleCursorPosition(hOut, cursorPosition);

    // algorytm do rysowania symbolu
    for (int i=0; i <= 2*symbSize; i++) {
        symbolPartCoord.Y = cursorPosition.Y - i;
        if (i <= symbSize) {
            symbolPartCoord.X = cursorPosition.X + i;
        } else {
            symbolPartCoord.X = cursorPosition.X + (2 * symbSize) % i;
        }
        // windows.h, rysuje znak sign we wsp�rz�dnych symbolPartCoord
        FillConsoleOutputCharacterA(hOut, sign, 1, symbolPartCoord, &written);
    }
}

// funkcja do zarz�dzania wielko�ci� symbolu
void symbolSizeManager(HANDLE hOut, int &symbSize, int mod, COORD cursorPosition) {
    // pobieranie rozmiaru okna
    COORD maxSize = heightWidthWindow(hOut);
    int newSize = symbSize + mod;

    // warunek ograniczaj�cy wielko�� symbolu do rozmiaru okna
    if (cursorPosition.X<(maxSize.X - newSize) && cursorPosition.Y>(2 * newSize) && newSize >= 2)
        symbSize = newSize;
}

// funkcja ustalaj�ca granice w poruszaniu si�
COORD boundaries(HANDLE hOut, COORD cursorPosition, COORD newPosition, int &symbSize) {
    // pobieranie rozmiaru okna
    COORD maxSize = heightWidthWindow(hOut);
    COORD returnPosition;

    // warunek okre�laj�cy granice poruszania si�
    if (newPosition.X >= 0 && newPosition.X <= (maxSize.X - symbSize) &&
        newPosition.Y > (2 * symbSize) && newPosition.Y < maxSize.Y + 1) {
        returnPosition = newPosition;
    } else
        returnPosition = cursorPosition;

    return returnPosition;
}

// funkcja �ledzi eventy na wej�ciu i zwraca virtual-key codes (numery klawiszy)
DWORD keyEvent(HANDLE hIn) {
    INPUT_RECORD eventInputArray[128];  // tablica event�w
    DWORD numInput; // liczba zarejestrowanych event�w
    DWORD keyCode = 0;  // zwracany kod


    // windows.h, rejestruje eventy na wej�ciu
    ReadConsoleInput(hIn, eventInputArray, 128, &numInput);
    for (DWORD i=0; i < numInput; i++) {
        // rejestruje tylko wci�ni�cie klawisza (puszczenie to osobny event)
        if (eventInputArray[i].Event.KeyEvent.bKeyDown) {
            keyCode = eventInputArray[i].Event.KeyEvent.wVirtualKeyCode;
        }
    }
    // zwraca virtual-key code
    return keyCode;
}

// funkcja do zarz�dzania sterowaniem
void controls(HANDLE hOut, bool &run, DWORD vKeyCode, char sign, int &symbSize) {
    // pobiera pozycj� kursora
    COORD cursorPosition = GetCursorPosition(hOut);
    COORD newPosition = cursorPosition;
    int mod;

    switch (vKeyCode) {
        case 37 : {     // left arrow
            newPosition.X--;
            newPosition = boundaries(hOut, cursorPosition, newPosition, symbSize);
            //drawSymbol(hOut, sign, symbSize, newPosition, true);
            break; }
        case 38 : {     // up arrow
            newPosition.Y--;
            newPosition = boundaries(hOut, cursorPosition, newPosition, symbSize);
            //drawSymbol(hOut, sign, symbSize, newPosition, true);
            break; }
        case 39 : {     // right arrow
            newPosition.X++;
            newPosition = boundaries(hOut, cursorPosition, newPosition, symbSize);
            //drawSymbol(hOut, sign, symbSize, newPosition, true);
            break; }
        case 40 : {     // down arrow
            newPosition.Y++;
            newPosition = boundaries(hOut, cursorPosition, newPosition, symbSize);
            //drawSymbol(hOut, sign, symbSize, newPosition, true);
            break; }
        case 107 : {    // plus na numerycznej
            mod = 1;
            symbolSizeManager(hOut, symbSize, mod, newPosition);
            //drawSymbol(hOut, sign, symbSize, cursorPosition, true);
            break; }
        case 109 : {    // minus na numerycznej
            mod = -1;
            symbolSizeManager(hOut, symbSize, mod, newPosition);
            // drawSymbol(hOut, sign, symbSize, cursorPosition, true);
            break; }
        case 187 : {    // plus
            mod = 1;
            symbolSizeManager(hOut, symbSize, mod, newPosition);
            // drawSymbol(hOut, sign, symbSize, cursorPosition, true);
            break; }
        case 189 : {    // minus
            mod = -1;
            symbolSizeManager(hOut, symbSize, mod, newPosition);
            // drawSymbol(hOut, sign, symbSize, cursorPosition, true);
            break; }
        case 27 : {     // ESC
            run = false;
            break; }
        default: {}
    }
    drawSymbol(hOut, sign, symbSize, newPosition);
}
