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

#include "assert.h"
#include <QImage>
#include <QHash>
#include <QVector>
#include <QTimer>
#include <QPixmapCache>
#include "contactmodel.h"
#include "libosso-abook/osso-abook-aggregator.h"
#include "libosso-abook/osso-abook-contact.h"

#define DEBUG
#ifdef DEBUG
#define VERIFY_MODEL() priv->verify();
#else
#define VERIFY_MODEL()
#endif

class Contact::Private
{
public:
    Private(OssoABookContact *contact);
    ~Private();

    OssoABookContact *contact;
    QPixmapCache::Key avatarKey;

    static QPixmap getDefaultAvatar();
    static QPixmapCache::Key defaultAvatarKey;
    static QPixmapCache pixmaps;
};

QPixmapCache Contact::Private::pixmaps;
QPixmapCache::Key Contact::Private::defaultAvatarKey;

Contact::Private::Private(OssoABookContact *c)
{
    contact = c;
    if (contact)
        g_object_ref(contact);
}

Contact::Private::~Private()
{
    if (contact)
        g_object_unref(contact);
}

QPixmap Contact::Private::getDefaultAvatar()
{
    QPixmap pixmap;

    if (pixmaps.find(defaultAvatarKey, &pixmap)) {
        return pixmap;
    }

    GtkIconTheme *theme = gtk_icon_theme_get_default();

    GtkIconInfo *info = gtk_icon_theme_lookup_icon(theme, "general_default_avatar", 64, GTK_ICON_LOOKUP_NO_SVG);
    QString path = QString::fromUtf8(gtk_icon_info_get_filename(info));
    gtk_icon_info_free(info);

    pixmap = QPixmap(path);
    defaultAvatarKey = pixmaps.insert(pixmap);

    return pixmap;
}

Contact::Contact()
{
    priv = NULL;
}

Contact::~Contact()
{
    delete priv;
}

const QString Contact::getName() const
{
    return QString::fromUtf8(osso_abook_contact_get_display_name(priv->contact));
}

const QString Contact::getUID() const
{
    return QString::fromUtf8(osso_abook_contact_get_uid(priv->contact));
}

const QStringList Contact::getPhoneNumbers() const
{
    GList *numbers;
    QStringList result;

    numbers = (GList*)e_contact_get_const(E_CONTACT(priv->contact), E_CONTACT_TEL);

    while (numbers) {
        result << QString::fromUtf8((const char*)numbers->data);
        numbers = numbers->next;
    }

    return result;
}

const QStringList Contact::getEmailAddresses() const
{
    GList *emails;
    QStringList result;

    emails = (GList*)e_contact_get_const(E_CONTACT(priv->contact), E_CONTACT_EMAIL);

    while (emails) {
        result << QString::fromUtf8((const char*)emails->data);
        emails = emails->next;
    }

    return result;
}

const QPixmap Contact::getAvatar() const
{
    QPixmap pixmap;

    if (priv->pixmaps.find(priv->avatarKey, &pixmap)) {
        return pixmap;
    }
    return Contact::Private::getDefaultAvatar();
}

const void Contact::loadAvatar()
{
    QPixmap pixmap;

    if (priv->pixmaps.find(priv->avatarKey, &pixmap)) {
        return;
    }

    GdkPixbuf *pixbuf;
    pixbuf = osso_abook_avatar_get_image_rounded
                    (OSSO_ABOOK_AVATAR(priv->contact), 64, 64, TRUE, 4, NULL);

    if (!pixbuf) {
        priv->avatarKey = priv->defaultAvatarKey;
        return;
    }

    int width, height, has_alpha, stride;
    const guchar *pixels;
    g_object_get (pixbuf,
                  "width", &width,
                  "height", &height,
                  "has-alpha", &has_alpha,
                  "rowstride", &stride,
                  "pixels", &pixels,
                  NULL);

    QImage image(pixels, width, height, stride, has_alpha ? QImage::Format_ARGB32 : QImage::Format_RGB32);
    pixmap = QPixmap::fromImage(image.rgbSwapped());

    g_object_unref(pixbuf);
    priv->avatarKey = priv->pixmaps.insert(pixmap);
}

bool Contact::collate(Contact* a, Contact* b, Qt::SortOrder order)
{
    OssoABookNameOrder nameOrder;

    nameOrder = osso_abook_settings_get_name_order();

    int res = osso_abook_contact_collate (a->priv->contact, b->priv->contact, nameOrder);
    switch (order) {
        case Qt::AscendingOrder:
            return res < 0;
        case Qt::DescendingOrder:
            return res > 0;
    }
    return res < 0;
}

