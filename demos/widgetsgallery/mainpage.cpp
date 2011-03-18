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
#include "textentryattributeextensionpage.h"
#include "mlistpage.h"
#include "mgridpage.h"
#include "sliderpage.h"
#include "comboboxpage.h"
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
#include "sheetspage.h"
#include "bannerspage.h"
#include "notificationspage.h"
#include "bubblepage.h"
#include "drilldownlistitem.h"
#include "customnavigationbarpage.h"
#include "debugpage.h"
#include "screenshotpage.h"

#include "../../benchmarks/performancebenchmark/emptymainloophelper.h"

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
#include <MLocale>
#include <MLayout>
#include <MButton>
#include <MLabel>
#include <MAction>
#include <MWidgetAction>
#include <MDialog>
#include <MButtonGroup>
#include <MTheme>
#include <MGridLayoutPolicy>
#include <MLinearLayoutPolicy>
#include <MApplicationWindow>
#include <MComboBox>

#ifdef HAVE_GCONF
#include <MGConfItem>
#endif

class WidgetsGalleryDataModel : public QAbstractItemModel {
public:
    typedef QList<TemplatePage*> TemplatePageList;
    typedef QList<QString> TemplatePageNameList;
    typedef QList<int> TemplatePageTypeList;

    enum PageType {
        MainCategoryPageType,
        ApplicationMenuPageType,
        DisplayModesPageType,
        ToolBarPageType,
        CustomNavigationBarPageType,
        LabelPageType,
        ImagePageType,
        ProgressBarPageType,
        SpinnerPageType,
        PushButtonPageType,
        IconButtonPageType,
        SwitchPageType,
        CheckboxPageType,
        ButtonGroupPageType,
        SingleSelectionDialogsPageType,
        QueryDialogsPageType,
        SheetsPageType,
        BannersPageType,
        NotificationsPageType,
        MListPageType,
        MGridPageType,
        ObjectMenuPageType,
        ComboBoxPageType,
        ContentItemsPageType,
        BubblePageType,
        TextEntryPageType,
        TextEntryNavigationPageType,
        TextEntryAttributeExtensionPageType,
        SliderPageType,
        FeedbackPageType,
        DebugPageType,
        ScreenshotPageType,
    };

public:
    WidgetsGalleryDataModel()
        : QAbstractItemModel(),
          categoryPageNames(),
          categoryPages(),
          galleryPages(),
          galleryPageNames()
    {
        //% "Application View"
        addCategory(qtTrId("xx_wg_categories_application_view"));
        //% "Application Menu"
        addGalleryPage(0, qtTrId("xx_wg_application_view_view_menu"), WidgetsGalleryDataModel::ApplicationMenuPageType);
        //% "Display Modes"
        addGalleryPage(0, qtTrId("xx_wg_application_view_display_modes"), WidgetsGalleryDataModel::DisplayModesPageType);
        //% "Toolbar (including Tab Bar)"
        addGalleryPage(0, qtTrId("xx_wg_application_view_toolbar_incl_tab_bar"), WidgetsGalleryDataModel::ToolBarPageType);
        //% "Custom Navigation Bar"
        addGalleryPage(0, qtTrId("xx_wg_application_view_custom_navigation_bar"), WidgetsGalleryDataModel::CustomNavigationBarPageType);

        //% "Simple Widgets"
        addCategory(qtTrId("xx_wg_categories_simple_widgets"));
        //% "Labels"
        addGalleryPage(1, qtTrId("xx_wg_simple_widgets_labels"), WidgetsGalleryDataModel::LabelPageType);
        //% "Image"
        addGalleryPage(1, qtTrId("xx_wg_simple_widgets_image"), WidgetsGalleryDataModel::ImagePageType);
        //% "Progress Bar"
        addGalleryPage(1, qtTrId("xx_wg_simple_widgets_progress_bar"), WidgetsGalleryDataModel::ProgressBarPageType);
        //% "Spinner"
        addGalleryPage(1, qtTrId("xx_wg_simple_widgets_spinner"), WidgetsGalleryDataModel::SpinnerPageType);

        //% "Buttons"
        addCategory(qtTrId("xx_wg_categories_buttons"));
        //% "Push Button"
        addGalleryPage(2, qtTrId("xx_wg_buttons_push_button"), WidgetsGalleryDataModel::PushButtonPageType);
        //% "Icon Button"
        addGalleryPage(2, qtTrId("xx_wg_buttons_icon_button"), WidgetsGalleryDataModel::IconButtonPageType);
        //% "Switch"
        addGalleryPage(2, qtTrId("xx_wg_buttons_switch"), WidgetsGalleryDataModel::SwitchPageType);
        //% "Checkbox"
        addGalleryPage(2, qtTrId("xx_wg_buttons_checkbox"), WidgetsGalleryDataModel::CheckboxPageType);
        //% "Button Group"
        addGalleryPage(2, qtTrId("xx_wg_buttons_buttongroup"), WidgetsGalleryDataModel::ButtonGroupPageType);

        //% "Dialogs, Sheets and Banners"
        addCategory(qtTrId("xx_wg_categories_dialogs_and_banners"));
        //% "Single Selection Dialog"
        addGalleryPage(3, qtTrId("xx_wg_dialogs_and_banners_single_selection_dialog"), WidgetsGalleryDataModel::SingleSelectionDialogsPageType);
        //% "Query Dialog"
        addGalleryPage(3, qtTrId("xx_wg_dialogs_and_banners_query_dialog"), WidgetsGalleryDataModel::QueryDialogsPageType);
        //% "Sheet"
        addGalleryPage(3, qtTrId("xx_wg_dialogs_and_banners_sheet"), WidgetsGalleryDataModel::SheetsPageType);
        //% "Banner"
        addGalleryPage(3, qtTrId("xx_wg_dialogs_and_banners_banner"), WidgetsGalleryDataModel::BannersPageType);
        //% "Notifications"
        addGalleryPage(3, qtTrId("xx_wg_dialogs_and_banners_notifications"), WidgetsGalleryDataModel::NotificationsPageType);

        //% "Lists, Grids and Popups"
        addCategory(qtTrId("xx_wg_categories_lists_grids_and_popups"));
        //% "List"
        addGalleryPage(4, qtTrId("xx_wg_lists_grids_and_popups_list"), WidgetsGalleryDataModel::MListPageType);
        //% "Grid"
        addGalleryPage(4, qtTrId("xx_wg_lists_grids_and_popups_grid"), WidgetsGalleryDataModel::MGridPageType);
        //% "Object Menu"
        addGalleryPage(4, qtTrId("xx_wg_lists_grids_and_popups_object_menu"), WidgetsGalleryDataModel::ObjectMenuPageType);
        //% "ComboBox"
        addGalleryPage(4, qtTrId("xx_wg_lists_grids_and_popups_combobox"), WidgetsGalleryDataModel::ComboBoxPageType);
        //% "ContentItems"
        addGalleryPage(4, qtTrId("xx_wg_lists_grids_and_popups_contentitems"), WidgetsGalleryDataModel::ContentItemsPageType);
        //% "Speech bubble"
        addGalleryPage(4, qtTrId("xx_wg_lists_grids_and_popups_bubble"), WidgetsGalleryDataModel::BubblePageType);

        //% "User Input"
        addCategory(qtTrId("xx_wg_categories_user_input"));
        //% "Text Entries"
        addGalleryPage(5, qtTrId("xx_wg_user_input_text_entries"), WidgetsGalleryDataModel::TextEntryPageType);
        //% "Text Entries Navigation"
        addGalleryPage(5, qtTrId("xx_wg_user_input_text_entries_navigation"), WidgetsGalleryDataModel::TextEntryNavigationPageType);
        //% "Text Entries Attribute Extension"
        addGalleryPage(5, qtTrId("xx_wg_user_input_text_entries_attribute_extension"), WidgetsGalleryDataModel::TextEntryAttributeExtensionPageType);
        //% "Slider"
        addGalleryPage(5, qtTrId("xx_wg_user_input_slider"), WidgetsGalleryDataModel::SliderPageType);
        //% "Input Feedback"
        addGalleryPage(5, qtTrId("xx_wg_user_input_input_feedback"), WidgetsGalleryDataModel::FeedbackPageType);

        //TODO  it will be removed , only for development
        addCategory("Debug tools");

        //% "Information display"
        addGalleryPage(6, qtTrId("xx_wg_information_display"), WidgetsGalleryDataModel::DebugPageType);

        //% "Take a screenshot"
        addGalleryPage(6, qtTrId("xx_wg_take_screenshot"), WidgetsGalleryDataModel::ScreenshotPageType);
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
                return QVariant::fromValue((void *)const_cast<WidgetsGalleryDataModel*>(this)->getPage(index));
            } else if (role == MainPage::PageTimedemoName) {
                return getTimedemoName(index);
            } else if (role == MainPage::PageType) {
                return getPageType(index);
            }
        }

        return QVariant();
    }

