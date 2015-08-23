#include "playerprocess.h"
#include <QApplication>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QMessageBox>

PlayerProcess::PlayerProcess(QString mediaPath, QString subtitlePath, bool hwAccel):QProcess(),
    player("player.exe")
{
    QString pos = getLastPosition(mediaPath);
    if(!pos.isNull()){
        arguments << "-ss" << pos;
    }
    arguments << "-autoexit";
    if(hwAccel)
        arguments << "-hwaccel";

    arguments << "-workdir" << QApplication::applicationDirPath();
    QString suffix = QFileInfo(mediaPath).suffix();
    QString audioTypes[8] = {"mp3","wma","wav","m4a","amr","ogg","aac","ape"};
    bool isAudio = false;
    for(int i = 0; i < 8; i++){
        if(suffix == audioTypes[i]){
            isAudio = true;
            break;
        }
    }
    if(isAudio)
        arguments << "-vn";
    if(!subtitlePath.isNull() && !subtitlePath.isEmpty()){
        arguments << "-sub" << subtitlePath;
        if(isAudio)
            arguments << "-force_style" << "FontSize=30,MarginV=100";
    }
    arguments << mediaPath;

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("FONTCONFIG_PATH", QApplication::applicationDirPath()+"/fonts");
    env.insert("FONTCONFIG_FILE", QApplication::applicationDirPath()+"/fonts/fonts.conf");
    setProcessEnvironment(env);
}

PlayerProcess::~PlayerProcess()
{

}

QString PlayerProcess::getLastPosition(QString fileName)
{
    QFile file(QApplication::applicationDirPath()+"/history");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return NULL;
    }
    QTextStream history(&file);
    history.setCodec("utf-8");
    QMap<QString, QString> map;
    QString lastPosition = NULL;
    int lineCount = 0;
    while(1){
        QString line = history.readLine();
        if(line.isNull())
            break;
        lineCount++;
        QString position = line.right(line.length() - line.indexOf("last_position:") - 14);
        QString filePath = line.left(line.indexOf("last_position:") - 1);
        if(map.contains(filePath)){
            map.remove(filePath);
        }
        map.insert(filePath, position);
    }
    if(map.contains(fileName))
        lastPosition = map[fileName];

    QMap<QString, QString>::iterator it;
    if(map.size() > 10){
        it = map.begin();
        for(int i = 0; i < map.size() - 10; i++){
            map.erase(it);
            it++;
        }
    }
    if(lineCount != map.size()){
        file.close();
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        history.setDevice(&file);
        for(it = map.begin(); it != map.end(); it++){
            history << it.key() << " last_position:" << it.value() << endl;
        }
    }
    history.flush();
    file.close();
    return lastPosition;
}

void PlayerProcess::startPlay()
{
//    QMessageBox msgBox(QMessageBox::NoIcon, tr("test"), arguments.join(" "), QMessageBox::Ok, NULL, Qt::Sheet);
//    msgBox.exec();
    start(player, arguments);
}

