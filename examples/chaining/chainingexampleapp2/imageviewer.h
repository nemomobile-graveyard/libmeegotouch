#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QObject>

class MButton;

class ImageViewer: public QObject
{
    Q_OBJECT

public:
    ImageViewer();
    ~ImageViewer();

public slots:
    bool showImage(const QString &uri, const QStringList &uriList);
};

#endif // IMAGEVIEWER_H
