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

#include "melidedtext.h"

#include <QSizeF>
#include <QFont>
#include <QStringList>
#include <QFontMetricsF>
#include <QTextLayout>
#include <QTextLine>

namespace
{
    bool isEqual(const QTextOption &left, const QTextOption &right)
    {
        return left.flags() == right.flags()
               && left.alignment() == right.alignment()
               && left.tabArray() == right.tabArray()
               && left.tabStop() == right.tabStop()
               && left.tabs() == right.tabs()
               && left.textDirection() == right.textDirection()
               && left.useDesignMetrics() == right.useDesignMetrics()
               && left.wrapMode() == right.wrapMode();
    }
}

const QChar MElidedText::EllipsisChar(0x2026);
const QChar MElidedText::SeparatorChar = QChar(0x9c, 0);

MElidedText::MElidedText()
    : dirty(true)
{
}

QString MElidedText::doElideText(const QSizeF &renderSize,
                                 const QFont &font,
                                 const QStringList &textVariants,
                                 const QTextOption &textOption,
                                 bool *isMultipleLines,
                                 bool doElide)
{
    QFontMetricsF fm(font);

    QTextLayout textLayout;
    textLayout.setFont(font);
    textLayout.setTextOption(textOption);

    int elideAtLineNumber = -1;
    const qreal leading = fm.leading();

    QString text;
    bool firstVariant = true;

    if (isMultipleLines)
        *isMultipleLines = false;

    for (int i = 0; i < textVariants.count(); ++i) {
        qreal height = -leading;
        int lineCount = 0;
        elideAtLineNumber = -1;
        text = textVariants[i];
        if (!firstVariant) {
            textLayout.endLayout();
            textLayout.clearLayout();
        }
        firstVariant = false;
        textLayout.setText(text);
        textLayout.beginLayout();
        forever {
            QTextLine line = textLayout.createLine();
            if (!line.isValid())
                break;
            lineCount++;
            if (isMultipleLines)
                *isMultipleLines = (lineCount > 1);

            height += leading;
            if (height <= renderSize.height()) {
                line.setLineWidth(renderSize.width());
                height += line.height();
                if (line.naturalTextWidth() > renderSize.width() &&
                        (doElide || i < textVariants.count() - 1) ) {
                    elideAtLineNumber = lineCount -1;
                    // We've exceeded the maximum available width, so if we can elide, elide this string.
                    // If we have an alternative variant
                    // string, use that.  Else we have to just put up with the line being too long
                    break;
                }
            }
            if (height > renderSize.height() && lineCount > 1) {
                // We've exceeded the maximum available space, so we need to elide the previous line
                elideAtLineNumber = lineCount - 2;
                break;
            }
        }
        //If we don't need to elide, then the string fits, so no need to try the next string
        //variant
        if (elideAtLineNumber == -1)
            break;
    }

    if (elideAtLineNumber >= 0) {
        QTextLine line = textLayout.lineAt(elideAtLineNumber);
        Q_ASSERT(line.isValid());

        // The string might have a newline at the end, so it's important that we remove it
        // before eliding etc.  Also remove any other spaces from the end.
        int lineLength = line.textLength();
        while (lineLength && text.at(line.textStart() + lineLength - 1).isSpace())
            --lineLength;

        /* We want to just truncate the text, and not actually elide */
        if (!doElide)
            return text.left(line.textStart() + lineLength);

        // Get the text to elide, but here we use a little trick - we append the elide
        // character and then send it to elidedText.  There are three possible cases:
        //
        // 1. The original line length was too long to fit on the line.  In this case, it
        // doesn't matter that we've added an elide character, because it will be cut off
        // anyway.
        //
        // 2. The original line length was far too too short to fit on the line.  In this case,
        // elidedText will do nothing, and so the elide character that we've added is shown
        // only.
        //
        // 3. The original line length was only just too short to fit on the line.  In this
        // case, adding the additional elide character will make it just too long, and so
        // elidedText will remove it, along with a few of the last characters, and use its own
        // eliding mechanism.
        QString textToElide = text.mid(line.textStart(), lineLength) + EllipsisChar;
        return text.left(line.textStart()) + fm.elidedText(textToElide, Qt::ElideRight, renderSize.width(), /*We have no option for showing mnemonics*/0);
    }
    return text;
}

void MElidedText::setText(const QString &newText, QChar separatorChar)
{
    text = newText.split(separatorChar);
    dirty = true;
}

const QString & MElidedText::elided(const QSizeF &renderSize,
                                    const QFont &font,
                                    const QTextOption &textOption) const
{
    if (!dirty) {
        // invalidate cache if any parameter was changed
        dirty = (lastRenderSize != renderSize)
                || (lastFont != font)
                || !isEqual(lastTextOption, textOption);
    }

    if (!dirty) {
        return elidedText;
    }

    dirty = false;
    lastRenderSize = renderSize;
    lastFont = font;
    lastTextOption = textOption;

    elidedText = doElideText(renderSize, font, text, textOption);

    return elidedText;
}
