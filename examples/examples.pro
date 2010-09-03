TEMPLATE      = subdirs
SUBDIRS       = \
                apscanner \
                calculator \
                containertest \
                focus \
                helloworld \
                gestures \
                layout/mflowlayoutpolicy \
                layout/mfreestylelayoutpolicy \
                layout/mlinearlayoutpolicy \
                layout/mgridlayoutpolicy \
                layout/multiplepolicies \
                layout/layout_inside_layout \
                layout/hidden_widgets \
                layout/qgraphicslinearlayout \
                layout/qgraphicsgridlayout \
                layout/qgraphicslayout \
                layout/two_columns \
                lifecycle \
                multipleinstances \
                separatorTest \
                testwidget \
                #trackergrid \
                imtoolbar \

# for HAVE_DBUS
include( ../mkspecs/common.pri )

contains(DEFINES, HAVE_DBUS) {
    SUBDIRS += \
	    servicefw \
	    chaining \

}

