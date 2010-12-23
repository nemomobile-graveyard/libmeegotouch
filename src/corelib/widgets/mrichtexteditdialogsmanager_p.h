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

#ifndef MRICHTEXTEDITDIALOGSMANAGER_P_H
#define MRICHTEXTEDITDIALOGSMANAGER_P_H

#include <QGraphicsWidget>
#include <QPointer>
#include <QPair>
#include <MNamespace>

class MDialog;
class MComboBox;
class MBasicListItem;

// Rich text edit's FontStyle widget contains the implementation required for creating the
// ui of the font widgets (Font Size and Font Color widgets). It holds the items that
// are part of the font widgets' ui.
class MRichTextEditFontStyleWidget : public QGraphicsWidget
{
    Q_OBJECT

public:
    MRichTextEditFontStyleWidget(QGraphicsItem *parent = 0);

    ~MRichTextEditFontStyleWidget();

    // Updates the ui for the given orientation
    virtual void updateOrientation(M::Orientation orientation) = 0;

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    // Selects an item in the ui as current active item. /param index - item's index
    virtual void selectItem(int index) = 0;

    // Updates the size of the ui items using \param numRows and \param numColumns
    void updateSize(int numRows, int numColumns);

    // Each QRectF in the list represents an item in the ui
    QList<QRectF> items;
};

class MRichTextEditFontSizeWidget : public MRichTextEditFontStyleWidget
{
    Q_OBJECT

public:
    MRichTextEditFontSizeWidget(const QList<int> &fontSizeValues, QGraphicsItem *parent = 0);

    ~MRichTextEditFontSizeWidget();

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    virtual void updateOrientation(M::Orientation orientation);

    // Sets the current font size will be used for highlighting the corresponding item in the ui
    void setActiveSize(int size);

protected:

    virtual void selectItem(int index);

Q_SIGNALS:
    /**
     *  \brief Emitted whenever a font size is selected
     */
    void fontSizeSelected(int fontSize);

private:
    // List of font size values shown in the ui
    QList<int> sizeValues;
    // Active item index used for highlighting
    int activeIndex;
};

class MRichTextEditFontColorWidget : public MRichTextEditFontStyleWidget
{
    Q_OBJECT

public:
    MRichTextEditFontColorWidget(const QList<QColor> &fontColorValues, QGraphicsItem *parent = 0);

    ~MRichTextEditFontColorWidget();

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    virtual void updateOrientation(M::Orientation orientation);

    // Sets the current font color will be used for highlighting the corresponding item in the ui
    void setActiveColor(const QColor &color);

protected:

    virtual void selectItem(int index);

Q_SIGNALS:
    /**
     *  \brief Emitted whenever a font color is selected
     */
    void fontColorSelected(const QColor &color);

private:
    // List of font color values shown in the ui
    QList<QColor> colorValues;
    // Active item index used for highlighting
    int activeIndex;
};

class MRichTextEditDialogsManager : public QObject
{
    Q_OBJECT

public:
    /*!
     * \brief returns the MRichTextEditDialogsManager instance
     */
    static MRichTextEditDialogsManager *instance();
    /*!
     * \brief shows the text styling dialog with the current style
     */
    void showTextStylingDialog(const QString &fontfamily = "", int fontPointSize = -1,
                               const QColor &fontColor = QColor());

Q_SIGNALS:

    /**
     *  \brief Emitted whenever a font family is selected
     */
    void fontFamilySelected(const QString &fontFamily);

    /**
     *  \brief Emitted whenever a font size is selected
     */
    void fontSizeSelected(int fontPointSize);

    /**
     *  \brief Emitted whenever a font color is selected
     */
    void fontColorSelected(const QColor &color);

private Q_SLOTS:

    void setFontSize(int fontSize);
    void showFontSizeDialog();
    void updateFontSizeWidgetOrientation(M::Orientation orientation);

    void setFontColor(const QColor &color);
    void showFontColorDialog();
    void updateFontColorWidgetOrientation(M::Orientation orientation);

private:
    // QPointer<MDialog> in ActiveDialog will hold a dialog
    // bool in ActiveDialog is used to avoid calling its dialog's exec() when it is active
    typedef QPair<QPointer<MDialog>, bool> ActiveDialog;

    /*!
     * \brief private constructor to make the class singleton
     */
    MRichTextEditDialogsManager();
    /*!
     * \brief private destructor to avoid deleting the singleton object
     */
    virtual ~MRichTextEditDialogsManager();
    /*!
     * \brief initializes the TextStyles dialog
     */
    void initTextStylingDialog();
    /*!
     * \brief initializes the FontSize dialog
     */
    void initFontSizeDialog();
    /*!
     * \brief initializes the FontColor dialog
     */
    void initFontColorDialog();
    /*!
     * \brief updates the ui components of the TextStyles dialog to show the current style
     */
    void setTextStyleValues(const QString &fontfamily, int fontPointSize, const QColor &fontColor);
    /*!
     * \brief launches the dialog of given type
     */
    void execDialog(ActiveDialog *activeDialog);

    static MRichTextEditDialogsManager *dialogsManager;

    struct Dialogs
    {
        ActiveDialog textStyles;
        ActiveDialog fontSize;
        ActiveDialog fontColor;
    };
    Dialogs dialogs;

    struct FontSizeData
    {
        QList<int> sizeValues;
        QString titleName;
        int activeSizeIndex;
    };
    FontSizeData fontSizeData;

    struct FontColorData
    {
        QList<QColor> colorValues;
        QList<QString> iconIds;
        QString titleName;
        int activeColorIndex;
    };
    FontColorData fontColorData;

    MComboBox *fontFamilyCombo;
    MBasicListItem *fontSizeListItem;
    MRichTextEditFontSizeWidget *fontSizeWidget;
    MBasicListItem *fontColorListItem;
    MRichTextEditFontColorWidget *fontColorWidget;
    QPixmap *fontColorPixmap;
};

#endif
