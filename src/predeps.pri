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

build_moc.target = ../mmoc/mmoc
win32 {
    build_mmmoc.commands = \
        cd $$M_BUILD_TREE/mmmoc && mingw32-make

    # FIXME make it work for release build too
    MMOC_EXECUTABLE = $$M_BUILD_TREE/mgen/debug/mmoc.exe
} else {
    build_mmoc.commands = \
        cd $$M_BUILD_TREE/mmoc && qmake && make

    MMOC_EXECUTABLE = $$M_BUILD_TREE/mmoc/mmoc
}

MMOC_PERL_SCRIPT = $$M_BUILD_TREE/mmoc/mmoc.pl

MMOC_PERL = perl $$MMOC_PERL_SCRIPT
win32 {
    MMOC_PERL = perl.exe $$MMOC_PERL_SCRIPT
}
