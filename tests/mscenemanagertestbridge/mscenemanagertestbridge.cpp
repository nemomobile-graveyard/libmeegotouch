#include "mscenemanagertestbridge.h"

MSceneManagerTestBridge::MSceneManagerTestBridge(QObject *parent)
    : QObject(parent)
{
    setObjectName("_m_testBridge");
}

void MSceneManagerTestBridge::fastForwardPageSwitchAnimation()
{
    QObject *testInterface = children()[0];

    QMetaObject::invokeMethod(testInterface, "fastForwardPageSwitchAnimation",
            Qt::DirectConnection);
}

void MSceneManagerTestBridge::fastForwardSceneWindowTransitionAnimation(MSceneWindow *sceneWindow)
{
    QObject *testInterface = children()[0];

    QMetaObject::invokeMethod(testInterface, "fastForwardSceneWindowTransitionAnimation",
            Qt::DirectConnection,
            Q_ARG(MSceneWindow*, sceneWindow));
}

void MSceneManagerTestBridge::addSceneWindow(MSceneWindow *sceneWindow)
{
    QObject *testInterface = children()[0];

    QMetaObject::invokeMethod(testInterface, "addSceneWindow",
            Qt::DirectConnection,
            Q_ARG(MSceneWindow*, sceneWindow));
}

void MSceneManagerTestBridge::removeSceneWindow(MSceneWindow *sceneWindow)
{
    QObject *testInterface = children()[0];

    QMetaObject::invokeMethod(testInterface, "removeSceneWindow",
            Qt::DirectConnection,
            Q_ARG(MSceneWindow*, sceneWindow));
}
