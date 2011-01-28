
#include "imageviewer.h"

#include <QDebug>

#include <MButton>
#include <MApplication>
#include <MApplicationWindow>
#include <MApplicationPage>
#include <MSceneWindow>
#include <MApplicationIfProxy>
#include <MComponentCache>

ImageViewer::ImageViewer()
{
}

ImageViewer::~ImageViewer()
{
}

bool ImageViewer::showImage(const QString &uri, const QStringList &uriList)
{
    Q_UNUSED( uri );

    MApplicationWindow* mWin = MComponentCache::mApplicationWindow();
    mWin->setObjectName( "MApplicationWindow#2" );
    mWin->setAttribute( Qt::WA_DeleteOnClose, true );

    MApplicationPage *p = new MApplicationPage();
    mWin->show();
    p->appear( mWin, MSceneWindow::DestroyWhenDone );

    MButton *b = new MButton( p->centralWidget() );
    b->setText( uriList[0] );

    return true;
}
