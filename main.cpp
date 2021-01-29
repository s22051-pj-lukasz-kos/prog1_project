/*
    Podstaw¹ pod zaimplementowane funkcje z biblioteki windows.h sta³ siê Microsoft'owy dokument o konsoli
    https://docs.microsoft.com/en-us/windows/console/
*/

#include <iostream>
// szereg zaimplementowanych funkcji do zarz¹dzania kursorem, oknem, screen bufforem i klawiatur¹
#include <windows.h>

using namespace std;

COORD heightWidthWindow(HANDLE hOut);  // funkcja do wyliczania wysokoœci i szerokoœci screen buffor w oknie
void startScreen(HANDLE hOut, int symbSize); // ekran powitalny
void inputChar(char &sign); // wpisywanie wyœwietlanego znaku ASCII
void inputSize(HANDLE hOut, int &symbSize); // wpisanie wielkoœci symbolu
void clearScreen(HANDLE hOut, bool all);  // czyœci ekran w ca³oœci lub od 2-go wiersza
void tutor(HANDLE hOut);    // sterowanie wyœwietlane u góry ekranu
COORD startCursorPosition(HANDLE hOut); // pozycja startowa symbolu
COORD getCursorPosition(HANDLE hOut);   // zwraca pozycjê kursora
// funkcja do rysowania symbolu
void drawSymbol(HANDLE hOut, char sign, int symbSize, COORD cursorPosition);
// funkcja ograniczaj¹ca rozmiar symbolu
void symbolSizeManager(HANDLE hOut, int &symbSize, int mod, COORD cursorPosition);
// funkcja ograniczaj¹ca poruszanie siê
COORD boundaries(HANDLE hOut, COORD cursorPosition, COORD newPosition, int &symbSize);
DWORD keyEvent(HANDLE hIn);   // funkcja zwraca wybrane virtual-key codes (numery klawiszy)
// funkcja do poruszania siê kursorem
void controls(HANDLE hOut, bool &run, DWORD vKeyCode, char sign, int &symbSize);


int main()
{
    setlocale(LC_ALL, "");

    // windows.h, handle ze standardowym wyjœciem i wejœciem konsoli
    HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
    // zaimplementowany typ zmiennej do zmiany wygl¹du kursora
    CONSOLE_CURSOR_INFO conCurInfo = {100, TRUE};
    char letter;            // wyœwietlany znak
    int symbolSize = 2;     // wielkoœæ wyœwietlanego symbolu
    COORD startPosition;    // œrodek okna
    bool running = true;    // zmienna do przerywania dzia³ania programu

    // windows.h, tytu³ okna
    SetConsoleTitle("Grupa 115, Lukasz Kos, s22051, Projekt 115");
    startScreen(hConsoleOut, symbolSize);   // ekran powitalny
    inputChar(letter);                      // wpisywanie znaku
    inputSize(hConsoleOut, symbolSize);     // wpisywanie pocz¹tkowego rozmiaru
    clearScreen(hConsoleOut, true);         // czyszczenie ekranu
    tutor(hConsoleOut);                     // sterowanie wyœwietlane u góry ekranu

    // ustawia kursor na œrodek ekranu
    startPosition = startCursorPosition(hConsoleOut);
    // windows.h, zmienia wygl¹d kursora
    SetConsoleCursorInfo(hConsoleOut, &conCurInfo);
    // drukowanie symbolu
    drawSymbol(hConsoleOut, letter, symbolSize, startPosition);

    // sterowanie, ustalanie wielkoœci i przerywanie programu
    do {
        controls(hConsoleOut, running, keyEvent(hConsoleIn), letter, symbolSize);
    } while (running);

    return 0;
}

// funkcja do wyliczania wysokoœci i szerokoœci buffor screen w oknie
COORD heightWidthWindow (HANDLE hOut) { // standardowe wyjœcie z konsoli
    // zaimplementowana zmienna, która otrzyma informacje o screen buffer
    CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo;
    // zaimplementowana zmienna, która zwraca wysokoœæ i szerokoœæ screen buffer w oknie
    COORD maxSize;

    // windows.h, funkcja zbiera informacje o screen buffer i przekazuje do zmiennej consoleBufferInfo
    GetConsoleScreenBufferInfo(hOut, &consoleBufferInfo);
    maxSize.X = consoleBufferInfo.srWindow.Right - consoleBufferInfo.srWindow.Left;
    maxSize.Y = consoleBufferInfo.srWindow.Bottom - consoleBufferInfo.srWindow.Top;

    return maxSize;
}

