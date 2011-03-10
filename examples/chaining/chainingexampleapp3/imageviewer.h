#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QObject>

#include <MButton>
#include "chainingexampleinterface.h"

class ImageViewer: public QObject
{
    Q_OBJECT

public:
    ImageViewer( const MButton &button );
    ~ImageViewer();

public slots:
    void showImage();

private:
    ChainingExampleInterface *chainingExampleInterface;
};

#endif // IMAGEVIEWER_H
