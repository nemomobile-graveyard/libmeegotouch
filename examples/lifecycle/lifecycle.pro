TEMPLATE = app
TARGET   = lifecycle
CONFIG  += meegotouch
SOURCES += main.cpp mainpage.cpp anotherpage.cpp containerpage.cpp
HEADERS += mainpage.h anotherpage.h containerpage.h

target.path         = /usr/bin
desktop_entry.path  = /usr/share/applications
desktop_entry.files = lifecycle.desktop
services.path       = /usr/share/dbus-1/services
services.files      = com.nokia.lifecycle.service

# Install instructions
INSTALLS += target desktop_entry services
