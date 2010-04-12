
# install
basetheme.files = ./theme/base/*
basetheme.path = $$M_THEME_DIR/base/meegotouch/widgetsgallery/
basetheme.CONFIG += no_check_exist

develtheme.files = ./theme/devel/*
develtheme.path = $$M_THEME_DIR/devel/meegotouch/widgetsgallery/
develtheme.CONFIG += no_check_exist

INSTALLS += \
    basetheme \
    develtheme \
