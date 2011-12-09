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

#include "clipboardpage.h"

#include <MApplication>
#include <MBanner>
#include <MButton>
#include <MLabel>
#include <MLinearLayoutPolicy>
#include <MList>

#include <QClipboard>
#include <QGraphicsLinearLayout>
#include <QGraphicsGridLayout>

namespace {
    static const QString lipsum("Nam quis nulla vitae erat consectetur fermentum. Aenean ullamcorper quam in ante mollis scelerisque. Duis vitae magna odio, in rhoncus nibh. "
                                "Integer feugiat tellus tempus nibh lacinia aliquam. In tincidunt pulvinar sem. Suspendisse vestibulum rhoncus arcu quis hendrerit. "
                                "Fusce at imperdiet est. Vestibulum ornare sapien eget augue posuere eu ornare enim eleifend. Pellentesque ornare nisi vel dui eleifend dictum. "
                                "Etiam condimentum dignissim mauris, non convallis tellus fermentum vel.");

    static const QString lipsumHe = QString::fromUtf8("יש המון גרסאות זמינות לפסקאות של. אבל רובם עברו שינויים בצורה זו או אחרת "
                                                      "על ידי השתלת הומור או מילים אקראיות שלא נראות אפילו מעט אמינות. "
                                                      "אם אתה הולך להשתמש במקטעים של של אתה צריך להיות בטוח שאין משהו מביך חבוי בתוך הטקסט. "
                                                      "כל מחוללי הטקסט של שנמצאים ברשת האינטרנט מכוונים לחזור על טקסטים מוגדרים מראש לפי הנדרש. "
                                                      "וזה הופך אותנו למחוללי טקסט אמיתיים ראשונים באינטרנט. "
                                                      "אנו משתמשים במילון עם מעל 200 ערכים בלטינית משולבים במבני משפטים על מנת לשוות לטקט מראה הגיוני. "
                                                      "ולכן הטקסט של לעולם לא יכיל טקסטים חוזרים, הומור, או מילים לא מאופייניות וכדומה");

