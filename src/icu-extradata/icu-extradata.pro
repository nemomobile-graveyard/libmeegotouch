include($${M_BUILD_TREE}/mkspecs/features/meegotouch_defines.prf)

ICUBINDIR = $$system(icu-config --bindir)
ICUVERSION = $$system(icu-config --version)
ICUDATA = $$system(icu-config --icudata)      # for example “icudt44l”
ICUUSRDATA = $$replace(ICUDATA, icudt, usrdt) # for example “usrdt44l”

greaterThan(ICUVERSION, 4.4) {
    # the contents of the compiled resource files can be dumped
    # with commands like:
    # derb -c -i /usr/share/meegotouch/icu/usrdt44l/ lang/en_GB.res
    LOCALESTXT = data/ca.txt \
                 data/cs.txt \
                 data/da.txt \
                 data/de.txt \
                 data/en_GB.txt \
                 data/en_IE.txt \
                 data/as_IN.txt \
                 data/bn_IN.txt \
                 data/bo_IN.txt \
                 data/en_IN.txt \
                 data/gu_IN.txt \
                 data/hi_IN.txt \
                 data/kn_IN.txt \
                 data/kok_IN.txt \
                 data/ml_IN.txt \
                 data/mr_IN.txt \
                 data/ne_IN.txt \
                 data/or_IN.txt \
                 data/pa_Guru_IN.txt \
                 data/pa_IN.txt \
                 data/ta_IN.txt \                 
                 data/te_IN.txt \                 
                 data/en_SG.txt \
                 data/es_AR.txt \
                 data/fi.txt \
                 data/fr_CA.txt \
                 data/gl.txt \
                 data/he.txt \
                 data/hi.txt \
                 data/hu.txt \
                 data/id.txt \
                 data/it.txt \
                 data/kk.txt \
                 data/ms.txt \
                 data/ms_BN.txt \
                 data/nn.txt \
                 data/pl.txt \
                 data/sk.txt \
                 data/sv.txt \
                 data/th.txt \
                 data/uk.txt \
                 data/vi.txt \
                 data/zh_Hant.txt \
                 data/zh_Hant_HK.txt \
                 data/zh_HK.txt \
                 data/zh_TW.txt \
                 data/zh_Hant_TW.txt \
                 data/ar_LB.txt \
                 data/ar_IQ.txt

    greaterThan(ICUVERSION, 4.5) {
        LOCALESTXT += data46/ar.txt \
                      data46/ar_QA.txt \
                      data46/ar_SA.txt \
                      data46/ar_SY.txt \
                      data46/ar_TN.txt \
                      data46/ar_YE.txt \
                      data46/es.txt \
                      data46/es_419.txt \
                      data46/fa.txt \
                      data46/fa_AF.txt \
                      data46/pt.txt \
                      data46/pt_BR.txt \
                      data46/pt_PT.txt \
                      data46/ur.txt \
                      data46/ur_IN.txt
    } else {
        LOCALESTXT += data/ar.txt \
                      data/ar_AE.txt \
                      data/ar_QA.txt \
                      data/ar_SA.txt \
                      data/ar_SY.txt \
                      data/ar_TN.txt \
                      data/ar_YE.txt \
                      data/es.txt \
                      data/es_419.txt \
                      data/fa.txt \
                      data/fa_AF.txt \
                      data/pt.txt \
                      data/pt_BR.txt \
                      data/pt_PT.txt \
                      data/ur.txt \
                      data/ur_IN.txt
    }
    LANGTXT    = data/lang/en.txt \
                 data/lang/es.txt \
                 data/lang/fr.txt \
                 data/lang/id.txt \
                 data/lang/ms.txt \
                 data/lang/pt.txt \
                 data/lang/pt_PT.txt \
                 data/lang/zh.txt \
                 data/lang/zh_Hant.txt
    COLLTXT    = data/coll/zh.txt \
                 data/coll/ru.txt \
                 data/coll/sk.txt
    CURRTXT    = data/curr/en_HK.txt \
                 data/curr/zh_HK.txt \
                 data/curr/zh_Hans_HK.txt \
                 data/curr/zh_Hant_HK.txt
    ZONETXT    = data/zone/bg.txt \
                 data/zone/sk.txt \
                 data/zone/sr.txt \
                 data/zone/zh.txt
    REGIONTXT  =
} else {
    # old icu version, just do nothing
    LOCALESTXT = # empty, do nothing
    LANGTXT =    # empty, do nothing
    COLLTXT =    # empty, do nothing
    CURRTXT =    # empty, do nothing
    ZONETXT =    # empty, do nothing
    REGIONTXT =  # empty, do nothing
    dummy.files = dummy-icu42-workaround-this-file-never-exists
    dummy.CONFIG += no_check_exist
    dummy.path = $$M_ICUEXTRADATA_DIR/$$ICUUSRDATA
    INSTALLS += dummy
}

