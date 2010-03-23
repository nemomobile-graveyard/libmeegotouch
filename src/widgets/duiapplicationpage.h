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

#ifndef DUIAPPLICATIONPAGE_H
#define DUIAPPLICATIONPAGE_H

#include <duiscenewindow.h>
#include <QList>
#include "duinamespace.h"
#include <duiapplicationpagemodel.h>
#include <duiescapebuttonpanelmodel.h>

class DuiApplicationPagePrivate;
class DuiAction;
class DuiApplicationWindow;

/*!
    \class DuiApplicationPage
    \brief DuiApplicationPage provides a framework for building an application's user interface.
    By default the page creates a pannable area where a user can place his component by using
    centralWidget() or setCentralWidget(DuiWidget *).

    There are two functions to show the page on the screen:
    \li appear() shows the page with the associated animation.
    \li appearNow() shows the page immediately on the screen.

    A page can contain actions, which will be shown in the navigation bar or on a view menu.

    \li addAction()
    \li actions()
    \li clearActions()

    To switch off panning, or change direction of panning:
    \li setPannableAreaInteractive()
    \li setPannableAreaDirection()

    A minimal application which would show a toolbar would look like this:

    \code
      #include <DuiApplication>
      #include <DuiApplicationWindow>
      #include <DuiApplicationPage>
      #include <DuiButton>

      int main(int argc, char *argv[]){
          DuiApplication application(argc, argv);
          DuiApplicationWindow window;
          window.show();
          DuiApplicationPage page;
          new DuiButton("Hello", page.centralWidget());
          page.appear();
          return a.exec();
       }
    \endcode

    centralWidget() returns DuiWidget, layout can be used to group more than one widget.
*/

class DUI_EXPORT DuiApplicationPage : public DuiSceneWindow
{
    Q_OBJECT
    DUI_CONTROLLER(DuiApplicationPage)

    Q_PROPERTY(bool autoMarginsForComponentsEnabled READ autoMarginsForComponentsEnabled WRITE setAutoMarginsForComponentsEnabled)
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(bool contentCreated READ isContentCreated)
    Q_PROPERTY(DuiApplicationPageModel::PageEscapeMode escapeMode READ escapeMode WRITE setEscapeMode)
    Q_PROPERTY(bool pannableAreaInteractive READ isPannableAreaInteractive WRITE setPannableAreaInteractive)
    Q_PROPERTY(Qt::Orientations pannableAreaDirection READ pannableAreaDirection WRITE setPannableAreaDirection)
    Q_PROPERTY(bool rememberPosition READ rememberPosition WRITE setRememberPosition)
    Q_PROPERTY(bool progressIndicatorVisible READ isProgressIndicatorVisible WRITE setProgressIndicatorVisible)

public:

    /*!
        This enum is used to specify components in componentDisplayMode()
        and setComponentsDisplayMode().

        Components are widgets held by the application window whose behavior
        is affected by the page being currently displayed.
     */
    enum Component {
        NavigationBar = 0x1, /*!< The navigation bar  */
        HomeButton = 0x2, /*!< The home button  */
        EscapeButton = 0x4, /*!< The escape button  */
        AllComponents = NavigationBar | HomeButton | EscapeButton /*!< All components */
    };
    Q_DECLARE_FLAGS(Components, Component)

    /*!
     * \brief Default constructor. Creates a new page.
     */
    DuiApplicationPage(QGraphicsItem *parent = 0);

    /*!
      \brief Destroys the page.
     */
    virtual ~DuiApplicationPage();

    /*!
     * \brief This function will be called before page becomes visible. Override it in your page class
     * and create content for a page. Framework calls this function only once.
     */
    virtual void createContent();

    /*! \brief Returns the display mode of a component.

        All components are shown by default.

        \sa setComponentsDisplayMode()
     */
    DuiApplicationPageModel::ComponentDisplayMode componentDisplayMode(Component) const;