    static const QString longLipsum("Lorem ipsum dolor sit amet, consectetur adipiscing elit. In feugiat felis in mi posuere vitae tincidunt sem interdum. Etiam eu nunc enim. Donec sed tortor augue. "
                                    "Quisque dui ante, dapibus at vehicula eu, volutpat at quam. Mauris non lacus urna, vitae molestie mi. Aenean condimentum dapibus ante vitae lacinia. Nunc ac convallis "
                                    "velit. Mauris vel erat lectus, id vehicula turpis. Praesent tincidunt congue dolor, sed tempor enim sagittis tristique. Praesent sagittis viverra magna, vitae iaculis "
                                    "lorem accumsan vel. Suspendisse potenti. Vivamus pulvinar est eget sem egestas molestie. Nunc sit amet urna velit. Mauris id nunc vel ante ultrices ullamcorper."
                                    "\n"
                                    "Vivamus eget tortor sed dolor dapibus suscipit. Vivamus neque elit, vulputate quis dapibus a, gravida et neque. Praesent ornare tortor nec erat ultrices iaculis. Donec "
                                    "pharetra neque nibh, ut volutpat augue. Donec velit mi, egestas vitae tincidunt at, consectetur accumsan nunc. Phasellus ornare ullamcorper luctus. Morbi sit amet arcu "
                                    "ac urna interdum dapibus. In feugiat lacus sit amet nisi posuere nec blandit nibh pretium. Nulla a neque nisi. Etiam sem nisi, accumsan dignissim placerat vel, ornare "
                                    "nec erat. Duis eu venenatis massa. Suspendisse sollicitudin tincidunt facilisis. "
                                    "\n"
                                    "Mauris tempus risus vitae elit semper et semper magna pharetra. Ut vel velit erat. Nulla magna purus, laoreet nec aliquam eget, fermentum sit amet enim. Sed tristique "
                                    "aliquam felis. Vivamus interdum pulvinar eleifend. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur mattis purus eget eros tincidunt et ultricies nunc "
                                    "fermentum. Maecenas consectetur, lorem ac lobortis imperdiet, tellus metus facilisis est, vitae molestie est urna sed orci. Integer bibendum blandit ipsum a congue. "
                                    "Etiam tellus sem, volutpat eget venenatis eget, tincidunt ut eros. Etiam mauris dolor, consectetur eget rhoncus et, iaculis in urna. Fusce scelerisque elit eget lorem "
                                    "accumsan sed scelerisque felis feugiat."
                                    "\n"
                                    "Morbi mauris purus, tempor sed convallis eu, aliquam at tortor. Duis pellentesque est et nulla luctus molestie. Vivamus eu nulla eu ipsum pretium blandit eu ac erat. "
                                    "Sed dignissim, felis id mattis scelerisque, arcu lorem gravida mauris, eu fermentum justo libero at lorem. Proin et purus lorem. Nunc at odio in velit congue rutrum. "
                                    "Phasellus tincidunt commodo leo sit amet pulvinar. Quisque arcu odio, lobortis at pretium ut, facilisis quis neque. In porttitor sollicitudin nibh eu dictum. Ut iaculis "
                                    "nisi id enim luctus blandit. Aenean luctus gravida aliquam. Aenean quis imperdiet mi. Nunc iaculis mauris vitae erat lacinia bibendum adipiscing tellus imperdiet."
                                    "\n"
                                    "Donec consequat, arcu in ornare facilisis, metus nunc sagittis magna, a feugiat velit mauris non sapien. Suspendisse porta mauris quis justo tincidunt mollis. Praesent "
                                    "ligula nibh, fermentum non fermentum eget, faucibus non erat. Phasellus eu augue eu turpis pulvinar rhoncus. Donec diam justo, sollicitudin in vehicula et, dignissim "
                                    "quis erat. Duis luctus mattis tristique. Proin orci risus, venenatis ac porta quis, convallis ullamcorper sapien. Praesent non sem ipsum, a porta enim. Phasellus "
                                    "vulputate posuere lobortis. Proin vulputate tincidunt pharetra. Aliquam tristique magna id ipsum imperdiet ac aliquam risus scelerisque. Duis adipiscing fermentum "
                                    "leo non mollis. Nullam lobortis blandit dictum. Nunc lacinia, dolor vitae bibendum pharetra, nibh ante condimentum magna, at tempor massa nunc ut lacus. Nam vitae "
                                    "urna eros, ac congue erat. Aenean congue dictum ante, sed aliquam elit cursus vel."
                                    "\n"
                                    "Pellentesque ornare elit a elit tincidunt pulvinar. Quisque nec magna felis. Duis ac lectus a magna tristique tincidunt. Donec ac neque at nulla elementum imperdiet "
                                    "vitae sit amet velit. Nullam fermentum felis id dui hendrerit ullamcorper. Donec ac magna mauris. Pellentesque pulvinar feugiat venenatis. In consequat, leo eu pharetra "
                                    "molestie, erat quam semper lectus, vitae varius urna ipsum sed metus. Donec sagittis, arcu in feugiat cursus, metus augue ornare orci, et aliquam nunc tortor eget nunc. "
                                    "Proin posuere euismod odio quis bibendum. Duis id ornare sapien. Morbi sollicitudin est vel dolor posuere aliquet. Nulla vulputate dolor vitae metus molestie dapibus "
                                    "ullamcorper eget diam. Suspendisse molestie, risus ut sollicitudin consectetur, diam neque cursus nisi, nec blandit erat dui quis orci.");

    static const QString numberArabic(QString("0123456789").repeated(5));
    static const QString numberArabicIndic(QString::fromUtf8("٠١٢٣٤٥٦٧٨٩").repeated(5));
}


ClipboardPage::ClipboardPage()
    : TemplatePage(TemplatePage::ApplicationView)
{
}

ClipboardPage::~ClipboardPage()
{
}

