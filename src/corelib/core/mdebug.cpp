/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "mdebug.h"
#include "mwidgetcontroller.h"
#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
#include <QGraphicsAnchorLayout>
#include <QFile>
#include <mlayout.h>
#include <mlinearlayoutpolicy.h>
#include <mgridlayoutpolicy.h>
#include <mflowlayoutpolicy.h>
#include <mlabel.h>

#include <QTextStream>

namespace {
    MDebug::MMsgHandler handler(0);

    static bool isColoringPossible()
    {
    #if defined(Q_OS_WIN32) || defined(Q_OS_WINCE)
        /* Windows doesn't at all support ANSI escape codes*/
        return false;
    #else
        /* We use QFile::handle() to get the file descriptor. It's a bit unsure
         * whether it's 2 on all platforms and in all cases, so hopefully this layer
         * of abstraction helps handle such cases. */
        QFile file;
        file.open(stderr, QIODevice::WriteOnly);
        return isatty(file.handle());
    #endif
    }

    static QString emphasize() {
        if (!isColoringPossible())
            return QString();
        return QChar(0x1B) + QString("[1m");
    }
    static QString normal() {
        if (!isColoringPossible())
            return QString();
        return QChar(0x1B) + QString("[0m");
    }
    static QString blue() {
        if (!isColoringPossible())
            return QString();
        return QChar(0x1B) + QString("[34m");
    }
    static QString yellow() {
        if (!isColoringPossible())
            return QString();
        return QChar(0x1B) + QString("[33m");
    }

    static QString deemphasize() {
        if (!isColoringPossible())
            return QString();
        return QChar(0x1B) + QString("[1;30m");
    }

    static QString numberToString(long number)
    {
        if (number == QWIDGETSIZE_MAX)
            return "MAX";
        return QString::number(number);
    }

    static QString rectToString(const QRect& rect, const QSizeF &preferredSize) {
        if (preferredSize == rect.size() || !isColoringPossible())
            return QString("(%1,%2 %3x%4) ").arg(rect.x()).arg(rect.y()).arg(rect.width()).arg(rect.height());
        else
            return QString("(%1,%2 %3%4x%5%6) ").arg(rect.x()).arg(rect.y()).arg(yellow()).arg(rect.width()).arg(rect.height()).arg(normal());
    }

    static QString sizeToString(const QSize& size) {
        return QString("%1x%2 ").arg(numberToString(size.width())).arg(numberToString(size.height()));
    }

    static QString sizePolicyPolicyToString (QSizePolicy::Policy policy) {
        switch (policy) {
            case QSizePolicy::Fixed:
                return "Fixed";
            case QSizePolicy::Preferred:
                return "Preferred";
            case QSizePolicy::Minimum:
                return "Minimum";
            case QSizePolicy::Maximum:
                return "Maximum";
            case QSizePolicy::Expanding:
                return "Expanding";
            case QSizePolicy::MinimumExpanding:
                return "MinimumExpanding";
            case QSizePolicy::Ignored:
                return "Ignored";
            default:
                return QString::number(policy);
        }
    }

    static QString sizePolicyToString (const QSizePolicy &policy) {
        if (policy.horizontalPolicy() == QSizePolicy::Preferred &&
                policy.verticalPolicy() == QSizePolicy::Preferred)
            return QString();;  //Most widgets are just preferred, so print nothing in this case
        QString str =  sizePolicyPolicyToString(policy.horizontalPolicy());
        if (policy.horizontalPolicy() != policy.verticalPolicy())
            str += ',' + sizePolicyPolicyToString(policy.verticalPolicy());
        return str;
    }
}

MDebug::MDebug(int type)
    : stream(new Stream(type))
{
}

MDebug::MDebug(const MDebug &debug)
    : stream(debug.stream)
{
    ++stream->ref;
}

MDebug::~MDebug()
{
    flush();
}

void MDebug::installMessageHandler(MMsgHandler handler)
{
    ::handler = handler;
}

