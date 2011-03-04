TEMPLATE    = subdirs

include(../mkspecs/common.pri)

SUBDIRS     = \
 mt_mbutton \
 mt_mbuttongroup \
 mt_mimagewidget \
 mt_mlist \
 mt_mmenu \
 mt_mtoolbar \
 mt_mlabel \
 \
 pt_mapplication \
 pt_mbutton \
 pt_mcomponentcache \
 pt_mcomponentdata \
 pt_mcomponentdata2 \
 pt_mgles2renderer \
 pt_mimagewidget \
 pt_mmenu \
 pt_minimalmapplication \
 pt_minimalqtapplication \
 pt_mlayout \
 pt_mlocationdatabase \
 pt_mprogressindicator \
 pt_qapplication \
 pt_mlabel \
 pt_mlocale \
 pt_mslider \
 pt_mstylesheet \
 pt_mtheme \
 pt_mtoolbar \
 pt_widgetsgallery \
 pt_mwidgetcontroller \
 pt_mscalableimage \

contains(DEFINES, HAVE_ICU) {
SUBDIRS += \
 pt_mcalendar \
 pt_mcharsetdetector
}

include(shell.pri)

