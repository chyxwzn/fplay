#ifndef PLAYERPROCESS_H
#define PLAYERPROCESS_H

#include <QProcess>

class PlayerProcess : public QProcess
{
public:
    PlayerProcess(QString mediaPath, QString subtitlePath, bool hwAccel = false);
    ~PlayerProcess();
    void startPlay();
private:
    QString player;
    QStringList arguments;
    QString getLastPosition(QString fileName);
};

#endif // PLAYERPROCESS_H
