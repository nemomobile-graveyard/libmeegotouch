/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "spinnerpage.h"
#include "utils.h"
#include <QTimer>
#include <MBasicListItem>
#include <MLabel>
#include <MTheme>
#include <MLocale>
#include <MAction>
#include <MLinearLayoutPolicy>
#include <MGridLayoutPolicy>
#include <MFlowLayoutPolicy>
#include <MProgressIndicator>
#include <MDialog>
#include <MContainer>
#include <QGraphicsLinearLayout>
#include <QStringListModel>
#include <MImageWidget>
#include <QDir>
#include <MDebug>
#include <MTextEdit>
#include <MCompleter>
#include <MButton>

static const int ImageSize = 64;

SpinnerPage::SpinnerPage() :
    TemplatePage(TemplatePage::SimpleWidgets),
    container(0),
    spinner(0),
    spinnerLayout1(0),
    spinnerLayout2(0),
    header(0),
    description(0),
    view(Unknown),
    imageContainerPolicy(0),
    completer(0),
    actionInMainArea(0),
    actionInContainerHeader(0),
    actionInDialog(0),
    actionInCompleter(0)
{
}

SpinnerPage::~SpinnerPage()
{
}

QString SpinnerPage::timedemoTitle()
{
    return "Spinner";
}

void SpinnerPage::createContent()
{
    TemplatePage::createContent();

    actionInMainArea = new MAction(this);
    actionInMainArea->setObjectName("actionInMainArea");
    actionInMainArea->setLocation(MAction::ApplicationMenuLocation);
    connect(actionInMainArea, SIGNAL(triggered()), this, SLOT(inApplicationMainArea()));
    addAction(actionInMainArea);

    actionInContainerHeader = new MAction(this);
    actionInContainerHeader->setObjectName("actionInContainerHeader");
    actionInContainerHeader->setLocation(MAction::ApplicationMenuLocation);
    connect(actionInContainerHeader, SIGNAL(triggered()), this, SLOT(inContainerHeader()));
    addAction(actionInContainerHeader);

    actionInDialog = new MAction(this);
    actionInDialog->setObjectName("actionInDialog");
    actionInDialog->setLocation(MAction::ApplicationMenuLocation);
    connect(actionInDialog, SIGNAL(triggered()), this, SLOT(inDialog()));
    addAction(actionInDialog);

    actionInCompleter = new MAction(this);
    actionInCompleter->setObjectName("actionInCompleter");
    actionInCompleter->setLocation(MAction::ApplicationMenuLocation);
    connect(actionInCompleter, SIGNAL(triggered()), this, SLOT(inCompleter()));
    addAction(actionInCompleter);

    inApplicationMainArea();

    retranslateUi();
}

void SpinnerPage::retranslateUi()
{
    //% "Spinner"
    setTitle(qtTrId("xx_spinner_page_title"));
    if (!isContentCreated())
        return;

    //% "Spinner is used to indicate that a progress is ongoing "
    //% "and the duration is unknown.\n\n"
    //% "Spinners can also be used in situations when duration is known, "
    //% "but available space on screen does not permit using a Progress bar.\n\n"
    //% "See the menu for examples of different use cases."
    infoLabel->setText("<a></a>" + qtTrId("xx_spinner_page_info_label").replace('\n', "<br>"));

    //% "In application main area"
    actionInMainArea->setText(qtTrId("xx_spinner_page_application_main_area"));
    //% "In container header"
    actionInContainerHeader->setText(qtTrId("xx_spinner_page_container_header"));
    //% "In dialog"
    actionInDialog->setText(qtTrId("xx_spinner_page_dialog"));
    //% "In completer"
    actionInCompleter->setText(qtTrId("xx_spinner_page_completer"));
}

