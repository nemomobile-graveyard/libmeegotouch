
#include <QDebug>

#include "chainingexampleinterface.h"

#include "imageviewer.h"

ImageViewer::ImageViewer( const MButton &button ) :
    chainingExampleInterface( new ChainingExampleInterface )
{
    QObject::connect( &button, SIGNAL( released() ), this, SLOT( showImage() ) );
}

ImageViewer::~ImageViewer()
{
    delete chainingExampleInterface;
}

void ImageViewer::showImage()
{
    QString imageFilename( "this is a chained view" );
    QStringList list;
    list << imageFilename;

    if ( chainingExampleInterface->isValid() ) {
        chainingExampleInterface->showImage( imageFilename, list, "title set by app1" );
    } else {
        qDebug() << "chainingExampleInterface not valid : " << chainingExampleInterface;
    }
}
