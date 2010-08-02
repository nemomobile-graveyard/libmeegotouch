#include "samplepage.h"

#include <MSceneManager>
#include <MButton>

SamplePage::SamplePage(int level) : MApplicationPage(0), level(level)
{
    QString title;

    if (level == 1) {
        title = "Root level";
    } else {
        title = QString("Level %1").arg(level);
    }

    setTitle(title);

    QString buttonTitle = QString("Open page level %1").arg(level + 1);
    MButton *button = new MButton(buttonTitle);
    connect(button, SIGNAL(clicked()), SLOT(openNextPage()));

    setCentralWidget(button);
}

void SamplePage::openNextPage()
{
    SamplePage *nextPage = new SamplePage(level + 1);
    nextPage->appear(scene(), MSceneWindow::DestroyWhenDismissed);
}
