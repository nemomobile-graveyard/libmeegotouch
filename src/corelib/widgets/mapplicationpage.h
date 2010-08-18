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

#ifndef MAPPLICATIONPAGE_H
#define MAPPLICATIONPAGE_H

#include <mscenewindow.h>
#include <QList>
#include "mnamespace.h"
#include <mapplicationpagemodel.h>
#include <mescapebuttonpanelmodel.h>

class MApplicationPagePrivate;
class MAction;
class MApplicationWindow;
class MPannableViewport;


/*!
    \class MApplicationPage
    \brief MApplicationPage provides a framework for building an application's user interface.
    By default the page creates a pannable viewport where a user can place his component by using
    centralWidget() or setCentralWidget(MWidget *).

    The page can be shown on the screen using appear(QGraphicsScene*, DeletionPolicy) or
    appear(MWindow*, DeletionPolicy). A call to appear() involves running
    the assiciated show animation for the page. If you want to show a page instantly, refer to
    MSceneManager API.

    A page can contain actions, which will be shown in the navigation bar in the application menu.

    \li addAction()
    \li actions()
    \li clearActions()

    To switch off panning, or change direction of panning:
    \li setPannable()
    \li setPanningDirection()

    A minimal application which would show a page would look like this:

    \code
      #include <MApplication>
      #include <MApplicationWindow>
      #include <MApplicationPage>
      #include <MButton>

      int main(int argc, char *argv[]){
          MApplication application(argc, argv);
          MApplicationWindow window;
          MApplicationPage page;

          new MButton("Hello", page.centralWidget());
          page.appear(&window);

          window.show();

          return application.exec();
       }
    \endcode

    centralWidget() returns MWidget, layout can be used to group more than one widget.

    \section navigating-between-pages Navigating between pages.

    Only one page can be displayed at any given time. The framework enforces this behavior.
    Thus in order to make your application navigate to a child page all you have to do is
    call the child's appear() method. That will automatically make the current page disappear
    to make room for the child page that is starting to appear.

    In an hypothetical music browser application, the following code snippet
    would make it transition from the current album page to a child page displaying
    one of its songs:

    \code
    class AlbumPage : public MApplicationPage {
        ...
        void displaySong(Song song) {
            MApplicationPage *songPage = new SongPage(song);

            // I don't need this page anymore after it gets dismissed
            songPage->appear(MSceneWindow::DestroyWhenDismissed);
        }
        ...
    };
    \endcode

    The code above will put AlbumPage onto the top of the page history's stack and make
    SongPage be displayed. Since the escapeMode of the SongPage is MApplicationPageModel::EscapeAuto
    (the default value) the escape button panel will automatically show a back button that, when clicked,
    will bring the application back to the AlbumPage. Therefore no code is required to make your
    application navigate back to its previous page.

    To implement a different behavior for the escape button you have set the escapeMode of the page to either
    MApplicationPageModel::EscapeManualBack or MApplicationPageModel::EscapeCloseWindow.

    You can also check and manually modify the page navigation history of your application using the methods
    MSceneManager::pageHistory() and MSceneManager::setPageHistory().
*/

class M_EXPORT MApplicationPage : public MSceneWindow
{
    Q_OBJECT
    M_CONTROLLER(MApplicationPage)

    Q_PROPERTY(bool autoMarginsForComponentsEnabled READ autoMarginsForComponentsEnabled WRITE setAutoMarginsForComponentsEnabled)
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(bool contentCreated READ isContentCreated)
    Q_PROPERTY(MApplicationPageModel::PageEscapeMode escapeMode READ escapeMode WRITE setEscapeMode)
    Q_PROPERTY(bool pannable READ isPannable WRITE setPannable)
    Q_PROPERTY(Qt::Orientations panningDirection READ panningDirection WRITE setPanningDirection)
    Q_PROPERTY(bool rememberPosition READ rememberPosition WRITE setRememberPosition)
    Q_PROPERTY(bool progressIndicatorVisible READ isProgressIndicatorVisible WRITE setProgressIndicatorVisible)
    Q_PROPERTY(QRectF exposedContentRect READ exposedContentRect NOTIFY exposedContentRectChanged)

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
    MApplicationPage(QGraphicsItem *parent = 0);

    /*!
      \brief Destroys the page.
     */
    virtual ~MApplicationPage();

    /*!
     * \brief This function will be called before page becomes visible. Override it in your page class
     * and create content for a page. Framework calls this function only once, in enterDisplayEvent().
     *
     * \sa enterDisplayEvent()
     */
    virtual void createContent();

    /*! \brief Returns the display mode of a component.

        All components are shown by default.

        \sa setComponentsDisplayMode()
     */
    MApplicationPageModel::ComponentDisplayMode componentDisplayMode(Component) const;

    /*! \brief Sets the display mode of components.

        Example code on how to make a page take the entire screen space (i.e., go fullscreen).
        \code
        page->setComponentsDisplayMode(MApplicationPage::AllComponents,
                                       MApplicationPageModel::AutoHide);
        \endcode

        \param components The components whose display mode you want to set.
        \param displayMode The new display mode for the given components.

        \sa componentDisplayMode(), MApplicationPageModel::ComponentDisplayMode
     */
    void setComponentsDisplayMode(Components components, MApplicationPageModel::ComponentDisplayMode displayMode);

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
     * position when hidden and restore it when shown again using appear().
     * \sa setRememberPosition()
     */
    bool rememberPosition() const;

