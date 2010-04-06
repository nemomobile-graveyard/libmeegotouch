###############################################################################
# DuiInternationalization module
############################################################################### 
I18N_SRC_DIR=./i18n
INCLUDEPATH+=./i18n


PUBLIC_HEADERS += \
    $$I18N_SRC_DIR/duibreakiterator.h \
    $$I18N_SRC_DIR/duilocale.h

SOURCES += \
    $$I18N_SRC_DIR/duibreakiterator.cpp \
    $$I18N_SRC_DIR/duilocale.cpp
    
contains(DEFINES, HAVE_ICU) {
    PUBLIC_HEADERS += \
        $$I18N_SRC_DIR/duicalendar.h \
        $$I18N_SRC_DIR/duicollator.h \

    PRIVATE_HEADERS += \
        $$I18N_SRC_DIR/duiicubreakiterator.h \
        $$I18N_SRC_DIR/duiicuconversions.h

    SOURCES += \
        $$I18N_SRC_DIR/duicalendar.cpp \
        $$I18N_SRC_DIR/duicollator.cpp \
        $$I18N_SRC_DIR/duiicubreakiterator.cpp \
        $$I18N_SRC_DIR/duiicuconversions.cpp
} else {
    PRIVATE_HEADERS += \
        $$I18N_SRC_DIR/duinullbreakiterator.h \

    SOURCES += \
        $$I18N_SRC_DIR/duinullbreakiterator.cpp
}
