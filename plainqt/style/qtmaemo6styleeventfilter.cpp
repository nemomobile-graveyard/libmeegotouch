/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#include <QWidget>
#include <QEvent>
#include <QDebug>
#include <QMainWindow>
#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QToolButton>
#include <QTreeView>
#include <QComboBox>

#include <DuiComponentData>
#include <duifeedbackplayer.h>
#include <duideviceprofile.h>
#include <DuiTheme>
#include <duibuttonstyle.h>
#include <DuiScalableImage>

//krazy:excludeall=qclasses

QtMaemo6StyleEventFilter::QtMaemo6StyleEventFilter(QtMaemo6TestStyle *parent)
    : QtMaemo6TestStyleEventFilter(parent)
{
}

bool QtMaemo6StyleEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    QWidget *widget = qobject_cast<QWidget *>(obj);

    //unused
    //QWidget * parent = widget->parentWidget();

    switch (event->type()) {
    case QEvent::Show: {
        if (NULL != widget) {
            if (widget->isWindow()) {
                if (QDialog *dialog = qobject_cast<QDialog *>(widget)) {
                    qCritical() << "Generating and Showing DuiDialog";
                    QtMaemo6DialogProxy *dialogProxy = new QtMaemo6DialogProxy(dialog, m_style->m_windowDecoration);

                    dialogProxy->setTitle(widget->windowTitle());

                    const QPixmap *closePixmap = DuiTheme::pixmap("Icon-close", QSize(28, 28));
                    dialogProxy->setPixmap(*closePixmap);

                    dialogProxy->showFastMaximized();
                    QtMaemo6StylePrivate::drawWindowBackground(widget);

                    //ensure that the mainwindow takes at least the width of the screen
                    //widget->setMinimumWidth(dialogProxy->maxViewportSize().width());
                    return true;
                }
                else if (!qobject_cast<QtMaemo6Window *>(widget)) {
                    qDebug() << "Generating and Showing DuiWindowDecoration";
                    m_style->m_windowDecoration = new QtMaemo6WindowDecoration(widget);
                    m_style->m_windowDecoration->showFastMaximized();
                    QtMaemo6StylePrivate::drawWindowBackground(m_style->m_windowDecoration);

                    //ensure that the mainwindow takes at least the width of the screen
                    //widget->setMinimumWidth(m_style->m_windowDecoration->maxViewportSize().width());
                    return true;
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
        //qDebug( "eventFilter got mousePress" );

        // now send press feedback
        DuiFeedbackPlayer *feedbackPlayer = DuiComponentData::feedbackPlayer();

        if (feedbackPlayer) {
            feedbackPlayer->play(DuiFeedbackPlayer::Press);
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
        //qDebug( "eventFilter got mouseRelease" );

        // now send release feedback
        DuiFeedbackPlayer *feedbackPlayer = DuiComponentData::feedbackPlayer();

        if (feedbackPlayer) {
            feedbackPlayer->play(DuiFeedbackPlayer::Release);
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

