#ifndef MOBJECTMENUBASICSTYLE_H
#define MOBJECTMENUBASICSTYLE_H

#include <mscenewindowstyle.h>

class MScalableImage;

class MObjectMenuBasicStyle : public MSceneWindowStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MObjectMenuBasicStyle)
};

class MObjectMenuBasicStyleContainer : public MSceneWindowStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MObjectMenuBasicStyle)
};

#endif

