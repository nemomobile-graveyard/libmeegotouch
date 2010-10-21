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

#include "mstylablewidget.h"
#include "mscalableimage.h"
#include "mwidgetmodel.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MStylableWidget)


//! \internal
/* Private class acting as a creator for stylable widgets
   subclasses. It also takes responsibility for deleting
   creators when the application is quitting.
*/
class MStylableWidgetCreator : MWidgetCreatorBase
{
public:

    static void registerCreator(const QMetaObject *metaObject,
                                const char *widgetAssemblyName,
                                M::AssemblyType widgetAssemblyType) {
        const MStylableWidgetCreator *widgetCreator = creatorInstanceHolder.value(metaObject->className(), NULL);
        if (!widgetCreator) {
            MStylableWidgetCreator *creator = new MStylableWidgetCreator(metaObject,
                    widgetAssemblyName,
                    widgetAssemblyType);
            creatorInstanceHolder.insert(metaObject->className(), creator);
        }
    }

protected:
    MStylableWidgetCreator(const QMetaObject *_widgetMetaObject,
                             const char *widgetAssemblyName,
                             M::AssemblyType widgetAssemblyType) :
        MWidgetCreatorBase(_widgetMetaObject->className(), widgetAssemblyName, widgetAssemblyType),
        widgetMetaObject(_widgetMetaObject)
    {}

    virtual ~MStylableWidgetCreator()
    {}

    virtual MWidgetController *create() const {
        return NULL;
    }

    virtual const QMetaObject *metaObject() const {
        return widgetMetaObject;
    }

private:
    class CreatorInstanceHolder : public QMap<QString, MStylableWidgetCreator *>
    {
    public:
        ~CreatorInstanceHolder() {
            foreach(MStylableWidgetCreator * creator, this->values())
            delete creator;
        }
    };

    static CreatorInstanceHolder creatorInstanceHolder;
    const QMetaObject *widgetMetaObject;
};
//! \internal_end

MStylableWidgetCreator::CreatorInstanceHolder MStylableWidgetCreator::creatorInstanceHolder;

MStylableWidget::MStylableWidget(QGraphicsItem *parent) : MWidgetController(parent)
{
}

MStylableWidget::~MStylableWidget()
{
}

void MStylableWidget::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);
    if (!style()->backgroundTiles().isValid() && !style()->backgroundImage() && !style()->backgroundColor().isValid())
        return;

    qreal oldOpacity = painter->opacity();
    painter->setOpacity(style()->backgroundOpacity() * effectiveOpacity());

    QSizeF s = size() - QSizeF(style()->marginLeft() + style()->marginRight(), style()->marginTop() + style()->marginBottom());
    if (style()->backgroundTiles().isValid()) {
        style()->backgroundTiles()[model()->layoutPosition()]->draw(0.0,0.0, s.width(), s.height(), painter);
    }else if (style()->backgroundImage()) {
        style()->backgroundImage()->draw(0.0, 0.0, s.width(), s.height(), painter);
    } else { //style background color must be valid
        painter->fillRect(QRectF(QPointF(0, 0), s), QBrush(style()->backgroundColor()));
    }
    painter->setOpacity(oldOpacity);
}

void MStylableWidget::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
}

void MStylableWidget::drawForeground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
}

void MStylableWidget::applyStyle()
{
    if (layoutDirection() == Qt::RightToLeft)
        setContentsMargins(
            style()->paddingRight() + style()->marginRight(),
            style()->paddingTop() + style()->marginTop(),
            style()->paddingLeft() + style()->marginLeft(),
            style()->paddingBottom() + style()->marginBottom());
    else
        setContentsMargins(
            style()->paddingLeft() + style()->marginLeft(),
            style()->paddingTop() + style()->marginTop(),
            style()->paddingRight() + style()->marginRight(),
            style()->paddingBottom() + style()->marginBottom());

    updateGeometry();
    update();
}

void MStylableWidget::registerStylableWidgetType(const QMetaObject *metaObject,
        const char *widgetAssemblyName,
        M::AssemblyType widgetAssemblyType)
{
    MStylableWidgetCreator::registerCreator(metaObject, widgetAssemblyName, widgetAssemblyType);
}
