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

#include "qtmaemo6styleeventfilter.h"
#include "qtmaemo6style_p.h"
#include "qtmaemo6windowdecoration.h"
#include "qtmaemo6dialogproxy.h"
#include "qtmaemo6comboboxpopup.h"
#include "qtmaemo6submenu.h"

#include <QWidget>
#include <QEvent>
#include <QDebug>
#include <QLayout>
#include <QMainWindow>
#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QToolButton>
#include <QTreeView>
#include <QComboBox>
#include <QMenu>

#include <MComponentData>
#include <mfeedbackplayer.h>
#include <mdeviceprofile.h>
#include <MTheme>
#include <mbuttonstyle.h>
#include <MScalableImage>

#include "qtmaemo6clicklabel.h"

//krazy:excludeall=qclasses

QtMaemo6StyleEventFilter::QtMaemo6StyleEventFilter(QtMaemo6TestStyle *parent)
    : QtMaemo6TestStyleEventFilter(parent)
{
}

bool QtMaemo6StyleEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    QWidget *widget = qobject_cast<QWidget *>(obj);

    switch (event->type()) {
    case QEvent::Show: {
        if (NULL != widget) {
            if (widget->isWindow()) {
                if (QDialog *dialog = qobject_cast<QDialog *>(widget)) {
                    QtMaemo6DialogProxy *dialogProxy = new QtMaemo6DialogProxy(dialog, m_style->m_windowDecoration);

                    dialogProxy->setTitle(widget->windowTitle());

                    const QPixmap *closePixmap = MTheme::pixmap("Icon-close", QSize(28, 28));
                    dialogProxy->setPixmap(*closePixmap);

                    dialogProxy->showFastMaximized();
                    QtMaemo6StylePrivate::drawWindowBackground(widget);
                    return true;
                } else if(QMenu* menu = qobject_cast<QMenu*>(widget)) {
                    //also show menus styled like m menus, even if they are not called from a menubar
                    QtMaemo6SubMenu *subMenu = new QtMaemo6SubMenu(menu, NULL);
                    QtMaemo6WindowDecoration *decoration = new QtMaemo6WindowDecoration(subMenu, NULL);
                    decoration->showFastMaximized();
                    //these both must be done after the show, because the status- and
                    // menubar is added on show event
                    decoration->setStatusBar(NULL);
                    decoration->setMenuBar(NULL);
                    QtMaemo6StylePrivate::drawWindowBackground(decoration);
                } else if (!qobject_cast<QtMaemo6Window *>(widget) &&
                           !widget->inherits("QTipLabel")) {  //don't create a new window for every tooltip!
                    if(0 == qobject_cast<QtMaemo6WindowDecoration*>(widget->parent())) {
                        m_style->m_windowDecoration = new QtMaemo6WindowDecoration(widget);
                        m_style->m_windowDecoration->showFastMaximized();
                        QtMaemo6StylePrivate::drawWindowBackground(m_style->m_windowDecoration);
                        return true;
                    }
                }
            }
            // Unfortunately this can't be safely done in polish:
            if (QDialogButtonBox *box = qobject_cast<QDialogButtonBox *>(widget)) {
                QPushButton *okButton = box->button(QDialogButtonBox::Ok);
                //~ uispec-document DirectUI_Common_Strings_UI_Specification_0.7.doc
                //: Command for confirmation
                //% "Done"
                if (okButton) okButton->setText(qtTrId("qtn_comm_command_done"));
                QPushButton *cancelButton = box->button(QDialogButtonBox::Cancel);
                if (cancelButton) cancelButton->hide();
            }
            if (QPushButton *button = qobject_cast<QPushButton *>(widget)) {
                QString okString = tr("OK");
                //~ uispec-document DirectUI_Common_Strings_UI_Specification_0.7.doc
                //: Command for confirmation
                //% "Done"
                QString doneString = qtTrId("qtn_comm_command_done");
                if (button->text() == okString) button->setText(doneString);
                QString cancelString = tr("Cancel");
                if (button->text() == cancelString) {
                    button->hide();
                }
            }
            if (QTreeView *treeWidget = qobject_cast<QTreeView *>(widget)) {
                //unfortunately this must be done in showEvent, because it's
                // a property of QTreeWidget, that can't be influenzed by the
                // style

                //FIXME: really bad to do all that in showEvent
                QToolButton button;
                QStyleOptionToolButton option;
                option.initFrom(&button);
                option.text = '+';
                QSize size = m_style->sizeFromContents(QStyle::CT_ToolButton, &option, QSize(), &button);

                //only set the indentation if the required buttonsize is bigger
                // than original indentation
                if (size.width() > treeWidget->indentation()) {
                    treeWidget->setIndentation(size.width());
                }
            }
        }

    }
    break;
    case QEvent::MouseButtonDblClick:
    case QEvent::MouseButtonPress: {
        // now send press feedback
        MFeedbackPlayer *feedbackPlayer = MComponentData::feedbackPlayer();

        if (feedbackPlayer) {
            feedbackPlayer->play(MFeedbackPlayer::Press);
        }
        if(QComboBox* comboBox = qobject_cast<QComboBox*>(widget)) {
            //done in mousePress, because in this way the original popup is completely suppressed
            QtMaemo6ComboBoxPopup *comboBoxPopup = new QtMaemo6ComboBoxPopup(comboBox, NULL);
            QtMaemo6DialogProxy* dialog = new QtMaemo6DialogProxy(comboBoxPopup, comboBox);
            dialog->showFastMaximized();
            dialog->setTitle(tr("Choose a value"));
            QtMaemo6StylePrivate::drawWindowBackground(comboBoxPopup);
            return true;
        }
    }
    break;
    case QEvent::MouseButtonRelease: {
        //qCritical( "eventFilter got mouseRelease" );

        // now send release feedback
        MFeedbackPlayer *feedbackPlayer = MComponentData::feedbackPlayer();

        if (feedbackPlayer) {
            feedbackPlayer->play(MFeedbackPlayer::Release);
        }
    }
    break;
    case QEvent::Wheel: {
        if(qobject_cast<QComboBox*>(widget)) {
            return true;
        }
    }
    break;
    default:
        break;
    }
    // standard event processing
    return QObject::eventFilter(obj, event);
}

