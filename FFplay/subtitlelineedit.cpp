#include "subtitlelineedit.h"

SubtitleLineEdit::SubtitleLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    setDragEnabled(true);
}

void SubtitleLineEdit::dragEnterEvent(QDragEnterEvent *event)
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
                event->acceptProposedAction();
                setFocus();
            }
            else
            {
                event->ignore();
            }
        }
    }
    else
    {
        event->ignore();
    }
}

void SubtitleLineEdit::dropEvent(QDropEvent *event)
{
    QString localFile;
    QUrl url = event->mimeData()->urls().at(0);
    localFile = url.toLocalFile();
    setText(localFile);
}
