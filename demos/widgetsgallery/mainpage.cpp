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

#include "mainpage.h"
#include "maincategorypage.h"
#include "templatepage.h"
#include "labelpage.h"
#include "pushbuttonpage.h"
#include "buttongrouppage.h"
#include "iconbuttonpage.h"
#include "switchpage.h"
#include "imagepage.h"
#include "applicationmenupage.h"
#include "toolbarpage.h"
#include "textentrypage.h"
#include "textentrynavigationpage.h"
#include "mlistpage.h"
#include "mgridpage.h"
#include "sliderpage.h"
#include "comboboxpage.h"
#include "containerpage.h"
#include "progressbarpage.h"
#include "spinnerpage.h"
#include "checkboxpage.h"
#include "objectmenupage.h"
#include "displaymodespage.h"
#include "languagepage.h"
#include "feedbackpage.h"
#include "contentitemspage.h"
#include "singleselectiondialogspage.h"
#include "querydialogspage.h"
#include "bannerspage.h"
#include "bubblepage.h"

#include <QGraphicsLayoutItem>
#include <QGraphicsLinearLayout>
#include <QGraphicsGridLayout>
#include <QApplication>
#include <QPointer>
#include <QDebug>
#include <QSettings>
#include <QStringListModel>
#include <QDir>

#include <MApplication>
#include <MAbstractCellCreator>
#include <MBasicListItem>
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

class WidgetsGalleryDataModel : public QAbstractItemModel {
public:
    typedef QList<TemplatePage*> TemplatePageList;
    typedef QList<QString> TemplatePageNameList;
public:
    WidgetsGalleryDataModel()
        : QAbstractItemModel(),
          categoryPageNames(),
          categoryPages(),
          galleryPages(),
          galleryPageNames()
    {
        //% "Application View"
        addCategory(qtTrId("xx_wg_categories_application_view"), new MainCategoryPage(this, createIndex(0,0)));
        //% "Application Menu"
        addGalleryPage(index(0,0), qtTrId("xx_wg_application_view_view_menu"), new ApplicationMenuPage());
        //% "Display Modes"
        addGalleryPage(index(0,0), qtTrId("xx_wg_application_view_display_modes"), new DisplayModesPage());
        //% "Toolbar (including Tab Bar)"
        addGalleryPage(index(0,0), qtTrId("xx_wg_application_view_toolbar_incl_tab_bar"), new ToolBarPage());


        //% "Simple Widgets"
        addCategory(qtTrId("xx_wg_categories_simple_widgets"), new MainCategoryPage(this, createIndex(1,0)));
        //% "Labels"
        addGalleryPage(index(1,0), qtTrId("xx_wg_simple_widgets_labels"), new LabelPage());
        //% "Image"
        addGalleryPage(index(1,0), qtTrId("xx_wg_simple_widgets_image"), new ImagePage());
        //% "Progress Bar"
        addGalleryPage(index(1,0), qtTrId("xx_wg_simple_widgets_progress_bar"), new ProgressBarPage());
        //% "Container"
        addGalleryPage(index(1,0), qtTrId("xx_wg_simple_widgets_container"), new ContainerPage());
        //% "Spinner"
        addGalleryPage(index(1,0), qtTrId("xx_wg_simple_widgets_spinner"), new SpinnerPage());

        //% "Buttons"
        addCategory(qtTrId("xx_wg_categories_buttons"), new MainCategoryPage(this, createIndex(2,0)));
        //% "Push Button"
        addGalleryPage(index(2,0), qtTrId("xx_wg_buttons_push_button"), new PushButtonPage());
        //% "Icon Button"
        addGalleryPage(index(2,0), qtTrId("xx_wg_buttons_icon_button"), new IconButtonPage());
        //% "Switch"
        addGalleryPage(index(2,0), qtTrId("xx_wg_buttons_switch"), new SwitchPage());
        //% "Checkbox"
        addGalleryPage(index(2,0), qtTrId("xx_wg_buttons_checkbox"), new CheckboxPage());
        //% "Button Group"
        addGalleryPage(index(2,0), qtTrId("xx_wg_buttons_buttongroup"), new ButtonGroupPage());

        //% "Dialogs and Banners"
        addCategory(qtTrId("xx_wg_categories_dialogs_and_banners"), new MainCategoryPage(this, createIndex(3,0)));
        //% "Single Selection Dialog"
        addGalleryPage(index(3,0), qtTrId("xx_wg_dialogs_and_banners_single_selection_dialog"), new SingleSelectionDialogsPage());
        //% "Query Dialog"
        addGalleryPage(index(3,0), qtTrId("xx_wg_dialogs_and_banners_query_dialog"), new QueryDialogsPage());
        //% "Banner"
        addGalleryPage(index(3,0), qtTrId("xx_wg_dialogs_and_banners_banner"), new BannersPage());

        //% "Lists, Grids and Popups"
        addCategory(qtTrId("xx_wg_categories_lists_grids_and_popups"), new MainCategoryPage(this, createIndex(4,0)));
        //% "List"
        addGalleryPage(index(4,0), qtTrId("xx_wg_lists_grids_and_popups_list"), new MListPage());
        //% "Grid"
        addGalleryPage(index(4,0), qtTrId("xx_wg_lists_grids_and_popups_grid"), new MGridPage());
        //% "Object Menu"
        addGalleryPage(index(4,0), qtTrId("xx_wg_lists_grids_and_popups_object_menu"), new ObjectMenuPage());
        //% "ComboBox"
        addGalleryPage(index(4,0), qtTrId("xx_wg_lists_grids_and_popups_combobox"), new ComboBoxPage());
        //% "ContentItems"
        addGalleryPage(index(4,0), qtTrId("xx_wg_lists_grids_and_popups_contentitems"), new ContentItemsPage());
        //% "Speech bubble"
        addGalleryPage(index(4,0), qtTrId("xx_wg_lists_grids_and_popups_bubble"), new BubblePage());

        //% "User Input"
        addCategory(qtTrId("xx_wg_categories_user_input"), new MainCategoryPage(this, createIndex(5,0)));
        //% "Text Entries"
        addGalleryPage(index(5,0), qtTrId("xx_wg_user_input_text_entries"), new TextEntryPage());
        //% "Text Entries Navigation"
        addGalleryPage(index(5,0), qtTrId("xx_wg_user_input_text_entries_navigation"), new TextEntryNavigationPage());
        //% "Slider"
        addGalleryPage(index(5,0), qtTrId("xx_wg_user_input_slider"), new SliderPage());
        //% "Input Feedback"
        addGalleryPage(index(5,0), qtTrId("xx_wg_user_input_input_feedback"), new FeedbackPage());

    }

