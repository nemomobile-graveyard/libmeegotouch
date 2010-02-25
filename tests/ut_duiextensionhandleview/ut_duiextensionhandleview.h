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

#ifndef UT_DUIEXTENSIONHANDLEVIEW
#define UT_DUIEXTENSIONHANDLEVIEW

#include <QObject>
#include "duiextensionhandleview.h"
#include "duiextensionhandleview_p.h"

class DuiTestExtensionHandleView : public DuiExtensionHandleView
{
    DUI_VIEW(DuiExtensionHandleModel, DuiExtensionHandleStyle)

public:
    DuiTestExtensionHandleView(DuiExtensionHandle *handle);
    void click();
    QSizeF askSizeHints(Qt::SizeHint which, QSizeF &constraint);
    void appletPixmapModified(const QRectF &rect);

    bool brokenDialogShown;
    QString installationFailedDialogError;
    mutable bool pixmapDrawn;
};

class DuiTestExtensionHandleViewPrivate : public DuiExtensionHandleViewPrivate
{
    Q_DECLARE_PUBLIC(DuiTestExtensionHandleView)

public:
    DuiTestExtensionHandleViewPrivate(DuiExtensionHandle *handle);
    virtual ~DuiTestExtensionHandleViewPrivate();

    /*! Shows a dialog for deciding what to do with a broken applet. */
    void showBrokenDialog();

    /*! Shows a dialog for deciding what to do with an installation failed applet. */
    void showInstallationFailedDialog(const QString &error);

    //! \reimp
    virtual void drawPixmap(QPainter *painter, const QRectF &sourceGeometry, const QRectF &targetGeometry, bool brokenState) const;
    //! \reimp_end
};

// Test case must inherit QObject
class Ut_DuiExtensionHandleView : public QObject
{
    Q_OBJECT

public:
    static int returnedPixmapWidth;
    static int returnedPixmapHeight;
    static QRectF updatedRect;
    static QRectF drawnTargetRect;
    static QRectF drawnSourceRect;

private:
    //! DuiExtensionHandleView instance under testing.
    DuiTestExtensionHandleView *handleView;
    DuiExtensionHandle *handle;

signals:
    void openAppletSettings();

private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    void testScaling_data();
    void testScaling();
    void testSetGeometry();
    void testReturnedSizeHintManipulation();
    void testStateFeedback();
    void testBrokenAppletDialog();
    void testPixmapCleanup();
    void testPixmapModified();
    void testInstallationFailedDialog();
};
#endif // UT_DUIEXTENSIONHANDLEVIEW
