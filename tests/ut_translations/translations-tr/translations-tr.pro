QMAKE_EXTRA_TARGETS += check
check.depends = .
check.commands = echo "nothing to do here for “make check”"

QMAKE_EXTRA_TARGETS += check-xml
check-xml.depends = .
check-xml.commands = echo "nothing to do here for “make check-xml”"

LANGUAGES = en_GB fi_FI
DISABLE_QTTRID_ENGINEERING_ENGLISH = yes
CATALOGNAME = ut_translations-tr
SOURCEDIR = $$PWD/..
TRANSLATIONDIR = $$PWD
TRANSLATION_INSTALLDIR = $$[QT_INSTALL_LIBS]/libdui-tests/translations-tr
# add “-verbose” to force without “-idbased” which is the default in translations.pri
LRELEASE_OPTIONS = "-verbose"
DUIROOT = ../../..
# these include files are installed to $$[QT_INSTALL_DATA]/mkspecs/features
# and included in the "libdui-dev" package:
include($${DUI_BUILD_TREE}/mkspecs/features/dui_defines.prf)
include($${DUIROOT}/mkspecs/features/dui_translations.prf)