    QModelIndex parent(const QModelIndex &child) const {
        Q_UNUSED(child);

        if (child.internalId() >= 0)
            return index(child.internalId(), 0);

        return QModelIndex();
    }

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const {
        if (parent.isValid())
            return createIndex(row, column, parent.row());
        else
            return createIndex(row, column, -1);
    }

    int columnCount(const QModelIndex &parent) const {
        Q_UNUSED(parent);

        return 1;
    }

    int rowCount(const QModelIndex &parent) const {
        Q_UNUSED(parent);

        if (parent.isValid())
            return galleryPages[parent.row()].count();
        else
            return categoryPageNames.count();
    }

    QVariant data(const QModelIndex &index, int role) const {
        Q_UNUSED(index);

        if (index.isValid()) {
            if (role == Qt::DisplayRole || role == MainPage::PageName) {
                return getName(index);
            } else if (role == MainPage::Page) {
                return QVariant::fromValue((void *)getPage(index));
            } else if (role == MainPage::PageTimedemoName) {
                return getTimedemoName(index);
            }
        }

        return QVariant();
    }

private:
    void addCategory(const QString &name, MainCategoryPage *page) {
        page->setTitle(name);

        categoryPageNames.append(name);
        categoryPages.append(page);
    }

    void addGalleryPage(const QModelIndex &parent, const QString &name, TemplatePage *page) {
        galleryPageNames[parent.row()].append(name);
        galleryPages[parent.row()].append(page);
    }

    MApplicationPage *getPage(const QModelIndex &index) const {
        if (index.parent().isValid())
            return galleryPages[index.parent().row()].at(index.row());
        else
            return categoryPages[index.row()];
    }

