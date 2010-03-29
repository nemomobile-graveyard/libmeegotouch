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

#ifndef FAKEINSTALLATIONSOURCE_H
#define FAKEINSTALLATIONSOURCE_H

#include <duiappletinstallationsourceinterface.h>
#include <QString>
#include <DuiWidget>

class InstallationSourceWidget;

class FakeInstallationSource : public QObject, public DuiAppletInstallationSourceInterface
{
    Q_OBJECT
    Q_INTERFACES(DuiAppletInstallationSourceInterface DuiApplicationExtensionInterface)

public:

    FakeInstallationSource();
    virtual ~FakeInstallationSource();
    //! Methods derived from DuiApplicationExtensionInterface
    virtual bool initialize(const QString&);
    virtual DuiWidget *widget();

    //! Methods derived from DuiAppletInstallationSourceInterface
    void setDuiAppletInventoryInterface(DuiAppletInventoryInterface &installationSource) ;

    //! For accessing applet inventory interface
    DuiAppletInventoryInterface *appletInventoryInterface() const;
private:
    InstallationSourceWidget *source;
    DuiAppletInventoryInterface *appletInventory;
};

class InstallationSourceWidget : public DuiWidget
{
    Q_OBJECT
    Q_PROPERTY(QString installationSourceTitle READ installationSourceTitle)
    Q_PROPERTY(QString installationSourceText READ installationSourceText)
    Q_PROPERTY(QString installationSourceIcon READ installationSourceIcon)

public:
    InstallationSourceWidget(FakeInstallationSource &source);

    QString installationSourceTitle() {
        return "Applet Store";
    };
    QString installationSourceText() {
        return "42";
    };
    QString installationSourceIcon() {
        return "icon-s-alarm";
    };

private slots:

    /*!
      * Called to instantiate an applet from a package
      *
      * \param packageName the name of the applet package to be instantiated
      */
    void instantiateAppletsInPackage(const QString &packageName);

    /*!
     * Installs an applet from a local debian package. The package name is fetched
     * from an environment variable called FAKEAPPLETINSTALLATIONSOURCEPACKAGE.
     */
    void installApplet();

    /*!
     * Installs an applet from a package called fakeappletpackage-installable.deb
     */
    void installableApplet();

    /*!
     * Installs an applet from a package called fakeappletpackage-uninstallable.deb
     */
    void uninstallableApplet();

    /*!
     * Installs an applet from a package called fakeappletpackage-notdownloadable.deb
     */
    void notDownloadableApplet();

    /*!
     * Installs an applet from a package called fakeappletpackage-metadataonly.deb
     */
    void metadataOnly();

    /*!
     * Installs an applet from a package called fakeappletpackage-nonexistent.deb
     */
    void nonExistent();

private:
    //! For accessing installation source
    FakeInstallationSource &installationSource;
};

#endif // FAKEINSTALLATIONSOURCE_H
