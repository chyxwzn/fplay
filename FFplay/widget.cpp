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
    aboutAction = new QAction(tr("about"), this);
    connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(show_about()));
    quitAction = new QAction(tr("quit"), this);
    connect(quitAction, SIGNAL(triggered(bool)), this, SLOT(quit()));
    trayMenu = new QMenu(this);
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIcon_activited(QSystemTrayIcon::ActivationReason)));
    trayMenu->addAction(showAction);
    trayMenu->addAction(helpAction);
    trayMenu->addAction(aboutAction);
    trayMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();
    setAcceptDrops(true);
}

Widget::~Widget()
{
    delete quitAction;
    delete helpAction;
    delete aboutAction;
    delete showAction;
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
    QString program = "player.exe";
    QStringList arguments;
    QString media = ui->leMedia->text();
    QString sub = ui->leSubtitle->text();
    QString pos = getLastPosition(media);
    if(!pos.isNull()){
        arguments << "-ss" << pos;
    }
    arguments << "-autoexit";
    QString suffix = QFileInfo(media).suffix();
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
    if(!sub.isNull() && !sub.isEmpty()){
        arguments << "-sub" << sub;
        if(isAudio)
            arguments << "-force_style" << "FontSize=30,MarginV=100";
    }
    if(!isAudio)
        arguments << "-af" << "volume=8dB";
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
    this->show();
}

void Widget::trayIcon_activited(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::DoubleClick){
        this->activateWindow();
        this->showNormal();
    }
}

QString Widget::getLastPosition(QString fileName)
{
    QFile file("history");
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
            << "down/up          \n"
            << "page down/page up\n"
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
          << "volume down/volume up\n"
          << "seek backward/forward 5 seconds if there is a subtitle, or 20 seconds\n"
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
