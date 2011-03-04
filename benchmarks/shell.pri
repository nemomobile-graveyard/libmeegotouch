shell_scripts.commands += $$PWD/gen-benchmarks-xml.sh > $$OUT_PWD/tests.xml
shell_scripts.files += $$OUT_PWD/tests.xml
shell_scripts.CONFIG += no_check_exist

include(../mkspecs/common.pri)

shell_scripts.path += $$M_INSTALL_DATA/libmeegotouch-benchmarks
shell_scripts.depends = FORCE

INSTALLS    += \
              shell_scripts
