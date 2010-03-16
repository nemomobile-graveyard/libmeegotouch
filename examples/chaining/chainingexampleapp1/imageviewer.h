#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QObject>

#include <DuiButton>
#include "chainingexampleinterface.h"

class ImageViewer: public QObject
{
    Q_OBJECT

public:
    ImageViewer( const DuiButton &button );
    ~ImageViewer();

public slots:
    void showImage();

private:
    ChainingExampleInterface *chainingExampleInterface;
};

#endif // IMAGEVIEWER_H
