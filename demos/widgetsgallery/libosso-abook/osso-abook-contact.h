/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*- */
/*
 * This file is part of libosso-abook
 *
 * Copyright (C) 2006-2009 Nokia Corporation. All rights reserved.
 *
 * Contact: Joergen Scheibengruber <jorgen.scheibengruber@nokia.com>
 */

#ifndef __OSSO_ABOOK_CONTACT_H__
#define __OSSO_ABOOK_CONTACT_H__

#define protected protected_
#include <gtk/gtk.h>
#undef protected
#include <libebook/e-book.h>
#include <gio/gio.h>

#include "osso-abook-avatar.h"
#include "osso-abook-caps.h"
#include "osso-abook-settings.h"

G_BEGIN_DECLS

#define OSSO_ABOOK_TYPE_CONTACT \
                (osso_abook_contact_get_type ())
#define OSSO_ABOOK_CONTACT(obj) \
                (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                 OSSO_ABOOK_TYPE_CONTACT, \
                 OssoABookContact))
#define OSSO_ABOOK_CONTACT_CLASS(cls) \
                (G_TYPE_CHECK_CLASS_CAST ((cls), \
                 OSSO_ABOOK_TYPE_CONTACT, \
                 OssoABookContactClass))
#define OSSO_ABOOK_IS_CONTACT(obj) \
                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                 OSSO_ABOOK_TYPE_CONTACT))
#define OSSO_ABOOK_IS_CONTACT_CLASS(obj) \
                (G_TYPE_CHECK_CLASS_TYPE ((obj), \
                 OSSO_ABOOK_TYPE_CONTACT))
#define OSSO_ABOOK_CONTACT_GET_CLASS(obj) \
                (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                 OSSO_ABOOK_TYPE_CONTACT, \
                 OssoABookContactClass))

/**
 * OSSO_ABOOK_VCA_GENDER:
 *
 * VCard attribute name for storing the gender.
 */
#define OSSO_ABOOK_VCA_GENDER          "X-GENDER"

/**
 * OSSO_ABOOK_VCA_OSSO_MASTER_UID:
 *
 * VCard attribute name for storing the set of master contact UIDs.
 */
#define OSSO_ABOOK_VCA_OSSO_MASTER_UID          "X-OSSO-MASTER-UID"

/**
 * OSSO_ABOOK_VCA_TELEPATHY_HANDLE:
 *
 * VCard attribute name for reporting the contact's Telepathy handle.
 */
#define OSSO_ABOOK_VCA_TELEPATHY_HANDLE         "X-TELEPATHY-HANDLE"

/**
 * OSSO_ABOOK_VCA_TELEPATHY_PRESENCE:
 *
 * VCard attribute name for reporting the contact's presence state.
 * Valid values: [detailed-name;]{available,away,...}[;custom-message]
 */
#define OSSO_ABOOK_VCA_TELEPATHY_PRESENCE       "X-TELEPATHY-PRESENCE"

/**
 * OSSO_ABOOK_VCA_TELEPATHY_CAPABILITIES:
 *
 * VCard attribute name for reporting the contact's communication capabilities.
 * Valid values: {text,audio,video}*
 */
#define OSSO_ABOOK_VCA_TELEPATHY_CAPABILITIES   "X-TELEPATHY-CAPABILITIES"

/**
 * OSSO_ABOOK_VCA_TELEPATHY_BLOCKED:
 *
 * VCard attribute name for reporting the contact's blocking state.
 * Valid values: yes, no, local-pending, remote-pending
 */
#define OSSO_ABOOK_VCA_TELEPATHY_BLOCKED        "X-TELEPATHY-BLOCKED"

/**
 * OSSO_ABOOK_VCA_TELEPATHY_SUBSCRIBED:
 *
 * VCard attribute name for reporting the contact's subscription state.
 * Valid values: yes, no, local-pending, remote-pending
 */
#define OSSO_ABOOK_VCA_TELEPATHY_SUBSCRIBED     "X-TELEPATHY-SUBSCRIBED"

/**
 * OSSO_ABOOK_VCA_TELEPATHY_PUBLISHED:
 *
 * VCard attribute name for reporting the contact's publication state.
 * Valid values: yes, no, local-pending, remote-pending
 */
#define OSSO_ABOOK_VCA_TELEPATHY_PUBLISHED      "X-TELEPATHY-PUBLISHED"

/**
 * OSSO_ABOOK_VCP_OSSO_READONLY:
 *
 * This paramter marks a vCard attribute as read-only.
 * Valid values: yes, no
 */
