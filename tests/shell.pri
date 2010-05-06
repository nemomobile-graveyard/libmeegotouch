shell_scripts.commands += $$PWD/gen-tests-xml.sh > $$OUT_PWD/tests.xml
shell_scripts.files += $$OUT_PWD/tests.xml
shell_scripts.CONFIG += no_check_exist

include(../mkspecs/common.pri)

# fixed path required by TATAM
# You mean hardcoding /usr instead of using $$M_INSTALL_DATA? Why? murrayc
shell_scripts.path += $$M_INSTALL_DATA/libmeegotouch-tests
shell_scripts.depends = FORCE

INSTALLS    += \
              shell_scripts
