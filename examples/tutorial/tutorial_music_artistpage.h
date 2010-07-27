// Filename: artistpage.h
#ifndef ARTISTPAGE_H
#define ARTISTPAGE_H

#include <MApplicationPage>

#include "data.h"

class ArtistPage : public MApplicationPage {
    Q_OBJECT
public:
    ArtistPage(const Artist *artist, QGraphicsItem *parent = 0);

protected:
    virtual void createContent();

private slots:
    void displayAlbum(int albumIndex);

private:
    const Artist *artist;
};

#endif
