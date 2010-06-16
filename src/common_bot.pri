# # Files and paths
HEADERS += $$PUBLIC_HEADERS \
    $$PRIVATE_HEADERS
install_headers.path = $$M_INSTALL_HEADERS
install_headers.files = $$PUBLIC_HEADERS
INSTALLS += target \
    install_headers
