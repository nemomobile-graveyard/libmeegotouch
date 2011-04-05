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

#ifndef MCONTAINER_H
#define MCONTAINER_H

#include <MWidgetController>
#include <MProgressIndicator>
#include <mexport.h>
#include <mcontainermodel.h>

class MContainerPrivate;

/*!
 * \class MContainer
 * \brief A MContainer is a generic container which can be used for any MWidgets.
 *
 * \section MContainerOverview Overview
 * Containers are used in settings views as well as building blocks for native
 * and WRT (web run time) applets in Experience Canvases.
 *
 *  Example:
 *  \code
 *
 * MContainer *box = new MContainer();
 *
 * box->setTitle("Header");
 * box->setText("More...");
 * box->setIconID("My-Icon-ID");
 *
 * // Create a button ...
 * MButton *button = new MButton("Hello container");
 *
 * // and set it as the centralWidget
 * box->setCentralWidget(button);
 *
 * // clicking the button hides the header
 * QObject::connect(button, SIGNAL(clicked()), box, SLOT(toggleHeaderVisible()));
 *
 *  \endcode
 *
 * \section MContainerUseGuidlines Usage guidelines
 * Containers can be used for building applets or in application views. They
 * may contain a layout of multiple widgets.
 * Container consist of the content placement box and an optional header, which contains the
 * label and an optional additional label. The header row can be tappable or
 * non-tappable. Containers for web-runtime (WRT) applets don't have a header.
 * If the header row is tappable, the label might be used. It is also
 * recommended to show the number of all items in  the additional label.
 * Containers and Applets keep their functionality independent of the device
 * orientation.
 *
 * For special cases there is an option to have "number of new items / number
 * of all items" in the additional label.
 *
 * \image html mcontainer.png
 *
 * The content placement box is the area where the content is drawn by the
 * native / WRT applet. In case the container is not used for an applet,
 * other UI Controls are drawn in the area.
 * The header is as wide as its container. The user can decide if the container
 * has a sub view, in which case it will be themed differently.
 * Titles can be either text, an icon or both of them. This is set by the
 * application or the applet, as well as their values. If not, the title is
 * empty. If the title is too long it will be truncated.
 * The application/applet also defines the usage of the additional label and
 * its content. If it doesn't specify it, it will be empty. The additional
 * label truncates.
 *
 * \sa MContainerModel, MContainerView, \see \ref appletdevelopment
 */

class M_CORE_EXPORT MContainer : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(MContainer)

    //! \brief Current widget in the container
    Q_PROPERTY(QGraphicsWidget *centralWidget READ centralWidget WRITE setCentralWidget)

    //! \brief Title of the container
    Q_PROPERTY(QString title READ title WRITE setTitle)

    //! \brief Text in the header
    Q_PROPERTY(QString text READ text WRITE setText)

    //! \brief Icon in the header
    Q_PROPERTY(QString iconID READ iconID WRITE setIconID)

    //! \brief Header visible or not
    Q_PROPERTY(bool headerVisible READ headerVisible WRITE setHeaderVisible)

    //! \brief Progress indicator visible or not
    Q_PROPERTY(bool progressIndicatorVisible READ isProgressIndicatorVisible WRITE setProgressIndicatorVisible)

public:
    /*!
     * \brief Constructs a container with no title and a \a parent.
     */
    MContainer(QGraphicsItem *parent = 0);

    /*!
     * \overload
     * \brief Constructs a container with the title \a title and parent \a parent.
    */
    MContainer(const QString &title, QGraphicsItem *parent = 0);

    /*!
     * \brief Destroys the container.
     */
    virtual ~MContainer();

    /*!
     * \brief Returns the widget which is the 'container area'.
     */
    QGraphicsWidget *centralWidget();

    /*!
     * \brief Sets the central widget for container.
     * By default MContainer takes ownership of the centralWidget.
     *
     * If \a destroy is false, the old centralWidget is removed
     * from the scene and its ownership passed back to caller. If \a destroy
     * is true (default value), the old central widget is deleted.
     */
    void setCentralWidget(QGraphicsWidget *centralWidget, bool destroy = true);

    /*!
     * \brief Returns the title of the container.
     */
    QString title() const;

    /*!
     * \brief Returns the text of the container.
     */
    QString text() const;

    /*!
     * \brief Returns the logical ID of the icon
     */
    QString iconID() const;

    /*!
     * \brief Returns the logical ID of the icon
     */
    bool headerVisible() const;

    /*!
     * \brief Returns if the progress indicator is shown
     */
    bool isProgressIndicatorVisible() const;

public slots :

    /*!
     * \brief Sets the header visibility
     * \param visibility new visibility state for header
     */
    void setHeaderVisible(const bool visibility);

    /*!
     * \brief Sets the progress indicator visibility
     * \param visibility new visibility state for progressindicator true for visible, false for hidden
     */
    void setProgressIndicatorVisible(bool visibility);

    /*!
     * \brief Toggle the header visibility
     */
    void toggleHeaderVisible();

    /*!
     * \brief Toggles the progress indicator visibility
     */
    void toggleProgressIndicatorVisible();


    /*!
     * \brief Set the title of container.
     * \param title New title for the container
     */
    void setTitle(const QString &title);

    /*!
     * \brief Sets the logical ID of the icon
     * \param iconID New icon for the container
     */
    void setIconID(const QString &iconID);

    /*!
     * \brief Set the text of container.
     * \param text New text for the container
     */
    void setText(const QString &text);

Q_SIGNALS:

    /*!
     * \brief Signal for informing that the header of the container was clicked
     */
    void headerClicked();

private:
    Q_DECLARE_PRIVATE(MContainer)
    Q_DISABLE_COPY(MContainer)
    Q_PRIVATE_SLOT(d_func(), void _q_onCentralWidgetDestroyed())

#ifdef UNIT_TEST
    friend class Ut_MContainer;
#endif
};

#endif
