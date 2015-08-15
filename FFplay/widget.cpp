#include <QDesktopWidget>
#include "widget.h"
#include "ui_widget.h"

Widget::Widget(PlayerProcess *playerProcess, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    playProcess(playerProcess)
{
    if(playProcess != NULL){
        directPlay = true;
        connect(playProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(playProcess_finished(int,QProcess::ExitStatus)));
    }
    else{
        directPlay = false;
    }
    if(!directPlay){
        ui->setupUi(this);
        resetSize();
    }
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/icon.png"));
    if(!directPlay){
        showAction = new QAction(tr("show"),this);
        connect(showAction, SIGNAL(triggered(bool)), this, SLOT(show_mainWindow()));
    }
    helpAction = new QAction(tr("help"), this);
    connect(helpAction, SIGNAL(triggered(bool)), this, SLOT(show_help()));
    aboutAction = new QAction(tr("about"), this);
    connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(show_about()));
    quitAction = new QAction(tr("quit"), this);
    connect(quitAction, SIGNAL(triggered(bool)), this, SLOT(quit()));
    trayMenu = new QMenu(this);
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIcon_activited(QSystemTrayIcon::ActivationReason)));
    if(!directPlay){
        trayMenu->addAction(showAction);
    }
    trayMenu->addAction(helpAction);
    trayMenu->addAction(aboutAction);
    trayMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();
    setAcceptDrops(true);
}

Widget::~Widget()
{
    if(!directPlay){
        delete showAction;
    }
    delete quitAction;
    delete helpAction;
    delete aboutAction;
    delete trayMenu;
    delete trayIcon;
    delete ui;
    if(playProcess != NULL)
        delete playProcess;
}

void Widget::on_buttonMedia_clicked()
{
    QString dir;
    if(!mediaFile.isNull() && !mediaFile.isEmpty()){
        dir = QFileInfo(mediaFile).absolutePath();
    }
    else if(!subtitleFile.isNull() && !subtitleFile.isEmpty()){
        dir = QFileInfo(subtitleFile).absolutePath();
    }
    else{
        dir = "d:/";
    }
    mediaFile = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    dir,
                                                    tr("Media (*)"));
    ui->leMedia->setText(mediaFile);
    QString path = QFileInfo(mediaFile).absoluteFilePath();
    QString basePath = path.left(path.indexOf(QFileInfo(mediaFile).suffix()));
    QString suffixes[3] = {"srt", "ass", "lrc"};
    for(int i = 0; i < 3; i++){
        if(QFileInfo(basePath + suffixes[i]).exists()){
            ui->leSubtitle->setText(basePath + suffixes[i]);
            break;
        }
    }
}

void Widget::on_buttonSubtitle_clicked()
{
    QString dir;
    if(!subtitleFile.isNull() && !subtitleFile.isEmpty()){
        dir = QFileInfo(subtitleFile).absolutePath();
    }
    else if(!mediaFile.isNull() && !mediaFile.isEmpty()){
        dir = QFileInfo(mediaFile).absolutePath();
    }
    else{
        dir = "d:/";
    }
    subtitleFile = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    dir,
                                                    tr("Subtitle (*.srt *.ass *.lrc)"));
    ui->leSubtitle->setText(subtitleFile);
}

void Widget::on_buttonPlay_clicked()
{
    playProcess = new PlayerProcess(ui->leMedia->text(), ui->leSubtitle->text());
    playProcess->startPlay();
    connect(playProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(playProcess_finished(int,QProcess::ExitStatus)));
    ui->buttonPlay->setEnabled(false);
    this->hide();
}

void Widget::playProcess_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if(!directPlay){
        ui->buttonPlay->setEnabled(true);
        this->show();
    }
    else{
        close();
    }
}

void Widget::trayIcon_activited(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::DoubleClick){
        this->activateWindow();
        this->showNormal();
    }
}