private:
    void addCategory(const QString &name) {
        createIndex(categoryPages.count(), 0);

        categoryPageNames.append(name);
        categoryPages.append(NULL);
    }

    void addGalleryPage(int category, const QString &name, PageType pageType) {
        galleryPageNames[category].append(name);
        galleryPages[category].append(NULL);
        galleryPageTypes[category].append((int)pageType);
    }

    MApplicationPage *getPage(const QModelIndex &index) {
        if (index.parent().isValid()) {
            if (!galleryPages[index.parent().row()].at(index.row())) {
                MApplicationPage *page = NULL;
                switch (getPageType(index)) {
                //category 0
                case ApplicationMenuPageType:
                    page = new ApplicationMenuPage();
                    break;
                case DisplayModesPageType:
                    page = new DisplayModesPage();
                    break;
                case ToolBarPageType:
                    page = new ToolBarPage();
                    break;
                case CustomNavigationBarPageType:
                    page = new CustomNavigationBarPage();
                    break;
                //category 1
                case LabelPageType:
                    page = new LabelPage();
                    break;
                case ImagePageType:
                    page = new ImagePage();
                    break;
                case ProgressBarPageType:
                    page = new ProgressBarPage();
                    break;
                case SpinnerPageType:
                    page = new SpinnerPage();
                    break;
                //category 2
                case PushButtonPageType:
                    page = new PushButtonPage();
                    break;
                case IconButtonPageType:
                    page = new IconButtonPage();
                    break;
                case SwitchPageType:
                    page = new SwitchPage();
                    break;
                case CheckboxPageType:
                    page = new CheckboxPage();
                    break;
                case ButtonGroupPageType:
                    page = new ButtonGroupPage();
                    break;
                //category 3
                case SingleSelectionDialogsPageType:
                    page = new SingleSelectionDialogsPage();
                    break;
                case QueryDialogsPageType:
                    page = new QueryDialogsPage();
                    break;
                case SheetsPageType:
                    page = new SheetsPage();
                    break;
                case BannersPageType:
                    page = new BannersPage();
                    break;
                case NotificationsPageType:
                    page = new NotificationsPage();
                    break;
                //category 4
                case MListPageType:
                    page = new MListPage();
                    break;
                case MGridPageType:
                    page = new MGridPage();
                    break;
                case ObjectMenuPageType:
                    page = new ObjectMenuPage();
                    break;
                case ComboBoxPageType:
                    page = new ComboBoxPage();
                    break;
                case ContentItemsPageType:
                    page = new ContentItemsPage();
                    break;
                case BubblePageType:
                    page = new BubblePage();
                    break;
                //category 5
                case TextEntryPageType:
                    page = new TextEntryPage();
                    break;
                case TextEntryNavigationPageType:
                    page = new TextEntryNavigationPage();
                    break;
                case TextEntryAttributeExtensionPageType:
                    page = new TextEntryAttributeExtensionPage();
                    break;
                case SliderPageType:
                    page = new SliderPage();
                    break;
                case FeedbackPageType:
                    page = new FeedbackPage();
                    break;
                //category 6
                case DebugPageType:
                    page = new DebugPage();
                    break;
                case ScreenshotPageType:
                    page = new ScreenShotPage();
                    break;
                default:
                    break;
                }

                galleryPages[index.parent().row()].replace(index.row(), (TemplatePage*)page);
            }

            return galleryPages[index.parent().row()].at(index.row());

        } else {
            if (!categoryPages[index.row()])
                categoryPages[index.row()] = new MainCategoryPage(this, index);
            return categoryPages[index.row()];
        }

        return NULL;
    }

    const QString &getName(const QModelIndex &index) const {
        if (index.parent().isValid())
            return galleryPageNames[index.parent().row()].at(index.row());
        else
            return categoryPageNames[index.row()];
    }

    QString getTimedemoName(const QModelIndex &index) const {
        if (index.parent().isValid())
            return ((TimedemoPage*)const_cast<WidgetsGalleryDataModel*>(this)->getPage(index))->timedemoTitle();

        return QString();
    }

    int getPageType(const QModelIndex &index) const {
        if (index.parent().isValid())
            return galleryPageTypes[index.parent().row()].at(index.row());

        return -1;
    }

