#include "mainwindow.h"
#include <QApplication>

// Standardowa postać funkcji main() dla aplikacji Qt
// zwykle nie ma potrzeby dokonywania żadnych zmian
int main(int argc, char *argv[])
{
    // Tworzymy obiekt klasy QApplication reprezentujący naszą aplikację
    QApplication a(argc, argv);

    // Tworzymy obiekt zaprojektowanej przez nas klasy MainWindow (główne okno naszej aplikacji)
    // Deklaracja klasy znajduje sie w pliku mainwindow.h, a definicje metod w pliku mainwindow.cpp
    MainWindow w;

    // Wyświetlamy naszą aplikację na ekranie
    w.show();

    // Uruchamiamy główną pętlę programu, od tej pory program oczekuje
    // i reaguje na działania użytkownika (np. ruchy i kliknięcia myszą)
    return a.exec();
}
