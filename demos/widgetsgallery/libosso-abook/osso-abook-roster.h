/*
 * This file is part of libosso-abook
 *
 * Copyright (C) 2008-2009 Nokia Corporation. All rights reserved.
 *
 * Contact: Joergen Scheibengruber <jorgen.scheibengruber@nokia.com>
 */

#ifndef __OSSO_ABOOK_ROSTER_H__
#define __OSSO_ABOOK_ROSTER_H__

#include "osso-abook-caps.h"
#include "osso-abook-settings.h"

#include <libebook/e-book.h>
#include <libmcclient/mc-account.h>

G_BEGIN_DECLS

#define OSSO_ABOOK_TYPE_ROSTER \
           (osso_abook_roster_get_type ())
#define OSSO_ABOOK_ROSTER(obj) \
          (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
           OSSO_ABOOK_TYPE_ROSTER, \
           OssoABookRoster))
#define OSSO_ABOOK_ROSTER_CLASS(cls) \
          (G_TYPE_CHECK_CLASS_CAST ((cls), \
           OSSO_ABOOK_TYPE_ROSTER, \
           OssoABookRosterClass))
#define OSSO_ABOOK_IS_ROSTER(obj) \
          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
           OSSO_ABOOK_TYPE_ROSTER))
#define OSSO_ABOOK_IS_ROSTER_CLASS(obj) \
          (G_TYPE_CHECK_CLASS_TYPE ((obj), \
           OSSO_ABOOK_TYPE_ROSTER))
#define OSSO_ABOOK_ROSTER_GET_CLASS(obj) \
          (G_TYPE_INSTANCE_GET_CLASS ((obj), \
           OSSO_ABOOK_TYPE_ROSTER, \
           OssoABookRosterClass))

/**
 * OssoABookRoster:
 *
 * All the fields of this structure are private to the object's
 * implementation and should never be accessed directly.
 */
struct _OssoABookRoster {
        GObject parent_instance;
};

struct _OssoABookRosterClass {
        GObjectClass parent_class;

        /*< signals >*/
        void (* contacts_added)    (OssoABookRoster   *roster,
                                    OssoABookContact **contacts);
        void (* contacts_changed)  (OssoABookRoster   *roster,
                                    OssoABookContact **contacts);
        void (* contacts_removed)  (OssoABookRoster   *roster,
                                    const char       **uids);
        void (* sequence_complete) (OssoABookRoster   *roster,
                                    EBookViewStatus    status);
        void (* status_message)    (OssoABookRoster   *roster,
                                    const char        *message);

        /*< virtual methods >*/
        void (* start)             (OssoABookRoster   *roster);
        void (* stop)              (OssoABookRoster   *roster);
        void (* set_book_view)     (OssoABookRoster   *roster,
                                    EBookView         *book_view);
};

GType
osso_abook_roster_get_type         (void) G_GNUC_CONST;

OssoABookRoster *
osso_abook_roster_new              (const char        *name,
                                    EBookView         *book_view,
                                    const char        *vcard_field);

const char *
osso_abook_roster_get_name         (OssoABookRoster   *roster);

McAccount *
osso_abook_roster_get_account      (OssoABookRoster   *roster);

McProfile *
osso_abook_roster_get_profile      (OssoABookRoster   *roster);

const char *
osso_abook_roster_get_vcard_field  (OssoABookRoster   *roster);

OssoABookCapsFlags
osso_abook_roster_get_capabilities (OssoABookRoster   *roster);

EBookView *
osso_abook_roster_get_book_view    (OssoABookRoster   *roster);

const char *
osso_abook_roster_get_book_uri     (OssoABookRoster   *roster);

EBook *
osso_abook_roster_get_book         (OssoABookRoster   *roster);

void
osso_abook_roster_set_name_order   (OssoABookRoster   *roster,
                                    OssoABookNameOrder order);

OssoABookNameOrder
osso_abook_roster_get_name_order   (OssoABookRoster   *roster);

gboolean
osso_abook_roster_is_running       (OssoABookRoster   *roster);

void
osso_abook_roster_start            (OssoABookRoster   *roster);

void
osso_abook_roster_stop             (OssoABookRoster   *roster);

G_END_DECLS

#endif /* __OSSO_ABOOK_ROSTER_H__ */
