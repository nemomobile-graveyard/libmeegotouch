QMAKE_EXTRA_TARGETS += build_mgen build_mmoc

build_gen.target = ../mgen/mgen
win32 {
    build_mgen.commands = \
        cd $$M_BUILD_TREE/mgen && mingw32-make

    # FIXME make it work for release build too
    MGEN_EXECUTABLE = $$M_BUILD_TREE/mgen/debug/mgen.exe
} else {
    build_mgen.commands = \
        cd $$M_BUILD_TREE/mgen && qmake && make

    MGEN_EXECUTABLE = $$M_BUILD_TREE/mgen/mgen
}

build_mmoc.target = ../mmoc/mmoc
win32 {
    build_mmoc.commands = \
        cd $$M_BUILD_TREE/mmoc && mingw32-make

    # FIXME make it work for release build too
    MMOC_EXECUTABLE = $$M_BUILD_TREE/mmoc/debug/mmoc.exe
} else {
    build_mmoc.commands = \
        cd $$M_BUILD_TREE/mmoc && qmake && make

    MMOC_EXECUTABLE = $$M_BUILD_TREE/mmoc/mmoc
}
