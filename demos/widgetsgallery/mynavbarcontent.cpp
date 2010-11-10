#include "mynavbarcontent.h"

#include <MButton>
#include <MSlider>

#include <QGraphicsLinearLayout>

class MyNavBarContentPrivate {
public:
    MyNavBarContentPrivate(MyNavBarContent *q);
    virtual ~MyNavBarContentPrivate();
    MyNavBarContent *q;
};

MyNavBarContentPrivate::MyNavBarContentPrivate(MyNavBarContent *q)
{
    this->q = q;
}

MyNavBarContentPrivate::~MyNavBarContentPrivate()
{
}

MyNavBarContent::MyNavBarContent(QGraphicsItem *parent)
    : QGraphicsWidget(parent), d(new MyNavBarContentPrivate(this))
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    setLayout(layout);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    MSlider *slider = new MSlider;
    slider->setRange(0, 100);
    slider->setMinLabelVisible(true);
    slider->setMaxLabelVisible(true);
    slider->setHandleLabelVisible(true);
    layout->addItem(slider);

    MButton *okButton = new MButton("OK");
    okButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    okButton->setStyleName("ToolBarLabelOnlyCommonButton");
    connect(okButton, SIGNAL(clicked()), SIGNAL(okClicked()));
    layout->addItem(okButton);
}