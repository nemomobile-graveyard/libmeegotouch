include( ../../mkspecs/common.pri )

CONFIG += ordered
SUBDIRS += \
    provider \
    user \
    misc \

contains( DEFINES, HAVE_QTM_SERVICEFRAMEWORK ) {
    SUBDIRS += qtmuser
}

TEMPLATE = subdirs
