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

#include "mdebug.h"
#include "mwidgetcontroller.h"

#include <QTextStream>

namespace {
    MDebug::MMsgHandler handler(0);
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
