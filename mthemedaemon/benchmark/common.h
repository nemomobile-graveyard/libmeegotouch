#ifndef COMMON_H
#define COMMON_H

#include <QTime>
#include <QList>
#include <QPair>
#include <QSize>
#include <QMap>
#include <QTextStream>

class QString;
class QSize;

struct RequestInfo {
    int delay() const {
        return request.msecsTo(answer);
    }

    QTime request;
    QTime answer;
    QSize size;
};

class Common {
public:
    static QList<QPair<QString,QSize> > getPixmapsToRequest();
    static void printResults(const QString& serverType, QMap<QString, RequestInfo> answeredRequests, QMap<QString, RequestInfo> unansweredRequests);

};

#endif // COMMON_H
