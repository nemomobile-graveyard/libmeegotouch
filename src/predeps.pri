QMAKE_EXTRA_TARGETS += build_gen

build_gen.target = ../duigen/duigen
win32 {
    build_gen.commands = \
        cd $$DUI_BUILD_TREE/duigen && \
        mingw32-make && \
        copy debug/duigen.exe .

    DUIGEN_EXECUTABLE = $$DUI_BUILD_TREE/duigen/duigen.exe
} else {
    build_gen.commands = cd ../duigen && qmake && make

    DUIGEN_EXECUTABLE = $$DUI_BUILD_TREE/duigen/duigen
}

