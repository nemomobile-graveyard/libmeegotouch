#ifndef BENCHMARKLOCALCLIENT_H
#define BENCHMARKLOCALCLIENT_H

#include <QObject>

class BenchmarkLocalClient : public QObject
{
    Q_OBJECT
public:
    BenchmarkLocalClient();

signals:
    void finished();

public slots:
    void run();
};

#endif // BENCHMARKLOCALCLIENT_H
