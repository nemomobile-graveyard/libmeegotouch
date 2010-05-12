#include "maincategorypage.h"
#include "mainpage.h"

#include <MAbstractCellCreator>
#include <MContentItem>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MList>

class WidgetsGalleryCategoryDataModel : public QAbstractListModel {
public:
    WidgetsGalleryCategoryDataModel(QAbstractItemModel *parentModel, const QModelIndex &parentIndex)
        : QAbstractListModel(), widgetsGalleryModel(parentModel), categoryIndex(parentIndex) {

    }

    QModelIndex parent(const QModelIndex &child) const {
        Q_UNUSED(child);

        return categoryIndex;
    }

    int rowCount(const QModelIndex &index) const {
        Q_UNUSED(index);

        return widgetsGalleryModel->rowCount(categoryIndex);
    }

    QVariant data(const QModelIndex &index, int role) const {
        return widgetsGalleryModel->data(index, role);
    }
private:
    QAbstractItemModel *widgetsGalleryModel;
    QModelIndex categoryIndex;
};

class WidgetGalleryCellCreator : public MAbstractCellCreator<MContentItem>
{
public:
    WidgetGalleryCellCreator() : MAbstractCellCreator<MContentItem>() {
    }

    MWidget *createCell(const QModelIndex &index, MWidgetRecycler &recycler) const {
        Q_UNUSED(index);

        MContentItem *cell = dynamic_cast<MContentItem *>(recycler.take(MContentItem::staticMetaObject.className()));
        if (cell == NULL) {
            cell = new MContentItem(MContentItem::SingleTextLabel);
            cell->setObjectName("wgMainCategoryPageGalleryItem");
        }
        updateCell(index, cell);

        return cell;
    }

    void updateCell(const QModelIndex &index, MWidget *cell) const {
        MContentItem *item = qobject_cast<MContentItem*>(cell);
        if(!item)
            return;

        item->setTitle(index.data().toString());
    }
};

MainCategoryPage::MainCategoryPage(QAbstractItemModel *demosDataModel, const QModelIndex &parentIndex) :
        dataModel(new WidgetsGalleryCategoryDataModel(demosDataModel, parentIndex))
{
}

QString MainCategoryPage::timedemoTitle()
{
    return "MainCategoryPage";
}

void MainCategoryPage::createContent()
{
    TimedemoPage::createContent();

    QGraphicsWidget *panel = centralWidget();

    MLayout *layout = new MLayout(panel);
    layout->setContentsMargins(0, 0, 0, 0);
    panel->setLayout(layout);
    policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    policy->setSpacing(0);

    populateLayout();
}

void MainCategoryPage::populateLayout()
{
    list = new MList(centralWidget());
    list->setObjectName("wgList");
    list->setCellCreator(new WidgetGalleryCellCreator());
    list->setItemModel(dataModel);

    policy->addItem(list, Qt::AlignCenter);

    connect(list, SIGNAL(itemClicked(QModelIndex)), this, SLOT(galleryPageItemClicked(QModelIndex)));
}

void MainCategoryPage::galleryPageItemClicked(const QModelIndex &index)
{
    TemplatePage *page = static_cast<TemplatePage *>(index.data(MainPage::Page).value<void *>());
    page->setParent(this);
    page->appear();
}