void ClipboardPage::createContent()
{
    TemplatePage::createContent();

    listModel = new ClipboardPageListModel(this); // this as QObject parent so model gets destroyed
    MList *list = createList(listModel, new CopyListItemCreator);
    list->setIndexDisplayMode(MList::Floating);
    containerPolicy->addItem(list);

    retranslateUi();
}

QString ClipboardPage::timedemoTitle()
{
    return "Clipboard";
}

void ClipboardPage::retranslateUi()
{
    //% "<p>This <b>clipboard page</b> contains text that can be used for testing.</p>"
    //% "<p>Click the appropriate item to copy its contents to clipboard.</p>"
    //% "<p>Switch button next to content will split the text into multiple lines using line breaks.</p>"
    infoLabel->setText(qtTrId("xx_clipboard_page_info"));

    populateListModel();
}

MList *ClipboardPage::createList(QAbstractItemModel *model , MCellCreator *creator)
{
    MList *list = new MList;
    list->setIndexDisplayMode(MList::Show);
    list->setObjectName("list");
    list->setStyleName(inv("CommonList"));
    list->setCellCreator(creator);
    list->setItemModel(model);
    list->setShowGroups(true); // -> model is two-level tree. first level is headers, second their items
    return list;
}

void ClipboardPage::populateListModel()
{
    listModel->clearCopyItems();

    listModel->addCopyItems(
        //% "long text"
        qtTrId("xx_clipboard_copy_header_long"),
        (CopyItemDataList()
         //% "Paragraph"
         << CopyItemData(qtTrId("xx_clipboard_copy_item_lipsum"), lipsum)
         //% "Paragraph, Hebrew"
         << CopyItemData(qtTrId("xx_clipboard_copy_item_lipsumrtl"), lipsumHe)
         //% "Many paragraphs"
         << CopyItemData(qtTrId("xx_clipboard_copy_item_paragraphs"), longLipsum)
         //% "Without spaces"
         << CopyItemData(qtTrId("xx_clipboard_copy_item_lipsum_no_spaces"), QString(lipsum).replace(" ", ""))));

    listModel->addCopyItems(
        //% "Numbers"
        qtTrId("xx_clipboard_copy_header_numbers"),
        (CopyItemDataList()
         //% "long Arabic number"
         << CopyItemData(qtTrId("xx_clipboard_copy_item_long_number_arabic"), numberArabic)
         //% "long Arabic-Indic number"
         << CopyItemData(qtTrId("xx_clipboard_copy_item_long_number_arabic_indic"), numberArabicIndic)));

    listModel->addCopyItems(
        //% "special"
        qtTrId("xx_clipboard_copy_header_special"),
        (CopyItemDataList()
         //% "LTR marker (LRE)"
         << CopyItemData(qtTrId("xx_clipboard_copy_item_lre"), QString::fromUtf8("\u202A"))
         //% "RTL marker (RLE)"
         << CopyItemData(qtTrId("xx_clipboard_copy_item_rle"), QString::fromUtf8("\u202B"))
         //% "Pop format (PDF)"
         << CopyItemData(qtTrId("xx_clipboard_copy_item_pdf"), QString::fromUtf8("\u202C"))));
}


CopyListItem::CopyListItem(QGraphicsItem *parent)
    : MListItem(parent),
      lineBreakSwitch(0),
      headerIndex(-1),
      itemIndex(-1)
{
    setObjectName("CommonLargePanel");

    connect(this, SIGNAL(clicked()),
            this, SLOT(handleClicked()));

    // Layout
    layoutGrid = new QGraphicsGridLayout(this);

    // Title
    titleLabel = new MLabel;
    titleLabel->setTextElide(true);
    titleLabel->setObjectName("CommonTitle");

    // Copy content
    subtitleLabel = new MLabel;
    subtitleLabel->setTextElide(true);
    subtitleLabel->setWordWrap(true);
    subtitleLabel->setPreferredLineCount(3);
    subtitleLabel->setObjectName("CommonSubTitle");

    MLabel *iconLabel = new MLabel;
    iconLabel->setText(QString::fromUtf8("✍"));
    iconLabel->setObjectName("CommonTitle");

    // Construct layout
    layoutGrid->addItem(iconLabel, 0, 0);
    layoutGrid->addItem(titleLabel, 0, 1);
    layoutGrid->addItem(subtitleLabel, 1, 1);

    retranslateUi();
}

