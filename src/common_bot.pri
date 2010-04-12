## Files and paths

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS

macx {
    FRAMEWORK_HEADERS.version = Versions
    FRAMEWORK_HEADERS.files = $$system(find include/M*) $$HEADERS
    FRAMEWORK_HEADERS.path = Headers
    QMAKE_BUNDLE_DATA += FRAMEWORK_HEADERS
}

install_headers.path = $$M_INSTALL_HEADERS
install_headers.files = $$PUBLIC_HEADERS

INSTALLS += \
    target \
    install_headers \