void MDebug::flush()
{
    if (!--stream->ref) {
        if (stream->output) {
            switch (stream->type) {
            case Debug:
            case Warning:
            case Critical:
            case Fatal:
            case System:
                qt_message_output(convertMsgType(stream->type), stream->buffer.toLocal8Bit().constData());
                break;
            case Performance:
            case User:
            default:
                if (handler)
                    handler(stream->type, stream->buffer.toLocal8Bit().constData());
            };
        }
        delete stream;
    }
}

QtMsgType MDebug::convertMsgType(int type)
{
    switch(type) {
    case Debug:
        return QtDebugMsg;
    case Warning:
        return QtWarningMsg;
    case Critical:
        return QtCriticalMsg;
    case Fatal:
        return QtFatalMsg;
    case System:
        return QtSystemMsg;
    }

    return QtDebugMsg;
}

MDebug &MDebug::operator <<(const QString &string)
{
    stream->ts << string << ' ';
    return *this;
}

MDebug &MDebug::operator <<(const MWidgetController *controller)
{
    stream->ts << controller->metaObject()->className() << "(styleName = \"" + controller->styleName() << ", parent = ";
    if (controller->parentWidget())
        stream->ts << controller->parentWidget()->metaObject()->className();
    else
        stream->ts << "NULL";
    stream->ts << ") ";
    return *this;
}