    /*! \brief Sets the display mode of components.

        Example code on how to make a page take the entire screen space (i.e., go fullscreen).
        \code
        page->setComponentsDisplayMode(DuiApplicationPage::AllComponents,
                                       DuiApplicationPageModel::AutoHide);
        \endcode

        \param components The components whose display mode you want to set.
        \param displayMode The new display mode for the given components.

        \sa componentDisplayMode(), DuiApplicationPageModel::ComponentDisplayMode
     */
    void setComponentsDisplayMode(Components components, DuiApplicationPageModel::ComponentDisplayMode displayMode);

    /* \brief Whether margins will be automatically added around the central widget to
     *        avoid occlusion by other components.
     *
     * \sa setAutoMarginsForComponentsEnabled()
     */
    bool autoMarginsForComponentsEnabled() const;

    /*!
     * Returns true if createContent() was called by framework, otherwise false.
     */
    bool isContentCreated() const;

    /*!
     * Returns true if the page is expected to rememeber its pannable viewport
     * position when hidden and restore it when shown again using appear() or appearNow().
     * \sa setRememberPosition()
     */
    bool rememberPosition() const;

    /*!
     * Returns true if page's pannable are is interactive, e.g. can be panned.
     * \sa setPannableAreaInteractive(bool)
     */
    bool isPannableAreaInteractive() const;

    /*!
     * Returns combination of orientation flags in which pannable area pans. It can be
     * Qt::Horizontal, Qt::Vertical or Qt::Horizontal | Qt::Vertical.
     */
    Qt::Orientations pannableAreaDirection() const;

    /**
     * \brief Returns the central widget for the page.
     * By default page provides a widget (panel) on which
     * other widgets can be placed. It's also possible to set a central widget for a page with
     * setCentralWidget(DuiWidget*) function.
     * \return the pointer to the central widget.
     */
    QGraphicsWidget *centralWidget();

    /*!
     * Convenience function that returns a pointer to the application window into
     * which the application page is shown or a null pointer if appear() or appearNow()
     * hasn't been called yet. When you call appear() and the application window
     * specified as a parameter exists, this method will return it.
     *
     * \sa appear(), createContent(), QGraphicsItem::scene(), QGraphicsScene::views()
     */
    // FIXME: Do we really need (or want to have) this method? If so, at least
    //        make it DuiSceneWindow::window() instead.
    DuiApplicationWindow *applicationWindow();

    /**
     * \return Get the page's title. The title appears in the command area.
     */
    const QString title() const;

    /**
     * \brief Returns whether progress indicator is visible
     * \return <code>true</code> if the progress indicator is visible, otherwise <code>false</code>
     * \sa setProgressIndicatorVisible()
     */
    bool isProgressIndicatorVisible() const;

    /*!
     * \brief Returns the page's escape mode
     *
     * By default, it's EscapeAuto.
     *
     * \sa DuiApplicationPageMode::PageEscapeMode, setEscapeMode()
     */
    DuiApplicationPageModel::PageEscapeMode escapeMode() const;

Q_SIGNALS:
    //! Signal emitted when back button called
    void backButtonClicked();

    /*! \brief Signal emitted when close button is clicked.
     * This can happen when escapeButtonMode() is set to DuiEscapeButtonPanelModel::CloseMode.
     *
     * Note that the close() slot of the associated DuiApplicationWindow will also get called
     * when the close button is clicked.
     *
     * For code that should be executed when a DuiApplicationWindow is closed you should
     * consider reimplementing the QWidget::closeEvent() method of your DuiApplicationWindow
     * instead of listening to this signal.
     *
     * \sa QWidget::close()
     */
    void closeButtonClicked();

    /*! Signal emitted when action is added(removed) from the page.
     */
    void actionUpdated(QActionEvent *e);

public Q_SLOTS:
    /*!
     * \brief Makes page visible on the current window. Associated animation will be used to show page.
     *
     * Existing page will disappear.
     */
    // TODO: Remove that now useless method override after API freeze period
    void appear(DuiSceneWindow::DeletionPolicy policy = KeepWhenDone);

