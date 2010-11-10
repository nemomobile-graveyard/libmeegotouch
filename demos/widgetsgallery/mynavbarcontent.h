#ifndef MYNAVBARCONTENT_H
#define MYNAVBARCONTENT_H

#include <MWidget>

class MyNavBarContentPrivate;

class MyNavBarContent : public QGraphicsWidget
{
    Q_OBJECT

public:
    MyNavBarContent(QGraphicsItem *parent = 0);

signals:
    void okClicked();

private:
   MyNavBarContentPrivate *d;
};

#endif