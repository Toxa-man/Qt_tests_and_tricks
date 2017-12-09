#include "audioclass.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AudioClass w;
    w.show();

    return a.exec();
}
