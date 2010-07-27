// Filename: albumpage.cpp
#include "albumpage.h"

#include <MLabel>
#include <MBasicListItem>
#include <MImageWidget>
#include <QGraphicsLinearLayout>

AlbumPage::AlbumPage(const Album *album, QGraphicsItem *parent)
    : MApplicationPage(parent), album(album)
{
    setTitle(album->title);
}

void AlbumPage::createContent()
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    centralWidget()->setLayout(layout);

    MImageWidget *albumCover = new MImageWidget(new QImage(album->coverArtFilename));
    layout->addItem(albumCover);

    QString byArtist = QString("By: %1").arg(album->artist);
    layout->addItem(new MLabel(byArtist));

    layout->addItem(new MLabel("Songs:"));

    MLabel *songLabel;
    for (int i = 0; i < album->songs.count(); i++) {
        songLabel = new MLabel(album->songs.at(i));
        layout->addItem(songLabel);
    }
}
