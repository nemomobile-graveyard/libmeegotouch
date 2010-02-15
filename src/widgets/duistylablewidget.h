/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#ifndef DUISTYLABLEWIDGET_H
#define DUISTYLABLEWIDGET_H

#include <DuiWidgetStyle>
#include <DuiWidgetController>
#include <DuiStyle>

#ifdef DUI_LIBRARY_NAME
#define DUI_STYLABLE_WIDGET(STYLE) \
    public: \
    inline static const char* staticStyleType() { return #STYLE; } \
    inline virtual const char* styleType() const { return #STYLE; } \
    protected: \
    virtual DuiWidgetStyleContainer* createStyleContainer() const { return new STYLE##Container(); } \
    private: \
    inline STYLE##Container& style() { return static_cast<STYLE##Container&>(DuiStylableWidget::style()); } \
    inline const STYLE##Container& style() const { return static_cast<const STYLE##Container&>(DuiStylableWidget::style()); } \
    class StylableWidgetCreatorRegisterer { \
    public: \
        StylableWidgetCreatorRegisterer() { \
            registerStylableWidgetType(&staticMetaObject, DUI_LIBRARY_NAME, Dui::Library); \
        }; \
    }; \
    StylableWidgetCreatorRegisterer stylableWidgetCreatorRegisterer;
#elif defined DUI_APPLICATION_NAME
#define DUI_STYLABLE_WIDGET(STYLE) \
    public: \
    inline static const char* staticStyleType() { return #STYLE; } \
    inline virtual const char* styleType() const { return #STYLE; } \
    protected: \
    virtual DuiWidgetStyleContainer* createStyleContainer() const { return new STYLE##Container(); } \
    private: \
    inline STYLE##Container& style() { return static_cast<STYLE##Container&>(DuiStylableWidget::style()); } \
    inline const STYLE##Container& style() const { return static_cast<const STYLE##Container&>(DuiStylableWidget::style()); } \
    class StylableWidgetCreatorRegisterer { \
    public: \
        StylableWidgetCreatorRegisterer() { \
            registerStylableWidgetType(&staticMetaObject, DUI_APPLICATION_NAME, Dui::Application); \
        }; \
    }; \
    StylableWidgetCreatorRegisterer stylableWidgetCreatorRegisterer;
#endif

/*!
    \class DuiStylableWidget

    \brief DuiStylableWidget is a convenience class to create simple, non-MVC widgets.

    DuiStylableWidget class implements basic functionality to support
    simple, non-MVC widgets which can be stylable at the same time.

    In order to use it, a new class should be inherited from DuiStylableWidget
    and DUI_STYLABLE_WIDGET macro should be used to define the style
    class for this widget:

    \code
        class MyStylableClass : public DuiStylableWidget
        {
        public:
            MyStylableClass();

        protected:
            // Method to be overridden for custom painting operations.
            virtual void drawContents(QPainter* painter, const QStyleOptionGraphicsItem* option) const;
        private:
            DUI_STYLABLE_WIDGET(MyStyle)
        };
    \endcode

    MyStyle class should follow typical style declaration:
    \code
        #include <DuiWidgetStyle>

        class DUI_EXPORT MyStyle : public DuiWidgetStyle
        {
            Q_OBJECT
            DUI_STYLE(MyStyle)

            DUI_STYLE_ATTRIBUTE(bool,    drawTiledHorizontal, DrawTiledHorizontal)
            DUI_STYLE_ATTRIBUTE(QString, imageHorizontal, ImageHorizontal)
            DUI_STYLE_ATTRIBUTE(QString, imageVertical, ImageVertical)
        };

        class DUI_EXPORT MyStyleContainer : public DuiWidgetStyleContainer
        {
            DUI_STYLE_CONTAINER(MyStyle)
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
class DUI_EXPORT DuiStylableWidget : public DuiWidgetController
{
    Q_OBJECT

    friend class DuiStylableWidgetView;

public:
    /*!
     * \brief Constructor that sets up the widget.
     * \param parent Parent widget.
     */
    explicit DuiStylableWidget(QGraphicsItem *parent = 0);

    /*!
     * \brief Destructor
     */
    virtual   ~DuiStylableWidget();

    inline static const char *staticStyleType() {
        return "DuiWidgetStyle";
    }
    inline virtual const char *styleType() const {
        return "DuiWidgetStyle";
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
     * used by DUI_STYLABLE_WIDGET macro definition.
     */
    static void registerStylableWidgetType(const QMetaObject *metaObject,
                                           const char *widgetAssemblyName,
                                           Dui::AssemblyType widgetAssemblyType);

protected:
    /*!
      This method should only be overridden by DUI_STYLABLE_WIDGET macro.
      It provides widget style container to be used in derived classes.
     */
    virtual DuiWidgetStyleContainer *createStyleContainer() const {
        return new DuiWidgetStyleContainer();
    }

private:
    Q_DISABLE_COPY(DuiStylableWidget)

};

#endif // DUISTYLABLEWIDGET_H
