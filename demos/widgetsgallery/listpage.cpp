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

#include "listpage.h"
#include "templatepage.h"
#include "labelpage.h"
#include "pushbuttonpage.h"
#include "iconbuttonpage.h"
#include "switchpage.h"
#include "dialogsandnotificationspage.h"
#include "imagepage.h"
#include "applicationmenupage.h"
#include "toolbarpage.h"
#include "textentrypage.h"
#include "mlistpage.h"
#include "mgridpage.h"
#include "sliderpage.h"
#include "comboboxpage.h"
#include "containerpage.h"
#include "progressbarpage.h"
#include "spinnerpage.h"
#include "checkboxpage.h"
#include "objectmenupage.h"
#include "navigationbarpage.h"
#include "languagepage.h"
#include "feedbackpage.h"
#include "timingscene.h"

#ifdef HAVE_GSTREAMER
#include "videocontainerpage.h"
#endif

#include <QGraphicsLayoutItem>
#include <QGraphicsLinearLayout>
#include <QGraphicsGridLayout>
#include <QApplication>
#include <QPointer>
#include <QDebug>
#include <QSettings>
#include <QDir>

#include <MApplication>
#include <MLocale>
#include <MLayout>
#include <MButton>
#include <MLabel>
#include <MAction>
#include <MDialog>
#include <MButtonGroup>
#include <MTheme>
#include <MGridLayoutPolicy>
#include <MLinearLayoutPolicy>
#include <MApplicationWindow>

#ifdef HAVE_GCONF
#include <MGConfItem>
#endif

MyContainer::MyContainer(QGraphicsItem *parent)
    : MContainer(parent)
{
    MLayout *layout = new MLayout(centralWidget());

    landscapePolicy = new MGridLayoutPolicy(layout);
    landscapePolicy->setContentsMargins(0, 0, 0, 0);
    landscapePolicy->setSpacing(0);
    //To make sure that both columns have the same width, give them the same preferred width.
    landscapePolicy->setColumnPreferredWidth(0, 800);
    landscapePolicy->setColumnPreferredWidth(1, 800);

    portraitPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    portraitPolicy->setContentsMargins(0, 0, 0, 0);
    portraitPolicy->setSpacing(0);

    layout->setLandscapePolicy(landscapePolicy);
    layout->setPortraitPolicy(portraitPolicy);
}

MyContainer::~MyContainer()
{
}

void MyContainer::addItem(QGraphicsLayoutItem *button)
{
    int count = landscapePolicy->count();

    int row = count / 2;
    int column = count % 2;

    landscapePolicy->addItem(button, row, column);
    portraitPolicy->addItem(button);
}

ListPage::ListPage(const QString &title)
    : shownPage(NULL)
{
    setTitle(title);

    connect(this, SIGNAL(appeared()), this, SLOT(showInitialPage()));
}

ListPage::~ListPage()
{
}

QString ListPage::timedemoTitle()
{
    return "MainPage";
}

void ListPage::createContent()
{
    MApplicationPage::createContent();

    addPage(new ApplicationMenuPage);
    addPage(new ObjectMenuPage);
    addPage(new DialogsAndNotificationsPage);
    addPage(new NavigationBarPage);
    addPage(new ToolBarPage);
    addPage(new ContainerPage);
    addPage(new LabelPage);
    addPage(new ImagePage);
    addPage(new PushButtonPage);
    addPage(new IconButtonPage);
    addPage(new SwitchPage);
    addPage(new ProgressBarPage);
    addPage(new SpinnerPage);
    addPage(new MListPage);
    addPage(new MGridPage);
    addPage(new CheckboxPage);
    addPage(new ComboBoxPage);
    addPage(new TextEntryPage);
    addPage(new SliderPage);
    addPage(new LanguagePage);
    addPage(new FeedbackPage);
#ifdef HAVE_GSTREAMER
    addPage(new VideoContainerPage);
#endif

    QGraphicsWidget *panel = centralWidget();

    MLayout *layout = new MLayout(panel);
    panel->setLayout(layout);
    policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    policy->setContentsMargins(6, 6, 6, 6);
    policy->setSpacing(0);

    populateLayout();

    MAction *action = new MAction("Themes", this);
    action->setLocation(MAction::ApplicationMenuLocation);
    this->addAction(action);
    connect(action, SIGNAL(triggered()), SLOT(showThemeSelectionDialog()));

    action = new MAction("Orientation", this);
    action->setLocation(MAction::ApplicationMenuLocation);
    this->addAction(action);
    connect(action, SIGNAL(triggered()), SLOT(showOrientationSelectionDialog()));

    action = new MAction("Toggle FPS", this);
    action->setLocation(MAction::ApplicationMenuLocation);
    this->addAction(action);
    connect(action, SIGNAL(triggered()), SLOT(toggleFps()));

    retranslateUi();
}

