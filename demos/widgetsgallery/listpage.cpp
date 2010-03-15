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
#include "duilistpage.h"
#include "duigridpage.h"
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

#ifndef Q_WS_MACX
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

#include <DuiApplication>
#include <DuiLocale>
#include <DuiLayout>
#include <DuiButton>
#include <DuiLabel>
#include <DuiAction>
#include <DuiDialog>
#include <DuiButtonGroup>
#include <DuiTheme>
#include <DuiGridLayoutPolicy>
#include <DuiLinearLayoutPolicy>
#include <DuiApplicationWindow>

#ifdef HAVE_GCONF
#include <DuiGConfItem>
#endif

MyContainer::MyContainer(QGraphicsItem *parent)
    : DuiContainer(parent)
{
    DuiLayout *layout = new DuiLayout(centralWidget());

    landscapePolicy = new DuiGridLayoutPolicy(layout);
    landscapePolicy->setContentsMargins(0, 0, 0, 0);
    landscapePolicy->setSpacing(0);
    //To make sure that both columns have the same width, give them the same preferred width.
    landscapePolicy->setColumnPreferredWidth(0, 800);
    landscapePolicy->setColumnPreferredWidth(1, 800);

    portraitPolicy = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
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

    connect(this, SIGNAL(windowShown()), this, SLOT(showInitialPage()));
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
    DuiApplicationPage::createContent();

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
    addPage(new DuiListPage);
    addPage(new DuiGridPage);
    addPage(new CheckboxPage);
    addPage(new ComboBoxPage);
    addPage(new TextEntryPage);
    addPage(new SliderPage);
    addPage(new LanguagePage);
    addPage(new FeedbackPage);
#ifndef Q_WS_MACX
    addPage(new VideoContainerPage);
#endif

    DuiWidget *panel = centralWidget();

    DuiLayout *layout = new DuiLayout(panel);
    panel->setLayout(layout);
    policy = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    policy->setContentsMargins(6, 6, 6, 6);
    policy->setSpacing(0);

    populateLayout();

    DuiAction *action = new DuiAction("Themes", this);
    action->setLocation(DuiAction::ApplicationMenuLocation);
    this->addAction(action);
    connect(action, SIGNAL(triggered()), SLOT(showThemeSelectionDialog()));

    action = new DuiAction("Orientation", this);
    action->setLocation(DuiAction::ApplicationMenuLocation);
    this->addAction(action);
    connect(action, SIGNAL(triggered()), SLOT(showOrientationSelectionDialog()));

    action = new DuiAction("Toggle FPS", this);
    action->setLocation(DuiAction::ApplicationMenuLocation);
    this->addAction(action);
    connect(action, SIGNAL(triggered()), SLOT(toggleFps()));

    retranslateUi();
}