bool Contact::equals(const Contact *a, const Contact *b)
{
    const char *a_uid = osso_abook_contact_get_uid(a->priv->contact);
    const char *b_uid = osso_abook_contact_get_uid(b->priv->contact);

    return (0 == strcmp (a_uid, b_uid));
}

class ContactModel::Private
{
public:
    Private(ContactModel* model);
    ~Private();

    static Contact* createContact(OssoABookContact *abc);
    static void onContactsAdded(OssoABookRoster *roster, OssoABookContact *contacts[], gpointer user_data);
    static void onContactsChanged(OssoABookRoster *roster, OssoABookContact *contacts[], gpointer user_data);
    static void onContactsRemoved(OssoABookRoster *roster, char *contacts[], gpointer user_data);

    void recreateUIDtoRow();
    void updatePersistentIndices(QVector<Contact*> &old_contacts);
    void verify();

    ContactModel *model;
    OssoABookRoster *aggregator;
    QHash<QString, int> uid_to_row;
    QVector<Contact*> contacts;
    Qt::SortOrder order;
};

ContactModel::Private::Private(ContactModel *m) : model(m)
{
    GError *error = NULL;

    aggregator = osso_abook_aggregator_get_default(&error);
    if (error) {
        qCritical("Could not get aggregator: %s", error->message);
        g_clear_error(&error);
    } else {
        g_signal_connect(aggregator, "contacts-added", G_CALLBACK(ContactModel::Private::onContactsAdded), this);
        g_signal_connect(aggregator, "contacts-changed", G_CALLBACK(ContactModel::Private::onContactsChanged), this);
        g_signal_connect(aggregator, "contacts-removed", G_CALLBACK(ContactModel::Private::onContactsRemoved), this);
    }
    order = Qt::AscendingOrder;
}

ContactModel::Private::~Private()
{
    g_signal_handlers_disconnect_by_func(aggregator, (void*)ContactModel::Private::onContactsAdded, this);
    g_signal_handlers_disconnect_by_func(aggregator, (void*)ContactModel::Private::onContactsChanged, this);
    g_signal_handlers_disconnect_by_func(aggregator, (void*)ContactModel::Private::onContactsRemoved, this);

    qDeleteAll(contacts);
}

void ContactModel::Private::verify()
{
    for (int i=0; i < contacts.size(); i++) {
        QString uid = contacts[i]->getUID();
        int idx = uid_to_row[uid];

        assert(idx == i);
    }
}

void ContactModel::Private::recreateUIDtoRow()
{
    for (int i = 0; i < contacts.size(); i++) {
        QString uid = contacts[i]->getUID();
        uid_to_row[uid] = i;
    }
}

void ContactModel::Private::updatePersistentIndices(QVector<Contact*> &old_contacts)
{
    for (int i = 0; i < contacts.size(); i++) {
        Contact *oldc = old_contacts[i];
        Contact *newc = contacts[i];

        if (!Contact::equals(oldc, newc)) {
            int new_idx = uid_to_row[newc->getUID()];
            model->changePersistentIndex(model->createIndex(i, 0), model->createIndex(new_idx, 0));
        }
    }
}

Contact* ContactModel::Private::createContact(OssoABookContact *abc)
{
    Contact *contact = new Contact();

    contact->priv = new Contact::Private(abc);

    return contact;
}

void
ContactModel::Private::onContactsAdded(OssoABookRoster  *roster,
                                       OssoABookContact *contacts[],
                                       gpointer          user_data)
{
    ContactModel::Private *priv = (ContactModel::Private*)user_data;
    ContactModel *model = priv->model;

    Q_UNUSED (roster);

    int rows = 0;
    for (int i = 0; contacts[i]; i++) {
        rows++;
    }
    int position = priv->contacts.size();

    VERIFY_MODEL()
    model->beginInsertRows(QModelIndex(), position, position + rows - 1);
    for (int i = 0; i < rows; i++) {
        Contact *contact = createContact(contacts[i]);
        QString uid = contact->getUID();

        priv->contacts.append(contact);
        priv->uid_to_row[uid] = position + i;
    }
    model->endInsertRows();
    VERIFY_MODEL()

    model->sort(0, priv->order == Qt::AscendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder);

    emit model->rowCountChanged(priv->contacts.size());
}