    const QString &getName(const QModelIndex &index) const {
        if (index.parent().isValid())
            return galleryPageNames[index.parent().row()].at(index.row());
        else
            return categoryPageNames[index.row()];
    }

     QString getTimedemoName(const QModelIndex &index) const {
        if (index.parent().isValid())
            return galleryPages[index.parent().row()].at(index.row())->timedemoTitle();

        return QString();
    }

private:
    QList<QString> categoryPageNames;
    QList<MainCategoryPage*> categoryPages;
    QMap<int, TemplatePageList> galleryPages;
    QMap<int, TemplatePageNameList> galleryPageNames;
};

class WidgetGalleryCategoryCellCreator : public MAbstractCellCreator<MBasicListItem>
{
public:
    WidgetGalleryCategoryCellCreator() : MAbstractCellCreator<MBasicListItem>() {
    }

    MWidget *createCell(const QModelIndex &index, MWidgetRecycler &recycler) const {
        Q_UNUSED(index);

        MBasicListItem *cell = dynamic_cast<MBasicListItem *>(recycler.take(MBasicListItem::staticMetaObject.className()));
        if (cell == NULL) {
            cell = new MBasicListItem(MBasicListItem::SingleTitle);
            cell->setLayoutPosition(M::CenterPosition);
        }
        updateCell(index, cell);

        return cell;
    }

    void updateCell(const QModelIndex &index, MWidget *cell) const {
        MBasicListItem *item = qobject_cast<MBasicListItem*>(cell);
        if(!item)
            return;

        item->setTitle(index.data().toString());
    }
};

MainPage::MainPage(const QString &title)
    : TimedemoPage(),
      list(0),
      actionThemes(0),
      actionOrientation(0),
      actionToggleFPS(0),
      actionLanguage(0),
      shownPage(0),
      policy(0),
      languageSettingsPage(0),
      buttons(),
      initialPageToShow()
{
    setTitle(title);

    connect(this, SIGNAL(appeared()), this, SLOT(showInitialPage()));
}

MainPage::~MainPage()
{
}

QString MainPage::timedemoTitle()
{
    return "MainPage";
}

void MainPage::createContent()
{
    MApplicationPage::createContent();

    QGraphicsWidget *panel = centralWidget();

    MLayout *layout = new MLayout(panel);
    layout->setContentsMargins(0, 0, 0, 0);
    panel->setLayout(layout);
    policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    policy->setContentsMargins(0, 0, 0, 0);
    policy->setSpacing(0);

    populateLayout();

    actionThemes = new MAction(this);
    actionThemes->setLocation(MAction::ApplicationMenuLocation);
    this->addAction(actionThemes);
    connect(actionThemes, SIGNAL(triggered()), SLOT(showThemeSelectionDialog()));

    actionOrientation = new MAction(this);
    actionOrientation->setLocation(MAction::ApplicationMenuLocation);
    this->addAction(actionOrientation);
    connect(actionOrientation, SIGNAL(triggered()), SLOT(showOrientationSelectionDialog()));

    actionToggleFPS = new MAction(this);
    actionToggleFPS->setLocation(MAction::ApplicationMenuLocation);
    this->addAction(actionToggleFPS);
    connect(actionToggleFPS, SIGNAL(triggered()), SLOT(toggleFps()));

    actionLanguage = new MAction( this);
    actionLanguage->setLocation(MAction::ApplicationMenuLocation);
    this->addAction(actionLanguage);
    connect(actionLanguage, SIGNAL(triggered()), SLOT(showLanguageSettingsPage()));

    retranslateUi();
}

void MainPage::retranslateUi()
{
    //% "Widgets Gallery"
    setTitle(qtTrId("xx_application_title"));

    if (!isContentCreated())
        return;

    //% "Themes"
    actionThemes->setText(qtTrId("xx_mainpage_themes"));
    //% "Orientation"
    actionOrientation->setText(qtTrId("xx_mainpage_orientation"));
    //% "Toggle FPS"
    actionToggleFPS->setText(qtTrId("xx_mainpage_toggle_fps"));
    //% "Language Settings"
    actionLanguage->setText(qtTrId("xx_mainpage_language_settings"));
}