#define OSSO_ABOOK_VCP_OSSO_READONLY            "X-OSSO-READONLY"

/**
 * OSSO_ABOOK_VCP_OSSO_VALID:
 *
 * VCard parameter name for marking an attribute as known-invalid (eg,
 * after confirming with the server that a username is invalid/missing).
 * Valid values: yes, no, &lt;parameter missing&gt; (validity unconfirmed)
 */
#define OSSO_ABOOK_VCP_OSSO_VALID               "X-OSSO-VALID"

/**
 * OSSO_ABOOK_DTMF_CHARS:
 *
 * Set of characters starting a DTMF sequence.
 */
#define OSSO_ABOOK_DTMF_CHARS                   "PpWwXx"

#define OSSO_ABOOK_HOME_APPLET_PREFIX           "osso-abook-applet-"


/**
 * OssoABookContactAction:
 *
 * OSSO_ABOOK_CONTACT_ACTION_NONE: no actions supported
 * OSSO_ABOOK_CONTACT_ACTION_TEL: Start a telephone call
 * OSSO_ABOOK_CONTACT_ACTION_SMS: Send an SMS message
 * OSSO_ABOOK_CONTACT_ACTION_CHATTO: Start a text chat
 * OSSO_ABOOK_CONTACT_ACTION_VOIPTO: Start a VoIP call
 * OSSO_ABOOK_CONTACT_ACTION_VOIPTO_AUDIO: Start a VoIP audio call
 * OSSO_ABOOK_CONTACT_ACTION_VOIPTO_VIDEO: Start a VoIP video call
 * OSSO_ABOOK_CONTACT_ACTION_DATE: Open a date in the calendar
 * OSSO_ABOOK_CONTACT_ACTION_MAILTO: Send an email
 * OSSO_ABOOK_CONTACT_ACTION_MAP: Open location in map application
 * OSSO_ABOOK_CONTACT_ACTION_URL: Open URL in the browser
 * OSSO_ABOOK_CONTACT_ACTION_CREATE_ACCOUNT: create an IM or VoIP account
 * OSSO_ABOOK_CONTACT_ACTION_BIND: binding an IM or VoIP username to an existing account
 * OSSO_ABOOK_CONTACT_ACTION_CHATTO_ERROR: Display an error for invalid IM username
 * OSSO_ABOOK_CONTACT_ACTION_VOIPTO_ERROR: Display an error for invalid VoIP username
 *
 * Actions that are supported by different fields of an #OssoABookContact
 */
typedef enum {
        OSSO_ABOOK_CONTACT_ACTION_NONE,
        OSSO_ABOOK_CONTACT_ACTION_TEL,
        OSSO_ABOOK_CONTACT_ACTION_SMS,
        OSSO_ABOOK_CONTACT_ACTION_CHATTO,
        OSSO_ABOOK_CONTACT_ACTION_VOIPTO,
        OSSO_ABOOK_CONTACT_ACTION_VOIPTO_AUDIO,
        OSSO_ABOOK_CONTACT_ACTION_VOIPTO_VIDEO,
        OSSO_ABOOK_CONTACT_ACTION_DATE,
        OSSO_ABOOK_CONTACT_ACTION_MAILTO,
        OSSO_ABOOK_CONTACT_ACTION_MAP,
        OSSO_ABOOK_CONTACT_ACTION_URL,
        OSSO_ABOOK_CONTACT_ACTION_CREATE_ACCOUNT,
        OSSO_ABOOK_CONTACT_ACTION_BIND,
        OSSO_ABOOK_CONTACT_ACTION_CHATTO_ERROR,
        OSSO_ABOOK_CONTACT_ACTION_VOIPTO_ERROR,
} OssoABookContactAction;

/**
 * OssoABookContactWriteToFileCb:
 * @contact: the #OssoABookContact that was written
 * @file: the file that was written
 * @error: possible error value, or %NULL on success
 * @user_data: additional user_data
 *
 * A function that will be called when osso_abook_contact_write_to_file() is
 * complete
 */
typedef void (*OssoABookContactWriteToFileCb) (OssoABookContact *contact,
                                               GFile            *file,
                                               const GError     *error,
                                               gpointer          user_data);

/**
 * OssoABookContactActionStartCb:
 * @error: possible error value, or %NULL on success
 * @parent: a parent #GtkWindow
 * @user_data: additional user_data
 *
 * A function that will be called after an action has been started to report the
 * status of the action
 */
typedef void (*OssoABookContactActionStartCb) (const GError     *error,
                                               GtkWindow        *parent,
                                               gpointer          user_data);
