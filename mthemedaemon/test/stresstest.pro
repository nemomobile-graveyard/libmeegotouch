TEMPLATE = subdirs

#DEFINES += PRINT_INFO_MESSAGES

# Check for mixing of const and non-const iterators,
# which can cause problems when built with some compilers:
DEFINES += QT_STRICT_ITERATORS

# enable QString optimizations
DEFINES += QT_USE_FAST_CONCATENATION QT_USE_FAST_OPERATOR_PLUS

SUBDIRS = mthemedaemontest.pro \
  testdaemon
