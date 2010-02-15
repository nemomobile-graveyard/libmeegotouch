TEMPLATE=subdirs
SUBDIRS = galleryapplet

check.target = check
QMAKE_EXTRA_TARGETS += check

check-xml.target = check-xml
QMAKE_EXTRA_TARGETS += check-xml
