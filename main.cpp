#include <QApplication>
#include <QTranslator>

#include "ChWindow.h"


int main(int ac, char *av[])
{
    QApplication app(ac, av);

    QTranslator translator;
    translator.load("chords_ru");
    app.installTranslator(&translator);

    ChWindow main;
    main.show();

    return app.exec();
}


