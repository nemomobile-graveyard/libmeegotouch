#ifndef MAINCATEGORYPAGE_H
#define MAINCATEGORYPAGE_H

#include "templatepage.h"

#include <QModelIndex>

class MLinearLayoutPolicy;
class MList;

class QAbstractItemModel;

class MainCategoryPage : public TimedemoPage
{
    Q_OBJECT
public:
    MainCategoryPage(QAbstractItemModel* demosDataModel, const QModelIndex &parentIndex);

    QString timedemoTitle();

    void createContent();
    void populateLayout();

private Q_SLOTS:
    void galleryPageItemClicked(const QModelIndex &index);

private:
    QAbstractItemModel *dataModel;
    MList *list;

    MLinearLayoutPolicy *policy;
};

#endif // MAINCATEGORYPAGE_H
