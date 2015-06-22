#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QProcess>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_buttonMedia_clicked();

    void on_buttonSubtitle_clicked();

    void on_buttonPlay_clicked();

private:
    Ui::Widget *ui;
    QString mediaFile;
    QString subtitleFile;
};

#endif // WIDGET_H
