#ifndef TEXTENTRYNAVIGATIONPAGE_H
#define TEXTENTRYNAVIGATIONPAGE_H

#include "templatepage.h"
#include <MTextEdit>
#include <MRichTextEdit>

class TextEntryNavigationPage : public TemplatePage
{
    Q_OBJECT

public:
    TextEntryNavigationPage();
    virtual ~TextEntryNavigationPage();
    virtual QString timedemoTitle();

    virtual void createContent();
};

#endif // TEXTENTRYNAVIGATIONPAGE_H
