
#include "imageviewer.h"

#include <QDebug>

#include <DuiButton>
#include <DuiApplication>
#include <DuiApplicationWindow>
#include <DuiApplicationIfProxy>

ImageViewer::ImageViewer( DuiButton *button )
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

    DuiApplication *duiApp = DuiApplication::instance();
    duiApp->activeWindow()->raise();

    button->setText( uri );

    return true;
}
