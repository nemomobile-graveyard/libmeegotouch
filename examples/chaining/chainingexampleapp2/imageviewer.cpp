
#include "imageviewer.h"

#include <QDebug>

#include <MButton>
#include <MApplication>
#include <MApplicationWindow>
#include <MApplicationIfProxy>

ImageViewer::ImageViewer( MButton *button )
{
    this->button = button;
}

ImageViewer::~ImageViewer()
{
}

bool ImageViewer::showImage(const QString &uri, const QStringList &uriList)
{
    qDebug() << __PRETTY_FUNCTION__;
    Q_UNUSED( uriList );

    MApplication *mApp = MApplication::instance();
    mApp->activeWindow()->raise();

    button->setText( uri );

    return true;
}
