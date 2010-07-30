#ifndef SAMPLEPAGE_H
#define SAMPLEPAGE_H

#include <MApplicationPage>

class SamplePage : public MApplicationPage {
    Q_OBJECT

public:
    SamplePage(int level);

private slots:
    void openNextPage();

private:
    int level;
};

#endif
