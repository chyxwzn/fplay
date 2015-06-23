#ifndef SUBTITLELINEEDIT_H
#define SUBTITLELINEEDIT_H

#include <QtWidgets/QLineEdit>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

class SubtitleLineEdit : public QLineEdit
{
public:
    explicit SubtitleLineEdit(QWidget *parent = 0);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
};

#endif // SUBTITLELINEEDIT_H
