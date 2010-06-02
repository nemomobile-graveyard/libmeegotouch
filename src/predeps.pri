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

win32 {
     MMOC_EXECUTABLE = $$M_BUILD_TREE/mmoc/mmoc
     contains(DEFINES, IS_ARMEL) {
        MMOC_EXECUTABLE = perl.exe $$M_BUILD_TREE/mmoc/mmoc.pl
     }
} else {
     MMOC_EXECUTABLE = $$M_BUILD_TREE/mmoc/mmoc
     contains(DEFINES, IS_ARMEL) {
        MMOC_EXECUTABLE = perl $$M_BUILD_TREE/mmoc/mmoc.pl
     }
}
