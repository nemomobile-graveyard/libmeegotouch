#ifndef ALBUMPAGE_H
#define ALBUMPAGE_H

#include <MApplicationPage>

class MImageWidget;
class QGestureEvent;
class QPinchGesture;

#include "data.h"

class AlbumPage : public MApplicationPage {
    Q_OBJECT
public:
    AlbumPage(const Album *album, QGraphicsItem *parent = 0);

protected:
    virtual void createContent();

    // From MWidget
    virtual void pinchGestureEvent(QGestureEvent *event, QPinchGesture *gesture);

private slots:
    void fadeInAlbumCover();

private:
    const Album *album;
    MImageWidget *albumCover;
};

#endif
