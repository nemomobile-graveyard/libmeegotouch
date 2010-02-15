# serviceif must be done before impl
CONFIG += ordered
SUBDIRS = \
    serviceif \
    impl \

TEMPLATE = subdirs
