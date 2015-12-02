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
        QString path = QFileInfo(mediaPath).absoluteFilePath();
        QString basePath = path.left(path.indexOf(QFileInfo(mediaPath).suffix()));
        QString suffixes[3] = {"srt", "ass", "lrc"};
        QString subPath = NULL;
        for(int i = 0; i < 3; i++){
            if(QFileInfo(basePath + suffixes[i]).exists()){
                subPath = basePath + suffixes[i];
                break;
            }
        }
        playProcess = new PlayerProcess(mediaPath, subPath);
        playProcess->startPlay();
    }
    Widget w(playProcess);
    if(playProcess == NULL)
        w.show();
    return a.exec();
}