// ekran powitalny
void startScreen (HANDLE hOut, int symbSize) {
    cout << "Program wyœwietla poni¿szy symbol i umo¿liwa poruszanie siê nim " <<
         "po ekranie oraz zmianê jego rozmiaru.\n" << endl;

    // czêœæ wyœwietlaj¹ca symbol na œrodku ekranu
    COORD cursorPosition;
    cursorPosition.X = heightWidthWindow(hOut).X / 2;
    cursorPosition.Y = heightWidthWindow(hOut).Y / 4;
    drawSymbol (hOut, 'x', symbSize, cursorPosition);

    // resetowanie pozycji kursora pod symbol
    cursorPosition.X = 0;
    cursorPosition.Y++;
    SetConsoleCursorPosition(hOut, cursorPosition);
}

// funkcja do wpisywania drukowanego znaku
void inputChar(char &sign) {
    cout << "Proszê wpisaæ dowolny znak" << endl;
    cin >> sign;
}

// funkcja do wpisywania rozmiaru symbolu
void inputSize(HANDLE hOut, int &symbSize) {
    // pobieranie rozmiaru okna
    COORD maxSize = heightWidthWindow(hOut);
    // obliczenie maksymalnej wartoœci
    int mSize = maxSize.Y / 2 - 1;

    cout << "\nProszê wpisaæ rozmiar symbolu (2 - " << mSize << ")" << endl;
    cin >> symbSize;

    // warunek do sprawdzenia czy wpisany rozmiar mieœci siê w wymaganym zakresie
    while (symbSize < 2 || symbSize > mSize) {
        cout << "Wpisa³eœ rozmiar poza zakresem. Wpisz wartoœæ od 2 do " << mSize << endl;
        cin >> symbSize;
    }
}

// czyœci ekran w ca³oœci b¹dŸ od drugiego wiersza (tutorial w pierwszej linijce)
void clearScreen(HANDLE hOut, bool all) {
    // zaimplementowana zmienna, która otrzyma informacje o screen buffer
    CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo;
    DWORD screenBufferTotalChar;    // sumaryczna liczba znaków screen buffer
    COORD startPoint;   // punkt startowy czyszczenia ekranu
    /* zwracana wartoœæ. Musia³em j¹ zadeklarowaæ, bo inaczej funkcja
        FillConsoleOutputCharacter przerywa³¹ dzia³anie programu */
    DWORD charWritten;

    // windows.h, funkcja zbiera informacje o screen buffer i przekazuje do zmiennej consoleBufferInfo
    GetConsoleScreenBufferInfo(hOut, &consoleBufferInfo);
    // obliczam iloœæ znaków (miejsc) na ekranie
    screenBufferTotalChar = consoleBufferInfo.dwSize.X * consoleBufferInfo.dwSize.Y;

    // jeœli parametr all = false to ekran jest czyszczony od drugiego wiersza
    startPoint.X = 0;
    if (all) {
        startPoint.Y = 0;
    } else {
        startPoint.Y = 1;
    }
    // windows.h, funkcja do wype³nienia ekranu ci¹giem znaków od wybranej pozycji
    FillConsoleOutputCharacter(hOut, ' ', screenBufferTotalChar, startPoint, &charWritten);
}

// sterowanie wyœwietlane u góry ekranu
void tutor(HANDLE hOut) {
    COORD cursorPosition = {0, 0};

    SetConsoleCursorPosition(hOut, cursorPosition);
    cout << "Strza³ki : sterowanie \t+/- : zmiana rozmiaru \t ESC : wyjœcie z programu" << endl;
}

// pozycja startowa ustawiaj¹ca symbol w lewym dolnym rogu
COORD startCursorPosition(HANDLE hOut) {
    COORD maxSize = heightWidthWindow(hOut);
    COORD startPosition;

    startPosition.X = 0;
    startPosition.Y = maxSize.Y;

    return startPosition;
}

// zwraca pozycjê kursora
COORD getCursorPosition(HANDLE hOut) {
    // zaimplemmentowana zmienna, która otrzyma informacje o screen buffer
    CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo;

    // windows.h, funkcja zbiera informacje o screen buffer i przekazuje do zmiennej consoleBufferInfo
    GetConsoleScreenBufferInfo(hOut, &consoleBufferInfo);

    return consoleBufferInfo.dwCursorPosition;
}

// funkcja do rysowania symbolu
void drawSymbol (HANDLE hOut, char sign, int symbSize, COORD cursorPosition) {
    DWORD written;
    // zmienna do poruszania siê po fragmentach symbolu
    COORD symbolPartCoord = cursorPosition;

    // czyszczenie ekranu
    clearScreen(hOut, false);

    // zaimplementowana funkcja z windows.h do ustalania pozycji kursora
    SetConsoleCursorPosition(hOut, cursorPosition);

    /* algorytm do rysowania symbolu,
		kursor znajduje siê w lewym dolnym rogu i od tego miejsca
		zaczyna siê drukowanie znaków */

    for (int i=0; i <= 2*symbSize; i++) {
        symbolPartCoord.Y = cursorPosition.Y - i;
        if (i <= symbSize) {
            symbolPartCoord.X = cursorPosition.X + (2 * i);
        } else {
            symbolPartCoord.X = cursorPosition.X + ((2 * symbSize) % i) * 2;
        }
        // windows.h, rysuje znak sign we wspó³rzêdnych symbolPartCoord
        FillConsoleOutputCharacterA(hOut, sign, 1, symbolPartCoord, &written);
    }
}

