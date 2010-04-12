CONFIG += ordered
TEMPLATE = subdirs
SUBDIRS += \
    corelib \
    views \
    settings \
    extensions
macx:SUBDIRS -= settings extensions