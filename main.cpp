#include "mainwindow.h"
#include "loginwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Tower_of_Saviors_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow *w = new MainWindow;
    //w.show();
    loginwindow l;
    QObject::connect(&l,&loginwindow::start,w,&MainWindow::show);
    QObject::connect(&l,&loginwindow::start,w,&MainWindow::onCharactersSelected);
    l.show();
    return a.exec();
}
