CONFIG += link_prl
QMAKE_EXTRA_TARGETS += check
check.depends = $$TARGET
check.commands = @LD_LIBRARY_PATH=../../lib:$$(LD_LIBRARY_PATH) ./$$TARGET


QMAKE_EXTRA_TARGETS += check-xml
check-xml.depends  = $$TARGET
check-xml.commands = @../rt.sh ./$$TARGET