typedef enum {
        OSSO_ABOOK_CONTACT_ACTION_ERROR_SUCCESS,
        OSSO_ABOOK_CONTACT_ACTION_ERROR_ADDRESS,
} OssoABookContactActionError;

/**
 * OssoABookContact:
 *
 * All the fields of this structure are private to the object's
 * implementation and should never be accessed directly.
 */
struct _OssoABookContact {
        EContact parent;
};

struct _OssoABookContactClass {
        EContactClass parent_class;

        /*< virtual methods >*/

        guint (* async_commit)      (OssoABookContact *contact,
                                     EBook            *book,
                                     EBookCallback     callback,
                                     gpointer          user_data);

        guint (* async_add)         (OssoABookContact *contact,
                                     EBook            *book,
                                     EBookIdCallback   callback,
                                     gpointer          user_data);

        guint (* async_remove)      (OssoABookContact *contact,
                                     EBook            *book,
                                     EBookCallback     callback,
                                     gpointer          user_data);

        guint (* async_remove_many) (GList            *contacts,
                                     EBook            *book,
                                     EBookCallback     callback,
                                     gpointer          user_data);

        char * (* get_default_name) (OssoABookContact *contact);

        /*< signals >*/

        void  (* reset)            (OssoABookContact *contact);
        void  (* contact_attached) (OssoABookContact *contact,
                                    OssoABookContact *roster_contact);
        void  (* contact_detached) (OssoABookContact *contact,
                                    OssoABookContact *roster_contact);
};


/********************************************************************** 
 * Constructors and Related Functions
 **********************************************************************/

GType
osso_abook_contact_get_type                (void) G_GNUC_CONST;

OssoABookContact *
osso_abook_contact_new                     (void);

OssoABookContact *
osso_abook_contact_new_from_template       (EContact                 *templ);

OssoABookContact *
osso_abook_contact_new_from_vcard          (const char               *uid,
                                            const char               *vcard);

/**********************************************************************
 * Convenience API for Attribute Handling
 **********************************************************************/

/**
 * osso_abook_contact_get_uid:
 * @contact: a #OssoABookContact
 *
 * Returns: @contact's UID.  The returned string is owned by @contact and should not
 * be freed.
 */
inline static const char *
osso_abook_contact_get_uid                 (OssoABookContact         *contact)
{
        return (const char*)e_contact_get_const (E_CONTACT (contact), E_CONTACT_UID);
}

/**
 * osso_abook_contact_set_uid:
 * @contact: a #OssoABookContact
 * @uid: new UID for @contact
 *
 * Sets @contact's UID to @uid
 */
inline static void
osso_abook_contact_set_uid                 (OssoABookContact         *contact,
                                            const char               *uid)
{
        e_contact_set (E_CONTACT (contact), E_CONTACT_UID, (gpointer) uid);
}

const char *
osso_abook_contact_get_persistent_uid      (OssoABookContact         *contact);

char *
osso_abook_contact_get_value               (EContact                 *contact,
                                            const char               *attr_name);

GList *
osso_abook_contact_get_values              (EContact                 *contact,
                                            const char               *attr_name);

G_GNUC_WARN_UNUSED_RESULT GList *
osso_abook_contact_get_attributes          (EContact                 *contact,
                                            const char               *attr_name);

void
osso_abook_contact_set_value               (EContact                 *contact,
                                            const char               *attr_name,
                                            const char               *value);

gboolean
osso_abook_contact_add_value               (EContact                 *contact,
                                            const char               *attr_name,
                                            GCompareFunc              value_check,
                                            const char               *value);

void
osso_abook_contact_remove_value            (EContact                 *contact,
                                            const char               *attr_name,
                                            const char               *value);

gboolean
osso_abook_contact_find_matching_values    (EContact                 *contact_a,
                                            EContact                 *contact_b,
                                            const char               *attr_name,
                                            GList                   **values);

void
osso_abook_contact_reset                   (OssoABookContact         *contact,
                                            OssoABookContact         *replacement);

void
osso_abook_contact_attribute_set_readonly  (EVCardAttribute          *attribute,
                                            gboolean                  readonly);

gboolean
osso_abook_contact_attribute_is_readonly   (EVCardAttribute          *attribute);

void
osso_abook_contact_attribute_set_profile   (EVCardAttribute          *attribute,
                                            const char               *profile_name);

McProfile *
osso_abook_contact_attribute_get_profile   (EVCardAttribute          *attribute);

/**********************************************************************
 * Roster Contact List Handling
 **********************************************************************/

