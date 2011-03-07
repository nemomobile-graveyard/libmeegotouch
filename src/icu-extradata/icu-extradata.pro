include($${M_BUILD_TREE}/mkspecs/features/meegotouch_defines.prf)

ICUBINDIR = $$system(icu-config --bindir)
ICUVERSION = $$system(icu-config --version)
ICUDATA = $$system(icu-config --icudata)      # for example “icudt44l”
ICUUSRDATA = $$replace(ICUDATA, icudt, usrdt) # for example “usrdt44l”

greaterThan(ICUVERSION, 4.4) {
    # the contents of the compiled resource files can be dumped
    # with commands like:
    # derb -c -i /usr/share/meegotouch/icu/usrdt44l/ lang/en_GB.res
    LOCALESTXT = data/cs.txt \
                 data/da.txt \
                 data/en_GB.txt \
                 data/en_SG.txt \
                 data/fa.txt \
                 data/fi.txt \
                 data/gl.txt \
                 data/id.txt \
                 data/it.txt \
                 data/ms.txt \
                 data/ms_BN.txt \
                 data/pt_PT.txt \
                 data/uk.txt \
                 data/ur.txt \
                 data/zh_Hant.txt
    LANGTXT    = data/lang/en.txt \
                 data/lang/es.txt \
                 data/lang/fr.txt \
                 data/lang/id.txt \
                 data/lang/ms.txt \
                 data/lang/pt.txt \
                 data/lang/pt_PT.txt \
                 data/lang/zh.txt \
                 data/lang/zh_Hant.txt
    REGIONTXT  = 
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
       val = $$replace(a, data, $${ICUUSRDATA})
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
