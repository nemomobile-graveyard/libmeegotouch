#ifndef MOBJECTMENUBASICVIEW_H
#define MOBJECTMENUBASICVIEW_H

#include "mscenewindowview.h"
#include "mobjectmenumodel.h"
#include "mobjectmenubasicstyle.h"
#include <mnamespace.h>
#ifdef HAVE_CONTENTACTION
#include <contentaction/contentaction.h>
#endif

class MObjectMenu;
class MButton;
class MAction;
class MLayout;
class MLinearLayoutPolicy;
class MGridLayoutPolicy;

class MObjectMenuBasicView : public MSceneWindowView
{
    Q_OBJECT
    Q_DISABLE_COPY(MObjectMenuBasicView)
    M_VIEW(MObjectMenuModel, MObjectMenuBasicStyle)

public:
    MObjectMenuBasicView(MObjectMenu *controller);
    virtual ~MObjectMenuBasicView();

protected:

    //! \reimp
    virtual void setupModel();
    virtual void updateData(const QList<const char *> &modifications);
    //! \reimp_end

protected slots:
    void actionAdded(MAction *action);
    void actionModified(MAction *action);
    void actionRemoved(MAction *action);
private slots:
    void contentActionTriggered();

private:


#ifdef HAVE_CONTENTACTION
    QHash<MAction*, ContentAction::Action> contentActions;
#endif
    QHash<MAction *, MButton *> buttons;

    MLayout *layout;
    MLinearLayoutPolicy *portraitPolicy;
    MGridLayoutPolicy *landscapePolicy;

    MObjectMenu *controller;
};

#endif
