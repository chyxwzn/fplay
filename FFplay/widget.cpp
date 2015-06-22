#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_buttonMedia_clicked()
{
    mediaFile = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/home",
                                                    tr("Media (*)"));
    ui->leMedia->setText(mediaFile);
}

void Widget::on_buttonSubtitle_clicked()
{
    subtitleFile = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/home",
                                                    tr("Subtitle (*.srt *.ass *.lrc)"));
    ui->leSubtitle->setText(subtitleFile);
}

void Widget::on_buttonPlay_clicked()
{
    QString program = "fplay.exe";
    QStringList arguments;
    if(!subtitleFile.isNull() && !subtitleFile.isEmpty()){
        arguments << "-sub" << subtitleFile;
        QString suffix = QFileInfo(mediaFile).suffix();
        if(suffix == "mp3"){
            arguments << "-force_style" << "FontSize=30";
        }
    }
    arguments << "-af" << "volume=10dB";
    arguments << mediaFile;

    QProcess *myProcess = new QProcess(this);
    myProcess->start(program, arguments);
}