void SpinnerPage::inApplicationMainArea()
{
    reset();
    view = ApplicationMainArea;

    description = new MLabel(centralWidget());
    description->setObjectName("appAreaDescriptionLabel");
    description->setStyleName(inv("CommonBodyText"));
    //% "Spinner can be used while content is loading."
    description->setText("<a></a>" + qtTrId("xx_spinner_page_application_area_description"));
    description->setWordWrap(true);
    containerPolicy->addItem(description);

    spinnerLayout1 = new MLayout(0);

    MLinearLayoutPolicy* spinnerLayoutPolicy1 = new MLinearLayoutPolicy(spinnerLayout1, Qt::Vertical);
    spinnerLayout1->setPolicy(spinnerLayoutPolicy1);
    spinnerLayoutPolicy1->addStretch();

    containerPolicy->addItem(spinnerLayout1);

    spinner = new MProgressIndicator(centralWidget(), MProgressIndicator::spinnerType);
    spinner->setObjectName("spinner");
    spinner->setStyleName(inv("CommonLargeSpinner"));
    spinner->setUnknownDuration(true);
    containerPolicy->addItem(spinner, Qt::AlignCenter);

    spinnerLayout2 = new MLayout(0);

    MLinearLayoutPolicy* spinnerLayoutPolicy2 = new MLinearLayoutPolicy(spinnerLayout2, Qt::Vertical);
    spinnerLayout2->setPolicy(spinnerLayoutPolicy2);
    spinnerLayoutPolicy2->addStretch();

    containerPolicy->addItem(spinnerLayout2);
}

void SpinnerPage::inContainerHeader()
{
    reset();
    view = ContainerHeader;

    container = new MContainer(centralWidget());
    container->setObjectName("container");
    container->setStyleName(inv("CommonContainer"));
    container->setProgressIndicatorVisible(true);
    //% "Online albums"
    container->setTitle(qtTrId("xx_spinner_page_container_title"));
    containerPolicy->addItem(container);

    MLayout *layout = new MLayout(container->centralWidget());
    imageContainerPolicy = new MFlowLayoutPolicy(layout);

    description = new MLabel(centralWidget());
    description->setObjectName("containerDescriptionLabel");
    description->setStyleName(inv("CommonBodyText"));
    //% "Spinner can be used in container header to indicate that the items inside the container "
    //% "are being updated, but visible items can be interacted with."
    description->setText("<a></a>" + qtTrId("xx_spinner_page_container_header_description"));
    description->setWordWrap(true);
    containerPolicy->addItem(description);

    connect(&timer, SIGNAL(timeout()), SLOT(timeout()));
    timer.setSingleShot(true);
    timer.start(2000);
}

void SpinnerPage::timeout()
{
    Q_ASSERT(imageContainerPolicy != NULL);

    if (imageContainerPolicy) {
        QString contactsDir = Utils::contactsDir();
        QDir imagesDir(contactsDir);
        imagesDir.setNameFilters(QStringList() << "*.png");

        QStringList imageContacts = imagesDir.entryList(QDir::Files);
        if(imageContacts.isEmpty()) {
            mWarning("SpinnerPage") << "SpinnerPage - Sample image directory does not contain any .png files:" << contactsDir;
            return;
        }

        MImageWidget *image = new MImageWidget(container->centralWidget());
        image->setObjectName("image");
        image->setPixmap(QPixmap(contactsDir + QDir::separator() + imageContacts[imageContainerPolicy->count() % imageContacts.size()]));
        image->setMinimumSize(ImageSize, ImageSize);
        image->setMaximumSize(ImageSize, ImageSize);
        imageContainerPolicy->addItem(image);

        if (imageContainerPolicy->count() < 12) {
            timer.setSingleShot(true);
            timer.start(2000);
        }
    }
}

void SpinnerPage::inDialog()
{
    reset();
    view = Dialog;

    description = new MLabel(centralWidget());
    description->setObjectName("dialogDescriptionLabel");
    description->setStyleName(inv("CommonBodyText"));
    //% "Spinner can be placed in dialog header to indicate changing content."
    description->setText("<a></a>" + qtTrId("xx_spinner_page_dialog_header_description"));
    description->setWordWrap(true);
    containerPolicy->addItem(description);

    launchDialog();
}

void SpinnerPage::launchDialog()
{
    MWidget *container = new MWidget();
    container->setObjectName("container");
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setContentsMargins(0, 0, 0, 0);
    container->setLayout(layout);

    MBasicListItem *wifi = new MBasicListItem();
    wifi->setObjectName("wifiListItem");
    wifi->setStyleName("CommonSmallPanelInverted");
    wifi->setTitle("Free Wifi");
    layout->addItem(wifi);
    MBasicListItem *wlan = new MBasicListItem();
    wlan->setObjectName("wlanListItem");
    wlan->setStyleName("CommonSmallPanelInverted");
    wlan->setTitle("Public WLAN");
    layout->addItem(wlan);
    MBasicListItem *starbucks = new MBasicListItem();
    starbucks->setObjectName("starbucksListItem");
    starbucks->setStyleName("CommonSmallPanelInverted");
    starbucks->setTitle("Starbucks");
    layout->addItem(starbucks);

    //% "Select Internet connection"
    MDialog dialog(qtTrId("xx_spinner_page_dialog_header"),
                     M::NoStandardButton);
    dialog.setCentralWidget(container);
    dialog.setProgressIndicatorVisible(true);
    dialog.exec();
}

