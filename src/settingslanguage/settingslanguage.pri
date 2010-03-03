# Input
SETTINGSLANGUAGE_SRC_DIR = ./settingslanguage
INCLUDEPATH += ./settingslanguage
QT += xml

duigen_model_settingslanguage.name = duigenerator \
    model
duigen_model_settingslanguage.input = SETTINGSLANGUAGE_MODEL_HEADERS
duigen_model_settingslanguage.output = $$GEN_DIR/gen_${QMAKE_FILE_BASE}data.cpp
duigen_model_settingslanguage.depends = $$DUIGEN_EXECUTABLE
duigen_model_settingslanguage.commands += $$DUIGEN_EXECUTABLE --model \
    ${QMAKE_FILE_NAME} \
    $$GEN_DIR/
duigen_model_settingslanguage.clean += $$GEN_DIR/gen_*
duigen_model_settingslanguage.CONFIG = target_predeps no_link
duigen_model_settingslanguage.variable_out = GENERATED_SOURCES
QMAKE_EXTRA_COMPILERS += duigen_model_settingslanguage

duigen_style_settingslanguage.name = duigenerator style
duigen_style_settingslanguage.input = SETTINGSLANGUAGE_STYLE_HEADERS
duigen_style_settingslanguage.depends = $$DUIGEN_EXECUTABLE
duigen_style_settingslanguage.output = $$GEN_DIR/gen_${QMAKE_FILE_BASE}data.cpp
duigen_style_settingslanguage.commands += $$DUIGEN_EXECUTABLE --style ${QMAKE_FILE_NAME} $$GEN_DIR
duigen_style_settingslanguage.clean += $$GEN_DIR/gen_*
duigen_style_settingslanguage.CONFIG = target_predeps no_link
duigen_style_settingslanguage.variable_out = GENERATED_SOURCES

QMAKE_EXTRA_COMPILERS += duigen_style_settingslanguage
HEADERS += $$SETTINGSLANGUAGE_MODEL_HEADERS \
    $$SETTINGSLANGUAGE_STYLE_HEADERS \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguageparser.h \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguagebinary.h \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguagewidget.h \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguagewidgetfactory.h \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguagesettingsfactory.h \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguageselectionfactory.h \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguagetextfactory.h \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguageintegerfactory.h \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguageselectioncontroller.h \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguagetextcontroller.h \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguageintegercontroller.h \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguagenode.h \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguageoption.h \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguageselection.h \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguagesettings.h \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguagetext.h \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguagebooleancontroller.h \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguagebooleanfactory.h \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguageboolean.h \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguageinteger.h \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguagegroup.h
SOURCES += $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguageparser.cpp \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguagebinary.cpp \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguagewidget.cpp \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguagewidgetfactory.cpp \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguagesettingsfactory.cpp \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguageselectionfactory.cpp \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguagetextfactory.cpp \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguageintegerfactory.cpp \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguageselectioncontroller.cpp \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguagetextcontroller.cpp \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguageintegercontroller.cpp \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguagenode.cpp \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguageoption.cpp \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguageselection.cpp \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguagesettings.cpp \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguagetext.cpp \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguagebooleancontroller.cpp \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguagebooleanfactory.cpp \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguageboolean.cpp \
    $$SETTINGSLANGUAGE_SRC_DIR/duisettingslanguageinteger.cpp
