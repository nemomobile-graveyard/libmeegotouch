include(../mkspecs/shared.pri)
DOXYGEN_BIN=$$findFile(doxygen)

QMAKE_EXTRA_TARGETS += doc
doc.target = doc
isEmpty(DOXYGEN_BIN) {
    doc.commands = @echo "Unable to detect doxygen in PATH"
} else {
    system( mkdir -p $${OUT_PWD}/doc/html )

    doc.commands = ( sed -e \"s:@M_SRC_DIR@:$${IN_PWD}:g\" \
                         -e \"s:@M_BUILD_DIR@:$${M_BUILD_TREE}:g\" \
                         -e \"s/@M_DOC_VERSION_STRING@/$${M_DOC_VERSION_STRING}/g\" \
                         $${IN_PWD}/mdoxy.cfg.in > doc/mdoxy.cfg );

    doc.commands+= ( $${IN_PWD}/generate-toc.pl < $${M_SOURCE_TREE}/doc/src/i18n.dox > $${M_BUILD_TREE}/doc/i18n-toc.html );
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
