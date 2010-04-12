#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QObject>

class MButton;

class ImageViewer: public QObject
{
    Q_OBJECT

public:
    ImageViewer( MButton *button );
    ~ImageViewer();

public slots:
    bool showImage(const QString &uri, const QStringList &uriList);

private:
    MButton *button;
};

#endif // IMAGEVIEWER_H
