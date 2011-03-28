TEMPLATE    = subdirs
#SUBDIRS     = $$system(ls -1d ut_*/)

include(../mkspecs/common.pri)

HEADERS += \
    utils.h \

SUBDIRS     =  \
    ft_mservicefwgen \
    ft_mstylesheetparser \
    ft_mstylesheet \
    ft_mdialog \
    ft_mprogressindicator \
    ft_mscenemanager \
    ft_mgconfitem \
    ft_theme \
    ft_mbutton \
    ft_mslider \
    ft_mscalableimage \
    ut_allwidgets \
#     ut_mapplication \ // Depends on private class
     ut_madvancedlistitem \
     ut_mapplicationpage \
     ut_mapplicationwindow \
     ut_mapplicationservice \
     ut_mbasiclistitem \
     ut_mcomponentcache \
     ut_minputmethodstate \
     ut_minputwidgetrelocator \
     ut_mscrollchain \
     ut_mbutton \
     ut_mbuttonview \
     ut_mbuttongroup \
     ut_mbubbleitem \
     ut_mcombobox \
     ut_mdetailedlistitem \
     ut_mdialog \
     ut_mdialogview \
     ut_mmessagebox \
     ut_mnavigationbar \
     ut_mnavigationbarview \
     ut_mlist \
     ut_mlistviewgroupheader \
     ut_mlistviewmulticolumn \
     ut_mlistviewseparators \
     ut_mlistfilter \
     ut_mlistindexfloatingview \
     ut_mflowlayoutpolicy \
     ut_mfreestylelayoutpolicy \
     ut_mgridlayoutpolicy \
     ut_mimagedirectory \
     ut_mimagewidget \
     ut_mlayout \
     ut_mlinearlayoutpolicy \
     ut_mapplicationmenu \
     ut_mapplicationmenuview \
     ut_mobjectmenu \
     ut_mobjectmenuview \
     ut_morientationtracker \
     ut_moverlay \
     ut_mpannableviewport \
     ut_mpannablewidget \
     ut_mpositionindicator \
     ut_mpositionindicatorview \
     ut_mpopuplist \
     ut_mprogressindicator \
     ut_mprogressindicatorspinnerview \
     ut_mprogressindicatorbarview \
     ut_mrichtextedit \
     ut_mscenemanager \
     ut_mseparator \
     ut_mseparatorview \
     ut_mslider \
     ut_mseekbar \
     ut_msliderview \
    ut_mtextedit \
    ut_mtexteditview \
    ut_mtextmagnifier \
    ut_meditortoolbar \
     ut_mthemedaemonprotocol \
    ut_mtheme \
    ut_mtoolbar \
    ut_mtoolbarview \
    ut_mwidget \
    ut_mwidgetview \
    ut_mwindow \
    ut_mphysics2dpanning \
    ut_mlabel \
    ut_mlogicalvalues \
    ut_mdesktopentry \
    ut_mextensionwatcher \
    ut_mcontainer \
    ut_mcontentitem \
    ut_minfobanner \
    ut_mbanner \
    ut_mbannerview \
    ut_mwidgetcontroller \
    ut_mservicemapper \
    ut_mservicefwbaseif \
    ut_mscene \
    ut_mappletalivemessagerequest \
    ut_mappletalivemessageresponse \
    ut_mappletbutton \
    ut_mappletcommunicator \
    ut_mappletserver \
    ut_mappletclient \
    ut_mapplethandle \
    ut_mappletsharedmutex \
    ut_mappletinstancemanager \
    ut_mappletinstancedata \
    ut_mappletinventory \
    ut_mappletinventoryview \
    ut_mappletmetadata \
    ut_mappletmessage \
    ut_mappletmessagefactory \
    ut_mappletmousemessage \
    ut_mappletorientationmessage \
    ut_mappletrunner \
    ut_mappletsetgeometrymessage \
    ut_mappletpixmaptakenintousemessage \
    ut_mappletvisibilitymessage \
    ut_mappletupdategeometrymessage \
    ut_mappletpixmapmodifiedmessage \
    ut_mappletcancelmessage \
    ut_mappletloader \
    ut_mappletid \
    ut_mappletsettings \
    ut_mappletsettingsdialog \
    ut_mbasicsheetheader \
    ut_mbasicsheetheaderview \
    ut_mfiledatastore \
    ut_maggregatedataaccess \
    ut_mmashupcanvas \
    ut_mmashupcanvasview \
    ut_msubdatastore \
    ut_mremoteaction \
    ut_mserviceaction \
    ut_mserviceinvoker \
    ut_mscenewindow \
    ut_mnotification \
    ut_mnotificationmanager \
    ut_msettingslanguageparser \
    ut_msettingslanguagebinary \
    ut_msettingslanguagewidget \
    ut_msettingslanguagewidgetfactory \
    ut_msettingslanguagesettingsfactory \
    ut_msettingslanguageselectionfactory \
    ut_msettingslanguagebooleanfactory \
    ut_msettingslanguagetextfactory \
    ut_msettingslanguageintegerfactory \
    ut_msettingslanguageselectioncontroller \
    ut_msettingslanguagebooleancontroller \
    ut_msettingslanguagetextcontroller \
    ut_msettingslanguageintegercontroller \
    ut_msettingslanguagesettings \
    ut_msettingslanguageselection \
    ut_msettingslanguageboolean \
    ut_msettingslanguagetext \
    ut_msettingslanguageoption \
    ut_msettingslanguageinteger \
    ut_mextendingbackgroundview \
    ut_mgconfdatastore \
    ut_mshareddata \
    ut_mappletobjectmenurequestmessage \
    ut_mappletobjectmenumessage \
    ut_mappletobjectmenuactionselectedmessage \
    ut_mfeedback \
    ut_mfeedbackplayer \
    ut_mcompleter \
    ut_translations \
    ut_translations/translations-tr \
    ut_translations/translations-qttrid \
    ut_mextensionarea \
    ut_mextensionrunner \
    ut_mapplicationextensionloader \
    ut_mapplicationextensionrunner \
    ut_mapplicationextensionarea \
    ut_mapplicationextensionareaview \
    ut_mapplicationextensionmanager \
    ut_mapplicationextensionmetadata \
    ut_mextensionhandle \
    ut_mextensionhandleview \
    ut_mtapandholdrecognizer \
    ut_mservicefwgen \
    ut_mpanrecognizer \
    ut_mstatusbarview \
    ut_mabstractitemmodel \
    ut_phonenumberformatting \
    ut_mlocationdatabase \
    ut_mlocalthemedaemon \
    ut_mremotethemedaemon \
    ut_mswiperecognizer \
    ut_mstylablewidget \
    ut_mdynamicpropertywatcher \
    ut_mtapstatemachine \
    ut_mcommonpixmaps \
    ut_mgles2renderer \

# enable only when we have icu available

contains(DEFINES, HAVE_ICU) {
SUBDIRS += \
    ft_breakiterator \
    ft_locales \
    ft_localedata \
    ft_mlocalebuckets \
    ft_numbers \
    ft_sorting \
    ft_mcharsetdetector \
    ut_mcalendar \
    ut_mappletinstantiator \
    ft_mmoc
}

QMAKE_STRIP = echo
include(shell.pri)
include(runtests.pri)


check.target = check
check.CONFIG = recursive
QMAKE_EXTRA_TARGETS += check


check-xml.target = check-xml
check-xml.CONFIG = recursive
QMAKE_EXTRA_TARGETS += check-xml

QMAKE_CLEAN += **/*.log.xml ./coverage.log.xml **/*.log
