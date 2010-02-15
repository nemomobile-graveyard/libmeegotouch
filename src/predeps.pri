QMAKE_EXTRA_TARGETS +=  build_moc \
                        build_gen \

build_moc.target = ../duimoc/duimoc
build_moc.commands = cd ../duimoc && qmake && make

build_gen.target = ../duigen/duigen
build_gen.commands = cd ../duigen && qmake && make