private:
    QList<QString> categoryPageNames;
    QList<MainCategoryPage*> categoryPages;
    QHash<int, TemplatePageList> galleryPages;
    QHash<int, TemplatePageNameList> galleryPageNames;
    QHash<int, TemplatePageTypeList> galleryPageTypes;
};

class WidgetGalleryCategoryCellCreator : public MAbstractCellCreator<DrillDownListItem>
{
public:
    WidgetGalleryCategoryCellCreator(MList *list)
        : MAbstractCellCreator<DrillDownListItem>(),
        list(list)
    {
    }

    MWidget *createCell(const QModelIndex &index, MWidgetRecycler &recycler) const {
        Q_UNUSED(index);

        DrillDownListItem *cell = dynamic_cast<DrillDownListItem *>(recycler.take(DrillDownListItem::staticMetaObject.className()));
        if (cell == NULL) {
            cell = new DrillDownListItem(list);
            cell->setLayoutPosition(M::CenterPosition);
        }
        updateCell(index, cell);

        return cell;
    }

    void updateCell(const QModelIndex &index, MWidget *cell) const {
        DrillDownListItem *item = qobject_cast<DrillDownListItem*>(cell);
        if(!item)
            return;

        item->setTitle(index.data().toString());
    }

private:
    MList *list;
};