void CopyListItem::setTitle(const QString &title)
{
    titleLabel->setText(title);
}

void CopyListItem::setCopyContent(const QString &newContent)
{
    subtitleLabel->setText(newContent);

    const bool hasLines = newContent.contains(".\n");
    const bool hasSentences = newContent.contains(". ");

    if (static_cast<unsigned char>(hasLines) ^ static_cast<unsigned char>(hasSentences)) {
        if (!lineBreakSwitch) {
            lineBreakSwitch = new MButton;
            lineBreakSwitch->setViewType(MButton::switchType);
            lineBreakSwitch->setCheckable(true);
            layoutGrid->addItem(lineBreakSwitch, 1, 0);

            connect(lineBreakSwitch, SIGNAL(toggled(bool)),
                    this, SLOT(handleLineBreakSwitchToggled(bool)));
        }
    } else {
        delete lineBreakSwitch;
        lineBreakSwitch = 0;
    }

    if (lineBreakSwitch) {
        lineBreakSwitch->setChecked(hasLines);
    }
}

void CopyListItem::setHeaderAndItemIndex(int headerIndex, int itemIndex)
{
    this->headerIndex = headerIndex;
    this->itemIndex = itemIndex;
}

void CopyListItem::handleClicked()
{
    copyContentToClipboard(subtitleLabel->text(),
                           titleLabel->text());
}

void CopyListItem::copyContentToClipboard(const QString &clipboardText,
                                          const QString &contentDescription)
{
    if (clipboardText.isEmpty()) {
        return;
    }

    QClipboard *clipboard = QApplication::clipboard();
    if (clipboard) {
        clipboard->setText(clipboardText);

        if (!contentDescription.isEmpty()) {
            showBanner(bannerTitle.arg(contentDescription));
        }
    }
}

void CopyListItem::retranslateUi()
{
    MListItem::retranslateUi();

    //% "Copied to clipboard: %1"
    bannerTitle = qtTrId("xx_copybutton_banner_title");
}

void CopyListItem::handleLineBreakSwitchToggled(bool checked)
{
    const bool breakIntoLines = checked;

    QString text = subtitleLabel->text();

    if (breakIntoLines) {
        text = text.replace(". ", ".\n");
    } else {
        text = text.replace(".\n", ". ");
    }

    emit updateCopyContent(headerIndex, itemIndex, text);
}

void CopyListItem::showBanner(const QString &text)
{
    MBanner *banner = new MBanner();
    banner->setStyleName(MBannerType::ShortEventBanner);
    banner->setTitle(text);
    banner->appear(scene(), MSceneWindow::DestroyWhenDone);
}


ClipboardPageListModel::ClipboardPageListModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

QModelIndex ClipboardPageListModel::parent(const QModelIndex &child) const
{
    if (child.internalId() == -1)
        return QModelIndex();

    return index(child.internalId(), 0);
}

QModelIndex ClipboardPageListModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.parent().isValid())
        return QModelIndex();

    if (parent.isValid())
        // Use parent.row() as internal id. It doesn't have to be unique since we
        // we only use it for fetching parent.
        return createIndex(row, column, parent.row());
    else
        return createIndex(row, column, -1);
}

int ClipboardPageListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() && parent.parent().isValid())
        return 0;

    if (parent.isValid()) {
        // Item count for given group.
        const CopyItemGroup &group = copyItemGroups.at(parent.row());
        return group.second.count();
    }

    // Header count
    return copyItemGroups.count();
}

int ClipboardPageListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant ClipboardPageListModel::data(const QModelIndex &index, int role) const
{
    QVariant result;

    if (!index.parent().isValid()) {
        // List view's header
        if (role == Qt::DisplayRole) {
            const QString headerTitle = copyItemGroups.at(index.row()).first;
            result = QVariant::fromValue(headerTitle);
        }
    } else {

        const int itemRow = index.row();
        const int groupRow = index.parent().row();
        const CopyItemGroup &group = copyItemGroups.at(groupRow);
        const CopyItemData &item = group.second.at(itemRow);

        switch (role) {
        case Qt::DisplayRole:
        case DescriptionRole:
            result = QVariant(item.first);
            break;
        case CopyContentRole:
            result = QVariant(item.second);
            break;
        default:
            break;
        }
    }

    return result;
}

bool ClipboardPageListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool set = false;

    if (index.parent().isValid()) {
        const int itemRow = index.row();
        const int groupRow = index.parent().row();
        CopyItemGroup &group = copyItemGroups[groupRow];
        CopyItemData &item = group.second[itemRow];

        switch (role) {
        case DescriptionRole:
            item.first = value.toString();
            set = true;
            break;
        case CopyContentRole:
            item.second = value.toString();
            set = true;
            break;
        default:
            break;
        }
    }

    if (set) {
        emit dataChanged(index, index);
    }

    return set;
}

void ClipboardPageListModel::clearCopyItems()
{
    if (!copyItemGroups.empty()) {
        beginResetModel();
        copyItemGroups.clear();
        endResetModel();
    }
}

void ClipboardPageListModel::addCopyItems(QString headerText, QList<CopyItemData> items)
{
    QModelIndex headerIndex;

    int groupRow = 0;
    for (; groupRow < copyItemGroups.count(); ++groupRow) {
        if (copyItemGroups[groupRow].first == headerText) {
            headerIndex = index(groupRow, 0);
            break;
        }
    }

    if (!headerIndex.isValid()) {
        beginInsertRows(QModelIndex(), groupRow, groupRow);
        copyItemGroups.append(CopyItemGroup(headerText, items));
        endInsertRows();
    } else {
        CopyItemDataList *list = &copyItemGroups[groupRow].second;
        beginInsertRows(headerIndex, list->count(), list->count() + items.count() - 1);
        list->append(items);
        endInsertRows();
    }
}

void ClipboardPageListModel::updateCopyContent(int headerIndex, int itemIndex, QString text)
{
    QModelIndex header(index(headerIndex, 0));
    QModelIndex item(index(itemIndex, 0, header));
    setData(item, QVariant(text), CopyContentRole);
}

MWidget *CopyListItemCreator::createCell(const QModelIndex &index, MWidgetRecycler &recycler) const
{
    Q_UNUSED(index);
    CopyListItem *cell = dynamic_cast<CopyListItem*>(recycler.take(CopyListItem::staticMetaObject.className()));

    if (cell == NULL) {
        cell = new CopyListItem;
        if (MApplication::instance()->objectName() == "widgetsgallery") {
            cell->setStyleName("CommonBasicListItem");
        } else {
            cell->setStyleName("CommonBasicListItemInverted");
        }
        cell->setLayoutPosition(M::CenterPosition);

        const ClipboardPageListModel *model = static_cast<const ClipboardPageListModel *>(index.model());
        QObject::connect(cell, SIGNAL(updateCopyContent(int, int, QString)),
                         model, SLOT(updateCopyContent(int, int, QString)));
    }
    updateCell(index, cell);
    return cell;
}

void CopyListItemCreator::updateCell(const QModelIndex &index, MWidget *cell) const
{
    CopyListItem *item = dynamic_cast<CopyListItem*>(cell);

    if (!item || !index.isValid() || !index.parent().isValid()) {
        return;
    }

    item->setHeaderAndItemIndex(index.parent().row(), index.row());
    item->setTitle(index.data(ClipboardPageListModel::DescriptionRole).toString());
    item->setCopyContent(index.data(ClipboardPageListModel::CopyContentRole).toString());
}
