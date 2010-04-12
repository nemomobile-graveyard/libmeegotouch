#####################################################################
# DirectUI project file
#####################################################################

CONFIG += ordered
TEMPLATE = subdirs

!win32 {
    !exists($${OUT_PWD}/mkspecs/meegotouchconfig.pri) {
        error("Please run ./configure before proceeding")
    }
}

include(mkspecs/common.pri)
isEqual( IN_PWD, $${OUT_PWD} ) {
    IS_OUT_OF_SOURCE = 0
} else {
    IS_OUT_OF_SOURCE = 1
}

isEmpty(M_BUILD_PARTS) { #defaults
   M_BUILD_PARTS = libs demos
} else { #make sure the build order makes sense
   contains(M_BUILD_PARTS, libs) {
       M_BUILD_PARTS -= libs
       M_BUILD_PARTS = libs $$M_BUILD_PARTS
   }
}

#process the projects
for(PROJECT, $$list($$lower($$unique(M_BUILD_PARTS)))) {
    isEqual(PROJECT, libs) {
       SUBDIRS += \
                  mmoc \
                  mgen \
                  src \
                  src/translations \

       contains(DEFINES, HAVE_DBUS) {
           SUBDIRS += \
                  mappletrunner \
                  mapplicationextensionrunner \
                  mservicemapper \
                  mthemedaemon \
                  tools
       }
    } else:isEqual(PROJECT, plainqt) {
       SUBDIRS += plainqt
    } else:isEqual(PROJECT, tests) {
       SUBDIRS += tests
       macx:SUBDIRS -= tests
       win32:SUBDIRS -= tests
    } else:isEqual(PROJECT, benchmarks) {
       SUBDIRS += benchmarks
       macx:SUBDIRS -= benchmarks
       win32:SUBDIRS -= benchmarks
    }  else:isEqual(PROJECT, demos) {
       SUBDIRS += demos
    } else {
       message(Unknown PROJECT: $$PROJECT)
    }
}

# put mgen, src and plainqt dirs in right build order...

contains( SUBDIRS, plainqt ) {
    SUBDIRS -= plainqt
    SUBDIRS = plainqt $$SUBDIRS
}

contains( SUBDIRS, src ) {
    SUBDIRS -= src
    SUBDIRS = src $$SUBDIRS
}

contains( SUBDIRS, mgen ) {
    SUBDIRS -= mgen
    SUBDIRS = mgen $$SUBDIRS
}

# Docs are always explicitly built with "make doc"
include(doc/doc.pri)

# note: proper way to clean up extradata would be running pkgdata --clean...
# but listing *.a & *.o is easier with qmake
QMAKE_CLEAN += lib/libmeegotouch* build-stamp configure-stamp tests/*/*.log.xml tests/*/*.log *.log.xml *.log **/*.gcda extradata/*.o extradata/*.a
QMAKE_DISTCLEAN += lib/libmeegotouch* build-stamp configure-stamp tests/*/*.log.xml tests/*/*.log *.log.xml *.log **/*.gcda extradata/*.o extradata/*.a mkspecs/meegotouchconfig.pri

check.target = check
check.CONFIG = recursive
QMAKE_EXTRA_TARGETS += check

check-xml.target = check-xml
check-xml.CONFIG = recursive
QMAKE_EXTRA_TARGETS += check-xml