defineReplace(installPaths){
    for(a,$$1){
       val = $$replace(a, data46, $${ICUUSRDATA})
       val = $$replace(val, data, $${ICUUSRDATA})
       val = $$replace(val, txt, res)
       val = $$OUT_PWD/$$val
       result += $$val
    }
    return($$result)
}

LOCALESRES = $$installPaths(LOCALESTXT)
LANGRES    = $$installPaths(LANGTXT)
COLLRES    = $$installPaths(COLLTXT)
CURRRES    = $$installPaths(CURRTXT)
ZONERES    = $$installPaths(ZONETXT)
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

coll.name = icu-coll-extradata-builder
coll.input = COLLTXT
coll.output = $$OUT_PWD/$$ICUUSRDATA/coll/${QMAKE_FILE_BASE}.res
coll.commands += mkdir -p $$OUT_PWD/$$ICUUSRDATA/coll && $$ICUBINDIR/genrb -d $$OUT_PWD/$$ICUUSRDATA/coll -s $$IN_PWD -e UTF-8 ${QMAKE_FILE_IN}
coll.CONFIG = no_link target_predeps
QMAKE_EXTRA_COMPILERS += coll

curr.name = icu-curr-extradata-builder
curr.input = CURRTXT
curr.output = $$OUT_PWD/$$ICUUSRDATA/curr/${QMAKE_FILE_BASE}.res
curr.commands += mkdir -p $$OUT_PWD/$$ICUUSRDATA/curr && $$ICUBINDIR/genrb -d $$OUT_PWD/$$ICUUSRDATA/curr -s $$IN_PWD -e UTF-8 ${QMAKE_FILE_IN}
curr.CONFIG = no_link target_predeps
QMAKE_EXTRA_COMPILERS += curr

zone.name = icu-zone-extradata-builder
zone.input = ZONETXT
zone.output = $$OUT_PWD/$$ICUUSRDATA/zone/${QMAKE_FILE_BASE}.res
zone.commands += mkdir -p $$OUT_PWD/$$ICUUSRDATA/zone && $$ICUBINDIR/genrb -d $$OUT_PWD/$$ICUUSRDATA/zone -s $$IN_PWD -e UTF-8 ${QMAKE_FILE_IN}
zone.CONFIG = no_link target_predeps
QMAKE_EXTRA_COMPILERS += zone

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

collres.files = $$COLLRES
collres.CONFIG += no_check_exist
collres.path = $$M_ICUEXTRADATA_DIR/$$ICUUSRDATA/coll
INSTALLS += collres

currres.files = $$CURRRES
currres.CONFIG += no_check_exist
currres.path = $$M_ICUEXTRADATA_DIR/$$ICUUSRDATA/curr
INSTALLS += currres

zoneres.files = $$ZONERES
zoneres.CONFIG += no_check_exist
zoneres.path = $$M_ICUEXTRADATA_DIR/$$ICUUSRDATA/zone
INSTALLS += zoneres

regionres.files = $$REGIONRES
regionres.CONFIG += no_check_exist
regionres.path = $$M_ICUEXTRADATA_DIR/$$ICUUSRDATA/region
INSTALLS += regionres
