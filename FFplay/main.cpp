#include "widget.h"
#include <QApplication>
#include <QDebug>
#include "playerprocess.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PlayerProcess *playProcess = NULL;
    if(argc > 1){
        playProcess = new PlayerProcess(argv[1], NULL);
        playProcess->startPlay();
    }
    Widget w(playProcess);
    if(playProcess == NULL)
        w.show();
    return a.exec();
}
