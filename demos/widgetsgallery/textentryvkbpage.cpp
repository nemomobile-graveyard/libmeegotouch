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

#include "textentryvkbpage.h"
#include "rotationbenchmark.h"

TextEntryVkbPage::TextEntryVkbPage() {
    connect(this, SIGNAL(appeared()), this, SLOT(showVirtualKeyboard()));
}

TextEntryVkbPage::~TextEntryVkbPage() {
    disconnect(this, SIGNAL(appeared()), this, SLOT(showVirtualKeyboard()));
}

QString TextEntryVkbPage::timedemoTitle() {
    return "TextEntryVkb";
}

void TextEntryVkbPage::createBenchmarks(Timedemo *timedemo) {
    TimedemoPage::createBenchmarks(timedemo);
    foreach(QSharedPointer<TimedemoBenchmark> benchmark, m_benchmarks) {
        RotationBenchmark* rotationBenchmark = qobject_cast<RotationBenchmark*>(benchmark.data());
        if (rotationBenchmark) {
            // we must wait until MImRotationAnimation finishes
            rotationBenchmark->setMinimumRuntime(500);
        }
    }
}

void TextEntryVkbPage::showVirtualKeyboard() {
    if (Entries.count() > 0)
        Entries.at(0)->setFocus(Qt::MouseFocusReason);
}
