#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    playProcess(NULL)
{
    ui->setupUi(this);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(this->windowIcon());
    showAction = new QAction(tr("show"),this);
    connect(showAction, SIGNAL(triggered(bool)), this, SLOT(show_mainWindow()));
    helpAction = new QAction(tr("help"), this);
    connect(helpAction, SIGNAL(triggered(bool)), this, SLOT(show_help()));
    quitAction = new QAction(tr("quit"), this);
    connect(quitAction, SIGNAL(triggered(bool)), this, SLOT(quit()));
    trayMenu = new QMenu(this);
    trayMenu->addAction(showAction);
    trayMenu->addAction(helpAction);
    trayMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();
    setAcceptDrops(true);
}

Widget::~Widget()
{
    delete ui;
    delete trayIcon;
    delete trayMenu;
    delete quitAction;
    delete helpAction;
    delete showAction;
    if(playProcess != NULL)
        delete playProcess;
}

void Widget::on_buttonMedia_clicked()
{
    QString dir;
    if(!subtitleFile.isNull() && !subtitleFile.isEmpty()){
        dir = QFileInfo(subtitleFile).absoluteDir().absolutePath();
    }
    else{
        dir = "/";
    }
    mediaFile = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    dir,
                                                    tr("Media (*)"));
    ui->leMedia->setText(mediaFile);
}

void Widget::on_buttonSubtitle_clicked()
{
    QString dir;
    if(!mediaFile.isNull() && !mediaFile.isEmpty()){
        dir = QFileInfo(mediaFile).absoluteDir().absolutePath();
    }
    else{
        dir = "/";
    }
    subtitleFile = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    dir,
                                                    tr("Subtitle (*.srt *.ass *.lrc)"));
    ui->leSubtitle->setText(subtitleFile);
}

void Widget::on_buttonPlay_clicked()
{
    QString program = "player.exe";
    QStringList arguments;
    QString media = ui->leMedia->text();
    QString sub = ui->leSubtitle->text();
    if(!sub.isNull() && !sub.isEmpty()){
        arguments << "-sub" << sub;
        QString suffix = QFileInfo(media).suffix();
        if(suffix == "mp3"){
            arguments << "-force_style" << "FontSize=30,MarginV=100";
        }
    }
    arguments << "-af" << "volume=10dB";
    arguments << media;

    playProcess = new QProcess(this);
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("FONTCONFIG_PATH", QDir::currentPath()+"/fonts");
    env.insert("FONTCONFIG_FILE", QDir::currentPath()+"/fonts/fonts.conf");
    playProcess->setProcessEnvironment(env);
    playProcess->start(program, arguments);
    connect(playProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(playProcess_finished(int,QProcess::ExitStatus)));
    ui->buttonPlay->setEnabled(false);
    this->hide();
}

void Widget::playProcess_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    ui->buttonPlay->setEnabled(true);
}

void Widget::closeEvent(QCloseEvent *event)
{
    if(playProcess != NULL){
        playProcess->close();
    }
    event->accept();
}

void Widget::show_mainWindow()
{
    this->show();
}

void Widget::show_help()
{
    QFile help("./help.txt");
    help.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream text(&help);
    QMessageBox::about(NULL, "Help", text.readAll());
    help.close();
}

void Widget::quit()
{
    if(playProcess != NULL){
        playProcess->close();
    }
    close();
}