// funkcja do zarz¹dzania wielkoœci¹ symbolu
void symbolSizeManager(HANDLE hOut, int &symbSize, int mod, // modyfikator rozmiaru symbolu
                       COORD cursorPosition) {
    // pobieranie rozmiaru okna
    COORD maxSize = heightWidthWindow(hOut);
    // nowy rozmiar do przetestowania
    int newSize = symbSize + mod;

    // warunek ograniczaj¹cy wielkoœæ symbolu do rozmiaru okna
    if (cursorPosition.X<=(maxSize.X - (2 * newSize)) && cursorPosition.Y>(2 * newSize) && newSize >= 2)
        symbSize = newSize;
}

// funkcja ustalaj¹ca granice w poruszaniu siê
COORD boundaries(HANDLE hOut, COORD cursorPosition, // obecna pozycja
                 COORD newPosition, // nowa pozycja do sprawdzenia
                 int &symbSize) {
    // pobieranie rozmiaru okna
    COORD maxSize = heightWidthWindow(hOut);
    COORD returnPosition;

    // warunek okreœlaj¹cy granice poruszania siê
    if (newPosition.X >= 0 && newPosition.X <= (maxSize.X - (2 * symbSize)) &&
        newPosition.Y > (2 * symbSize) && newPosition.Y < maxSize.Y + 1) {
        // jeœli newPosition mieœci siê w screen buffer to jest zwracany
        returnPosition = newPosition;
    } else
        // jeœli nie to jest zwracana obecna pozycja
        returnPosition = cursorPosition;

    return returnPosition;
}

// funkcja œledzi eventy na wejœciu i zwraca virtual-key codes (numery klawiszy)
DWORD keyEvent(HANDLE hIn) {
    INPUT_RECORD eventInputArray[128];  // tablica eventów
    DWORD numInput; // liczba zarejestrowanych eventów
    DWORD keyCode = 0;  // zwracany kod


    // windows.h, rejestruje eventy na wejœciu
    ReadConsoleInput(hIn, eventInputArray, 128, &numInput);
    for (DWORD i=0; i < numInput; i++) {
        // rejestruje tylko wciœniêcie klawisza (puszczenie to osobny event)
        if (eventInputArray[i].Event.KeyEvent.bKeyDown) {
            keyCode = eventInputArray[i].Event.KeyEvent.wVirtualKeyCode;
        }
    }
    // zwraca virtual-key code
    return keyCode;
}

// funkcja do zarz¹dzania sterowaniem
void controls(HANDLE hOut, bool &run, // zmienna przerywaj¹ca dzia³anie programu
              DWORD vKeyCode, // virtual-key code dostarczany z funkcji keyEvent
              char sign, int &symbSize) {
    // pobiera pozycjê kursora
    COORD cursorPosition = getCursorPosition(hOut);
    // potencjalnie nowa pozycja do przetestowania (granica screen buffer)
    COORD newPosition = cursorPosition;
    // modyfikacja rozmiaru
    int mod;

    switch (vKeyCode) {
        case 37 : {     // left arrow
            newPosition.X--;
            /* newPosition jest testowany w boundaries i zwracana wartoœæ jest
               teraz nowym newPosition */
            newPosition = boundaries(hOut, cursorPosition, newPosition, symbSize);
            break; }
        case 38 : {     // up arrow
            newPosition.Y--;
            newPosition = boundaries(hOut, cursorPosition, newPosition, symbSize);
            break; }
        case 39 : {     // right arrow
            newPosition.X++;
            newPosition = boundaries(hOut, cursorPosition, newPosition, symbSize);
            break; }
        case 40 : {     // down arrow
            newPosition.Y++;
            newPosition = boundaries(hOut, cursorPosition, newPosition, symbSize);
            break; }
        case 107 : {    // plus na numerycznej
            // modyfikator rozmiaru
            mod = 1;
            // testowanie nowego rozmiaru symbolu
            symbolSizeManager(hOut, symbSize, mod, newPosition);
            break; }
        case 109 : {    // minus na numerycznej
            mod = -1;
            symbolSizeManager(hOut, symbSize, mod, newPosition);
            break; }
        case 187 : {    // plus
            mod = 1;
            symbolSizeManager(hOut, symbSize, mod, newPosition);
            break; }
        case 189 : {    // minus
            mod = -1;
            symbolSizeManager(hOut, symbSize, mod, newPosition);
            break; }
        case 27 : {     // ESC
            run = false;
            break; }
        default: {}
    }
    drawSymbol(hOut, sign, symbSize, newPosition);
}
