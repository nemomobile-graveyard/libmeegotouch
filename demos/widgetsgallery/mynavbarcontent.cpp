#include "mynavbarcontent.h"

#include <MApplication>
#include <MButton>
#include <MTextEdit>

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
    : MStylableWidget(parent), d(new MyNavBarContentPrivate(this))
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    setLayout(layout);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    MTextEdit *edit = new MTextEdit;
    edit->setObjectName("edit");
    edit->setViewType("toolbar");
    layout->addItem(edit);

    MButton *okButton = new MButton("OK");
    okButton->setObjectName("okButton");
    okButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    if (MApplication::instance()->objectName() == "widgetsgallery") {
        okButton->setStyleName("ToolBarLabelOnlyCommonButton");
    } else {
        okButton->setStyleName("ToolBarLabelOnlyCommonButtonInverted");
    }
    connect(okButton, SIGNAL(clicked()), SIGNAL(okClicked()));
    layout->addItem(okButton);
}

MyNavBarContent::~MyNavBarContent()
{
    delete d;
}
