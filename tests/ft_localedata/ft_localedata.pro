include(../common_top.pri)

TARGET = ft_localedata
LIBS += -lgcov
QMAKE_CXXFLAGS += -ftest-coverage -fprofile-arcs

# For gcov testing, disabled currently. Add -lgcov to LIBS if
# this is enabled
# QMAKE_CXXFLAGS += -ftest-coverage -fprofile-arcs

# unit
TEST_SOURCES =  \
#    $$MSRCDIR/mlocale.cpp \

# Input
HEADERS += ft_localedata.h
SOURCES += ft_localedata.cpp

GENRB_FILES = fi_FI.txt
ICUDATADIR = icudt42l
genrb.name = generating resource bundle files
genrb.input = GENRB_FILES
genrb.output = ${QMAKE_FILE_PATH}/$${ICUDATADIR}/${QMAKE_FILE_BASE}.res
genrb.commands = mkdir -p $${ICUDATADIR};
genrb.commands += genrb -e UTF-8 -d $${ICUDATADIR} ${QMAKE_FILE_IN};
genrb.CONFIG += no_link target_predeps
QMAKE_EXTRA_COMPILERS += genrb

support_files.files += $$ICUDATADIR

include(../common_bot.pri)