void SpinnerPage::inCompleter()
{
    reset();
    view = Completer;

    QStringList list;
    list << "Betty Brey <Betty.C.Brey@example.com>"
         << "Anne Rodriguez <Anne.E.Rodriguez@example.com>"
         << "Elizabeth Hutchings <Elizabeth.T.Hutchings@example.com>"
         << "Robert Hornbuckle <Robert.C.Hornbuckle@example.com>"
         << "Willie Logan <Willie.B.Logan@example.com>"
         << "Marcia Pineda <Marcia.J.Pineda@example.com>"
         << "Jessica Schmitt <Jessica.L.Schmitt@example.com>"
         << "Lynda Wan <Lynda.W.Wan@example.com>"
         << "Alphonso Stevens <Alphonso.I.Stevens@example.com>"
         << "Patricia Murphy <Patricia.K.Murphy@example.com>"
         << "Jeff Terry <Jeff.M.Terry@example.com>"
         << "Richard Orlando <Richard.B.Orlando@example.com>"
         << "Eva Quackenbush <Eva.D.Quackenbush@example.com>"
         << "Tim Parker <Tim.J.Parker@example.com>"
         << "Deborah Sanchez <Deborah.E.Sanchez@example.com>"
         << "Carol Reimer <Carol.J.Reimer@example.com>"
         << "Susan Amaya <Susan.H.Amaya@example.com>"
         << "Shelia Rudd <Shelia.E.Rudd@example.com>"
         << "Elizabeth Livingston <Elizabeth.B.Livingston@example.com>"
         << "Renee Heard <Renee.N.Heard@example.com>"
         << "Megan Gagne <Megan.R.Gagne@example.com>"
         << "Katherine Enos <Katherine.B.Enos@example.com>"
         << "Gary Hawkins <Gary.J.Hawkins@example.com>"
         << "Bianca Sparks <Bianca.M.Sparks@example.com>"
         << "Cleta Richardson <Cleta.E.Richardson@example.com>"
         << "Latoya Lawrence <Latoya.E.Lawrence@example.com>"
         << "Ethel Allen <Ethel.S.Allen@example.com>"
         << "Maria Austin <Maria.D.Austin@example.com>"
         << "John Smith <john.smith@example.com>"
         << "Jane Smith <jane.smith@example.com>"
         << "Doug Barker <Doug.J.Barker@example.com>"
         << "Sandra Cross <Sandra.J.Cross@example.com>"
         << "Debra Roberts <Debra.D.Roberts@example.com>"
         << "Mei Copeland <Mei.D.Copeland@example.com>"
         << "Raymond Slack <Raymond.A.Slack@example.com>"
         << "Martin Vidal <Martin.M.Vidal@example.com>"
         << "Patricia Rymer <Patricia.R.Rymer@example.com>"
         << "Maria Gilbreath <Maria.G.Gilbreath@example.com>"
         << "Mary Ramos <Mary.C.Ramos@example.com>"
         << "Michael Haller <Michael.K.Haller@example.com>"
         << "Randall Hood <Randall.J.Hood@example.com>"
         << "Bruce Lindsey <Bruce.D.Lindsey@example.com>"
         << "Heidi Martin <Heidi.S.Martin@example.com>"
         << "Helen Kennedy <Helen.E.Kennedy@example.com>"
         << "Margaret Worsham <Margaret.R.Worsham@example.com>"
         << "Sun Singleton <Sun.R.Singleton@example.com>"
         << "Alberto Prince <Alberto.A.Prince@example.com>"
         << "Norman Weiland <Norman.B.Weiland@example.com>"
         << "Carolyn Delia <Carolyn.G.Delia@example.com>"
         << "Lakesha Acosta <Lakesha.G.Acosta@example.com>";

    QStringList names, addresses;
    foreach(const QString & s, list) {
        QStringList split = s.split('<');
        if (split.count() >= 2) {
            names << split[0].trimmed();
            addresses << QString('<' + split[1]);
        }
    }

    IncrementalCompletionModel *model = new IncrementalCompletionModel(names, addresses);
    completer = new MCompleter(model);
    completer->setCharactersToTrim(QString("<>"));
    completer->setCharactersToTrimForCompletionPrefix(QString(" "));
    completer->setAcceptMultipleEntries(true);

    //% "Fetching contacts..."
    completer->setFetchInProgressLabel(qtTrId("xx_spinner_page_completer_fetching_label"));

    MTextEdit *edit = new MTextEdit;
    edit->setCompleter(completer);

    MButton *restartButton = new MButton("clear & start populating");
    connect(restartButton, SIGNAL(clicked()), model, SLOT(startPopulating()));

    //% "Completer's model is incrementally populated. During this time spinner is shown."
    MLabel *completerDescription = new MLabel(qtTrId("xx_spinner_page_completer_description"));
    completerDescription->setWordWrap(true);

    QGraphicsWidget *centralWidget = new QGraphicsWidget;
    QGraphicsLinearLayout *verticalLayout = new QGraphicsLinearLayout(Qt::Vertical, centralWidget);
    verticalLayout->addItem(edit);
    verticalLayout->addItem(restartButton);
    verticalLayout->addItem(completerDescription);

    MContainer *container = new MContainer(//% "Completer with spinner"
                                           qtTrId("xx_spinner_page_completer_header"));
    container->setCentralWidget(centralWidget);
    containerPolicy->addItem(container);

    // Focus in and start completing immediately.
    edit->setFocus();
    edit->setText("r");
    model->startPopulating();
    completer->complete();
}

