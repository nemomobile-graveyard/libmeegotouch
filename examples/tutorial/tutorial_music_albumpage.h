// Filename: albumpage.h
#ifndef ALBUMPAGE_H
#define ALBUMPAGE_H

#include <MApplicationPage>

#include "data.h"

class AlbumPage : public MApplicationPage {
    Q_OBJECT
public:
    AlbumPage(const Album *album, QGraphicsItem *parent = 0);

protected:
    virtual void createContent();

private:
    const Album *album;
};

#endif
