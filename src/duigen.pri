# definition of the duigen "compiler"

duigen_model.name = duigenerator model
duigen_model.input = DUIGEN_MODEL_HEADERS
duigen_model.output = $$GEN_DIR/gen_${QMAKE_FILE_BASE}data.cpp
duigen_model.depends = $$DUIGEN_EXECUTABLE
duigen_model.commands += $$DUIGEN_EXECUTABLE --model ${QMAKE_FILE_NAME} $$GEN_DIR
duigen_model.clean += $$GEN_DIR/gen_*
duigen_model.CONFIG = target_predeps no_link
duigen_model.variable_out = GENERATED_SOURCES

QMAKE_EXTRA_COMPILERS += duigen_model

duigen_style.name = duigenerator style
duigen_style.input = DUIGEN_STYLE_HEADERS
duigen_style.depends = $$DUIGEN_EXECUTABLE
duigen_style.output = $$GEN_DIR/gen_${QMAKE_FILE_BASE}data.cpp
duigen_style.commands += $$DUIGEN_EXECUTABLE --style ${QMAKE_FILE_NAME} $$GEN_DIR
duigen_style.clean += $$GEN_DIR/gen_*
duigen_style.CONFIG = target_predeps no_link
duigen_style.variable_out = GENERATED_SOURCES

QMAKE_EXTRA_COMPILERS += duigen_style
