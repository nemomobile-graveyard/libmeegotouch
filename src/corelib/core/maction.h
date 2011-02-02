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

#ifndef MACTION_H
#define MACTION_H

#include "mexport.h"

#include <QAction>

class MActionPrivate;

/**
   \class MAction
   \brief MAction implements an extension of the QAction for libmeegotouch
 */
class M_CORE_EXPORT MAction : public QAction
{
    Q_OBJECT

    /*!
       \ref logicalid "Logical ID" of the action icon.
    */
    Q_PROPERTY(QString iconID READ iconID WRITE setIconID)

    Q_FLAGS(Locations)

public:

    /*!
       \brief Possible locations for an action to reside in.
       An action may stay in different locations on the screen, but will have
       preferred ones. This information is useful if actions should be shifted
       from one location to another automatically.
     */
    enum Location {
        NoLocation = 0x0000, /*!< No location. */
        ToolBarPortraitLocation = 0x0001, /*!< In the tool bar, if in portrait orientation. */
        ToolBarLandscapeLocation = 0x0002, /*!< In the tool bar, if in landscape orientation. */
        ToolBarLocation = 0x0003, /*!< In the tool bar, regardless of the orientation. */
        ApplicationMenuLocation = 0x0004, /*!< In the application menu. */
        ObjectMenuLocation = 0x0008, /*!< In the object menu. */
        EveryLocation = 0xffff /*!< In all possible locations. */
    };

    Q_DECLARE_FLAGS(Locations, Location)

    /**
       \brief Default constructor
       \param parent  Parent object
     */
    explicit MAction(QObject *parent);

    /**
       \brief Constructor
       \param text  Text to be used as label for action button
       \param parent  Parent object
       This overloaded constructor is provided for convenience.
     */
    MAction(const QString &text, QObject *parent);

    /**
       \brief Constructor
       \param iconID  Identifier for icon to be shown on action button
       \param text  Text to be used as label for action button
       \param parent  Parent object
       This overloaded constructor is provided for convenience.
     */
    MAction(const QString &iconID, const QString &text, QObject *parent);

    /**
       \brief Destructor for an action
     */
    virtual ~MAction();

    /*!
      Returns the \ref logicalid "logical ID" associated with this action's icon.
     */
    QString iconID() const;

    /*!
      Sets the \ref logicalid "logical ID" associated with this action's icon to \a id.
     */
    void setIconID(const QString &id);

    /*!
      Returns the \ref logicalid "logical ID" associated with this action's toggled icon.
     */
    QString toggledIconID() const;

    /*!
      Sets the \ref logicalid "logical ID" associated with this action's toggled icon to \a id.
     */
    void setToggledIconID(const QString &id);

    /**
       \brief Return the possible locations this action may reside in
     */
    Locations location() const;

    /**
       \brief Set the locations this action may reside in
       \param location value of possible locations
     */
    void setLocation(Locations location);

    /*!
     * \brief sets the action as style action, which can be used to
     * set the style/order/presentation of the contents of the view.
     */
    void setStyleAction(bool styleAction);

    /*!
     * \brief Return whether this action is a style action or not
     */
    bool isStyleAction() const;

protected:
    /** \internal */
    MActionPrivate *const d_ptr;
    MAction(MActionPrivate &dd, QObject *parent = 0);
    /** \internal_end */

private:
    Q_DISABLE_COPY(MAction)
    Q_DECLARE_PRIVATE(MAction)

#ifdef UNIT_TEST
    friend class Ut_MAction;
#endif
};

Q_DECLARE_OPERATORS_FOR_FLAGS(MAction::Locations)

#endif
