/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*- */
/*
 * This file is part of libosso-abook
 *
 * Copyright (C) 2008-2009 Nokia Corporation. All rights reserved.
 *
 * Contact: Joergen Scheibengruber <jorgen.scheibengruber@nokia.com>
 */

#ifndef __OSSO_ABOOK_TYPES_H__
#define __OSSO_ABOOK_TYPES_H__

/* Forward declarations of all structures in libosso-abook.
 * This should help to resolve some cyclic dependency problems.
 */
typedef struct _OssoABookAccountManager                 OssoABookAccountManager;
typedef struct _OssoABookAccountManagerClass            OssoABookAccountManagerClass;
typedef struct _OssoABookAggregator                     OssoABookAggregator;
typedef struct _OssoABookAggregatorClass                OssoABookAggregatorClass;
typedef struct _OssoABookAllGroup                       OssoABookAllGroup;
typedef struct _OssoABookAllGroupClass                  OssoABookAllGroupClass;
typedef struct _OssoABookAllGroupPrivate                OssoABookAllGroupPrivate;
typedef struct _OssoABookAvatarCache                    OssoABookAvatarCache;
typedef struct _OssoABookAvatarCacheClass               OssoABookAvatarCacheClass;
typedef struct _OssoABookAvatarChooserDialog            OssoABookAvatarChooserDialog;
typedef struct _OssoABookAvatarChooserDialogClass       OssoABookAvatarChooserDialogClass;
typedef struct _OssoABookAvatarEditor                   OssoABookAvatarEditor;
typedef struct _OssoABookAvatarEditorClass              OssoABookAvatarEditorClass;
typedef struct _OssoABookAvatarEditorDialog             OssoABookAvatarEditorDialog;
typedef struct _OssoABookAvatarEditorDialogClass        OssoABookAvatarEditorDialogClass;
typedef struct _OssoABookAvatarIface                    OssoABookAvatarIface;
typedef struct _OssoABookAvatarImage                    OssoABookAvatarImage;
typedef struct _OssoABookAvatarImageClass               OssoABookAvatarImageClass;
typedef struct _OssoABookButton                         OssoABookButton;
typedef struct _OssoABookButtonClass                    OssoABookButtonClass;
typedef struct _OssoABookCapsIface                      OssoABookCapsIface;
typedef struct _OssoABookContact                        OssoABookContact;
typedef struct _OssoABookContactChooser                 OssoABookContactChooser;
typedef struct _OssoABookContactChooserClass            OssoABookContactChooserClass;
typedef struct _OssoABookContactClass                   OssoABookContactClass;
typedef struct _OssoABookContactDetailSelector          OssoABookContactDetailSelector;
typedef struct _OssoABookContactDetailSelectorClass     OssoABookContactDetailSelectorClass;
typedef struct _OssoABookContactDetailStore             OssoABookContactDetailStore;
typedef struct _OssoABookContactDetailStoreClass        OssoABookContactDetailStoreClass;
typedef struct _OssoABookContactEditor                  OssoABookContactEditor;
typedef struct _OssoABookContactEditorClass             OssoABookContactEditorClass;
typedef struct _OssoABookContactField                   OssoABookContactField;
typedef struct _OssoABookContactFieldAction             OssoABookContactFieldAction;
typedef struct _OssoABookContactFieldClass              OssoABookContactFieldClass;
typedef struct _OssoABookContactFieldSelector           OssoABookContactFieldSelector;
typedef struct _OssoABookContactFieldSelectorClass      OssoABookContactFieldSelectorClass;
typedef struct _OssoABookContactFilterIface             OssoABookContactFilterIface;
typedef struct _OssoABookContactModel                   OssoABookContactModel;
typedef struct _OssoABookContactModelClass              OssoABookContactModelClass;
typedef struct _OssoABookContactSubscriptions           OssoABookContactSubscriptions;
typedef struct _OssoABookContactSubscriptionsClass      OssoABookContactSubscriptionsClass;
typedef struct _OssoABookContactView                    OssoABookContactView;
typedef struct _OssoABookContactViewClass               OssoABookContactViewClass;
typedef struct _OssoABookFilterModel                    OssoABookFilterModel;
typedef struct _OssoABookFilterModelClass               OssoABookFilterModelClass;
typedef struct _OssoABookFilterModelPrivate             OssoABookFilterModelPrivate;
typedef struct _OssoABookGconfContact                   OssoABookGconfContact;
typedef struct _OssoABookGconfContactClass              OssoABookGconfContactClass;
typedef struct _OssoABookGroup                          OssoABookGroup;
typedef struct _OssoABookGroupClass                     OssoABookGroupClass;
typedef struct _OssoABookGroupPrivate                   OssoABookGroupPrivate;
typedef struct _OssoABookListStore                      OssoABookListStore;
typedef struct _OssoABookListStoreClass                 OssoABookListStoreClass;
typedef struct _OssoABookListStorePrivate               OssoABookListStorePrivate;
typedef struct _OssoABookListStoreRow                   OssoABookListStoreRow;
typedef struct _OssoABookMcAccountModel                 OssoABookMcAccountModel;
typedef struct _OssoABookMcAccountModelClass            OssoABookMcAccountModelClass;
typedef struct _OssoABookMcAccountSelector              OssoABookMcAccountSelector;
typedef struct _OssoABookMcAccountSelectorClass         OssoABookMcAccountSelectorClass;
typedef struct _OssoABookMecardView                     OssoABookMecardView;
typedef struct _OssoABookMecardViewClass                OssoABookMecardViewClass;
typedef struct _OssoABookMenuEntry                      OssoABookMenuEntry;
typedef struct _OssoABookMenuExtension                  OssoABookMenuExtension;
typedef struct _OssoABookMenuExtensionClass             OssoABookMenuExtensionClass;
typedef struct _OssoABookMessageMapping                 OssoABookMessageMapping;
typedef struct _OssoABookPresenceIcon                   OssoABookPresenceIcon;
typedef struct _OssoABookPresenceIconClass              OssoABookPresenceIconClass;
typedef struct _OssoABookPresenceIface                  OssoABookPresenceIface;
typedef struct _OssoABookProfileGroup                   OssoABookProfileGroup;
typedef struct _OssoABookProfileGroupClass              OssoABookProfileGroupClass;
typedef struct _OssoABookRecentGroup                    OssoABookRecentGroup;
typedef struct _OssoABookRecentGroupClass               OssoABookRecentGroupClass;
typedef struct _OssoABookRecentGroupPrivate             OssoABookRecentGroupPrivate;
typedef struct _OssoABookRoster                         OssoABookRoster;
typedef struct _OssoABookRosterClass                    OssoABookRosterClass;
typedef struct _OssoABookRosterManagerIface             OssoABookRosterManagerIface;
typedef struct _OssoABookRowModelIface                  OssoABookRowModelIface;
typedef struct _OssoABookLiveSearch                     OssoABookLiveSearch;
typedef struct _OssoABookLiveSearchClass                OssoABookLiveSearchClass;
typedef struct _OssoABookAlphaShortcuts                 OssoABookAlphaShortcuts;
typedef struct _OssoABookAlphaShortcutsClass            OssoABookAlphaShortcutsClass;
typedef struct _OssoABookSelfContact                    OssoABookSelfContact;
typedef struct _OssoABookSelfContactClass               OssoABookSelfContactClass;
typedef struct _OssoABookSendContactsDialog             OssoABookSendContactsDialog;
typedef struct _OssoABookSendContactsDialogClass        OssoABookSendContactsDialogClass;
typedef struct _OssoABookServiceGroup                   OssoABookServiceGroup;
typedef struct _OssoABookServiceGroupClass              OssoABookServiceGroupClass;
typedef struct _OssoABookServiceGroupPrivate            OssoABookServiceGroupPrivate;
typedef struct _OssoABookSettingsDialog                 OssoABookSettingsDialog;
typedef struct _OssoABookSettingsDialogClass            OssoABookSettingsDialogClass;
typedef struct _OssoABookTemporaryContactDialog         OssoABookTemporaryContactDialog;
typedef struct _OssoABookTemporaryContactDialogClass    OssoABookTemporaryContactDialogClass;
typedef struct _OssoABookTouchContactStarter            OssoABookTouchContactStarter;
typedef struct _OssoABookTouchContactStarterClass       OssoABookTouchContactStarterClass;
typedef struct _OssoABookTreeView                       OssoABookTreeView;
typedef struct _OssoABookTreeViewClass                  OssoABookTreeViewClass;
typedef struct _OssoABookTreeViewPrivate                OssoABookTreeViewPrivate;
typedef struct _OssoABookVoicemailContact               OssoABookVoicemailContact;
typedef struct _OssoABookVoicemailContactClass          OssoABookVoicemailContactClass;
typedef struct _OssoABookVoicemailSelector              OssoABookVoicemailSelector;
typedef struct _OssoABookVoicemailSelectorClass         OssoABookVoicemailSelectorClass;
typedef struct _OssoABookWaitableClosure                OssoABookWaitableClosure;
typedef struct _OssoABookWaitableIface                  OssoABookWaitableIface;

#ifdef OSSO_ABOOK_HAVE_SPEED_DIAL
typedef struct _OssoABookSpeedDialSettings              OssoABookSpeedDialSettings;
typedef struct _OssoABookSpeedDialSettingsClass         OssoABookSpeedDialSettingsClass;
typedef struct _OssoABookSpeedDialSettingsDialog        OssoABookSpeedDialSettingsDialog;
typedef struct _OssoABookSpeedDialSettingsDialogClass   OssoABookSpeedDialSettingsDialogClass;
#endif

#endif
