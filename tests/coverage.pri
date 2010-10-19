defineReplace(srcList) {
    LIST=$$1
    for(item,LIST) {
        ITEMLIST=$$item,$${ITEMLIST}
    }
    return($${ITEMLIST})
}

# OBJECTS_DIR = .obj
MOC_DIR = .moc

QMAKE_EXTRA_TARGETS += coverage
coverage.depends = check-xml
coverage.commands = @../coverage.py $$srcList($$TEST_SOURCES) .obj 90


QMAKE_EXTRA_TARGETS += coverage-xml
coverage-xml.depends = check-xml
coverage-xml.commands = @../coverage.py $$srcList($$TEST_SOURCES) .obj 90


QMAKE_CLEAN += *.log
#there are some xml files used in testing
#*.xml
QMAKE_DISTCLEAN += *.gcda *.gcno *.gcov *.log 
#*.xml
