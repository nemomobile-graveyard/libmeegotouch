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
include(mkspecs/features/meegotouch_coverage.prf)

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
                  tools/m-servicefwgen.d \
                  src \

       contains(DEFINES, HAVE_DBUS) {
           SUBDIRS += \
                  mappletrunner \
                  mapplicationextensionrunner \
                  mservicemapper \
                  mthemedaemon \
                  tools \
                  imgcachegen
       }
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
    }  else:isEqual(PROJECT, examples) {
       SUBDIRS += examples
    } else {
       message(Unknown PROJECT: $$PROJECT)
    }
}

# put mmoc, mgen and src dirs in right build order...

contains( SUBDIRS, src ) {
    SUBDIRS -= src
    SUBDIRS = src $$SUBDIRS
}

contains( SUBDIRS, mmoc ) {
    SUBDIRS -= mmoc
        SUBDIRS = mmoc $$SUBDIRS
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
QMAKE_DISTCLEAN += lib/libmeegotouch* build-stamp configure-stamp tests/*/*.log.xml tests/*/*.log *.log.xml *.log **/*.gcda extradata/*.o extradata/*.a mkspecs/meegotouchconfig.pri src/data/*.pc

check.target = check
check.CONFIG = recursive
QMAKE_EXTRA_TARGETS += check

check-xml.target = check-xml
check-xml.CONFIG = recursive
QMAKE_EXTRA_TARGETS += check-xml

# "make setup" can be used to install the minimal necessary data to run the
# MeeGo Touch demo applications inside the build tree.
setup.target = setup
setup.CONFIG += no_check_exist
setup.commands = mkdir -p $$M_TARGETS_CONF_DIR
setup.commands += && cp src/data/targets/*.conf $$M_TARGETS_CONF_DIR
QMAKE_EXTRA_TARGETS += setup