void MainPage::setInitialPageToShow(const QString& initialPageToShow)
{
    this->initialPageToShow = initialPageToShow;
}


void MainPage::populateLayout()
{
    list = new MList(centralWidget());
    list->setCellCreator(new WidgetGalleryCategoryCellCreator());
    list->setItemModel(new WidgetsGalleryDataModel());
    policy->addItem(list, Qt::AlignCenter);

    connect(list, SIGNAL(itemClicked(QModelIndex)), this, SLOT(categoryItemClicked(QModelIndex)));
}

void MainPage::handleListItemClick()
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

void MainPage::showPage(MApplicationPage *page)
{
    if (page) {
        page->appear(scene());
        shownPage = page;
    }
}

void MainPage::showLanguageSettingsPage()
{
    if (!languageSettingsPage) {
        languageSettingsPage = new LanguagePage();
        languageSettingsPage->setParent(this);
    }
    languageSettingsPage->appear(scene());
}

TemplatePage *MainPage::findPageByTimedemoTitle(const QString& title) const
{
    QAbstractItemModel *model = list->itemModel();
    for (int i = 0; i < model->rowCount(); ++i) {
        QModelIndex parent = model->index(i, 0);
        for (int j = 0; j < model->rowCount(parent); j++) {
            QModelIndex item = model->index(j, 0, parent);
            QString pageTitle = item.data(MainPage::PageTimedemoName).toString();
            if (pageTitle == title) {
                return static_cast<TemplatePage *>(item.data(MainPage::Page).value<void *>());
            }
        }
    }
    return 0;
}

QList<TemplatePage *> MainPage::demoPages()
{
    QList<TemplatePage *> pages;
    QAbstractItemModel *model = list->itemModel();
    for (int i = 0; i < model->rowCount(); ++i) {
        QModelIndex parent = model->index(i, 0);
        for (int j = 0; j < model->rowCount(parent); j++) {
            QModelIndex item = model->index(j, 0, parent);
            pages.append(static_cast<TemplatePage *>(item.data(MainPage::Page).value<void *>()));
        }
    }
    return pages;
}

void MainPage::showPageByTimedemoTitle(const QString& name)
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

    // step 3: we need to have X-MeeGoTouch-Metatheme group in index.theme

    // remove the X-DUI-Metatheme statement again when duitheme is phased out.
    if ((!themeIndexFile->childGroups().contains(QString("X-MeeGoTouch-Metatheme")))
        &&(!themeIndexFile->childGroups().contains(QString("X-DUI-Metatheme"))))
    {
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
        if (!themeIndexFile->value("X-MeeGoTouch-Metatheme/X-Visible", true).toBool()) {
            delete themeIndexFile;
            continue;
        }

        info.theme = dir.baseName();
        info.themeName = themeIndexFile->value("Desktop Entry/Name", "").toString();
        info.themeIcon = themeIndexFile->value("X-MeeGoTouch-Metatheme/X-Icon", "").toString();

        // remove this again, when duitheme is phased out
        if ( info.themeIcon.isEmpty() )
        {
            info.themeIcon = themeIndexFile->value("X-DUI-Metatheme/X-Icon", "").
                toString();
        }
        // end remove

        // ok it's a valid theme. Add it to list of themes
        themes.append(info);
        delete themeIndexFile;
    }

    return themes;
}

#ifndef HAVE_GCONF
extern void M_changeTheme(const QString &theme);
#endif

void MainPage::showThemeSelectionDialog()
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

void MainPage::showOrientationSelectionDialog()
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
            applicationWindow()->setOrientationAngle(M::Angle270);
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

void MainPage::toggleFps()
{
    MApplication::instance()->setShowFps(!MApplication::showFps());
}

void MainPage::showInitialPage()
{
    if (!initialPageToShow.isEmpty()) {
        showPageByTimedemoTitle(initialPageToShow);
    }
}

void MainPage::categoryItemClicked(const QModelIndex &index)
{
    MainCategoryPage *page = static_cast<MainCategoryPage *>(index.data(MainPage::Page).value<void *>());
    showPage(page);
}
