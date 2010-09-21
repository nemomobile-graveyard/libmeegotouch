include($${M_BUILD_TREE}/mkspecs/features/meegotouch_defines.prf)

ICUBINDIR = $$system(icu-config --bindir)
ICUVERSION = $$system(icu-config --version)
ICUDATA = $$system(icu-config --icudata)      # for example “icudt44l”
ICUUSRDATA = $$replace(ICUDATA, icudt, usrdt) # for example “usrdt44l”

greaterThan(ICUVERSION, 4.4) {
    LOCALESTXT = $${ICUVERSION}/en_SG.txt \
                 $${ICUVERSION}/fi.txt \
                 $${ICUVERSION}/it.txt \
                 $${ICUVERSION}/ms_BN.txt
    LANGTXT    = $${ICUVERSION}/lang/de_CH.txt
    REGIONTXT  = $${ICUVERSION}/region/de_CH.txt
} else {
    # old icu version, just do nothing
    LOCALESTXT = # empty, do nothing
    LANGTXT =    # empty, do nothing
    REGIONTXT =  # empty, do nothing
    dummy.files = dummy-icu42-workaround-this-file-never-exists
    dummy.CONFIG += no_check_exist
    dummy.path = $$M_ICUEXTRADATA_DIR/$$ICUUSRDATA
    INSTALLS += dummy
}

defineReplace(installPaths){
    for(a,$$1){
       val = $$replace(a, $${ICUVERSION}, $${ICUUSRDATA})
       val = $$replace(val, txt, res)
       val = $$OUT_PWD/$$val
       result += $$val
    }
    return($$result)
}

LOCALESRES = $$installPaths(LOCALESTXT)
LANGRES    = $$installPaths(LANGTXT)
REGIONRES  = $$installPaths(REGIONTXT)

isEqual(TEMPLATE_PREFIX, vc):vcproj=1
isEmpty(vcproj) {
    QMAKE_LINK = @: IGNORE THIS LINE
    OBJECTS_DIR =
    win32:CONFIG -= embed_manifest_exe
} else {
    CONFIG += console
    PHONY_DEPS = .
    phony_src.input = PHONY_DEPS
    phony_src.output = phony.c
    phony_src.variable_out = GENERATED_SOURCES
    phony_src.commands = echo int main() { return 0; } > phony.c
    phony_src.name = CREATE phony.c
    phony_src.CONFIG += combine
    QMAKE_EXTRA_COMPILERS += phony_src
}

locales.name = icu-locales-extradata-builder
locales.input = LOCALESTXT
locales.output = $$OUT_PWD/$$ICUUSRDATA/${QMAKE_FILE_BASE}.res
locales.commands += mkdir -p $$OUT_PWD/$$ICUUSRDATA && $$ICUBINDIR/genrb -d $$OUT_PWD/$$ICUUSRDATA -s $$IN_PWD -e UTF-8 ${QMAKE_FILE_IN}
locales.CONFIG = no_link target_predeps
QMAKE_EXTRA_COMPILERS += locales

lang.name = icu-lang-extradata-builder
lang.input = LANGTXT
lang.output = $$OUT_PWD/$$ICUUSRDATA/lang/${QMAKE_FILE_BASE}.res
lang.commands += mkdir -p $$OUT_PWD/$$ICUUSRDATA/lang && $$ICUBINDIR/genrb -d $$OUT_PWD/$$ICUUSRDATA/lang -s $$IN_PWD -e UTF-8 ${QMAKE_FILE_IN}
lang.CONFIG = no_link target_predeps
QMAKE_EXTRA_COMPILERS += lang

region.name = icu-region-extradata-builder
region.input = REGIONTXT
region.output = $$OUT_PWD/$$ICUUSRDATA/region/${QMAKE_FILE_BASE}.res
region.commands += mkdir -p $$OUT_PWD/$$ICUUSRDATA/region && $$ICUBINDIR/genrb -d $$OUT_PWD/$$ICUUSRDATA/region -s $$IN_PWD -e UTF-8 ${QMAKE_FILE_IN}
region.CONFIG = no_link target_predeps
QMAKE_EXTRA_COMPILERS += region

localesres.files = $$LOCALESRES
localesres.CONFIG += no_check_exist
localesres.path = $$M_ICUEXTRADATA_DIR/$$ICUUSRDATA
INSTALLS += localesres

langres.files = $$LANGRES
langres.CONFIG += no_check_exist
langres.path = $$M_ICUEXTRADATA_DIR/$$ICUUSRDATA/lang
INSTALLS += langres

regionres.files = $$REGIONRES
regionres.CONFIG += no_check_exist
regionres.path = $$M_ICUEXTRADATA_DIR/$$ICUUSRDATA/region
INSTALLS += regionres
