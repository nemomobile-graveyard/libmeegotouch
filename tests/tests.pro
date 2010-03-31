TEMPLATE    = subdirs
#SUBDIRS     = $$system(ls -1d ut_*/)

include(../mkspecs/common.pri)

SUBDIRS     =  \
    ft_duiservicefwgen \
    ft_duistylesheetparser \
    ft_duistylesheet \
    ft_duidialog \
    ft_duiprogressindicator \
    ft_duiscenemanager \
    ft_duigconfitem \
    ft_theme \
#    ft_duibutton \
    ft_duislider \
    ft_duiscalableimage \
#     ut_duiapplication \ // Depends on private class
     ut_duiapplicationpage \
     ut_duiapplicationwindow \
     ut_duiapplicationservice \
     ut_duiinputmethodstate \
     ut_duibutton \
     ut_duibuttonview \
     ut_duibuttongroup \
     ut_duicombobox \
     ut_duidialog \
     ut_duidialogview \
     ut_duinavigationbar \
    ut_duilistviewgroupheader \
    ut_duilistviewmulticolumn \
    ut_duilistviewseparators \
     ut_duiflowlayoutpolicy \
     ut_duifreestylelayoutpolicy \
     ut_duigridlayoutpolicy \
#     ut_duiimagewidget \ // CITA problems
     ut_duilayout \
     ut_duilinearlayoutpolicy \
     ut_duiapplicationmenu \
     ut_duiapplicationmenuview \
#     ut_duiobjectmenu \
     ut_duioverlay \
     ut_duipannableviewport \
     ut_duipannablewidget \
     ut_duipositionindicator \
     ut_duipositionindicatorview \
     ut_duipopuplist \
     ut_duiscenemanager \
     ut_duislider \
     ut_duiseekbar \
     ut_duisliderview \
    ut_duitextedit \
    ut_duitexteditview \
#    ut_duitheme \
    ut_duitoolbar \
    ut_duitoolbarview \
    ut_duiwidget \
    ut_duiwidgetview \
    ut_duiwindow \
    ut_duiphysics2dpanning \
    ut_duilabel \
    ut_duilogicalvalues \
    ut_duidesktopentry \
    ut_duicontainer \
    ut_duiinfobanner \
    ut_duiwidgetcontroller \
    ut_duiservicemapper \
    ut_duiservicefwbaseif \
    ut_duiscene \
    ut_duiappletalivemessagerequest \
    ut_duiappletalivemessageresponse \
    ut_duiappletbutton \
    ut_duiappletcommunicator \
    ut_duiappletserver \
    ut_duiappletclient \
    ut_duiapplethandle \
    ut_duiapplethandleview \
    ut_duiappletsharedmutex \
    ut_duiappletinstancemanager \
    ut_duiappletinstancedata \
    ut_duiappletinventory \
#    ut_duiappletinventoryview \ // Depends on private class
    ut_duiappletmetadata \
    ut_duiappletmessage \
    ut_duiappletmessagefactory \
    ut_duiappletmousemessage \
    ut_duiappletorientationmessage \
    ut_duiappletrunner \
    ut_duiappletsetgeometrymessage \
    ut_duiappletpixmaptakenintousemessage \
    ut_duiappletvisibilitymessage \
    ut_duiappletupdategeometrymessage \
    ut_duiappletpixmapmodifiedmessage \
    ut_duiappletcancelmessage \
    ut_duiappletloader \
    ut_duiappletid \
    ut_duiappletsettings \
    ut_duiappletsettingsdialog \
    ut_duifiledatastore \
    ut_duiaggregatedataaccess \
    ut_duimashupcanvas \
    ut_duimashupcanvasview \
    ut_duisubdatastore \
    ut_duiremoteaction \
    ut_duiserviceaction \
    ut_duiserviceinvoker \
    ut_duiscenewindow \
    ut_duinotification \
    ut_duinotificationmanager \
    ut_duisettingslanguageparser \
    ut_duisettingslanguagebinary \
    ut_duisettingslanguagewidget \
    ut_duisettingslanguagewidgetfactory \
    ut_duisettingslanguagesettingsfactory \
    ut_duisettingslanguageselectionfactory \
    ut_duisettingslanguagebooleanfactory \
    ut_duisettingslanguagetextfactory \
    ut_duisettingslanguageintegerfactory \
    ut_duisettingslanguageselectioncontroller \
    ut_duisettingslanguagebooleancontroller \
    ut_duisettingslanguagetextcontroller \
    ut_duisettingslanguageintegercontroller \
    ut_duisettingslanguagesettings \
    ut_duisettingslanguageselection \
    ut_duisettingslanguageboolean \
    ut_duisettingslanguagetext \
    ut_duisettingslanguageoption \
    ut_duisettingslanguageinteger \
    ut_duiextendingbackgroundview \
    ut_duigconfdatastore \
    ut_duishareddata \
    ut_duiappletobjectmenurequestmessage \
    ut_duiappletobjectmenumessage \
    ut_duiappletobjectmenuactionselectedmessage \
    ut_duifeedback \
    ut_duifeedbackplayer \
    ut_duicompleter \
    ut_translations \
    ut_translations/translations-tr \
    ut_translations/translations-qttrid \
    ut_duiextensionarea \
    ut_duiextensionrunner \
    ut_duiapplicationextensionloader \
    ut_duiapplicationextensionrunner \
    ut_duiapplicationextensionarea \
    ut_duiapplicationextensionareaview \
    ut_duiapplicationextensionmanager \
    ut_duiextensionhandle \
    ut_duiextensionhandleview \
    ut_duitapandholdrecognizer \
    ut_duivideowidget \
    ut_duiservicefwgen \

# enable only when we have icu available

contains(DEFINES, HAVE_ICU) {
SUBDIRS += \
    ft_breakiterator \
    ft_locales \
    ft_localedata \
    ft_numbers \
    ft_sorting \
    ut_duicalendar \
    ut_duiappletinstantiator
}

contains(DUI_BUILD_PARTS, "plainqt") {
SUBDIRS += \
  ut_qtmaemo6style
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
