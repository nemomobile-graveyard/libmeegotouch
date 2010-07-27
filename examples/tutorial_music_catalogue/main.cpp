#include <MApplication>
#include <MApplicationWindow>

// The definition of our data classes
#include "data.h"

#include "mainpage.h"

void fillOutData(QList<Artist *> &artistList);

int main(int argc, char **argv)
{
    MApplication app(argc, argv);
    MApplicationWindow window;
    MainPage *mainPage;

    // That's the data that will be displayed by our application.
    // For the sake of keeping the example as simple as possible we use
    // a very simplistic data structure.
    QList<Artist *> artistsList;
    fillOutData(artistsList);

    mainPage = new MainPage(artistsList);

    mainPage->appear(&window);
    window.show();

    return app.exec();
}

void fillOutData(QList<Artist *> &artistList)
{
    Artist *artist;
    Album *album;

    artist = new Artist;
    artist->name = "The Beatles";

    album = new Album;
    album->title = "Sgt. Pepper's Lonely Hearts Club Band";
    album->artist = "The Beatles";
    album->coverArtFilename = "album_cover.jpg";
    album->songs << "Sgt. Pepper's Lonely Hearts Club Band";
    album->songs << "With a Little Help from My Friends";
    album->songs << "Lucy in the Sky with Diamonds";
    album->songs << "Getting Better";
    album->songs << "Fixing a Hole";
    album->songs << "She's Leaving Home";
    album->songs << "Being for the Benefit of Mr. Kite!";
    artist->albums << album;

    album = new Album;
    album->title = "Yellow Submarine";
    album->artist = "The Beatles";
    album->coverArtFilename = "album_cover.jpg";
    album->songs << "Yellow Submarine";
    album->songs << "Only a Northern Song";
    album->songs << "All Together Now";
    album->songs << "Hey Bulldog";
    album->songs << "It's All Too Much";
    album->songs << "All You Need Is Love";
    artist->albums << album;

    album = new Album;
    album->title = "Abbey Road";
    album->artist = "The Beatles";
    album->coverArtFilename = "album_cover.jpg";
    album->songs << "Come Together";
    album->songs << "Something";
    album->songs << "Maxwell's Silver Hammer";
    album->songs << "Oh! Darling";
    album->songs << "Octopus's Garden";
    album->songs << "I Want You (She's So Heavy)";
    artist->albums << album;

    artistList << artist;



    artist = new Artist;
    artist->name = "Led Zeppelin";

    album = new Album;
    album->title = "Physical Graffiti";
    album->artist = "Led Zeppelin";
    album->coverArtFilename = "album_cover.jpg";
    album->songs << "Custard Pie";
    album->songs << "The Rover";
    album->songs << "In My Time of Dying";
    album->songs << "Houses of the Holy";
    album->songs << "Trampled Under Foot";
    album->songs << "Kashmir";
    artist->albums << album;

    album = new Album;
    album->title = "Houses of the Holy";
    album->artist = "Led Zeppelin";
    album->coverArtFilename = "album_cover.jpg";
    album->songs << "The Song Remains the Same";
    album->songs << "The Rain Song";
    album->songs << "Over the Hills and Far Away";
    album->songs << "The Crunge";
    album->songs << "Dancing Days";
    album->songs << "D'yer Mak'er";
    album->songs << "No Quarter";
    album->songs << "The Ocean";
    artist->albums << album;

    album = new Album;
    album->title = "Presence";
    album->artist = "Led Zeppelin";
    album->coverArtFilename = "album_cover.jpg";
    album->songs << "Achilles Last Stand";
    album->songs << "For Your Life";
    album->songs << "Royal Orleans";
    album->songs << "Nobody's Fault but Mine";
    album->songs << "Candy Store Rock";
    album->songs << "Hots On for Nowhere";
    album->songs << "Tea for One";
    artist->albums << album;

    artistList << artist;
}