void ListPage::retranslateUi()
{
    //% "Widgets Gallery"
    setTitle(qtTrId("xx_application_title"));

    QList<MButton *> keys = buttons.keys();
    const int keysCount = keys.count();
    for (int i = 0; i < keysCount; ++i) {
        MButton *button = keys[i];
        MApplicationPage *page = buttons.value(button);
        if (!page) continue;

        // the retranslateUi() is normally only executed when a
        // page is visible, but because we need the new translations
        // of the titles, we have to do this hack here.
        // we call retranslateUi for the pages.
        QEvent ev(QEvent::LanguageChange);
        qApp->sendEvent(page, &ev);

        button->setText(page->title());
    }

    if (!isContentCreated())
        return;

    QStringList groupNames = TemplatePage::groupNames();
    const int count = groupNames.count();
    for (int i = 0; i < count; ++i)
        containers[i]->setTitle(groupNames[i]);

}

MGridLayoutPolicy *ListPage::createAndSetupGridPolicy(MWidget *panel)
{
    MLayout *layout = new MLayout(panel);
    MGridLayoutPolicy *policy = new MGridLayoutPolicy(layout);
    policy->setContentsMargins(6, 6, 6, 6);
    policy->setSpacing(0);
    return policy;
}

void ListPage::setInitialPageToShow(const QString& initialPageToShow)
{
    this->initialPageToShow = initialPageToShow;
}


void ListPage::populateLayout()
{
    QStringList groupNames = TemplatePage::groupNames();

    const int pageCount = pages.count();
    const int groupNamesCount = groupNames.count();
    for (int i = 0; i < groupNamesCount; ++i) {

        MyContainer *container = new MyContainer;
        containers.append(container);
        policy->addItem(container);

        int numButtons = 0;
        for (int j = 0; j < pageCount; ++j) {
            TemplatePage *page = qobject_cast<TemplatePage *>(pages[j]);
            if (!page || page->groupID() != i) continue;

            MButton *listItem = new MButton(page->title());
            container->addItem(listItem);
            numButtons++;

            listItem->connect(listItem, SIGNAL(clicked()), this, SLOT(handleListItemClick()));
            buttons.insert(listItem, page);
        }
        //If we only have one item, add an empty spacer widget to ensure that we have two columns
        if (numButtons == 1) {
            QGraphicsWidget *widget = new QGraphicsWidget;
            widget->setMinimumSize(80, 0);
            container->addItem(widget);
        }
    }

    centralWidget()->setContentsMargins(0, 0, 0, 25);
}

void ListPage::addPage(TemplatePage *page)
{
    // Take ownership of page
    page->setParent(this);
    pages.append(page);
}

void ListPage::handleListItemClick()
{
    QObject *sender_object = sender();
    if (sender_object)   {
        MButton *button = qobject_cast<MButton *>(sender_object);

        if (button) {
            MApplicationPage *page = buttons.value(button);
            showPage(page);
        }
    }
}

int ListPage::pageCount() const
{
    return pages.count();
}

void ListPage::showPage(MApplicationPage *page)
{
    if (page) {
        page->appear();
        shownPage = page;
    }
}

void ListPage::showPageByIndex(int index)
{
    showPage(findPageByIndex(index));
}

TemplatePage *ListPage::findPageByIndex(int index) const
{
    if (index >= 0 && index < pages.count()) {
        return pages[ index ];
    }

    return 0;
}

