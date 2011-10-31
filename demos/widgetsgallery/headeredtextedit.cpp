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

#include "headeredtextedit.h"
#include <QDebug>
#include <MLocale>
#include <MTextEditView>

HeaderedTextEdit::HeaderedTextEdit(QGraphicsItem *parent)
    : MTextEdit(MTextEditModel::MultiLine, QString(), parent),
    headerLabel(0)
{
    connect(document(), SIGNAL(blockCountChanged(int)), this, SLOT(_q_resetNewBlockMargin()));
}

HeaderedTextEdit::~HeaderedTextEdit()
{
}

void HeaderedTextEdit::setHeaderText(const QString &text)
{
    if (!headerLabel) {
        headerLabel = new MLabel(this);
        headerLabel->setObjectName("headerLabel");
        headerLabel->setStyleName("CommonEditorInputFieldLabel");
        headerLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
        headerLabel->setPos(0, 0);
        headerLabel->setPreferredHeight(effectiveSizeHint(Qt::PreferredSize).height());
        headerLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    }
    headerLabel->setText(text);

    updateTextLeftMargin();
}

QString HeaderedTextEdit::staticHeaderText() const
{
    if(headerLabel) {
        return headerLabel->text();
    } else {
        return QString();
    }
}

void HeaderedTextEdit::setGeometry(const QRectF &rect)
{
    MTextEdit::setGeometry(rect);
    updateLabelPosition(rect);
}

void HeaderedTextEdit::updateTextLeftMargin()
{
    QSizeF labelSize = headerLabel->sizeHint(Qt::PreferredSize, QSizeF(-1, -1));
    QTextCursor cursor(document());
    cursor.setPosition(0);
    QTextBlockFormat tbf = cursor.blockFormat();
    
    // Will not overwrite label if LeftMargin for 
    // text block format is set. Usefuly for only inputs
    // such as emails.
    // tbf.setLeftMargin(labelSize.width());
    tbf.setTextIndent(labelSize.width());
    Qt::LayoutDirection dir = MLocale::directionForText(headerLabel->text());
    setLayoutDirection(dir);
    updateLabelPosition(geometry());
    cursor.setBlockFormat(tbf);    
}

void HeaderedTextEdit::updateLabelPosition(const QRectF &geom)
{
    Q_UNUSED(geom);
    const MTextEditStyle *s = static_cast<const MTextEditStyle *>(style().operator ->());
    // The header imitates beginning of text for the user.
    // So, we need to place header where the beginning is.
    if (layoutDirection() == Qt::RightToLeft) {
        // put header to right side of text entry
        QRectF geometry;
        QSizeF textSize = size();
        QSizeF labelSize = headerLabel->preferredSize();
        geometry.setSize(labelSize);
        geometry.setX(textSize.width() - labelSize.width() - qMin<int>(s->paddingRight(), s->textClippingRight()));
        headerLabel->setGeometry(geometry);
    } else {
        // put header to left side of text entry
        QRectF geometry;
        QSizeF labelSize = headerLabel->preferredSize();
        geometry.setSize(labelSize);
        headerLabel->setGeometry(geometry);
    }
}

void HeaderedTextEdit::_q_resetNewBlockMargin()
{
    // This method shouldn't be used if text block format has LeftMargin set.
    QTextBlockFormat tbf = textCursor().blockFormat();
    if (textCursor().blockNumber() == 0) {
        tbf.setTextIndent(headerLabel->size().width());
    } else {
        tbf.setTextIndent(0);
    }
    textCursor().setBlockFormat(tbf);
}
