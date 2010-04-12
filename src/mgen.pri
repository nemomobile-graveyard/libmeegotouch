# definition of the mgen "compiler"

mgen_model.name = mgenerator model
mgen_model.input = MGEN_MODEL_HEADERS
mgen_model.output = $$GEN_DIR/gen_${QMAKE_FILE_BASE}data.cpp
mgen_model.depends = $$MGEN_EXECUTABLE
mgen_model.commands += $$MGEN_EXECUTABLE --model ${QMAKE_FILE_NAME} $$GEN_DIR
mgen_model.clean += $$GEN_DIR/gen_*
mgen_model.CONFIG = target_predeps no_link
mgen_model.variable_out = GENERATED_SOURCES

QMAKE_EXTRA_COMPILERS += mgen_model

mgen_style.name = mgenerator style
mgen_style.input = MGEN_STYLE_HEADERS
mgen_style.depends = $$MGEN_EXECUTABLE
mgen_style.output = $$GEN_DIR/gen_${QMAKE_FILE_BASE}data.cpp
mgen_style.commands += $$MGEN_EXECUTABLE --style ${QMAKE_FILE_NAME} $$GEN_DIR
mgen_style.clean += $$GEN_DIR/gen_*
mgen_style.CONFIG = target_predeps no_link
mgen_style.variable_out = GENERATED_SOURCES

QMAKE_EXTRA_COMPILERS += mgen_style
