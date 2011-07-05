#ifndef ACCESSMANAGER_H
#define ACCESSMANAGER_H

/*
 * automatically generated with the command line :
 * m-servicefwgen -p com.nokia.widgetsgallery.AccessManager
 */

#include <QObject>

#include <mservicefwbaseif.h>

class AccessManager : public MServiceFwBaseIf
{
Q_OBJECT

public:
    bool login( bool fullscreen, bool autofocus, int orientation );
    bool login( bool fullscreen, bool autofocus, int orientation, const QString &_taskTitle );

public:
    /*!
     * @brief Constructs a base interface
     * @param preferredService the preferred service provider. Leave
     * empty if no preferred provider. In most cases, this should be left
     * empty.
     * @param parent Parent object
     */

    AccessManager( const QString& preferredService = "", QObject* parent = 0 );

    /*!
     * @brief Set the service name
     * @param service Name of the desired service
     */

    void setService(const QString & service);
Q_SIGNALS:

};
#endif