gboolean
osso_abook_contact_attach                  (OssoABookContact         *master_contact,
                                            OssoABookContact         *roster_contact);

gboolean
osso_abook_contact_detach                  (OssoABookContact         *master_contact,
                                            OssoABookContact         *roster_contact);

gboolean
osso_abook_contact_has_roster_contacts     (OssoABookContact         *master_contact);

G_GNUC_WARN_UNUSED_RESULT GList *
osso_abook_contact_get_roster_contacts     (OssoABookContact         *master_contact);

G_GNUC_WARN_UNUSED_RESULT GList *
osso_abook_contact_find_roster_contacts    (OssoABookContact         *master_contact,
                                            const char               *username,
                                            const char               *vcard_field);

G_GNUC_WARN_UNUSED_RESULT GList *
osso_abook_contact_find_roster_contacts_for_account
                                           (OssoABookContact         *master_contact,
                                            const char               *username,
                                            const char               *account_id);

GList *
osso_abook_contact_find_roster_contacts_for_attribute
                                           (OssoABookContact         *master_contact,
                                            EVCardAttribute          *attribute);

/**********************************************************************
 * Roster Contacts Properties
 **********************************************************************/

char *
osso_abook_create_temporary_uid            (void);

gboolean
osso_abook_is_temporary_uid                (const char               *uid);

GList *
osso_abook_contact_get_master_uids         (OssoABookContact         *roster_contact);

gboolean
osso_abook_contact_add_master_uid          (OssoABookContact         *roster_contact,
                                            const char               *master_uid);

gboolean
osso_abook_contact_remove_master_uid       (OssoABookContact         *roster_contact,
                                            const char               *master_uid);

gboolean
osso_abook_contact_is_temporary            (OssoABookContact         *contact);

gboolean
osso_abook_contact_is_roster_contact       (OssoABookContact         *contact);

gboolean
osso_abook_contact_is_sim_contact          (OssoABookContact         *contact);

OssoABookRoster *
osso_abook_contact_get_roster              (OssoABookContact         *contact);

McAccount *
osso_abook_contact_get_account             (OssoABookContact         *contact);

McProfile *
osso_abook_contact_get_profile             (OssoABookContact         *contact);

const char *
osso_abook_contact_get_vcard_field         (OssoABookContact         *contact);

const char *
osso_abook_contact_get_bound_name          (OssoABookContact         *contact);

gboolean
osso_abook_contact_has_invalid_username    (OssoABookContact         *contact);

void
osso_abook_contact_set_roster              (OssoABookContact         *contact,
                                            OssoABookRoster          *roster);

gboolean
osso_abook_contact_matches_phone_number    (OssoABookContact         *contact,
                                            const char               *phone_number,
                                            gboolean                  fuzzy_match);

gboolean
osso_abook_contact_matches_username        (OssoABookContact         *contact,
                                            const char               *username,
                                            const char               *vcard_field,
                                            const char               *account_name);

/**********************************************************************
 * Generic Contact Properties
 **********************************************************************/

void
osso_abook_contact_set_presence            (OssoABookContact         *contact,
                                            guint                     type,
                                            const gchar              *status,
                                            const gchar              *message);
gboolean
osso_abook_contact_photo_is_user_selected  (OssoABookContact         *contact);

GdkPixbuf *
osso_abook_contact_get_photo               (OssoABookContact         *contact);

void
osso_abook_contact_set_pixbuf              (OssoABookContact         *contact,
                                            GdkPixbuf                *pixbuf,
                                            EBook                    *book,
                                            GtkWindow                *window);

void
osso_abook_contact_set_photo               (OssoABookContact         *contact,
                                            const char               *filename,
                                            EBook                    *book,
                                            GtkWindow                *window);

void
osso_abook_contact_set_photo_data          (OssoABookContact         *contact,
                                            gconstpointer             data,
                                            gsize                     len,
                                            EBook                    *book,
                                            GtkWindow                *window);

OssoABookAvatar *
osso_abook_contact_get_avatar              (OssoABookContact         *contact,
                                            const char               *username,
                                            const char               *vcard_field);

GdkPixbuf *
osso_abook_contact_get_avatar_pixbuf       (OssoABookContact         *contact,
                                            const char               *username,
                                            const char               *vcard_field);

const char *
osso_abook_contact_get_name                (OssoABookContact         *contact);

const char *
osso_abook_contact_get_display_name        (OssoABookContact         *contact);

void
osso_abook_contact_get_name_components     (EContact                 *contact,
                                            OssoABookNameOrder        order,
                                            gboolean                  strict,
                                            char                    **primary_out,
                                            char                    **secondary_out);

