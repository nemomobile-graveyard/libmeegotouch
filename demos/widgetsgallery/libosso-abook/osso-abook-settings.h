/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*- */
/*
 * This file is part of libosso-abook
 *
 * Copyright (C) 2008-2009 Nokia Corporation. All rights reserved.
 *
 * Contact: Joergen Scheibengruber <jorgen.scheibengruber@nokia.com>
 */

#ifndef __OSSO_ABOOK_SETTINGS_H__
#define __OSSO_ABOOK_SETTINGS_H__

#include <gconf/gconf-client.h>

G_BEGIN_DECLS

#define OSSO_ABOOK_SETTINGS_KEY_CONTACT_ORDER \
        "/apps/osso-addressbook/contact-order"
#define OSSO_ABOOK_SETTINGS_KEY_HOME_APPLETS \
        "/apps/osso-addressbook/home-applets"
#define OSSO_ABOOK_SETTINGS_KEY_LAST_IMSI \
        "/apps/osso-addressbook/last-imsi"
#define OSSO_ABOOK_SETTINGS_KEY_NAME_ORDER \
        "/apps/osso-addressbook/name-order"
#define OSSO_ABOOK_SETTINGS_KEY_SELF_CARD \
        "/apps/osso-addressbook/self-vcard"
#define OSSO_ABOOK_SETTINGS_KEY_SELF_AVATAR_IMPORTED \
        "/apps/osso-addressbook/self-avatar-imported"
#define OSSO_ABOOK_SETTINGS_KEY_VOICEMAIL_VCARD \
        "/apps/osso-addressbook/voicemail-vcard"
#define OSSO_ABOOK_SETTINGS_KEY_VOICEMAIL_NUMBERS \
        "/apps/osso-addressbook/voicemail-numbers"

/* dropped feature */
#ifdef OSSO_ABOOK_HAVE_SPEED_DIAL
#define OSSO_ABOOK_SETTINGS_KEY_SPEED_DIAL_DIR \
        "/apps/osso-addressbook/speed-dial"                     /*< no-schema >*/
#define OSSO_ABOOK_SETTINGS_KEY_VOICEMAIL_INDEX \
        "/apps/osso-addressbook/vmbx-index"                     /*< no-schema >*/
#endif /* OSSO_ABOOK_HAVE_SPEED_DIAL */

/**
 * OssoABookContactOrder:
 * @OSSO_ABOOK_CONTACT_ORDER_NONE: Don't sort at all.
 * @OSSO_ABOOK_CONTACT_ORDER_NAME: Sort by (display) name.
 * @OSSO_ABOOK_CONTACT_ORDER_PRESENCE: Sort by presence, then name.
 * @OSSO_ABOOK_CONTACT_ORDER_CUSTOM: Use custom #OssoABookListStoreCompareFunc for sorting.
 *
 * The sorting modes of a #OssoABookListStore.
 */
typedef enum {
        OSSO_ABOOK_CONTACT_ORDER_NONE = -1,
        OSSO_ABOOK_CONTACT_ORDER_NAME,     /* Sort by name */
        OSSO_ABOOK_CONTACT_ORDER_PRESENCE, /* Sort by presence, then name */
        OSSO_ABOOK_CONTACT_ORDER_CUSTOM,
} OssoABookContactOrder;

/**
 * OssoABookNameOrder:
 * @OSSO_ABOOK_NAME_ORDER_FIRST:      Display given name first
 * @OSSO_ABOOK_NAME_ORDER_LAST:       Display family name first, separated by comma
 * @OSSO_ABOOK_NAME_ORDER_LAST_SPACE: Display family name first, separated by space
 * @OSSO_ABOOK_NAME_ORDER_NICK:       Display nickname only
 *
 * Preferred order for name components for sorting and display.
 */
typedef enum {
        OSSO_ABOOK_NAME_ORDER_FIRST,
        OSSO_ABOOK_NAME_ORDER_LAST,
        OSSO_ABOOK_NAME_ORDER_LAST_SPACE,
        OSSO_ABOOK_NAME_ORDER_NICK,
} OssoABookNameOrder;

/**
 * OssoABookVoicemailNumber:
 * @phone_number:  the phone number, e.g. "+491725500"
 * @operator_id:   the operator ID, e.g. "26202"
 * @operator_name: the operator name, e.g. "Vodafone.de"
 *
 * Description of a voicemail box number. The @operator_id consists of the
 * operator's mobile country code (MCC) and the mobile network code (MNC).
 * It is equal to the first five numbers of the IMSI numbers issued by the
 * operator.
 */
typedef struct {
        char *phone_number;
        char *operator_id;
        char *operator_name;
} OssoABookVoicemailNumber;


OssoABookVoicemailNumber *
osso_abook_voicemail_number_new               (const char               *phone_number,
                                               const char               *operator_id,
                                               const char               *operator_name);

void
osso_abook_voicemail_number_free              (OssoABookVoicemailNumber *number);

void
osso_abook_voicemail_number_list_free         (GSList                   *list);


GConfClient *
osso_abook_get_gconf_client                   (void);


OssoABookContactOrder
osso_abook_settings_get_contact_order         (void);

GSList *
osso_abook_settings_get_home_applets          (void);

OssoABookNameOrder
osso_abook_settings_get_name_order            (void);

const char *
osso_abook_settings_get_picture_folder        (void);

GSList *
osso_abook_settings_get_voicemail_numbers     (void);

gboolean
osso_abook_settings_set_contact_order         (OssoABookContactOrder order);

gboolean
osso_abook_settings_set_home_applets          (GSList               *list);

gboolean
osso_abook_settings_set_name_order            (OssoABookNameOrder    order);

gboolean
osso_abook_settings_set_voicemail_numbers     (GSList               *numbers);

#ifndef OSSO_ABOOK_DISABLE_DEPRECATED
G_GNUC_DEPRECATED gboolean
osso_abook_settings_get_self_avatar_imported  (void);

G_GNUC_DEPRECATED gboolean
osso_abook_settings_set_self_avatar_imported  (gboolean              imported);
#endif

G_END_DECLS

#endif /* __OSSO_ABOOK_SETTINGS_H__ */
