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

#include "mcolorlistview.h"
#include "mcolorlistview_p.h"

#include <MButton>
#include <MButtonGroup>
#include <MGridLayoutPolicy>
#include <MLayout>
#include <MTheme>
#include <QGraphicsGridLayout>
#include <MStylableWidget>

#include "mcolorlist.h"

M_REGISTER_VIEW_NEW(MColorListView, MColorList)

class SimpleColoredButton : public MButton
{
public:

    SimpleColoredButton() : m_color(QColor(0, 0, 0))
    {
    }

    SimpleColoredButton(const QColor &color) : m_color(color)
    {
    }

    ~SimpleColoredButton()
    {
    }

    QColor color()
    {
        return m_color;
    }

    void setColor(const QColor &color)
    {
        m_color = color;
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option , QWidget *widget)
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);
        QRectF paintRect(style()->marginLeft(),
                         style()->marginTop(),
                         size().width() - style()->marginLeft() - style()->marginRight(),
                         size().height() - style()->marginTop() - style()->marginBottom());

        painter->fillRect(paintRect, m_color);
        if (isChecked()) {
            painter->setPen(QColor(0, 0, 0));
            painter->drawRect(rect());
        }
    }

private:

    QColor m_color;

};

MColorListViewPrivate::MColorListViewPrivate(MColorListView *p, MColorList *controller)
    : q_ptr(p), controller(controller), landscapePolicy(0), portraitPolicy(0), buttonGroup(0)
{
    QGraphicsGridLayout *layout = new QGraphicsGridLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    controller->setLayout(layout);

    MStylableWidget* container = new MStylableWidget();
    container->setStyleName("ColorSelectionContainer");
    MLayout *containerLayout = new MLayout();
    container->setLayout(containerLayout);

    landscapePolicy = new MGridLayoutPolicy(containerLayout);
    landscapePolicy->setContentsMargins(0, 0, 0, 0);
    landscapePolicy->setSpacing(0);
    containerLayout->setLandscapePolicy(landscapePolicy);

    portraitPolicy = new MGridLayoutPolicy(containerLayout);
    portraitPolicy->setContentsMargins(0, 0, 0, 0);
    portraitPolicy->setSpacing(0);
    containerLayout->setPortraitPolicy(portraitPolicy);

    layout->addItem(container, 0, 0);

    // Current color list
    colors << QColor(0x92, 0xD1, 0x5E);
    colors << QColor(0x68, 0xBF, 0xDE);
    colors << QColor(0xBB, 0x7F, 0xDE);
    colors << QColor(0xFF, 0x87, 0x95);
    colors << QColor(0xFF, 0xE3, 0x59);
    colors << QColor(0xC0, 0xBD, 0xAB);

    colors << QColor(0x5F, 0xC2, 0x1D);
    colors << QColor(0x31, 0x9C, 0xDE);
    colors << QColor(0x95, 0x6F, 0xCF);
    colors << QColor(0xED, 0x4C, 0x4C);
    colors << QColor(0xFF, 0xC9, 0x1A);
    colors << QColor(0x8A, 0x87, 0x80);

    colors << QColor(0x58, 0xAD, 0x2D);
    colors << QColor(0x29, 0x7D, 0xD1);
    colors << QColor(0xAE, 0x4E, 0xDE);
    colors << QColor(0xC9, 0x3C, 0x32);
    colors << QColor(0xFF, 0xB1, 0x14);
    colors << QColor(0x4D, 0x4D, 0x4D);

    colors << QColor(0x23, 0x8C, 0x17);
    colors << QColor(0x1A, 0x66, 0xC9);
    colors << QColor(0x86, 0x33, 0xAF);
    colors << QColor(0xB3, 0x27, 0x1E);
    colors << QColor(0xFF, 0x88, 0x00);
    colors << QColor(0x33, 0x33, 0x33);

    colors << QColor(0x24, 0x73, 0x2E);
    colors << QColor(0x12, 0x3D, 0x82);
    colors << QColor(0x6A, 0x2A, 0x78);
    colors << QColor(0x94, 0x11, 0x00);
    colors << QColor(0xFF, 0x6F, 0x00);
    colors << QColor(0x1A, 0x1A, 0x1A);
}

MColorListViewPrivate::~MColorListViewPrivate()
{
    delete buttonGroup;
}

void MColorListViewPrivate::updateColors()
{
    Q_Q(MColorListView);

    // A bit crude, perhaps, but let's conserve memory. this is called
    // relatively rarely anyway.
    if (buttonGroup) {
        int i = landscapePolicy->count() - 1;
        Q_ASSERT(landscapePolicy->count() == portraitPolicy->count());
        while (i >= 0) {
            landscapePolicy->removeAt(i--);
            portraitPolicy->removeAt(i--);
        }
        QList<MButton*> list = buttonGroup->buttons();
        while (!list.isEmpty()) {
            delete list.takeLast();
        }
        delete buttonGroup;
    }

    buttonGroup = new MButtonGroup();
    QObject::connect(buttonGroup, SIGNAL(buttonClicked(MButton*)),
                     q, SLOT(buttonClicked(MButton*)));

    const QColor &selectedColor = q->model()->selectedColor();

    int landColumns = 6;
    int portRows = 6;
    for (int i = 0; i < colors.size(); ++i) {
        SimpleColoredButton* button = new SimpleColoredButton(colors[i]);
        button->setStyleName("ColorSelectionButton");
        button->setCheckable(true);
        // We must check for selected color here too as the button has
        // to be checked *before* it is added to the group.
        if (colors[i] == selectedColor) {
            button->setChecked(true);
        }
        buttonGroup->addButton(button);
        // TODO: Calendar ALG defines that the color list should roll
        // horizontally in landscape mode (row by row), and vertically
        // in portraint mode (column by column). The implementation
        // here doesn't work nicely (for portrait) if we have more or
        // less colors than 28.
        landscapePolicy->addItem(button, i/landColumns, i%landColumns);
        portraitPolicy->addItem(button, i%portRows, i/portRows);
    }
}

void MColorListViewPrivate::updateSelectedColor()
{
    Q_Q(MColorListView);
    const QColor &color = q->model()->selectedColor();

    QList<MButton*> buttons = buttonGroup->buttons();
    for (int i = 0; i < buttons.size(); ++i) {
        if (static_cast<SimpleColoredButton*>(buttons[i])->color() == color) {
            buttons[i]->setChecked(true);
            break;
        }
    }
}

MColorListView::MColorListView(MColorList *controller)
    :MWidgetView(controller), d_ptr(new MColorListViewPrivate(this, controller))
{

}

MColorListView::~MColorListView()
{
    delete d_ptr;
}

void MColorListView::applyStyle()
{
    MWidgetView::applyStyle();
}

void MColorListView::setupModel()
{
    MWidgetView::setupModel();
    Q_D(MColorListView);
    d->updateColors();
    /*
     * Note: d->updateSelectedColor() should not be called here because the
     * selected button has to be checked before it is added to a MButtonGroup
     * for the button group to function properly.
     */
}

void MColorListView::updateData(const QList<const char*> &modifications)
{
    Q_D(MColorListView);
    MWidgetView::updateData(modifications);

    const char *member;
    foreach (member, modifications) {
        if (member == MColorListModel::SelectedColor) {
            d->updateSelectedColor();
        }
    }
}

void MColorListView::buttonClicked(MButton *button)
{
    model()->setSelectedColor(static_cast<SimpleColoredButton*>(button)->color());
}
