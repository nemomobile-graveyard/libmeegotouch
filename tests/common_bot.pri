DEPENDS=
contains( M_BUILD_FEATURES, coverage ) {
    include(coverage.pri)
}