    /*!
     * \brief Makes page visible immediately, skipping animation.
     *
     * Existing page will disappear.
     */
    // TODO: Remove that now useless method override after API freeze period
    void appearNow(DuiSceneWindow::DeletionPolicy policy = KeepWhenDone);

    /*!
     * \brief Makes page visible on the application window specified by \a window.
     *
     * Uses associated animation will be used to show page. Existing page will disappear.
     */
    // TODO: Remove that now useless method after API freeze period
    void appear(DuiApplicationWindow *window, DuiSceneWindow::DeletionPolicy policy = KeepWhenDone);

    /*!
     * \brief Makes page visible immediately on the application window specified by \a window, skipping animation.
     *
     * Existing page will disappear.
     */
    // TODO: Remove that now useless method after API freeze period
    void appearNow(DuiApplicationWindow *window, DuiSceneWindow::DeletionPolicy policy = KeepWhenDone);

    /*!
     * \brief Makes page invisible with associated animation.
     */
    // TODO: Remove that now useless method override after API freeze period
    void disappear();

    /*!
     * \brief Makes page invisible immediately.
     */
    // TODO: Remove that now useless method override after API freeze period
    void disappearNow();

    /*!
        \brief Whether extra margins will be automatically added around the central
               widget to avoid occlusion by other components.

        If enabled, extra margins will be automatically added around the central
        widget according to the components that are currently being displayed in
        order to avoid parts of the central widget from being inaccessible.

        When a navigation bar appears on the upper edge of the application window, for
        instance, an extra margin will be automatically added to the top of the
        central widget of roughly the height of that navigation bar. Therefore
        effectively avoiding the top part of the central widget from being
        inaccessible.

        It's enabled by default.

        \note When the navigation bar is in AutoHide mode the top margin won't be added
        to the central widget.

        \sa autoMarginsForComponentsEnabled(), setComponentsDisplayMode()
    */
    void setAutoMarginsForComponentsEnabled(bool enabled);

    /**
     * \brief Sets the given widget to be the page's central widget.
     * It's also possible to use the default widget, as a parent for all widgets
     * using the centralWidget() function.
     * NOTE: DuiApplicationPage takes ownership of the widget pointer
     * and deletes it when needed.
     * \param  centralWidget the central widget.
     */
    void setCentralWidget(QGraphicsWidget *centralWidget);

    /**
     * \brief Set the title for this page.
     * Title appearing on a command area.
     * \param title New title for this page.
     */
    void setTitle(const QString &title);

    /*!
     * Sets whether the page should remember its pannable viewport position when
     * hidden. Default value is true. Set it to false to have the viewport
     * scrolled to the top with every call to appear() or appearNow(). The scrolling
     * is immediate, i.e. it's not animated at all.
     * \sa rememberPosition()
     */
    void setRememberPosition(bool remember);

    /*!
     * Sets if pannable area should be interactive, e.g. if page should pan.
     */
    void setPannableAreaInteractive(bool interactive);

    /*!
     * Sets to which direction(s) page should be. Possible values:
     * Qt::Horizontal, Qt::Vertical or Qt::Horizontal | Qt::Vertical.
     */
    void setPannableAreaDirection(Qt::Orientations directions);

    /*!
     * Sets the visible of progress indicator
     * \param bool visible
     */
    void setProgressIndicatorVisible(bool visible);

    /*!
     * \brief Sets the page escape mode
     * \sa DuiApplicationPageModel::PageEscapeMode, escapeMode()
     */
    void setEscapeMode(DuiApplicationPageModel::PageEscapeMode mode);

protected:
    //! \reimp
    virtual void actionEvent(QActionEvent *);
    //! \reimp_end

private:

#ifdef UNIT_TEST
    friend class Ut_DuiApplicationPage;
#endif
    friend class DuiApplicationWindow;
    friend class DuiApplicationWindowPrivate;

    Q_DISABLE_COPY(DuiApplicationPage)
    Q_DECLARE_PRIVATE(DuiApplicationPage)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(DuiApplicationPage::Components)

#endif
