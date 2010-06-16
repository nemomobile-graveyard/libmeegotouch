# QMAKE_EXTRA_TARGETS += runtests 
# QMAKE_EXTRA_TARGETS += coverage

# runtests.target = runtests
# runtests.commands = @find . -type f -perm +100 -name \"?t_*\" -exec ./runsingletest.sh {} \\;
# runtests.depends = FORCE

#coverage.target = coverage
#coverage.commands = @./coverage.py ../src/ ../src/.obj/ 50 ;
#coverage.depends = FORCE


coverage.target = coverage
# coverage.CONFIG = recursive
QMAKE_EXTRA_TARGETS += coverage

coverage-xml.target = coverage-xml
# coverage-xml.CONFIG = recursive
QMAKE_EXTRA_TARGETS += coverage-xml
