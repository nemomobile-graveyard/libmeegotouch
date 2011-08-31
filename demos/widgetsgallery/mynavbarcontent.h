#ifndef MYNAVBARCONTENT_H
#define MYNAVBARCONTENT_H

#include <MWidget>
#include <MStylableWidget>

class MyNavBarContentPrivate;

class MyNavBarContent : public MStylableWidget
{
    Q_OBJECT

public:
    MyNavBarContent(QGraphicsItem *parent = 0);
    virtual ~MyNavBarContent();

signals:
    void okClicked();

private:
   MyNavBarContentPrivate *d;
};

#endif
