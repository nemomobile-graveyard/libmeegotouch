/*
 * This file is part of libosso-abook
 *
 * Copyright (C) 2008-2009 Nokia Corporation. All rights reserved.
 *
 * Contact: Joergen Scheibengruber <jorgen.scheibengruber@nokia.com>
 */

#ifndef __OSSO_ABOOK_ROSTER_MANAGER_H__
#define __OSSO_ABOOK_ROSTER_MANAGER_H__

#include "osso-abook-types.h"
#include <glib-object.h>

G_BEGIN_DECLS

#define OSSO_ABOOK_TYPE_ROSTER_MANAGER \
                (osso_abook_roster_manager_get_type ())
#define OSSO_ABOOK_ROSTER_MANAGER(obj) \
                (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                 OSSO_ABOOK_TYPE_ROSTER_MANAGER, \
                 OssoABookRosterManager))
#define OSSO_ABOOK_IS_ROSTER_MANAGER(obj) \
                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                 OSSO_ABOOK_TYPE_ROSTER_MANAGER))
#define OSSO_ABOOK_ROSTER_MANAGER_GET_IFACE(obj) \
                (G_TYPE_INSTANCE_GET_INTERFACE ((obj), \
                 OSSO_ABOOK_TYPE_ROSTER_MANAGER, \
                 OssoABookRosterManagerIface))

/* This typedef cannot go in osso-abook-types.h or gtk-doc will not generate
 * cross-reference links correctly */
/**
 * OssoABookRosterManager:
 *
 * Dummy type for the #OssoABookRosterManager interface.
 */
typedef struct _OssoABookRosterManager OssoABookRosterManager;

/**
 * OssoABookRosterManagerIface:
 * @roster_created: virtual method for #OssoABookRosterManager::roster-created
 * @roster_removed: virtual method for #OssoABookRosterManager::roster-removed
 * @start: virtual method for osso_abook_roster_manager_start()
 * @stop: virtual method for osso_abook_roster_manager_stop()
 * @list_rosters: virtual method for osso_abook_roster_manager_list_rosters()
 * @get_roster: virtual method for osso_abook_roster_manager_get_roster()
 *
 * Virtual methods of the #OssoABookRosterManager interface.
 */
struct _OssoABookRosterManagerIface {
        /*< private >*/
        GTypeInterface g_iface;

        /*< public >*/
        void     (* roster_created) (OssoABookRosterManager *manager,
                                     OssoABookRoster        *roster);

        void     (* roster_removed) (OssoABookRosterManager *manager,
                                     OssoABookRoster        *roster);

        void     (* start)          (OssoABookRosterManager *manager);
        void     (* stop)           (OssoABookRosterManager *manager);

        GList *  (* list_rosters)   (OssoABookRosterManager *manager);

        OssoABookRoster *
                 (* get_roster)     (OssoABookRosterManager *manager,
                                     const char             *account_name);
};

GType
osso_abook_roster_manager_get_type     (void) G_GNUC_CONST;

void
osso_abook_roster_manager_start        (OssoABookRosterManager *manager);

void
osso_abook_roster_manager_stop         (OssoABookRosterManager *manager);

gboolean
osso_abook_roster_manager_is_running   (OssoABookRosterManager *manager);

G_GNUC_WARN_UNUSED_RESULT GList *
osso_abook_roster_manager_list_rosters (OssoABookRosterManager *manager);

OssoABookRoster *
osso_abook_roster_manager_get_roster   (OssoABookRosterManager *manager,
                                        const char             *account_name);

OssoABookRosterManager *
osso_abook_roster_manager_get_default  (void);

G_END_DECLS

#endif /* __OSSO_ABOOK_AGGREGATOR_H__ */