TemplatePage *ListPage::findPageByTimedemoTitle(const QString& title) const
{
    for (int i = 0; i < pages.size(); ++i) {
        if (pages.at(i)->timedemoTitle() == title) {
            return pages.at(i);
        }
    }
    return 0;
}

void ListPage::showPageByTimedemoTitle(const QString& name)
{
    TemplatePage *page = findPageByTimedemoTitle(name);
    if (page) {
        showPage(page);
    }
}

QSettings *themeFile(const QString &theme)
{
    // Determine whether this is a m theme:
    // step 1: we need to have index.theme file
    QDir themeDir(THEMEDIR);
    const QString themeIndexFileName = themeDir.absolutePath() + QDir::separator() + theme + QDir::separator() + "index.theme";
    if (!QFile::exists(themeIndexFileName))
        return NULL;

    // step 2: it needs to be a valid ini file
    QSettings *themeIndexFile = new QSettings(themeIndexFileName, QSettings::IniFormat);
    if (themeIndexFile->status() != QSettings::NoError) {
        delete themeIndexFile;
        return NULL;
    }

    // step 3: we need to have X-M-Metatheme group in index.theme
    if (!themeIndexFile->childGroups().contains(QString("X-M-Metatheme"))) {
        delete themeIndexFile;
        return NULL;
    }
    return themeIndexFile;
}

struct ThemeInfo {
    QString theme;
    QString themeName;
    QString themeIcon;
};
QList<ThemeInfo> findAvailableThemes()
{
    QList<ThemeInfo> themes;

    // find all directories under the theme directory
    QDir themeDir(THEMEDIR);
    const QFileInfoList directories = themeDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach(const QFileInfo & dir, directories) {
        ThemeInfo info;

        const QSettings *themeIndexFile = themeFile(dir.baseName());
        if (!themeIndexFile)
            continue;

        // check if this theme is visible
        if (!themeIndexFile->value("X-M-Metatheme/X-Visible", true).toBool()) {
            delete themeIndexFile;
            continue;
        }

        info.theme = dir.baseName();
        info.themeName = themeIndexFile->value("Desktop Entry/Name", "").toString();
        info.themeIcon = themeIndexFile->value("X-M-Metatheme/X-Icon", "").toString();

        // ok it's a valid theme. Add it to list of themes
        themes.append(info);
        delete themeIndexFile;
    }

    return themes;
}

#ifndef HAVE_GCONF
extern void M_changeTheme(const QString &theme);
#endif

void ListPage::showThemeSelectionDialog()
{
    QList<ThemeInfo> themes = findAvailableThemes();

    QPointer<MDialog> dialog = new MDialog("Select theme", M::OkButton | M::CancelButton);

    QGraphicsGridLayout *layout = new QGraphicsGridLayout();
    dialog->centralWidget()->setLayout(layout);

    MButtonGroup *group = new MButtonGroup(dialog->centralWidget());

    const int themesCount = themes.count();
    for (int i = 0; i < themesCount; ++i) {
        MButton *button = new MButton(themes[i].themeIcon, themes[i].themeName);
        button->setObjectName("theme-selection-button");
        button->setCheckable(true);
        if (MTheme::currentTheme() == themes[i].theme)
            button->setChecked(true);

        layout->addItem(button, i/4, i%4);
        group->addButton(button, i);
    }

    if (dialog->exec() == MDialog::Accepted) {
        int index = group->checkedId();
        if (index >= 0) {
#ifdef HAVE_GCONF
            MGConfItem themeName("/meegotouch/theme/name");
            themeName.set(themes[index].theme);
#else
            M_changeTheme(themes[index].theme);
#endif
        }
    }

    delete dialog;
}

