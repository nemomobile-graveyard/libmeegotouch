#include <QGraphicsLinearLayout>
#include <MTextEdit>
#include <MButton>
#include <MDebug>
#include <MAction>

#include "mypage.h"
#include "mydialog.h"

MyPage::MyPage()
{
    setTitle("Focus example");

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(centralWidget());

    textEdit = new MTextEdit;
    textEdit->setPrompt("Click to focus");
    layout->addItem(textEdit);

    MButton *button = new MButton("Open dialog");
    layout->addItem(button);

    MyDialog *dialog = new MyDialog();

    QObject::connect(button, SIGNAL(clicked()), dialog, SLOT(appear()));

    MAction *action = new MAction("Hello!", this);
    action->setLocation(MAction::ApplicationMenuLocation);
    addAction(action);
}

bool MyPage::event(QEvent *e)
{
    if (e->type() == QEvent::WindowBlocked) {
        mDebug("MyPage") << "Help! Help! I'm Being Oppressed!";
    }
    if (e->type() == QEvent::WindowUnblocked) {
        textEdit->setFocus();
    }

    return MApplicationPage::event(e);
}
