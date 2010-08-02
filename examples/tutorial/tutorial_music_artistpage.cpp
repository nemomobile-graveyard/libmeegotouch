// Filename: artistpage.cpp
#include "artistpage.h"

#include <MButton>
#include <MButtonGroup>
#include <MLabel>
#include <MSceneManager>
#include <QGraphicsLinearLayout>

#include "albumpage.h"

ArtistPage::ArtistPage(const Artist *artist, QGraphicsItem *parent)
    : MApplicationPage(parent), artist(artist)
{
    setTitle(artist->name);
}

void ArtistPage::createContent()
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    centralWidget()->setLayout(layout);

    layout->addItem(new MLabel("Albums:"));

    MButtonGroup *buttonGroup = new MButtonGroup(this);

    Album *album;
    MButton *albumButton;
    for (int i = 0; i < artist->albums.count(); i++) {
        album = artist->albums[i];

        albumButton = new MButton;
        albumButton->setText(album->title);

        layout->addItem(albumButton);
        buttonGroup->addButton(albumButton, i);
    }

    connect(buttonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(displayAlbum(int)));
}

void ArtistPage::displayAlbum(int albumIndex)
{
    Album *album = artist->albums[albumIndex];

    AlbumPage *albumPage = new AlbumPage(album);
    albumPage->appear(scene(), MSceneWindow::DestroyWhenDismissed);
}