void ListPage::showOrientationSelectionDialog()
{
    QPointer<MDialog> dialog = new MDialog("Select orientation (angle)", M::OkButton | M::CancelButton);

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    dialog->centralWidget()->setLayout(layout);

    MButtonGroup *group = new MButtonGroup(dialog->centralWidget());

    MButton *automatic = new MButton(//% "Automatic"
        qtTrId("xx_apporientation_auto"));
    automatic->setCheckable(true);
    automatic->setChecked(!applicationWindow()->isOrientationAngleLocked() &&
                          !applicationWindow()->isOrientationLocked());
    layout->addItem(automatic);
    group->addButton(automatic);

    MButton *portrait = new MButton(//% "Portrait"
        qtTrId("xx_apporientation_portrait"));
    portrait->setCheckable(true);
    portrait->setChecked(applicationWindow()->isOrientationLocked()
                         && applicationWindow()->orientation() == M::Portrait);
    layout->addItem(portrait);
    group->addButton(portrait);

    MButton *landscape = new MButton(//% "Landscape"
        qtTrId("xx_apporientation_landscape"));
    landscape->setCheckable(true);
    landscape->setChecked(applicationWindow()->isOrientationLocked()
                          && applicationWindow()->orientation() == M::Landscape);
    layout->addItem(landscape);
    group->addButton(landscape);

    MButton *angle0 = new MButton(//% "0 degrees"
        qtTrId("xx_apporientation_angle0"));
    angle0->setCheckable(true);
    angle0->setChecked(applicationWindow()->isOrientationAngleLocked()
                       && applicationWindow()->orientationAngle() == M::Angle0);
    layout->addItem(angle0);
    group->addButton(angle0);

    MButton *angle90 = new MButton(//% "90 degrees clockwise"
        qtTrId("xx_apporientation_angle90"));
    angle90->setCheckable(true);
    angle0->setChecked(applicationWindow()->isOrientationAngleLocked()
                       && applicationWindow()->orientationAngle() == M::Angle90);
    layout->addItem(angle90);
    group->addButton(angle90);

    MButton *angle180 = new MButton(//% "180 degrees clockwise"
        qtTrId("xx_apporientation_angle180"));
    angle180->setCheckable(true);
    angle0->setChecked(applicationWindow()->isOrientationAngleLocked()
                       && applicationWindow()->orientationAngle() == M::Angle180);
    layout->addItem(angle180);
    group->addButton(angle180);

    MButton *angle270 = new MButton(//% "270 degrees clockwise"
        qtTrId("xx_apporientation_angle270"));
    angle270->setCheckable(true);
    angle0->setChecked(applicationWindow()->isOrientationAngleLocked()
                       && applicationWindow()->orientationAngle() == M::Angle270);
    layout->addItem(angle270);
    group->addButton(angle270);

    if (dialog->exec() == MDialog::Accepted) {
        MButton *mode = group->checkedButton();
        if (mode == automatic) {
            applicationWindow()->setOrientationAngleLocked(false);
            applicationWindow()->setOrientationLocked(false);
        } else if (mode == portrait) {
            applicationWindow()->setOrientationAngleLocked(false);
            applicationWindow()->setOrientationAngle(M::Angle90);
            applicationWindow()->setOrientationLocked(true);
        } else if (mode == landscape) {
            applicationWindow()->setOrientationAngleLocked(false);
            applicationWindow()->setOrientationAngle(M::Angle0);
            applicationWindow()->setOrientationLocked(true);
        } else if (mode == angle0) {
            applicationWindow()->setOrientationLocked(false);
            applicationWindow()->setOrientationAngle(M::Angle0);
            applicationWindow()->setOrientationAngleLocked(true);
        } else if (mode == angle90) {
            applicationWindow()->setOrientationLocked(false);
            applicationWindow()->setOrientationAngle(M::Angle90);
            applicationWindow()->setOrientationAngleLocked(true);
        } else if (mode == angle180) {
            applicationWindow()->setOrientationLocked(false);
            applicationWindow()->setOrientationAngle(M::Angle180);
            applicationWindow()->setOrientationAngleLocked(true);
        } else if (mode == angle270) {
            applicationWindow()->setOrientationLocked(false);
            applicationWindow()->setOrientationAngle(M::Angle270);
            applicationWindow()->setOrientationAngleLocked(true);
        }
    }

    delete dialog;
}

void ListPage::toggleFps()
{
    MApplication::instance()->setShowFps(!MApplication::showFps());
}

void ListPage::showInitialPage()
{
    if (!initialPageToShow.isEmpty()) {
        showPageByTimedemoTitle(initialPageToShow);
    }
}