MainPage::MainPage(const QString &title)
    : TimedemoPage(),
      list(0),
      actionThemes(0),
      actionToggleFPS(0),
      actionLanguage(0),
      comboOrientation(0),
      shownPage(0),
      policy(0),
      languageSettingsPage(0),
      buttons(),
      initialPageToShow(),
      createEmpty(false)
{
    if (qApp->arguments().indexOf("-exitimmediately") >= 0)
        createEmpty = true;
    else {
        setTitle(title);
        connect(this, SIGNAL(appeared()), this, SLOT(showInitialPage()));
    }
    mainLoopHelper = 0;
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
    if (createEmpty)
        return;

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

    QStringList items;
    items << //% "Automatic"
        qtTrId("xx_apporientation_auto") << //% "Portrait"
        qtTrId("xx_apporientation_portrait") << //% "Landscape"
        qtTrId("xx_apporientation_landscape") << //% "0 degrees"
        qtTrId("xx_apporientation_angle0") << //% "90 degrees clockwise"
        qtTrId("xx_apporientation_angle90") << //% "180 degrees clockwise"
        qtTrId("xx_apporientation_angle180") << //% "270 degrees clockwise"
        qtTrId("xx_apporientation_angle270");
    comboOrientation = createComboBoxAction(NULL, items);
    connect(comboOrientation, SIGNAL(currentIndexChanged(int)), SLOT(changeOrientation(int)));

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
    if (createEmpty)
        return;

    //% "Widgets Gallery"
    setTitle(qtTrId("xx_application_title"));

    if (!isContentCreated())
        return;

    //% "Themes"
    actionThemes->setText(qtTrId("xx_mainpage_themes"));
    //% "Orientation"
    comboOrientation->setTitle(qtTrId("xx_mainpage_orientation"));
    //% "Toggle FPS"
    actionToggleFPS->setText(qtTrId("xx_mainpage_toggle_fps"));
    //% "Language Settings"
    actionLanguage->setText(qtTrId("xx_mainpage_language_settings"));
}