void
ContactModel::Private::onContactsChanged(OssoABookRoster  *roster,
                                         OssoABookContact *contacts[],
                                         gpointer          user_data)
{
    ContactModel::Private *priv = (ContactModel::Private*)user_data;
    ContactModel *model = priv->model;

    Q_UNUSED (roster);

    VERIFY_MODEL()
    for (int i = 0; contacts[i]; i++) {
        Contact *contact = createContact(contacts[i]);
        QString uid = contact->getUID();
        int row = priv->uid_to_row[uid];

        delete priv->contacts[row];
        priv->contacts[row] = contact;
    }
    VERIFY_MODEL()

    model->sort(0, priv->order == Qt::AscendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder);

    emit model->rowCountChanged(priv->contacts.size());
}

void ContactModel::Private::onContactsRemoved(OssoABookRoster *roster,
                                              char            *contacts[],
                                              gpointer         user_data)
{
    ContactModel::Private *priv = (ContactModel::Private*)user_data;
    ContactModel *model = priv->model;

    Q_UNUSED (roster);
    VERIFY_MODEL()
    for (int i = 0; contacts[i]; i++) {
        QString uid = QString::fromUtf8(contacts[i]);
        int row = priv->uid_to_row[uid];

        model->beginRemoveRows(QModelIndex(), row, row);
        priv->uid_to_row.remove(uid);
        delete priv->contacts[row];
        priv->contacts.remove(row);
        for (int j = row; j < priv->contacts.size(); j++) {
            QString uid = priv->contacts[j]->getUID();
            priv->uid_to_row[uid]--;
        }
        model->endRemoveRows();
    }
    VERIFY_MODEL()

    emit model->rowCountChanged(priv->contacts.size());
}

ContactModel::ContactModel(QObject *parent) : QAbstractListModel(parent)
{
    priv = new ContactModel::Private(this);
}

ContactModel::~ContactModel()
{
    delete priv;
}

int ContactModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return priv->contacts.size();
}

QVariant ContactModel::data(const QModelIndex &index, int role) const
{
    VERIFY_MODEL()

    if (!index.isValid())
        return QVariant();

    if (index.row() >= priv->contacts.size())
        return QVariant();

    Contact *contact = priv->contacts[index.row()];

    if (!contact)
        return QVariant();

    QVariant var;
    switch (role) {
        case Qt::DisplayRole:
            var.setValue(contact);
            return var;
            break;
        default:
            break;
    }

    return QVariant();
}

void ContactModel::avatarLoaded(const QModelIndex& index)
{
    emit dataChanged(index, index);
}

void ContactModel::sort(int column, Qt::SortOrder requested_order)
{
    if (column != 0 || priv->order == requested_order)
        return;

    priv->order = requested_order;

    emit layoutAboutToBeChanged();

    QVector<Contact*> old_contacts = priv->contacts;
    qStableSort(priv->contacts.begin(), priv->contacts.end(), Contact::Collator(priv->order));
    priv->recreateUIDtoRow();
    VERIFY_MODEL()

    priv->updatePersistentIndices(old_contacts);
    emit layoutChanged();
}

ContactImageLoader::ContactImageLoader()
{

}

ContactImageLoader::~ContactImageLoader()
{
    stopLoadingPictures();
}

void ContactImageLoader::loadPictures(const QModelIndex& firstVisibleRow, const QModelIndex& lastVisibleRow)
{
    int rowCount = lastVisibleRow.row();

    for(int i = firstVisibleRow.row(); i <= rowCount; i++)
    {
        QModelIndex index(firstVisibleRow.sibling(i, 0));
        if(!index.isValid())
            continue;
        QVariant data = index.data(Qt::DisplayRole);
        Contact *contact = data.value<Contact*>();

        // May happen if size of list model was changed
        if(contact == NULL)
            continue;

        Job job;
        job.contact = contact;
        job.row = index;
        thumbnailLoadingJobs << job;
    }

    // processJobQueue will issue single shots to continue loading images
    if(thumbnailLoadingJobs.count() != 0)
        QTimer::singleShot(0, this, SLOT(processJobQueue()));
}

void ContactImageLoader::stopLoadingPictures()
{
    // Lets not process any more requests to load image because list is panning now
    thumbnailLoadingJobs.clear();
}

void ContactImageLoader::processJobQueue()
{
    if(thumbnailLoadingJobs.isEmpty())
        return;

    Job job = thumbnailLoadingJobs.takeFirst();

    Contact *contact = job.contact;
    contact->loadAvatar();

    notifyModel(job.row);

    // Continue loading and letting UI thread do something
    if(thumbnailLoadingJobs.count() > 0)
        QTimer::singleShot(0, this, SLOT(processJobQueue()));
}

// There is no need to cast if you attach model to the image loader
// but this is one way to achieve same result.
void ContactImageLoader::notifyModel(const QModelIndex &index)
{
    ContactModel *model = (ContactModel*)index.model();
    model->avatarLoaded(index);
}