const char *
osso_abook_contact_get_name_with_order     (OssoABookContact         *contact,
                                            OssoABookNameOrder        order);

gboolean
osso_abook_contact_action_start            (OssoABookContactAction         action,
                                            OssoABookContact              *contact,
                                            EVCardAttribute               *attribute,
                                            McAccount                     *account,
                                            GtkWindow                     *parent);

gboolean
osso_abook_contact_action_start_with_callback (OssoABookContactAction         action,
                                               OssoABookContact              *contact,
                                               EVCardAttribute               *attribute,
                                               McAccount                     *account,
                                               GtkWindow                     *parent,
                                               OssoABookContactActionStartCb  callback,
                                               gpointer                       callback_data);

char *
osso_abook_contact_to_string               (OssoABookContact         *contact,
                                            EVCardFormat              format,
                                            gboolean                  inline_avatar);

char *
osso_abook_contact_get_basename            (OssoABookContact         *contact,
                                            gboolean                  strict);

void
osso_abook_contact_write_to_file           (OssoABookContact         *contact,
                                            EVCardFormat              format,
                                            gboolean                  inline_avatar,
                                            gboolean                  strict_filename,
                                            GFile                    *parent_file,
                                            OssoABookContactWriteToFileCb
                                                                      callback,
                                            gpointer                  user_data);

gboolean
osso_abook_contact_shortcut_exists         (OssoABookContact         *contact,
                                            GSList                  **ret_list);

gboolean
osso_abook_contact_shortcut_create         (OssoABookContact         *contact);

OssoABookContact *
osso_abook_contact_merge_roster_info       (OssoABookContact         *contact);

OssoABookContact *
osso_abook_contact_fetch_roster_info       (OssoABookContact         *contact);

/**********************************************************************
 * Contact Sorting
 **********************************************************************/

const char **
osso_abook_contact_get_collate_keys        (OssoABookContact         *contact,
                                            OssoABookNameOrder        order);

int
osso_abook_contact_collate                 (OssoABookContact         *a,
                                            OssoABookContact         *b,
                                            OssoABookNameOrder        order);

int
osso_abook_contact_uid_compare             (OssoABookContact         *a,
                                            OssoABookContact         *b);

/***************************************************/
/***** FOLLOWING FUNCTIONS NEED TO BE REVIEWED *****/
/***************************************************/

guint
osso_abook_contact_async_add               (OssoABookContact         *contact,
                                            EBook                    *book,
                                            EBookIdCallback           callback,
                                            gpointer                  user_data);

guint
osso_abook_contact_async_commit            (OssoABookContact         *contact,
                                            EBook                    *book,
                                            EBookCallback             callback,
                                            gpointer                  user_data);

void
osso_abook_contact_commit                  (OssoABookContact         *contact,
                                            gboolean                  create,
                                            EBook                    *book,
                                            GtkWindow                *window);

void
osso_abook_contact_accept                  (OssoABookContact         *contact,
                                            OssoABookContact         *master,
                                            GtkWindow                *parent);

void
osso_abook_contact_accept_for_uid          (OssoABookContact         *contact,
                                            const char               *master_uid,
                                            GtkWindow                *parent);

void
osso_abook_contact_async_accept            (OssoABookContact         *contact,
                                            const char               *master_uid,
                                            EBookIdCallback           callback,
                                            gpointer                  user_data);

void
osso_abook_contact_reject                  (OssoABookContact         *contact,
                                            GtkWindow                *parent);

void
osso_abook_contact_reject_for_uid          (OssoABookContact         *contact,
                                            const char               *master_uid,
                                            GtkWindow                *parent);

gboolean
osso_abook_contact_delete                  (OssoABookContact         *contact,
                                            EBook                    *book,
                                            GtkWindow                *window);

void
osso_abook_contact_delete_many             (GList                    *contacts,
                                            EBook                    *book,
                                            GtkWindow                *window);

gboolean
osso_abook_contact_get_blocked             (OssoABookContact         *contact);

gboolean
osso_abook_contact_can_block               (OssoABookContact         *contact,
                                            const char              **infoprint);

gboolean
osso_abook_contact_can_request_auth        (OssoABookContact         *contact,
                                            const char              **infoprint);

void
osso_abook_contact_vcard_clean             (EContact                 *contact);

gboolean
osso_abook_contact_has_valid_name          (OssoABookContact         *contact);

G_END_DECLS

#endif /* __OSSO_ABOOK_CONTACT_H__ */
