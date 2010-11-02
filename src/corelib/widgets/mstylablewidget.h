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

#ifndef MSTYLABLEWIDGET_H
#define MSTYLABLEWIDGET_H

#include <MWidgetStyle>
#include <MWidgetController>
#include <MStyle>

#ifdef M_LIBRARY_NAME
#define M_STYLABLE_WIDGET(STYLE) \
    public: \
    inline static const char* staticStyleType() { return #STYLE; } \
    inline virtual const char* styleType() const { return #STYLE; } \
    protected: \
    virtual MWidgetStyleContainer* createStyleContainer() const { return new STYLE##Container(); } \
    private: \
    inline STYLE##Container& style() { return static_cast<STYLE##Container&>(MStylableWidget::style()); } \
    inline const STYLE##Container& style() const { return static_cast<const STYLE##Container&>(MStylableWidget::style()); } \
    class StylableWidgetCreatorRegisterer { \
    public: \
        StylableWidgetCreatorRegisterer() { \
            registerStylableWidgetType(&staticMetaObject, M_LIBRARY_NAME, M::Library); \
        }; \
    }; \
    StylableWidgetCreatorRegisterer stylableWidgetCreatorRegisterer;
#elif defined M_APPLICATION_NAME
#define M_STYLABLE_WIDGET(STYLE) \
    public: \
    inline static const char* staticStyleType() { return #STYLE; } \
    inline virtual const char* styleType() const { return #STYLE; } \
    protected: \
    virtual MWidgetStyleContainer* createStyleContainer() const { return new STYLE##Container(); } \
    private: \
    inline STYLE##Container& style() { return static_cast<STYLE##Container&>(MStylableWidget::style()); } \
    inline const STYLE##Container& style() const { return static_cast<const STYLE##Container&>(MStylableWidget::style()); } \
    class StylableWidgetCreatorRegisterer { \
    public: \
        StylableWidgetCreatorRegisterer() { \
            registerStylableWidgetType(&staticMetaObject, M_APPLICATION_NAME, M::Application); \
        }; \
    }; \
    StylableWidgetCreatorRegisterer stylableWidgetCreatorRegisterer;
#endif

/*!
    \class MStylableWidget

    \brief MStylableWidget is a convenience class to create simple, non-MVC widgets.

    MStylableWidget class implements basic functionality to support
    simple, non-MVC widgets which can be stylable at the same time.

    In order to use it, a new class should be inherited from MStylableWidget
    and the M_STYLABLE_WIDGET macro should be used to define the style
    class for this widget.

    The Q_OBJECT macro must be used too.

    \code
        class MyStylableClass : public MStylableWidget
        {
            Q_OBJECT
            M_STYLABLE_WIDGET(MyStyle)

        public:
            MyStylableClass();

        protected:
            // Method to be overridden for custom painting operations.
            virtual void drawContents(QPainter* painter, const QStyleOptionGraphicsItem* option) const;
        };
    \endcode

    MyStyle class should follow typical style declaration:
    \code
        #include <MWidgetStyle>

        class M_CORE_EXPORT MyStyle : public MWidgetStyle
        {
            Q_OBJECT
            M_STYLE(MyStyle)

            M_STYLE_ATTRIBUTE(bool,    drawTiledHorizontal, DrawTiledHorizontal)
            M_STYLE_ATTRIBUTE(QString, imageHorizontal, ImageHorizontal)
            M_STYLE_ATTRIBUTE(QString, imageVertical, ImageVertical)
        };

        class M_CORE_EXPORT MyStyleContainer : public MWidgetStyleContainer
        {
            M_STYLE_CONTAINER(MyStyle)
        };
    \endcode

    Please note, that paint() should NOT be overridden in order to draw the contents of the widget.
    Following methods should be used instead: drawBackground() drawContents() drawForeground().

    When calculating the area to which the widget should be drawn, please take
    margins into consideration:
    \code
     QRectF paintingRect = QRectF( QPointF(style()->marginLeft(), style()->marginTop()),
                                   size() - QSizeF(style()->marginRight(),style()->marginBottom()));
    \endcode
*/
class M_CORE_EXPORT MStylableWidget : public MWidgetController
{
    Q_OBJECT

    friend class MStylableWidgetView;

public:
    /*!
     * \brief Constructor that sets up the widget.
     * \param parent Parent widget.
     */
    explicit MStylableWidget(QGraphicsItem *parent = 0);

    /*!
     * \brief Destructor
     */
    virtual   ~MStylableWidget();

    inline static const char *staticStyleType() {
        return "MWidgetStyle";
    }
    inline virtual const char *styleType() const {
        return "MWidgetStyle";
    }

protected:

    /*!
       Notification for derived classes. This method gets called when a new style is applied for this widget.
       This happens e.g. when the object is constructed and when a new object name is given to the widget.
     */
    virtual void applyStyle();

    /*!
     * Draws the background for this widget. This method should be overridden
     * in order to provide background drawing functionality. The base implementation
     * uses background-image css attribute to draw a background beneath the widget.
     */
    virtual void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;

    /*!
     * Draws the contents for this widget. This method should be overridden to draw
     * contents of the widget.
     */
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;

    /*!
     * Draws the foreground for this widget. This method should be overridden to draw
     * the foreground overlay of the widget.
     */
    virtual void drawForeground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;

    /*!
     * Creates a record about the stylable widget type which is then used
     * to match the widget to the proper style.
     * Note: This method should not be called directly, it is going to be
     * used by M_STYLABLE_WIDGET macro definition.
     */
    static void registerStylableWidgetType(const QMetaObject *metaObject,
                                           const char *widgetAssemblyName,
                                           M::AssemblyType widgetAssemblyType);

protected:
    /*!
      This method should only be overridden by M_STYLABLE_WIDGET macro.
      It provides widget style container to be used in derived classes.
     */
    virtual MWidgetStyleContainer *createStyleContainer() const {
        return new MWidgetStyleContainer();
    }

private:
    Q_DISABLE_COPY(MStylableWidget)

#ifdef UNIT_TEST
    // Test unit is defined as a friend of production code to access private members
    friend class Ut_MStylableWidget;
#endif

};

#endif // MSTYLABLEWIDGET_H