void ListPage::retranslateUi()
{
    //% "Widgets Gallery"
    setTitle(qtTrId("xx_application_title"));

    QList<DuiButton *> keys = buttons.keys();
    const int keysCount = keys.count();
    for (int i = 0; i < keysCount; ++i) {
        DuiButton *button = keys[i];
        DuiApplicationPage *page = buttons.value(button);
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

DuiGridLayoutPolicy *ListPage::createAndSetupGridPolicy(DuiWidget *panel)
{
    DuiLayout *layout = new DuiLayout(panel);
    DuiGridLayoutPolicy *policy = new DuiGridLayoutPolicy(layout);
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

            DuiButton *listItem = new DuiButton(page->title());
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

    connect(page, SIGNAL(backButtonClicked()), page, SLOT(dismiss()));
}

void ListPage::handleListItemClick()
{
    QObject *sender_object = sender();
    if (sender_object)   {
        DuiButton *button = qobject_cast<DuiButton *>(sender_object);

        if (button) {
            DuiApplicationPage *page = buttons.value(button);
            showPage(page);
        }
    }
}

int ListPage::pageCount() const
{
    return pages.count();
}

void ListPage::showPage(DuiApplicationPage *page)
{
    if (page) {
        page->setEscapeButtonMode(DuiEscapeButtonPanelModel::BackMode);
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
    // Determine whether this is a dui theme:
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

    // step 3: we need to have X-DUI-Metatheme group in index.theme
    if (!themeIndexFile->childGroups().contains(QString("X-DUI-Metatheme"))) {
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
        if (!themeIndexFile->value("X-DUI-Metatheme/X-Visible", true).toBool()) {
            delete themeIndexFile;
            continue;
        }

        info.theme = dir.baseName();
        info.themeName = themeIndexFile->value("Desktop Entry/Name", "").toString();
        info.themeIcon = themeIndexFile->value("X-DUI-Metatheme/X-Icon", "").toString();

        // ok it's a valid theme. Add it to list of themes
        themes.append(info);
        delete themeIndexFile;
    }

    return themes;
}

#ifndef HAVE_GCONF
extern void Dui_changeTheme(const QString &theme);
#endif

void ListPage::showThemeSelectionDialog()
{
    QList<ThemeInfo> themes = findAvailableThemes();

    QPointer<DuiDialog> dialog = new DuiDialog("Select theme", Dui::OkButton | Dui::CancelButton);

    QGraphicsGridLayout *layout = new QGraphicsGridLayout();
    dialog->centralWidget()->setLayout(layout);

    DuiButtonGroup *group = new DuiButtonGroup(dialog->centralWidget());

    const int themesCount = themes.count();
    for (int i = 0; i < themesCount; ++i) {
        DuiButton *button = new DuiButton(themes[i].themeIcon, themes[i].themeName);
        button->setObjectName("theme-selection-button");
        button->setCheckable(true);
        if (DuiTheme::currentTheme() == themes[i].theme)
            button->setChecked(true);

        layout->addItem(button, i/4, i%4);
        group->addButton(button, i);
    }

    if (dialog->exec() == DuiDialog::Accepted) {
        int index = group->checkedId();
        if (index >= 0) {
#ifdef HAVE_GCONF
            DuiGConfItem themeName("/Dui/theme/name");
            themeName.set(themes[index].theme);
#else
            Dui_changeTheme(themes[index].theme);
#endif
        }
    }

    delete dialog;
}

void ListPage::showOrientationSelectionDialog()
{
    QPointer<DuiDialog> dialog = new DuiDialog("Select orientation (angle)", Dui::OkButton | Dui::CancelButton);

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    dialog->centralWidget()->setLayout(layout);

    DuiButtonGroup *group = new DuiButtonGroup(dialog->centralWidget());

    DuiButton *automatic = new DuiButton(//% "Automatic"
        qtTrId("xx_apporientation_auto"));
    automatic->setCheckable(true);
    automatic->setChecked(!applicationWindow()->isOrientationAngleLocked() &&
                          !applicationWindow()->isOrientationLocked());
    layout->addItem(automatic);
    group->addButton(automatic);

    DuiButton *portrait = new DuiButton(//% "Portrait"
        qtTrId("xx_apporientation_portrait"));
    portrait->setCheckable(true);
    portrait->setChecked(applicationWindow()->isOrientationLocked()
                         && applicationWindow()->orientation() == Dui::Portrait);
    layout->addItem(portrait);
    group->addButton(portrait);

    DuiButton *landscape = new DuiButton(//% "Landscape"
        qtTrId("xx_apporientation_landscape"));
    landscape->setCheckable(true);
    landscape->setChecked(applicationWindow()->isOrientationLocked()
                          && applicationWindow()->orientation() == Dui::Landscape);
    layout->addItem(landscape);
    group->addButton(landscape);

    DuiButton *angle0 = new DuiButton(//% "0 degrees"
        qtTrId("xx_apporientation_angle0"));
    angle0->setCheckable(true);
    angle0->setChecked(applicationWindow()->isOrientationAngleLocked()
                       && applicationWindow()->orientationAngle() == Dui::Angle0);
    layout->addItem(angle0);
    group->addButton(angle0);

    DuiButton *angle90 = new DuiButton(//% "90 degrees clockwise"
        qtTrId("xx_apporientation_angle90"));
    angle90->setCheckable(true);
    angle0->setChecked(applicationWindow()->isOrientationAngleLocked()
                       && applicationWindow()->orientationAngle() == Dui::Angle90);
    layout->addItem(angle90);
    group->addButton(angle90);

    DuiButton *angle180 = new DuiButton(//% "180 degrees clockwise"
        qtTrId("xx_apporientation_angle180"));
    angle180->setCheckable(true);
    angle0->setChecked(applicationWindow()->isOrientationAngleLocked()
                       && applicationWindow()->orientationAngle() == Dui::Angle180);
    layout->addItem(angle180);
    group->addButton(angle180);

    DuiButton *angle270 = new DuiButton(//% "270 degrees clockwise"
        qtTrId("xx_apporientation_angle270"));
    angle270->setCheckable(true);
    angle0->setChecked(applicationWindow()->isOrientationAngleLocked()
                       && applicationWindow()->orientationAngle() == Dui::Angle270);
    layout->addItem(angle270);
    group->addButton(angle270);

    if (dialog->exec() == DuiDialog::Accepted) {
        DuiButton *mode = group->checkedButton();
        if (mode == automatic) {
            applicationWindow()->setOrientationAngleLocked(false);
            applicationWindow()->setOrientationLocked(false);
        } else if (mode == portrait) {
            applicationWindow()->setOrientationAngleLocked(false);
            applicationWindow()->setOrientationAngle(Dui::Angle90);
            applicationWindow()->setOrientationLocked(true);
        } else if (mode == landscape) {
            applicationWindow()->setOrientationAngleLocked(false);
            applicationWindow()->setOrientationAngle(Dui::Angle0);
            applicationWindow()->setOrientationLocked(true);
        } else if (mode == angle0) {
            applicationWindow()->setOrientationLocked(false);
            applicationWindow()->setOrientationAngle(Dui::Angle0);
            applicationWindow()->setOrientationAngleLocked(true);
        } else if (mode == angle90) {
            applicationWindow()->setOrientationLocked(false);
            applicationWindow()->setOrientationAngle(Dui::Angle90);
            applicationWindow()->setOrientationAngleLocked(true);
        } else if (mode == angle180) {
            applicationWindow()->setOrientationLocked(false);
            applicationWindow()->setOrientationAngle(Dui::Angle180);
            applicationWindow()->setOrientationAngleLocked(true);
        } else if (mode == angle270) {
            applicationWindow()->setOrientationLocked(false);
            applicationWindow()->setOrientationAngle(Dui::Angle270);
            applicationWindow()->setOrientationAngleLocked(true);
        }
    }

    delete dialog;
}

void ListPage::toggleFps()
{
    DuiApplication::instance()->setShowFps(!DuiApplication::showFps());
}

void ListPage::showInitialPage()
{
    if (!initialPageToShow.isEmpty()) {
        showPageByTimedemoTitle(initialPageToShow);
    }
}
