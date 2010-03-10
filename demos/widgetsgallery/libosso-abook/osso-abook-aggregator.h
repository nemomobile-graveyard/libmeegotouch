/*
 * This file is part of libosso-abook
 *
 * Copyright (C) 2008-2009 Nokia Corporation. All rights reserved.
 *
 * Contact: Joergen Scheibengruber <jorgen.scheibengruber@nokia.com>
 */

#ifndef __OSSO_ABOOK_AGGREGATOR_H__
#define __OSSO_ABOOK_AGGREGATOR_H__

#include "osso-abook-roster.h"
#include "osso-abook-contact-filter.h"
#include "osso-abook-roster-manager.h"

G_BEGIN_DECLS

#define OSSO_ABOOK_TYPE_AGGREGATOR \
                (osso_abook_aggregator_get_type ())
#define OSSO_ABOOK_AGGREGATOR(obj) \
                (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                 OSSO_ABOOK_TYPE_AGGREGATOR, \
                 OssoABookAggregator))
#define OSSO_ABOOK_AGGREGATOR_CLASS(cls) \
                (G_TYPE_CHECK_CLASS_CAST ((cls), \
                 OSSO_ABOOK_TYPE_AGGREGATOR, \
                 OssoABookAggregatorClass))
#define OSSO_ABOOK_IS_AGGREGATOR(obj) \
                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                 OSSO_ABOOK_TYPE_AGGREGATOR))
#define OSSO_ABOOK_IS_AGGREGATOR_CLASS(obj) \
                (G_TYPE_CHECK_CLASS_TYPE ((obj), \
                 OSSO_ABOOK_TYPE_AGGREGATOR))
#define OSSO_ABOOK_AGGREGATOR_GET_CLASS(obj) \
                (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                 OSSO_ABOOK_TYPE_AGGREGATOR, \
                 OssoABookAggregatorClass))

/**
 * OssoABookAggregatorState:
 * @OSSO_ABOOK_AGGREGATOR_PENDING: the aggregator is not yet running
 * @OSSO_ABOOK_AGGREGATOR_RUNNING: the aggregator is running
 * @OSSO_ABOOK_AGGREGATOR_MASTERS_READY: the master contacts are ready
 * @OSSO_ABOOK_AGGREGATOR_ROSTERS_READY: the roster contacts are ready
 * @OSSO_ABOOK_AGGREGATOR_READY: The aggregator is ready
 *
 * Flags representing the current state of an aggregator
 */
typedef enum {
        OSSO_ABOOK_AGGREGATOR_PENDING       = (0),
        OSSO_ABOOK_AGGREGATOR_RUNNING       = (1 << 0),
        OSSO_ABOOK_AGGREGATOR_MASTERS_READY = (1 << 1),
        OSSO_ABOOK_AGGREGATOR_ROSTERS_READY = (1 << 2),
        OSSO_ABOOK_AGGREGATOR_READY         = (OSSO_ABOOK_AGGREGATOR_MASTERS_READY | OSSO_ABOOK_AGGREGATOR_ROSTERS_READY),
} OssoABookAggregatorState;

/**
 * OssoABookContactPredicate:
 * @contact: a contact to operate on
 * @user_data: additional user data passed to the function
 *
 * A function that can be used to select or reject a contact.
 */
typedef gboolean (*OssoABookContactPredicate) (OssoABookContact *contact,
                                               gpointer user_data);

/**
 * OssoABookAggregator:
 *
 * All the fields of this structure are private to the object's
 * implementation and should never be accessed directly.
 */
struct _OssoABookAggregator {
        OssoABookRoster parent;
};

struct _OssoABookAggregatorClass {
        OssoABookRosterClass parent_class;

        void     (* roster_sequence_complete) (OssoABookAggregator *aggregator,
                                               OssoABookRoster     *roster,
                                               EBookViewStatus      status);
        gboolean (* ebook_status)             (OssoABookAggregator *aggregator,
                                               EBookStatus          status);
};

GType
osso_abook_aggregator_get_type             (void) G_GNUC_CONST;

OssoABookRoster *
osso_abook_aggregator_new                  (EBook                  *book,
                                            GError                **error);

OssoABookRoster *
osso_abook_aggregator_new_with_query       (EBook                  *book,
                                            EBookQuery             *query,
                                            GList                  *requested_fields,
                                            int                     max_results,
                                            GError                **error);

OssoABookRoster *
osso_abook_aggregator_new_with_view        (EBookView              *view);

OssoABookRoster *
osso_abook_aggregator_get_default          (GError                **error);

OssoABookAggregatorState
osso_abook_aggregator_get_state            (OssoABookAggregator    *aggregator);

GList *
osso_abook_aggregator_lookup               (OssoABookAggregator    *aggregator,
                                            const char             *uid);

G_GNUC_WARN_UNUSED_RESULT GList *
osso_abook_aggregator_resolve_master_contacts
                                           (OssoABookAggregator    *aggregator,
                                            OssoABookContact       *contact);

G_GNUC_WARN_UNUSED_RESULT GList *
osso_abook_aggregator_list_master_contacts (OssoABookAggregator    *aggregator);

G_GNUC_WARN_UNUSED_RESULT GList *
osso_abook_aggregator_list_roster_contacts (OssoABookAggregator    *aggregator);

unsigned
osso_abook_aggregator_get_master_contact_count
                                           (OssoABookAggregator    *aggregator);

G_GNUC_WARN_UNUSED_RESULT GList *
osso_abook_aggregator_find_contacts_full   (OssoABookAggregator    *aggregator,
                                            OssoABookContactPredicate
                                                                    predicate,
                                            gpointer                user_data);

G_GNUC_WARN_UNUSED_RESULT GList *
osso_abook_aggregator_find_contacts        (OssoABookAggregator    *aggregator,
                                            EBookQuery             *query);

G_GNUC_WARN_UNUSED_RESULT GList *
osso_abook_aggregator_find_contacts_for_phone_number
                                           (OssoABookAggregator    *aggregator,
                                            const char             *phone_number,
                                            gboolean                fuzzy_match);

G_GNUC_WARN_UNUSED_RESULT GList *
osso_abook_aggregator_find_contacts_for_im_contact
                                           (OssoABookAggregator    *aggregator,
                                            const char             *username,
                                            McAccount              *account);

G_GNUC_WARN_UNUSED_RESULT GList *
osso_abook_aggregator_find_contacts_for_email_address
                                           (OssoABookAggregator    *aggregator,
                                            const char             *address);

OssoABookRosterManager *
osso_abook_aggregator_get_roster_manager   (OssoABookAggregator    *aggregator);

void
osso_abook_aggregator_set_roster_manager   (OssoABookAggregator    *aggregator,
                                            OssoABookRosterManager *manager);

void
osso_abook_aggregator_add_filter           (OssoABookAggregator    *aggregator,
                                            OssoABookContactFilter *filter);

void
osso_abook_aggregator_remove_filter        (OssoABookAggregator    *aggregator,
                                            OssoABookContactFilter *filter);

G_END_DECLS

#endif /* __OSSO_ABOOK_AGGREGATOR_H__ */
