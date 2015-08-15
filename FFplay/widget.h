#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QDebug>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QGridLayout>
#include <QMapIterator>
#include "playerprocess.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(PlayerProcess *playerProcess, QWidget *parent = 0);
    ~Widget();

protected:
    void closeEvent(QCloseEvent *event);
    void changeEvent(QEvent *event);

private slots:
    void on_buttonMedia_clicked();

    void on_buttonSubtitle_clicked();

    void on_buttonPlay_clicked();

    void show_help();

    void show_about();

    void show_mainWindow();

    void quit();

    void playProcess_finished(int exitCode, QProcess::ExitStatus exitStatus);

    void trayIcon_activited(QSystemTrayIcon::ActivationReason reason);

private:
    Ui::Widget *ui;
    QSystemTrayIcon *trayIcon;
    QAction *showAction;
    QAction *helpAction;
    QAction *aboutAction;
    QAction *quitAction;
    QMenu *trayMenu;
    QString mediaFile;
    QString subtitleFile;
    bool directPlay;
    PlayerProcess *playProcess;

    void resetSize();
};

#endif // WIDGET_H
