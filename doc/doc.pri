include(../mkspecs/shared.pri)
DOXYGEN_BIN=$$findFile(doxygen)

QMAKE_EXTRA_TARGETS += doc
doc.target = doc
isEmpty(DOXYGEN_BIN) {
    doc.commands = @echo "Unable to detect doxygen in PATH"
} else {
    system( mkdir -p $${OUT_PWD}/doc/html )

    doc.commands = ( sed -e \"s:@M_SRC_DIR@:$${IN_PWD}:g\" \
                         $${IN_PWD}/mdoxy.cfg.in > doc/mdoxy.cfg );

    doc.commands+= ( $${DOXYGEN_BIN} doc/mdoxy.cfg );
    doc.commands+= ( cp $${IN_PWD}/src/images/* doc/html );
    doc.commands+= ( cd doc ; $${IN_PWD}/xmlize.pl );
    doc.commands+= ( $${IN_PWD}/generateSettingsLanguageDocs.sh $${OUT_PWD}/doc );

    # Install rules
    htmldocs.files = $${OUT_PWD}/doc/html/

    htmldocs.path = $$M_INSTALL_DATA/doc/libmeegotouch
    htmldocs.CONFIG += no_check_exist
    INSTALLS += htmldocs
}

doc.depends = FORCE
