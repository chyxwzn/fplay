#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    playProcess(NULL)
{
    ui->setupUi(this);
    setAcceptDrops(true);
}

Widget::~Widget()
{
    delete ui;
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
    QString program = "fplay.exe";
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
    playProcess->start(program, arguments);
    connect(playProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(playProcess_finished(int,QProcess::ExitStatus)));
    ui->buttonPlay->setEnabled(false);
}

void Widget::playProcess_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
//    qDebug() << exitCode;
    ui->buttonPlay->setEnabled(true);
}
