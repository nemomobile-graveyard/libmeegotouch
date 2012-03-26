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

#ifndef UT_MLABEL_H
#define UT_MLABEL_H


#include "mlabel.h"
#include <QtTest/QtTest>
#include <QImage>


Q_DECLARE_METATYPE(Qt::Alignment)

class TestMLabel: public MLabel
{
    Q_OBJECT

public:
    TestMLabel();
    virtual ~TestMLabel();

    void simulateClick(const QPointF &pos = QPointF(0, 0));
    QImage getLabelImage();
};


class Ut_MLabel : public QObject
{
    Q_OBJECT

private:

    void anchorHitTest(const QPointF &point, int expectedHits);
    QImage captureImage(MLabel *label);

    //Minumum-size rectangle that containts all non-transparent 
    //pixels in the image
    QRect contentRect(const QImage& image);

    //Height of the painted area in the image
    int contentHeight(const QImage& image);

    //Width of the painted area in the image
    int contentWidth(const QImage& image);

    QImage captureText(Qt::TextFormat textFormat, QString text);

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testTextAlignment_data();
    void testTextAlignmentFromCSS_data();
    void testAutomaticTextAlignment_data();
    void testLayoutDirection_data();
    void testTextDirection_data();
    void testTextWordWrap_data();
    void testTextElide_data();

    void testClear_data();
    void testPlainTextColor_data();

    void testTestConstruction();
    void testTextAlignment();
    void testTextAlignmentFromCSS();
    void testAutomaticTextAlignment();
    void testLayoutDirection();
    void testTextDirection();
    void testTextWordWrap();
    void testTextElide();
    void testRichTextElide_data();
    void testRichTextElide();
    void testImagesElide();

    void testHighlighting();
    void testHighlightingModel();

    void testAnchor();

    void testClear();
    void testPlainTextColor();

    void testSizeHint_data();
    void testSizeHint();

    void testWordWrappingSizeHint_data();
    void testWordWrappingSizeHint();

    void testUnknownSizeHint();

    void testFont_data();
    void testFont();

    void testColor_data();
    void testColor();

    void linefeedBeforeFirstTag();

    //regression test for bug 178905 regexp matching an empty string causes an infinite loop in MLabelHighlighter
    void highlighterEmptyRegexp();

    void removeAllHighlighters();

    void wrapModes_data();
    void wrapModes();

    void multiLengthSeparator();

    void testAutoTextModeRecognizesPlainText();

    void testAutoTextModeRecognizesRichText();

    void testPlainAndRichTextModes();

    //Regression test for bug 196025 -  MLabel with text ending in 'f'
    void rightBearing();

    void testTextFormat();

    void testRichTextTiles_data();
    void testRichTextTiles();

    void testRichTextWithoutPixmapCache();

    void testDiacritics_data();
    void testDiacritics();

    // Regression test for bug 293511 ("Thai vowel marks are not fully displayed")
    // and bug 294572 ("Inconsistent vertical alignment of simple-text and rich-text")
    void testPlainAndRichTextAlignments_data();
    void testPlainAndRichTextAlignments();

    void testLineBreakReplacement_data();
    void testLineBreakReplacement();

    void testPreferredLineCount_data();
    void testPreferredLineCount();

    void testSingleLineElidingWithWordwrap();

    void testPaintOffsetNoAffectOnGeometry();

    void testRichTextPreferredLineCount_data();
    void testRichTextPreferredLineCount();

    void testSizeHintIncreasingWithFixedConstraint_data();
    void testSizeHintIncreasingWithFixedConstraint();

    void testLabelShrinkingAndRegrowing_data();
    void testLabelShrinkingAndRegrowing();

    void testRenderedTextProperty_data();
    void testRenderedTextProperty();

    // Regression test for bug 285464 ("Dots in mlabel when elided is set to true have
    // different color than rest of the text.")
    void testEllipsisColor();

    // Regression test for commit 12b7055d3710a9a8fc9cfb131734ebec11860be6 ("Fix when we
    // have a long line that needs eliding but we don't have eliding on")
    void testLongLineWithDisabledEliding();

    // Regression test for bug 281046 ("Notification; Clipped notification with multiple new mail")
    void testMultipleLinesWithEnabledEliding();

    // Regression test for commit 18d7fb7760ea29849e0922ce64c45df57a18070c ("Add
    // support for preferredLineCount").
    void testPreferredLineCountBehavior();

    // Regression test for bug 267939 ("Line break in status message is incorrectly
    // displayed in Contacts")
    void testWindowsLineBreakReplacement();

    // Regression test for commit eaf5bcb877fef445d444092055c1d421dafa2590 ("When label text is
    // set before changing wrap mode, newline chars are not working.")
    void testTextAndWrapModeOrder();

    // Regression test for bug 257767 ("When switching from richtext to plaintext label is
    // rendered empty")
    void testRichToPlainSwitch();

    // Regression test for commit ba968c08477ac6ea8b84987e6117f6c774e3fc6b ("MLabelViewSimple -
    // fix the minimum size of a string with variants and without wordwrap or eliding")
    void testMinimumSizeOfStringVariants();

    // Regression test for bug 250354 ("Allow multilength variant strings with wordwrap")
    void testStringVariantsWithWordWrap();

    // Regression test for commit 409a7cbe247d405975db9d041f08084d2bef24b1 ("MLabelView Rich -
    // make the minimum height of the rich text to be the height of one line")
    void testRichTextMinimumHeight();

    // Regression test for bug 248110 ("Disabling elide mode for MLabel does not change appearance")
    void testDisableElidingChangesAppearance();

private:
    /**
     * Helper method for testSizeHint().
     * \return The required size for the text \a text, if a maximum width of \a width
     *         is available.
     */
    static QSizeF requiredTextSize(const QString &text, const QFont &font,
                                   QTextOption::WrapMode wrapMode, int width);

    TestMLabel *label;

};

#endif
