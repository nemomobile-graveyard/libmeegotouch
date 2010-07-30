#include "samplewindow.h"

#include <MLabel>

SampleWindow::SampleWindow(QWidget *parent) : MApplicationWindow(parent)
{
    currentAction = 0;

    actionGroup = new QActionGroup(this);

    QAction *alphaAction = createAction("Alpha");
    createAction("Beta");
    createAction("Gamma");
    createAction("Delta");
    createAction("Epsilon");

    connect(actionGroup, SIGNAL(triggered(QAction*)), SLOT(showPageForAction(QAction*)));

    showPageForAction(alphaAction);
}

void SampleWindow::showPageForAction(QAction *action)
{
    if (currentAction == action)
        return;

    MApplicationPage *page = createPage(action->text());
    page->appear(this, MSceneWindow::DestroyWhenDone);

    if (currentAction) {
        currentAction->setVisible(true);
    }

    currentAction = action;
    action->setVisible(false);
}

MApplicationPage *SampleWindow::createPage(const QString &name)
{
    MApplicationPage *page = new MApplicationPage;

    page->setTitle(name);

    QString contentText = QString("%1 Content").arg(name);
    page->setCentralWidget(new MLabel(contentText));

    page->setEscapeMode(MApplicationPageModel::EscapeCloseWindow);

    return page;
}

QAction *SampleWindow::createAction(const QString &name)
{
    MAction *action = new MAction(name, this);
    action->setLocation(MAction::ApplicationMenuLocation);
    action->setActionGroup(actionGroup);
    addAction(action);

    return action;
}
