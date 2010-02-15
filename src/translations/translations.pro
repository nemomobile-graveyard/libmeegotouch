LANGUAGES = ar de en fi hu ur zh_CN
CATALOGNAME = libdui
SOURCEDIR = $$PWD/.. $$PWD/../../plainqt
TRANSLATIONDIR = $$PWD
DUIROOT = ../..
# these include files are installed to $$[QT_INSTALL_DATA]/mkspecs/features
# and included in the "libdui-dev" package:
include($$DUIROOT/src/dui_defines.prf)
include($$DUIROOT/src/translations.prf)
