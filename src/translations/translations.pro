LANGUAGES = ar de en fi hu ur zh_CN
CATALOGNAME = libdui
SOURCEDIR = $$PWD/.. $$PWD/../../plainqt
TRANSLATIONDIR = $$PWD
DUIROOT = ../..
# these include files are installed to $$[QT_INSTALL_DATA]/mkspecs/features
# and included in the "libdui-dev" package:
include($${DUI_BUILD_TREE}/mkspecs/features/dui_defines.prf)
include($${DUIROOT}/mkspecs/features/dui_translations.prf)
