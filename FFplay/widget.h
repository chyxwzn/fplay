#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QProcess>
#include <QDebug>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QGridLayout>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
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
    QProcess *playProcess;
};

#endif // WIDGET_H
