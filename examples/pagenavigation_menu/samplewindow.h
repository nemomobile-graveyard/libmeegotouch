#ifndef SAMPLEWINDOW_H
#define SAMPLEWINDOW_H

#include <MApplicationWindow>

#include <MApplicationPage>
#include <MAction>
#include <QActionGroup>

class SampleWindow : public MApplicationWindow {
    Q_OBJECT

public:
    SampleWindow(QWidget *parent = 0);

public slots:
    void showPageForAction(QAction *action);

private:
    MApplicationPage *createPage(const QString &name);
    QAction *createAction(const QString &name);

    QAction *currentAction;
    QActionGroup *actionGroup;
};

#endif
