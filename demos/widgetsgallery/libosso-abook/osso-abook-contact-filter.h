/*
 * This file is part of libosso-abook
 *
 * Copyright (C) 2008-2009 Nokia Corporation. All rights reserved.
 *
 * Contact: Joergen Scheibengruber <jorgen.scheibengruber@nokia.com>
 */

#ifndef __OSSO_ABOOK_CONTACT_FILTER_H__
#define __OSSO_ABOOK_CONTACT_FILTER_H__

#include "osso-abook-types.h"
#include <glib-object.h>

G_BEGIN_DECLS

#define OSSO_ABOOK_TYPE_CONTACT_FILTER \
                (osso_abook_contact_filter_get_type ())
#define OSSO_ABOOK_CONTACT_FILTER(obj) \
                (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                 OSSO_ABOOK_TYPE_CONTACT_FILTER, \
                 OssoABookContactFilter))
#define OSSO_ABOOK_IS_CONTACT_FILTER(obj) \
                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                 OSSO_ABOOK_TYPE_CONTACT_FILTER))
#define OSSO_ABOOK_CONTACT_FILTER_GET_IFACE(obj) \
                (G_TYPE_INSTANCE_GET_INTERFACE ((obj), \
                 OSSO_ABOOK_TYPE_CONTACT_FILTER, \
                 OssoABookContactFilterIface))

typedef enum {
        OSSO_ABOOK_CONTACT_FILTER_ONLY_READS_UID = (1 << 0),
} OssoABookContactFilterFlags;

/* This typedef cannot go in osso-abook-types.h or gtk-doc will not generate
 * cross-reference links correctly */
/**
 * OssoABookContactFilter:
 *
 * Dummy type for the #OssoABookAvatar interface.
 */
typedef struct _OssoABookContactFilter OssoABookContactFilter;

/**
 * OssoABookContactFilterIface:
 * @accept: virtual method for osso_abook_contact_filter_accept()
 * @contact_filter_changed: virtual method for OssoABookContactFilter::contact-filter-changed
 * @get_flags: capabilities of the filter
 *
 * Virtual methods of the #OssoABookContactFilter interface.
 */
struct _OssoABookContactFilterIface {
        /*< private >*/
        GTypeInterface parent;

        /*< public >*/
        gboolean                    (* accept)                 (OssoABookContactFilter *filter,
                                                                const char             *uid,
                                                                OssoABookContact       *contact);
        OssoABookContactFilterFlags (* get_flags)              (OssoABookContactFilter *filter);

        void                        (* contact_filter_changed) (OssoABookContactFilter *filter);
};

GType
osso_abook_contact_filter_get_type  (void) G_GNUC_CONST;

gboolean
osso_abook_contact_filter_accept    (OssoABookContactFilter *filter,
                                     const char             *uid,
                                     OssoABookContact       *contact);

OssoABookContactFilterFlags
osso_abook_contact_filter_get_flags (OssoABookContactFilter *filter);

G_END_DECLS

#endif /* __OSSO_ABOOK_CONTACT_FILTER_H__ */