    /*!
     * Returns true if page's is pannable.
     * \sa setPannable(bool)
     */
    bool isPannable() const;

    /*!
     * Returns the orientations in which the page pans. The value can be
     * Qt::Horizontal, Qt::Vertical or Qt::Horizontal | Qt::Vertical.
     */
    Qt::Orientations panningDirection() const;

    /**
     * \brief Returns the central widget for the page.
     * By default page provides a widget (panel) on which
     * other widgets can be placed. It's also possible to set a central widget for a page with
     * setCentralWidget(MWidget*) function.
     * \return the pointer to the central widget.
     */
    QGraphicsWidget *centralWidget();

    /*!
     * Convenience function that returns a pointer to the application window into
     * which the application page is shown or a null pointer if appear()
     * hasn't been called yet. When you call appear() and the application window
     * specified as a parameter exists, this method will return it.
     *
     * \sa appear(QGraphicsScene*, DeletionPolicy), appear(MWindow*, DeletionPolicy),
     *     createContent(), QGraphicsItem::scene(), QGraphicsScene::views()
     */
    // FIXME: Do we really need (or want to have) this method? If so, at least
    //        make it MSceneWindow::window() instead.
    MApplicationWindow *applicationWindow();

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
     * \sa MApplicationPageMode::PageEscapeMode, setEscapeMode()
     */
    MApplicationPageModel::PageEscapeMode escapeMode() const;

    /*!
     * Returns the viewport responsible for panning the page.
     */
    MPannableViewport *pannableViewport();

    /*!
       \brief Area of the page whose content is not covered by any bar

       Area of the page whose content is not covered by any bar (status bar,
       navigation bar, tool bar) and thus is accessible to the user.

       Floating widgets can be implemented by making them children of the application
       page instead of the central widget (thus they are not panned). They should be
       positioned within this rectangle.

       Note that this area can be covered by transient scene windows such as
       dialogs and notifications as well as by overlays.

       The rectangle is in local item coordinates.

       \sa exposedContentRectChanged()
     */
    QRectF exposedContentRect() const;

Q_SIGNALS:
    //! Signal emitted when back button called
    void backButtonClicked();

    /*! \brief Signal emitted when close button is clicked.
     * This can happen when escapeButtonMode() is set to MEscapeButtonPanelModel::CloseMode.
     *
     * Note that the close() slot of the associated MApplicationWindow will also get called
     * when the close button is clicked.
     *
     * For code that should be executed when a MApplicationWindow is closed you should
     * consider reimplementing the QWidget::closeEvent() method of your MApplicationWindow
     * instead of listening to this signal.
     *
     * \sa QWidget::close()
     */
    void closeButtonClicked();

    /*! Signal emitted when action is added(removed) from the page.
     */
    void actionUpdated(QActionEvent *e);

    /*!
        \brief Emitted when the exposed content rectangle changes.

        That happens when some bar (navigation bar, status bar, tool bar) appears or
        disappears, which makes the exposed content area of the page bigger or smaller.

        Appearance or disappearance of transient scene windows such as dialogs and
        notifications does not change the exposed content rectangle of the page.
        Overlays are also not considered.

        \sa exposedContentRect()
     */
    void exposedContentRectChanged();

public Q_SLOTS:
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
     * NOTE: MApplicationPage takes ownership of the widget pointer
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
     * scrolled to the top with every call to appear(). The scrolling
     * is immediate, i.e. it's not animated at all.
     * \sa rememberPosition()
     */
    void setRememberPosition(bool remember);

    /*!
     * Sets if the page should pan
     */
    void setPannable(bool pannable);

    /*!
     * Sets to which direction(s) page should be pannable. Possible values:
     * Qt::Horizontal, Qt::Vertical or Qt::Horizontal | Qt::Vertical.
     */
    void setPanningDirection(Qt::Orientations directions);

    /*!
     * Sets the visible of progress indicator
     * \param bool visible
     */
    void setProgressIndicatorVisible(bool visible);

    /*!
     * \brief Sets the page escape mode
     * \sa MApplicationPageModel::PageEscapeMode, escapeMode()
     */
    void setEscapeMode(MApplicationPageModel::PageEscapeMode mode);

protected:
    //! \reimp
    virtual void actionEvent(QActionEvent *);
    //! \reimp_end

    /*!
     * Reimplemented to call createContent() if it hasn't been called yet.
     *
     * \note When reimplementing this event in your inherited classes, be sure to call
     *       base class's event handler to set up your page content properly. Otherwise
     *       createContent() will never be called.
     *
     * \sa createContent()
     */
    virtual void enterDisplayEvent();

private:
#ifdef UNIT_TEST
    friend class Ut_MApplicationPage;
#endif
    friend class MApplicationWindow;
    friend class MApplicationWindowPrivate;

    Q_DISABLE_COPY(MApplicationPage)
    Q_DECLARE_PRIVATE(MApplicationPage)

    // We are using a hack which will allow reimplementing the enterDisplayEvent
    // method in the application page without recompilation of all client applications.
    // This needs to be deleted when API unfreeze will finally happen.
    friend class MWidget;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(MApplicationPage::Components)

#endif
