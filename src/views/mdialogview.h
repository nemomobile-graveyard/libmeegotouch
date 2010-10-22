/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libmeegotouch.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#ifndef MDIALOGVIEW_H
#define MDIALOGVIEW_H

#include <mdialogstyle.h>
#include "mscenewindowview.h"
#include "mdialogmodel.h"

class MDialog;
class MDialogViewPrivate;
class MPannableViewport;

class QGraphicsLinearLayout;

/*!
    \class MDialogView
    \brief View class for standard dialogs.

    \ingroup views

    \section MDialogViewOverview Overview

        MDialogView is used to visualize dialog windows.  A dialog is comprised by
        three components: a title bar, central widget and a button box. Each of this components
        can be hidden. See MDialog for more details.

        When the button box is visible, buttons can have different appearance depending on their
        position - left button can have different background, than right or middle.
        It is done in style sheet by defining different button viewTypes: \em first, \em middle,
        \em last, \em single. The last one is used when there is only one button in the dialog.

        When the title bar is hidden, the dialog background can be styled differently,
        for example can have rounded corners. ViewType name for it is \em titlebarless.

        Example stylesheet:
        \code
            ...

            MPannableWidgetStyle[titlebarless]#MDialogContentsViewport {
                background-image: "mdialog-query-background" 8px 8px 8px 0px;
            }

            MButtonStyle[first].Landscape {
                margin-right: 0;
                background-image: "mdialog-left-button-background" 10px 10px 10px 10px;
            }

            MButtonStyle[first].Landscape:pressed {
                background-image: "mdialog-left-button-background-pressed" 10px 10px 10px 10px;
            }

            MButtonStyle[first].Portrait {
               margin-bottom: 0;
               background-image: "mdialog-top-button-background" 10px 10px 10px 10px;
            }

            MButtonStyle[first].Portrait:pressed {
                background-image: "mdialog-top-button-background-pressed" 10px 10px 10px 10px;
            }

            ...
        \endcode

    \sa MDialog, MDialogStyle
*/

class M_VIEWS_EXPORT MDialogView : public MSceneWindowView
{
    Q_OBJECT
    M_VIEW(MDialogModel, MDialogStyle)

public:
    MDialogView(MDialog *controller);
    virtual ~MDialogView();

protected:
    MDialogView(MDialogViewPrivate &dd, MDialog *controller);
    MDialogViewPrivate *const d_ptr;

    /*!
     * \brief Returns the layout used to arrange the dialog's contents.
     * The layout contains three items: title bar, central widget and button box.
     *
     * This method is useful for derived classes that wants to add new elements
     * to the dialog.
     *
     * Be careful not to remove any items already there since it will effectively
     * cripple the existing functionality.
     *
     * \return Layout with the dialog's contents.
     */
    QGraphicsLinearLayout *contentsLayout();

    MPannableViewport *contentsViewport();

    //! \reimp
    virtual QPainterPath shape() const;
    virtual void applyStyle();
    virtual void setupModel();

protected slots:
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    Q_DECLARE_PRIVATE(MDialogView)
    Q_DISABLE_COPY(MDialogView)
#ifdef UNIT_TEST
    friend class Ut_MDialog;
    friend class Ut_MDialogView;
#endif
};

#endif