void SpinnerPage::reset()
{
    switch (view) {
    case Unknown:
        break;

    case ApplicationMainArea: {
        containerPolicy->removeItem(header);
        containerPolicy->removeItem(spinnerLayout1);
        containerPolicy->removeItem(spinner);
        containerPolicy->removeItem(spinnerLayout2);
        delete description;
        delete spinnerLayout1;
        delete spinner;
        delete spinnerLayout2;
        description = NULL;
        spinnerLayout1 = NULL;
        spinner = NULL;
        spinnerLayout2 = NULL;
    } break;

    case ContainerHeader: {
        disconnect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
        containerPolicy->removeItem(container);
        containerPolicy->removeItem(description);
        delete container;
        delete description;
        container = NULL;
        description = NULL;
    } break;

    case Dialog: {
        containerPolicy->removeItem(description);
        delete description;
        description = NULL;
    } break;

    case Completer: {
        delete containerPolicy->itemAt(containerPolicy->count() - 1);
        delete completer;
        completer = 0;
    }
    }

    imageContainerPolicy = NULL;
    view = Unknown;
}


IncrementalCompletionModel::IncrementalCompletionModel(const QStringList &names,
                                                       const QStringList &addresses,
                                                       QObject *parent)
    : QAbstractTableModel(parent),
      column1(names),
      column2(addresses),
      populateCounter(0)
{
    populateTimer.setInterval(300);
    populateTimer.setSingleShot(false);
    connect(&populateTimer, SIGNAL(timeout()), this, SLOT(addOne()));
}

IncrementalCompletionModel::~IncrementalCompletionModel()
{
}

void IncrementalCompletionModel::startPopulating()
{
    populateCounter = 0;
    populateTimer.start();
}

void IncrementalCompletionModel::stopPopulating()
{
    populateTimer.stop();
}

void IncrementalCompletionModel::addOne()
{
    if (populateCounter == column1.count()) {
        populateTimer.stop();
        return;
    }

    beginInsertRows(QModelIndex(),
                    populateCounter + 1,
                    populateCounter + 1);
    ++populateCounter;
    endInsertRows();
}

void IncrementalCompletionModel::setData(const QStringList &names, const QStringList &addresses)
{
    beginResetModel();
    populateCounter = 0;
    column1 = names;
    column2 = addresses;
    endResetModel();
}

QModelIndex IncrementalCompletionModel::index(int row, int column, const QModelIndex &parent) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column, 0) : QModelIndex();
}

QModelIndex IncrementalCompletionModel::parent(const QModelIndex & /* index */) const
{
    return QModelIndex();
}

int IncrementalCompletionModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

int IncrementalCompletionModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return populateCounter;
}

QVariant IncrementalCompletionModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role);
    int row = index.row();
    int column = index.column();
    if (role != Qt::DisplayRole)
        return QVariant();
    QString value;
    if (row < rowCount() && row >= 0 && column < 2 && column >= 0) {
        if (column == 0 && row < column1.count())
            value = column1[row];
        else if (column == 1 && row < column2.count())
            value = column2[row];
    }
    return QVariant(value);
}

bool IncrementalCompletionModel::canFetchMore(const QModelIndex &parent) const
{
    return !parent.isValid() && populateTimer.isActive();
}
