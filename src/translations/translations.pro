LANGUAGES = ar de en fi hu ur zh_CN
CATALOGNAME = libmeegotouch
SOURCEDIR = $$PWD/.. $$PWD/../../plainqt
TRANSLATIONDIR = $$PWD
MROOT = ../..
# these include files are installed to $$[QT_INSTALL_DATA]/mkspecs/features
# and included in the "libmeegotouch-dev" package:
include($${M_BUILD_TREE}/mkspecs/features/meegotouch_defines.prf)
include($${MROOT}/mkspecs/features/meegotouch_translations.prf)
