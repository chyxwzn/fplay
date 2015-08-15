#include "widget.h"
#include <QApplication>
#include <QTextCodec>
#include "playerprocess.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PlayerProcess *playProcess = NULL;
    if(argc > 1){
        QTextCodec *codec = QTextCodec::codecForLocale();
        QString mediaPath = codec->toUnicode(argv[1]);
        playProcess = new PlayerProcess(mediaPath, NULL);
        playProcess->startPlay();
    }
    Widget w(playProcess);
    if(playProcess == NULL)
        w.show();
    return a.exec();
}
