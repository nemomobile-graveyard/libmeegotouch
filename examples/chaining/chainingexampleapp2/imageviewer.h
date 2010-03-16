#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QObject>

class DuiButton;

class ImageViewer: public QObject
{
    Q_OBJECT

public:
    ImageViewer( DuiButton *button );
    ~ImageViewer();

public slots:
    bool showImage(const QString &uri, const QStringList &uriList);

private:
    DuiButton *button;
};

#endif // IMAGEVIEWER_H
