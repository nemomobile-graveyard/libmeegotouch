/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*- */
/*
 * This file is part of libosso-abook
 *
 * Copyright (C) 2007-2009 Nokia Corporation. All rights reserved.
 *
 * Contact: Joergen Scheibengruber <jorgen.scheibengruber@nokia.com>
 */

#ifndef __OSSO_ABOOK_AVATAR_H__
#define __OSSO_ABOOK_AVATAR_H__

#include "osso-abook-types.h"

#include <glib-object.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

G_BEGIN_DECLS

#define OSSO_ABOOK_TYPE_AVATAR \
                (osso_abook_avatar_get_type ())
#define OSSO_ABOOK_AVATAR(obj) \
                (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                 OSSO_ABOOK_TYPE_AVATAR, \
                 OssoABookAvatar))
#define OSSO_ABOOK_IS_AVATAR(obj) \
                (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                 OSSO_ABOOK_TYPE_AVATAR))
#define OSSO_ABOOK_AVATAR_GET_IFACE(klass) \
                (G_TYPE_INSTANCE_GET_INTERFACE ((klass), \
                 OSSO_ABOOK_TYPE_AVATAR, \
                 OssoABookAvatarIface))

/* This typedef cannot go in osso-abook-types.h or gtk-doc will not generate
 * cross-reference links correctly */
/**
 * OssoABookAvatar:
 *
 * Dummy type for the #OssoABookAvatar interface.
 */
typedef struct _OssoABookAvatar OssoABookAvatar;

/**
 * OssoABookAvatarIface:
 * @get_image_scaled: virtual method for osso_abook_avatar_get_image_scaled()
 * @get_server_image_scaled: virtual method for osso_abook_avatar_get_server_image_scaled()
 * @get_fallback_icon_name: virtual method for osso_abook_avatar_get_fallback_icon_name()
 * @load_error: virtual method for #OssoABookAvatar::load-error
 *
 * Virtual methods of the #OssoABookAvatar interface.
 */
struct _OssoABookAvatarIface {
        /*< private >*/
        GTypeInterface parent;

        /*< public >*/
        GdkPixbuf *  (* get_image_scaled)        (OssoABookAvatar *avatar,
                                                  int              width,
                                                  int              height,
                                                  gboolean         crop);

        GdkPixbuf *  (* get_server_image_scaled) (OssoABookAvatar *avatar,
                                                  int              width,
                                                  int              height,
                                                  gboolean         crop);

        const char * (* get_fallback_icon_name)  (OssoABookAvatar *avatar);

        void         (* load_error)              (OssoABookAvatar *avatar,
                                                  const GError    *error);
};

GType
osso_abook_avatar_get_type                (void) G_GNUC_CONST;

gpointer
osso_abook_avatar_get_image_token         (OssoABookAvatar *avatar);

GdkPixbuf *
osso_abook_avatar_get_image               (OssoABookAvatar *avatar);

GdkPixbuf *
osso_abook_avatar_get_image_scaled        (OssoABookAvatar *avatar,
                                           int              width,
                                           int              height,
                                           gboolean         crop);

GdkPixbuf *
osso_abook_avatar_get_image_rounded       (OssoABookAvatar *avatar,
                                           int              width,
                                           int              height,
                                           gboolean         crop,
                                           int              radius,
                                           const guint8     border_color[4]);

GdkPixbuf *
osso_abook_avatar_get_server_image        (OssoABookAvatar *avatar);

GdkPixbuf *
osso_abook_avatar_get_server_image_scaled (OssoABookAvatar *avatar,
                                           int              width,
                                           int              height,
                                           gboolean         crop);

const char *
osso_abook_avatar_get_fallback_icon_name  (OssoABookAvatar *avatar);

gboolean
osso_abook_avatar_is_user_selected        (OssoABookAvatar *avatar);

gboolean
osso_abook_avatar_is_done_loading         (OssoABookAvatar *avatar);

G_END_DECLS

#endif /* __OSSO_ABOOK_AVATAR_H__ */
