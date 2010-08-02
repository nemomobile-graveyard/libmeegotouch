// mainpage.cpp
#include "mainpage.h"

#include <MButton>
#include <MButtonGroup>
#include <MLabel>
#include <MSceneManager>
#include <QGraphicsLinearLayout>

#include "artistpage.h"

MainPage::MainPage(QList<Artist *> artistsList, QGraphicsItem *parent)
    : MApplicationPage(parent), artistsList(artistsList)
{
    // OBS: in a real application you shouldn't hardcode strings used
    // in the GUI like that but use the translation system to fetch
    // the correct, localised, string instead.
    setTitle("Music Catalogue");
}

MainPage::~MainPage()
{
    qDeleteAll(artistsList);
}

// This is a virtual method that gets called only once by the framework,
// right before your page begins to appear on the scene.
void MainPage::createContent()
{
    // We want to organize our items in a single column. A vertical
    // layout enables us to easily achieve this arrangement.
    // Layout classes take care of setting the correct geometry (size and position)
    // of the items added to it.
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    centralWidget()->setLayout(layout);

    // Items in a vertical layout are arranged from top to bottom.
    // Thus the label "Artists:" will come first, therefore serving
    // the title of our list.
    layout->addItem(new MLabel("Artists:"));

    MButtonGroup *buttonGroup = new MButtonGroup(this);

    Artist *artist;
    MButton *artistButton;
    for (int i = 0; i < artistsList.count(); i++) {
        artist = artistsList[i];

        artistButton = new MButton;
        artistButton->setText(artist->name);

        layout->addItem(artistButton);
        buttonGroup->addButton(artistButton, i);
    }

    // A MButtonGroup is used because it provides a signal that is emitted
    // whenever any of the buttons added to it gets clicked.
    // MButtonGroup groups the buttons logically only. Their graphical
    // positioning is still handled by the layout.
    connect(buttonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(displayArtist(int)));
}

void MainPage::displayArtist(int artistIndex)
{
    Artist *artist = artistsList[artistIndex];

    ArtistPage *artistPage = new ArtistPage(artist);

    // When the back button is pressed, the page gets dismissed.
    // By setting MSceneWindow::DestroyWhenDismissed we don't have to
    // keep tabs on this page since it will be automatically destroyed
    // after the dismissal.
    artistPage->appear(scene(), MSceneWindow::DestroyWhenDismissed);
}
