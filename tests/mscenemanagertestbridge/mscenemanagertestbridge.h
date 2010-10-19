#ifndef MSCENEMANAGERTESTBRIDGE_H
#define MSCENEMANAGERTESTBRIDGE_H

#include <MSceneWindow>

class MSceneManagerTestBridge : public QObject
{
    Q_OBJECT
public:
    MSceneManagerTestBridge(QObject *parent = 0);
    void fastForwardPageSwitchAnimation();
    void fastForwardSceneWindowTransitionAnimation(MSceneWindow *sceneWindow);
    void addSceneWindow(MSceneWindow *sceneWindow);
    void removeSceneWindow(MSceneWindow *sceneWindow);
};

#endif // MSCENEMANAGERTESTBRIDGE_H