void MainPage::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    TimedemoPage::paint(painter, option, widget);
    if (mainLoopHelper)
        mainLoopHelper->quit();
}

void MainPage::setInitialPageToShow(const QString& initialPageToShow)
{
    this->initialPageToShow = initialPageToShow;
}

void MainPage::setMainLoopHelper(EmptyMainLoopHelper *helper)
{
    mainLoopHelper = helper;
}

void MainPage::populateLayout()
{
    list = new MList(centralWidget());
    list->setCellCreator(new WidgetGalleryCategoryCellCreator(list));
    list->setItemModel(new WidgetsGalleryDataModel());
    policy->addItem(list, Qt::AlignCenter);

    connect(list, SIGNAL(itemClicked(QModelIndex)), this, SLOT(categoryItemClicked(QModelIndex)));
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

MComboBox* MainPage::createComboBoxAction(const QString &title, const QStringList &itemsList)
{
    MWidgetAction *widgetAction = new MWidgetAction(centralWidget());
    widgetAction->setLocation(MAction::ApplicationMenuLocation);
    MComboBox *comboBox = new MComboBox;
    comboBox->setTitle(title);
    comboBox->setIconVisible(false);
    comboBox->addItems(itemsList);
    comboBox->setCurrentIndex(0);
    widgetAction->setWidget(comboBox);
    addAction(widgetAction);

    return comboBox;
}

void MainPage::changeOrientation(int index)
{
    switch(index) {
    case Automatic:
        applicationWindow()->setOrientationAngleLocked(false);
        applicationWindow()->setOrientationLocked(false);
        break;
    case Portrait:
        applicationWindow()->setOrientationAngleLocked(false);
        applicationWindow()->setOrientationAngle(M::Angle270);
        applicationWindow()->setOrientationLocked(true);
        break;
    case Landscape:
        applicationWindow()->setOrientationAngleLocked(false);
        applicationWindow()->setOrientationAngle(M::Angle0);
        applicationWindow()->setOrientationLocked(true);
        break;
    case Angle0:
        applicationWindow()->setOrientationAngleLocked(false);
        applicationWindow()->setOrientationAngle(M::Angle0);
        applicationWindow()->setOrientationLocked(true);
        break;
    case Angle90:
        applicationWindow()->setOrientationAngleLocked(false);
        applicationWindow()->setOrientationAngle(M::Angle90);
        applicationWindow()->setOrientationLocked(true);
        break;
    case Angle180:
        applicationWindow()->setOrientationAngleLocked(false);
        applicationWindow()->setOrientationAngle(M::Angle180);
        applicationWindow()->setOrientationLocked(true);
        break;
    case Angle270:
        applicationWindow()->setOrientationAngleLocked(false);
        applicationWindow()->setOrientationAngle(M::Angle270);
        applicationWindow()->setOrientationLocked(true);
        break;
    }
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
