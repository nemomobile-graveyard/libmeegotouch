QMAKE_EXTRA_TARGETS += build_mgen build_mmoc

build_moc.target = ../mmoc/mmoc
build_gen.target = ../mgen/mgen
win32 {
    build_mmoc.commands = \
        cd $$M_BUILD_TREE/mmmoc && mingw32-make

    build_mgen.commands = \
        cd $$M_BUILD_TREE/mgen && mingw32-make
} else {
    build_mmoc.commands = \
        cd $$M_BUILD_TREE/mmoc && qmake && make

    build_mgen.commands = \
        cd $$M_BUILD_TREE/mgen && qmake && make
}


# make sure mmoc executable is found
win32 {
    M_MMOC_EXECUTABLE = perl.exe $$M_SOURCE_TREE/mmoc/mmoc.pl
} else {
    contains(DEFINES, IS_ARMEL) {
        M_MMOC_EXECUTABLE = perl $$M_SOURCE_TREE/mmoc/mmoc.pl
    } else {
        M_MMOC_EXECUTABLE = $$M_BUILD_TREE/mmoc/mmoc
    }
}


# make sure mgen executable is found
win32 {
    debug {
        M_MGEN_EXECUTABLE = $$M_BUILD_TREE/mgen/debug/mgen.exe
    } else {
        M_MGEN_EXECUTABLE = $$M_BUILD_TREE/mgen/release/mgen.exe
    }
} else {
    M_MGEN_EXECUTABLE = $$M_BUILD_TREE/mgen/mgen
}

PRE_TARGETDEPS += $$M_MGEN_EXECUTABLE $$M_MMOC_EXECUTABLE
