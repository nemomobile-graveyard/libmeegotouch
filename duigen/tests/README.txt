These files are to test for regressions when regexes are changed in duigen.

For example:

../duigen --model duigentest.h
git diff gen_duigentestdata.{cpp,h}
