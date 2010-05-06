QMAKE_EXTRA_TARGETS += build_gen

build_gen.target = ../mgen/mgen
win32 {
    build_gen.commands = \
        cd $$M_BUILD_TREE/mgen && mingw32-make

    # FIXME make it work for release build too
    MGEN_EXECUTABLE = $$M_BUILD_TREE/mgen/debug/mgen.exe
} else {
    build_gen.commands = \
        cd $$M_BUILD_TREE/mgen && qmake && make

    MGEN_EXECUTABLE = $$M_BUILD_TREE/mgen/mgen
}