void Widget::resetSize()
{
    int screenWidth = QApplication::desktop()->width();
    if (screenWidth > 1920){
        double factor = 2;
        QWidget *widget;
        widget = this;
        qDebug() << screenWidth << ", " << factor;
        widget->resize((int)(widget->width()*factor),(int)(widget->height()*factor));
        widget = ui->labelMedia;
        ui->labelMedia->setText(QString::number(screenWidth));
        widget->setGeometry((int)(widget->x()*factor),(int)(widget->y()*factor),(int)(widget->width()*factor),(int)(widget->height()*factor));
        widget = ui->labelSubtitle;
        widget->setGeometry((int)(widget->x()*factor),(int)(widget->y()*factor),(int)(widget->width()*factor),(int)(widget->height()*factor));
        widget = ui->leSubtitle;
        widget->setGeometry((int)(widget->x()*factor),(int)(widget->y()*factor),(int)(widget->width()*factor),(int)(widget->height()*factor));
        widget = ui->leMedia;
        widget->setGeometry((int)(widget->x()*factor),(int)(widget->y()*factor),(int)(widget->width()*factor),(int)(widget->height()*factor));
        widget = ui->buttonMedia;
        widget->setGeometry((int)(widget->x()*factor),(int)(widget->y()*factor),(int)(widget->width()*factor),(int)(widget->height()*factor));
        widget = ui->buttonSubtitle;
        widget->setGeometry((int)(widget->x()*factor),(int)(widget->y()*factor),(int)(widget->width()*factor),(int)(widget->height()*factor));
        widget = ui->buttonPlay;
        widget->setGeometry((int)(widget->x()*factor),(int)(widget->y()*factor),(int)(widget->width()*factor),(int)(widget->height()*factor));
    }
}

void Widget::closeEvent(QCloseEvent *event)
{
    if(playProcess != NULL){
        playProcess->close();
    }
    event->accept();
}

void Widget::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    if(isMinimized()){
        this->hide();
    }
}

void Widget::show_mainWindow()
{
    this->showNormal();
}

void Widget::show_help()
{
    QStringList actions;
    actions << "q, ESC           \n"
            << "f                \n"
            << "p, Space         \n"
            << "a                \n"
            << "v                \n"
            << "t                \n"
            << "s                \n"
            << "n                \n"
            << "1-9              \n"
            << "r                \n"
            << "0                \n"
            << "left/right       \n"
            << "up/down          \n"
            << "Alt up/down      \n"
            << "Page up/down     \n"
            << "mouse click      \n";
    QStringList helps;
    helps << "quit\n"
          << "toggle full screen\n"
          << "pause\n"
          << "cycle audio channel in the current program\n"
          << "cycle video channel\n"
          << "cycle internal subtitle channel in the current program\n"
          << "toggle subtitle show\n"
          << "step to next frame\n"
          << "repeat to play current sentence n times\n"
          << "press to record, press again to stop and repeat to play the record\n"
          << "stop repeating\n"
          << "seek backward/forward one sentence if there is a subtitle, or 5 seconds\n"
          << "volume up/volume down\n"
          << "speed up/speed down playing\n"
          << "seek forward/backward 5 seconds if there is a subtitle, or 20 seconds\n"
          << "seek to percentage in file corresponding to fraction of width\n";

    QMessageBox msgBox(QMessageBox::NoIcon, tr("Help"), tr(""), QMessageBox::Ok, NULL, Qt::Sheet);
    QLabel action(actions.join(""));
    QGridLayout *layout = dynamic_cast< QGridLayout *>(msgBox.layout());
    layout->addWidget(&action, 0, 0, 1, 1);
    QLabel help(helps.join(""));
    layout->addWidget(&help, 0, 1, 1, 1);
    msgBox.exec();
}

void Widget::show_about()
{
    QMessageBox msgBox(QMessageBox::NoIcon, tr("About FFplay"), tr(""), QMessageBox::Ok, NULL, Qt::Sheet);
    QLabel about("Author: Shawn Ding\nEmail: chyxwzn@foxmail.com\nVersion: v1.0 based on ffmpeg\n\nIf you find out any bug or have any good idea, please contact me.");
    QGridLayout *layout = dynamic_cast< QGridLayout *>(msgBox.layout());
    layout->addWidget(&about, 0, 0, 1, 1);
    msgBox.exec();
}

void Widget::quit()
{
    if(playProcess != NULL){
        playProcess->close();
    }
    this->close();
}
