#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <MApplicationPage>

#include "data.h"

class MainPage : public MApplicationPage {
    Q_OBJECT
public:
    MainPage(QList<Artist *> artistsList, QGraphicsItem *parent = 0);
    virtual ~MainPage();

protected:
    void createContent();

private slots:
    void displayArtist(int artistIndex);

private:
    QList<Artist *> artistsList;
};

#endif
