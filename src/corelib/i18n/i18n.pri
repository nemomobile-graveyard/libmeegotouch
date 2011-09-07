###############################################################################
# Internationalization module
###############################################################################
I18N_SRC_DIR=./i18n
INCLUDEPATH+=./i18n

RESOURCES += ./i18n/i18n.qrc

PUBLIC_HEADERS += \
    $$I18N_SRC_DIR/mbreakiterator.h \
    $$I18N_SRC_DIR/mlocale.h \
    $$I18N_SRC_DIR/mlocalebuckets.h \
    $$I18N_SRC_DIR/mcountry.h \
    $$I18N_SRC_DIR/mcity.h \
    $$I18N_SRC_DIR/mlocationdatabase.h \

contains(DEFINES, HAVE_LIBMLOCALE) {
    SOURCES += \
        $$I18N_SRC_DIR/mbreakiterator_proxy.cpp \
        $$I18N_SRC_DIR/mlocale_proxy.cpp \
        $$I18N_SRC_DIR/mlocalebuckets_proxy.cpp \
        $$I18N_SRC_DIR/mcountry_proxy.cpp \
        $$I18N_SRC_DIR/mcity_proxy.cpp \
        $$I18N_SRC_DIR/mlocationdatabase_proxy.cpp \
        $$I18N_SRC_DIR/mlocalegconfconfigitemfactory.cpp \
        $$I18N_SRC_DIR/mlocalegconfconfigitem.cpp \

    PRIVATE_HEADERS += \
        $$I18N_SRC_DIR/mlocalegconfconfigitemfactory.h \
        $$I18N_SRC_DIR/mlocalegconfconfigitem.h \

} else {
    SOURCES += \
        $$I18N_SRC_DIR/mbreakiterator.cpp \
        $$I18N_SRC_DIR/mlocale.cpp \
        $$I18N_SRC_DIR/mlocalebuckets.cpp \
        $$I18N_SRC_DIR/mcountry.cpp \
        $$I18N_SRC_DIR/mcity.cpp \
        $$I18N_SRC_DIR/mlocationdatabase.cpp \

}

contains(DEFINES, HAVE_ICU) {
    PUBLIC_HEADERS += \
        $$I18N_SRC_DIR/mcalendar.h \
        $$I18N_SRC_DIR/mcollator.h \
        $$I18N_SRC_DIR/mcharsetdetector.h \
        $$I18N_SRC_DIR/mcharsetmatch.h \

    PRIVATE_HEADERS += \
        $$I18N_SRC_DIR/micubreakiterator.h \
        $$I18N_SRC_DIR/micuconversions.h \
        $$I18N_SRC_DIR/mcalendar_p.h \

    contains(DEFINES, HAVE_LIBMLOCALE) {
        SOURCES += \
            $$I18N_SRC_DIR/mcalendar_proxy.cpp \
            $$I18N_SRC_DIR/mcollator_proxy.cpp \
            $$I18N_SRC_DIR/mcharsetdetector_proxy.cpp \
            $$I18N_SRC_DIR/mcharsetmatch_proxy.cpp \

    } else {
        SOURCES += \
            $$I18N_SRC_DIR/mcalendar.cpp \
            $$I18N_SRC_DIR/mcollator.cpp \
            $$I18N_SRC_DIR/micubreakiterator.cpp \
            $$I18N_SRC_DIR/micuconversions.cpp \
            $$I18N_SRC_DIR/mcharsetdetector.cpp \
            $$I18N_SRC_DIR/mcharsetmatch.cpp \

    }
} else {
    PRIVATE_HEADERS += \
        $$I18N_SRC_DIR/mnullbreakiterator.h \

    !contains(DEFINES, HAVE_LIBMLOCALE) {
        SOURCES += \
            $$I18N_SRC_DIR/mnullbreakiterator.cpp

    }
}

contains(DEFINES, HAVE_QMSYSTEM2) {
    CONFIG+=qmsystem2
    QMAKE_CXXFLAGS -= -pedantic
}
