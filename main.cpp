#include "imagecarousel.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageCarousel w;
    w.show();
    return a.exec();
}
