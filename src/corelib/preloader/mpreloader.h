#ifndef MPRELOADER_H
#define MPRELOADER_H

#include "mexport.h"

class M_CORE_EXPORT MPreloader {
public:
    MPreloader();

    static bool isPopulating();

    void preloadFonts();
    void preloadStyles();
};

#endif // MPRELOADER_H