/* Static */
void MDebug::printDebugChildInformation(QGraphicsWidget *widget, int initialIndentationDepth)
{
    QTextStream s(stderr);
    s.setCodec("UTF-8");
    QString indentation;
    for (int i = 0; i < initialIndentationDepth * 4; i++)
        indentation += ' ';
    s << indentation;
    QSizeF hfwPreferredSize = widget->effectiveSizeHint( Qt::PreferredSize, QSizeF(widget->geometry().width(), -1));
    QSizeF preferredSize = widget->effectiveSizeHint( Qt::PreferredSize, QSizeF(widget->preferredWidth(), -1));
    if (widget->isVisible()) {
        s << rectToString(widget->geometry().toRect(), preferredSize);
        s << emphasize() << widget->metaObject()->className() << normal() << ' ';
    } else {
        s << deemphasize() << "invisible " << normal();
        s << widget->metaObject()->className() << ' ';
        s << deemphasize();
    }

    MWidgetController *mwidget = dynamic_cast<MWidgetController*>(widget);
    if (mwidget && !widget->objectName().isEmpty() && !mwidget->styleName().isEmpty() ) {
        if (widget->objectName() == QString(mwidget->styleName()))
            s << QString("(o:\"%1\", s:\"%2\") ").arg(widget->objectName(), mwidget->styleName());
        else
            s << QString("(o&s: \"%1\") ").arg(widget->objectName());
    } else if (!widget->objectName().isEmpty())
        s << QString("(o:\"%1\") ").arg(widget->objectName());
    else if (mwidget && !mwidget->styleName().isEmpty())
        s << QString("(s:\"%1\") ").arg(mwidget->styleName());

    s << sizeToString(widget->minimumSize().toSize());
    s << QString::fromUtf8("≤ ") << sizeToString( preferredSize.toSize());
    if (preferredSize.height() != hfwPreferredSize.height())
        s << "(hfw:" << sizeToString( hfwPreferredSize.toSize() ) << ')';
    if (widget->effectiveSizeHint( Qt::MaximumSize) != QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX))
        s << QString::fromUtf8("≤ ") << sizeToString(widget->maximumSize().toSize());

    s << sizePolicyToString(widget->sizePolicy());

    QStringList notes;
    if (widget->isVisible()) {
        if (!widget->transform().isIdentity())
            notes << "Has transform";
        if (!widget->geometry().isEmpty() && widget->parentWidget() && !widget->parentWidget()->geometry().contains(widget->geometry().translated(widget->parentWidget()->geometry().topLeft())))
            notes << "Not within the geometry of the parent";
        MLabel *label = dynamic_cast<MLabel*>(widget);
        if (label) {
            if (label->wordWrap())
                notes << "wordwrap";
            if (label->textElide())
                notes << "elide";

            QString text = label->text();
            text.replace('\r', "\\r");
            text.replace('\n', "\\n");
            if (text.length() > 55)
                notes << ("\"" + text.left(50) + "\" (cont..)");
            else
                notes << ("\"" + text + "\"");

            if (widget->testAttribute(Qt::WA_SetLayoutDirection)) {
                if (widget->layoutDirection() == Qt::LeftToRight)
                    notes << "LTR";
                else if (widget->layoutDirection() == Qt::RightToLeft)
                    notes << "RTL";
            }
        }
    }
    if (!notes.isEmpty())
        s << "(" << notes.join(", ") << ')';
    s << '\n';
    QGraphicsLayout *layout = widget->layout();
    MLayout *mLayout = dynamic_cast<MLayout*>(layout);
    if (layout) {
        QSizeF preferredSize = layout->effectiveSizeHint(Qt::PreferredSize, QSizeF(layout->preferredWidth(), -1));
        QSizeF hfwPreferredSize = layout->effectiveSizeHint(Qt::PreferredSize, QSizeF(layout->geometry().width(), -1));

        QGraphicsLinearLayout *linearLayout = dynamic_cast<QGraphicsLinearLayout*>(layout);
        s << indentation << " *"  << rectToString(layout->geometry().toRect(), preferredSize);

        s << normal();
        if (widget->isVisible())
            s << emphasize() << blue();
        if (widget->testAttribute(Qt::WA_SetLayoutDirection)) {
            if (widget->layoutDirection() == Qt::LeftToRight)
                s << "LTR ";
            else if (widget->layoutDirection() == Qt::RightToLeft)
                s << "RTL ";
        }
        if (dynamic_cast<QGraphicsGridLayout*>(layout)) {
            s << "QGraphicsGridLayout";
        } else if (dynamic_cast<QGraphicsAnchorLayout*>(layout)) {
            s << "QGraphicsAnchorLayout";
        } else if (linearLayout) {
            if (linearLayout->orientation() == Qt::Horizontal)
                s << "QGraphicsLinearLayout (Hor)";
            else
                s << "QGraphicsLinearLayout (Ver)";
        } else if (mLayout) {
            if (dynamic_cast<MGridLayoutPolicy*>(mLayout->policy()))
                s << "MGridLayoutPolicy";
            else if (dynamic_cast<MFlowLayoutPolicy*>(mLayout->policy()))
                s << "MFlowLayoutPolicy";
            else if (MLinearLayoutPolicy *linearPolicy = dynamic_cast<MLinearLayoutPolicy*>(mLayout->policy())) {
                if (linearPolicy->orientation() == Qt::Horizontal)
                    s << "MLinearLayoutPolicy (Hor)";
                else
                    s << "MLinearLayoutPolicy (Ver)";
            } else {
                s << "MLayout";
            }
        } else {
            s << "QGraphicsLayout";
        }
        s << ' ' << normal();
        if (!widget->isVisible())
            s << deemphasize();
 
        s << sizeToString(layout->minimumSize().toSize());
        s << QString::fromUtf8("≤ ") << sizeToString( preferredSize.toSize());
        if (preferredSize.height() != hfwPreferredSize.height())
            s << "(hfw:" << sizeToString(hfwPreferredSize.toSize()) << ')';
        if (layout->effectiveSizeHint( Qt::MaximumSize) != QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX))
            s << QString::fromUtf8("≤ ") << sizeToString(layout->maximumSize().toSize());

        s << sizePolicyToString(layout->sizePolicy());

        s << '\n';
    }
    s << normal();
    s.flush();

    foreach (QGraphicsItem *item, widget->childItems()) {
        if (item && item->isWidget()) {
            printDebugChildInformation(static_cast<QGraphicsWidget*>(item), initialIndentationDepth + 1);
        }
    }
}

void MDebug::printDebugSceneInformation(QGraphicsView *window)
{
    if (window && window->scene())
        MDebug::printDebugSceneInformation(window->scene());
}

void MDebug::printDebugSceneInformation(QGraphicsScene *scene)
{
    if (scene) {
        foreach (QGraphicsItem *item, scene->items()) {
            if (item && item->isWidget() && !item->parentWidget()) {
                MDebug::printDebugChildInformation(static_cast<QGraphicsWidget*>(item));
            }
        }
    }
}
