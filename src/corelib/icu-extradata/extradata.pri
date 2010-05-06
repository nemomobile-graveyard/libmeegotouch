extradata.name = libextradata builder
extradata.input = EXTRADATA_SOURCE
extradata.output = $$DESTDIR/lib${QMAKE_FILE_BASE}.a
extradata.commands += genrb -d $$OBJECTS_DIR -s $$IN_PWD -e UTF-8 ${QMAKE_FILE_BASE}.txt;
extradata.commands += pkgdata --name ${QMAKE_FILE_BASE} --mode static -v -d $$DESTDIR -T $$OBJECTS_DIR -s $$OBJECTS_DIR $$IN_PWD/packagelist.txt


extradata.clean += $$OBJECTS_DIR/*.mak $$OBJECTS_DIR/*.lst $$OBJECTS_DIR/*.ao $$OBJECTS_DIR/*.o $$OBJECTS_DIR/*.res $$OBJECTS_DIR/*.c $$DESTDIR/lib${QMAKE_FILE_BASE}.a $$DESTDIR/README_${QMAKE_FILE_BASE}.txt
extradata.CONFIG = target_predeps 

QMAKE_EXTRA_COMPILERS += extradata
