#ifndef MEDIALINEEDIT_H
#define MEDIALINEEDIT_H

#include <QtWidgets/QLineEdit>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

class MediaLineEdit : public QLineEdit
{
public:
    explicit MediaLineEdit(QWidget *parent = 0);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
};

#endif // MEDIALINEEDIT_H
