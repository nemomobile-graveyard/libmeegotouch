
# install
basetheme.files = ./theme/base/*
basetheme.path = $$DUI_THEME_DIR/base/dui/widgetsgallery/
basetheme.CONFIG += no_check_exist

develtheme.files = ./theme/devel/*
develtheme.path = $$DUI_THEME_DIR/devel/dui/widgetsgallery/
develtheme.CONFIG += no_check_exist

INSTALLS += \
    basetheme \
    develtheme \
