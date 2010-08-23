These files are to test for regressions when regexes are changed in mgen.

For example:

../mgen --model mgentest.h
git diff gen_mgentestdata.{cpp,h}
