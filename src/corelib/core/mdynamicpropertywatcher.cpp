#include "mdynamicpropertywatcher.h"
#include <QDynamicPropertyChangeEvent>

MDynamicPropertyWatcher::MDynamicPropertyWatcher(QObject* parent)
    : QObject(parent),
      watchedObject(0)
{
}

void MDynamicPropertyWatcher::watch(QObject *object) {
    if (object == watchedObject)
        return;

    if (watchedObject) {
        watchedObject->removeEventFilter(this);
        watchedObject = 0;

        lastPropertyValue.clear();
    }

    if (object) {
        object->installEventFilter(this);
        watchedObject = object;
    }
}

void MDynamicPropertyWatcher::setPropertyName(QString propertyName)
{
    this->propertyName = propertyName;
    lastPropertyValue.clear();
}

bool MDynamicPropertyWatcher::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::DynamicPropertyChange &&
        propertyName == static_cast<QDynamicPropertyChangeEvent*>(event)->propertyName())
    {
        QVariant newPropertyValue = watched->property(propertyName.toAscii());
        if (!lastPropertyValue.isValid() ||
            newPropertyValue != lastPropertyValue) {

            lastPropertyValue = newPropertyValue;
            emit propertyChanged();
        }
    }
    return false;
}


MMultiObjectsPropertyWatcher::MMultiObjectsPropertyWatcher(QObject* parent)
    : QObject(parent)
{
}

void MMultiObjectsPropertyWatcher::watch(QObject *object)
{
    if (object) {
        object->installEventFilter(this);
        lastValues[object] = QVariant();
    }
}

void MMultiObjectsPropertyWatcher::unwatch(QObject *object)
{
    if (object) {
        object->removeEventFilter(this);
        lastValues.remove(object);
    }
}

void MMultiObjectsPropertyWatcher::setPropertyName(QString propertyName)
{
    this->propertyName = propertyName;
}

bool MMultiObjectsPropertyWatcher::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::DynamicPropertyChange &&
        propertyName == static_cast<QDynamicPropertyChangeEvent*>(event)->propertyName())
    {
        QVariant newPropertyValue = watched->property(propertyName.toAscii());
        if (!lastValues[watched].isValid() ||
            newPropertyValue != lastValues[watched]) {

            lastValues[watched] = newPropertyValue;
            emit propertyChanged(watched);
        }
    }
    return false;
}
