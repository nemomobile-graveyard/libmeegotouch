LANGUAGES = de en ja
CATALOGNAME = animatedlayout
SOURCEDIR = $$PWD/..
TRANSLATIONDIR = $$PWD
DUIROOT = ../../..
# these include files are installed to $$[QT_INSTALL_DATA]/mkspecs/features
# and included in the "libdui-dev" package:
include($${DUI_BUILD_TREE}/mkspecs/features/dui_defines.prf)
include($${DUIROOT}/mkspecs/features/dui_translations.prf)
