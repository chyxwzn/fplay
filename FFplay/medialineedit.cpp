#include "medialineedit.h"

MediaLineEdit::MediaLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    setDragEnabled(true);
}

void MediaLineEdit::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        QString localFile;
        QRegExp rx(".*\\.(srt|ass|lrc)$",Qt::CaseInsensitive);
        if (event->mimeData()->urls().count()>1)
        {
            event->ignore();
            return;
        }
        foreach(QUrl url, event->mimeData()->urls())
        {
            localFile = url.toLocalFile();
            if (rx.indexIn(localFile) >= 0)
            {
                event->ignore();
            }
            else
            {
                event->acceptProposedAction();
                setFocus();
            }
        }
    }
    else
    {
        event->ignore();
    }
}

void MediaLineEdit::dropEvent(QDropEvent *event)
{
    QString localFile;
    QUrl url = event->mimeData()->urls().at(0);
    localFile = url.toLocalFile();
    setText(localFile);
}
