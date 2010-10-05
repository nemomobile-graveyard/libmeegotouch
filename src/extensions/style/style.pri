EXTENSIONS_STYLE_HEADERS = \
    style/mapplicationextensionareastyle.h \
    style/mextensionareastyle.h \
    style/mmashupcanvasstyle.h \

EXTENSIONS_PRIVATE_STYLE_HEADERS = \
    style/mapplethandlestyle.h \
    style/mappletinventorystyle.h \
    style/mextensionhandlestyle.h \

STYLE_HEADERS += $$EXTENSIONS_STYLE_HEADERS $$EXTENSIONS_PRIVATE_STYLE_HEADERS

PUBLIC_HEADERS += $$EXTENSIONS_STYLE_HEADERS
PRIVATE_HEADERS += $$EXTENSIONS_PRIVATE_STYLE_HEADERS
