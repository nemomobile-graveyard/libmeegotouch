#include <MLayout>
#include <MLinearLayoutPolicy>

...

void AlbumPage::createContent()
{
    MLayout *layout = new MLayout;
    centralWidget()->setLayout(layout);

    // Build a vertical layout that holds the cover art and the "By: Artist" label.
    QGraphicsLinearLayout *coverAndArtistLayout = new QGraphicsLinearLayout(Qt::Vertical);

    MImageWidget *albumCover = new MImageWidget(new QImage(album->coverArtFilename));
    coverAndArtistLayout->addItem(albumCover);

    QString byArtist = QString("By: %1").arg(album->artist);
    coverAndArtistLayout->addItem(new MLabel(byArtist));

    // Build a vertical layout that will hold the list of songs.
    QGraphicsLinearLayout *songsLayout = new QGraphicsLinearLayout(Qt::Vertical);
    songsLayout->addItem(new MLabel("Songs:"));
    MLabel *songLabel;
    for (int i = 0; i < album->songs.count(); i++) {
        songLabel = new MLabel(album->songs.at(i));
        songsLayout->addItem(songLabel);
    }

    // When in landscape orientation, have the cover and the songs list
    // side-by-side.
    MLinearLayoutPolicy *landscapePolicy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
    landscapePolicy->addItem(coverAndArtistLayout);
    landscapePolicy->addItem(songsLayout);
    layout->setLandscapePolicy(landscapePolicy);

    // When in portrait orientation, have the cover and the songs list
    // on top of each other.
    MLinearLayoutPolicy *portraitPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    portraitPolicy->addItem(coverAndArtistLayout);
    portraitPolicy->addItem(songsLayout);
    layout->setPortraitPolicy(portraitPolicy);
}
