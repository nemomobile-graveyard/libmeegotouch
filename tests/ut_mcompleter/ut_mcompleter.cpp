/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libmeegotouch.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include "ut_mcompleter.h"

#include <QTest>
#include <QString>
#include <QTextCursor>
#include <QSignalSpy>
#include <QStringListModel>
#include <QAbstractTableModel>
#include <mcompleter.h>
#include "mcompleter_p.h"
#include <mtextedit.h>

#include <MApplication>
#include <MApplicationWindow>
#include <MScene>

class CompleterTestModel : public  QAbstractTableModel
{
public :
    CompleterTestModel(const QStringList &, const QStringList &, QObject *parent = 0);
    ~CompleterTestModel();

    //! \reimp
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    //! \reimp_end
private:
    QStringList column1, column2;
};

CompleterTestModel::CompleterTestModel(const QStringList &list1, const QStringList &list2, QObject *parent)
    : QAbstractTableModel(parent),
      column1(list1),
      column2(list2)
{
}

CompleterTestModel::~CompleterTestModel()
{
}

QModelIndex CompleterTestModel::index(int row, int column, const QModelIndex &parent) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column, 0) : QModelIndex();
}

QModelIndex CompleterTestModel::parent(const QModelIndex & /* index */) const
{
    return QModelIndex();
}

int CompleterTestModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

int CompleterTestModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    int maxRow = qMax(column1.count(), column2.count());
    return maxRow;
}

QVariant CompleterTestModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role);
    int row = index.row();
    int column = index.column();
    QString value;
    if (row < rowCount() && row >= 0 && column < 2 && column >= 0) {
        if (column == 0 && row < column1.count())
            value = column1[row];
        else if (column == 1 && row < column2.count())
            value = column2[row];
    }
    return QVariant(value);
}

/*!
 * Called once before the first testcase is run.
 */
void Ut_MCompleter::initTestCase()
{
    int dummyArgc = 1;
    char *dummyArgv = (char *) "./ut_mcompleter";

    // prevent loading of minputcontext because we don't need it
    MApplication::setLoadMInputContext(false);

    m_app = new MApplication(dummyArgc, &dummyArgv);
    m_window = new MApplicationWindow();
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    m_editWidget = new MTextEdit(MTextEditModel::MultiLine, "");
    m_subject = new MCompleter();
    m_editWidget->setCompleter(m_subject);

    m_window->scene()->addItem(m_editWidget);
    m_window->scene()->addItem(m_subject);
    QEvent activate(QEvent::WindowActivate);
    qApp->sendEvent(m_window->scene(), &activate);

    QStringList modelColumn1;
    modelColumn1.append("Tom While");
    modelColumn1.append("Nancy");
    modelColumn1.append("Jenny Branc");
    modelColumn1.append("Park Hammer");
    modelColumn1.append("Tom Branc");
    modelColumn1.append("TomasJerry");
    modelColumn1.append("Antony");
    modelColumn1.append("Branc");
    modelColumn1.append("Charles");
    modelColumn1.append("davids");
    modelColumn1.append("Edison");
    modelColumn1.append("Frank");
    QStringList modelColumn2;
    modelColumn2.append("<tom.w@example.com>");
    modelColumn2.append("<nancy@example.com>");
    modelColumn2.append("<j.branc@example.com>");
    modelColumn2.append("<p.hammer@example.com>");
    modelColumn2.append("<tom.b@example.com>");
    modelColumn2.append("<tomas.j@example.com>");
    modelColumn2.append("<antony@example.com>");
    modelColumn2.append("<branc.j@example.com>");
    modelColumn2.append("<charles@example.com>");
    modelColumn2.append("<davids@example.com>");
    modelColumn2.append("<edison@example.com>");
    modelColumn2.append("<frank@example.com>");
    testModel = new CompleterTestModel(modelColumn1, modelColumn2, this);
}


/*!
 * Called once after the last testcase has finished.
 */
void Ut_MCompleter::cleanupTestCase()
{
    delete m_editWidget;
    m_editWidget = 0;
    delete m_subject;
    m_subject = 0;
    delete m_window;
    m_window = 0;
    delete m_app;
    m_app = 0;
}


/*!
 * Called before each testcase.
 */
void Ut_MCompleter::init()
{
    m_subject->setCandidateSourceModel(testModel);
    m_subject->setCharactersToTrim(QString("<>"));
    m_subject->setAcceptMultipleEntries(true);
    m_editWidget->setText("");
    //set the completing time interval to 0, avoid wait
    (static_cast<MCompleterPrivate *>(m_subject->d_ptr))->completerTimer->setInterval(0);

    m_window->scene()->setFocusItem(m_editWidget);
}


/*!
 * Called after each testcase.
 */
void Ut_MCompleter::cleanup()
{
}

void Ut_MCompleter::checkConstruct()
{
    QStringList testlist;
    testlist << "apple" << "appreciate" << "orange" << "offset";
    MCompleter *completer1 = new MCompleter(testlist);
    QStringList testlist2;
    QVariant var;
    for (int i = 0; i < completer1->candidateSourceModel()->rowCount(); i++) {
        var  = completer1->candidateSourceModel()->data(completer1->candidateSourceModel()->index(i, 0));
        if (var.isValid())
            testlist2 << var.toString();
    }
    QCOMPARE(testlist2, testlist);
    delete completer1;

    QStringList testlist1;
    testlist1.append("Tom While <tom.w@example.com>");
    testlist1.append("Nancy Lee <nancy@example.com>");
    testlist1.append("Jenny Branc <j.branc@example.com>");
    testlist1.append("Park Hammer <p.hammer@example.com>");
    testlist1.append("Tom Branc <tom.b@example.com>");
    testlist1.append("TomasJerry <tomas.j@example.com>");
    testlist1.append("Nice Bright <nice.b@example.com>");
    testlist1.append("Janice Manus <janice@example.com>");

    QStringList cs, c1, c2;
    foreach(QString s, testlist1) {
        cs = s.split("<");
        if (cs.count() >= 2) {
            c1 << cs[0];
            c2 << QString('<' + cs[1]);
        }
    }
    CompleterTestModel *testModel2 = new CompleterTestModel(c1, c2, this);
    MCompleter *completer2 = new MCompleter(testModel2);

    for (int column = 0; column < completer2->candidateSourceModel()->columnCount(); column++) {
        for (int row = 0; row < completer2->candidateSourceModel()->rowCount(); row++) {
            var  = completer2->candidateSourceModel()->data(completer2->candidateSourceModel()->index(row, 0));
            if (var.isValid()) {
                QVERIFY(row < testlist1.count());
                QVERIFY(testlist1[row].indexOf(var.toString()) >= 0);
            }
        }
    }

    delete testModel2;
    delete completer2;

    MCompleter *completer3 = new MCompleter();
    completer3->setCandidateSourceModel(testModel);
    delete completer3;


}

void Ut_MCompleter::checkWidget()
{
    //test the widget
    QCOMPARE(m_subject->widget(), m_editWidget);
}


void Ut_MCompleter::checkPrefix()
{
    // TODO: check for correct QString and QModelIndex in signals, too.
    QSignalSpy spy0(m_subject, SIGNAL(startCompleting(QString)));
    QSignalSpy spy1(m_subject, SIGNAL(startCompleting(QString, QModelIndex)));

    QString prefix2 = "ap";
    m_editWidget->setText(prefix2);
    m_subject->complete();
    while (qApp->hasPendingEvents()) {
        qApp->processEvents();
    }
    //the completion prefix should be the one set in text entry
    QCOMPARE(m_subject->completionPrefix(), prefix2);

    QCOMPARE(spy0.count(), 1);
    QCOMPARE(spy1.count(), 1);
}

void Ut_MCompleter::checkCandidates()
{
    // TODO: check for correct QString and QModelIndex in signals, too.
    QSignalSpy spy0(m_subject, SIGNAL(confirmed(QString)));
    QSignalSpy spy1(m_subject, SIGNAL(confirmed(QString, QModelIndex)));

    QStringList testlist;
    testlist << "apple" << "appreciate" << "orange" << "offset"
             << "test" << "judgement" << "plane" << "never" << "northern";
    m_subject->setCandidateSourceModel(new QStringListModel(testlist, this));

    //checke the candidates
    QVariant var;
    QStringList testlist2;
    for (int i = 0; i < m_subject->candidateSourceModel()->rowCount(); i++) {
        var  = m_subject->candidateSourceModel()->data(m_subject->candidateSourceModel()->index(i, 0));
        if (var.isValid())
            testlist2 << var.toString();
    }
    QCOMPARE(testlist2, testlist);

    QString prefix = "a";
    m_editWidget->setText(prefix);
    m_subject->complete();
    while (qApp->hasPendingEvents()) {
        qApp->processEvents();
    }
    //check the matched candidates
    QCOMPARE(m_subject->matchedCandidateModel()->rowCount(), 2);
    var = m_subject->matchedCandidateModel()->data(m_subject->matchedCandidateModel()->index(0, 0));
    QString toBeConfirm = var.toString();
    m_subject->confirm();
    QVERIFY(m_editWidget->text() == toBeConfirm);

    //using model
    m_subject->setCandidateSourceModel(testModel);
    prefix = "tomas";
    m_editWidget->setText(prefix);
    m_subject->complete();
    while (qApp->hasPendingEvents()) {
        qApp->processEvents();
    }
    QCOMPARE(m_subject->matchedCandidateModel()->rowCount(), 1);
    prefix = "tom";
    m_editWidget->setText(prefix);
    m_subject->complete();
    while (qApp->hasPendingEvents()) {
        qApp->processEvents();
    }
    QCOMPARE(m_subject->matchedCandidateModel()->rowCount(), 3);

    QCOMPARE(spy0.count(), 1);
    QCOMPARE(spy1.count(), 1);
}

void Ut_MCompleter::checkMultipleEntries()
{
    m_subject->setAcceptMultipleEntries(true);
    QVERIFY(m_subject->acceptMultipleEntries());

    //first confirm
    QString prefix = "tomas";
    m_editWidget->setText(prefix);
    m_subject->complete();
    while (qApp->hasPendingEvents()) {
        qApp->processEvents();
    }
    QCOMPARE(m_subject->completionPrefix(), prefix);
    QCOMPARE(m_subject->matchedCandidateModel()->rowCount(), 1);
    m_subject->confirm();
    QCOMPARE(m_editWidget->text(), QString("tomas.j@example.com"));

    //second confirm
    prefix = "nancy";
    m_editWidget->setText(QString("%1%2%3").arg(m_editWidget->text()).arg(",").arg(prefix));
    m_subject->complete();
    while (qApp->hasPendingEvents()) {
        qApp->processEvents();
    }
    QCOMPARE(m_subject->completionPrefix(), prefix);
    m_subject->confirm();
    //acceptMultipleEntries will accept second confirm
    QVERIFY(m_editWidget->text().indexOf(QString("nancy@example.com")) > 0);

    m_subject->setAcceptMultipleEntries(false);
    QVERIFY(!m_subject->acceptMultipleEntries());

    //first confirm
    prefix = "tomas";
    m_editWidget->setText(prefix);
    m_subject->complete();
    while (qApp->hasPendingEvents()) {
        qApp->processEvents();
    }
    QCOMPARE(m_subject->completionPrefix(), prefix);
    QCOMPARE(m_subject->matchedCandidateModel()->rowCount(), 1);
    m_subject->confirm();
    QCOMPARE(m_editWidget->text(), QString("tomas.j@example.com"));

    prefix = "nancy";
    m_editWidget->setText(QString("%1%2%3").arg(m_editWidget->text()).arg(",").arg(prefix));
    m_subject->complete();
    while (qApp->hasPendingEvents()) {
        qApp->processEvents();
    }
    m_subject->confirm();
    //if acceptMultipleEntries==false, will not  accept second confirm
    QVERIFY(m_editWidget->text().indexOf(QString("nancy@example.com")) == -1);

    m_subject->setAcceptMultipleEntries(false);
    QVERIFY(!m_subject->acceptMultipleEntries());
}

void Ut_MCompleter::checkComplete()
{
    //using model, and give left and right delimiters
    MCompleter *completer1 = new MCompleter(testModel);
    completer1->setCharactersToTrim(QString("<>"));
    MTextEdit *editWidget1 = new MTextEdit(MTextEditModel::MultiLine, "", 0);
    editWidget1->setCompleter(completer1);
    QCOMPARE(completer1->widget(), editWidget1);
    m_window->scene()->addItem(editWidget1);
    m_window->scene()->setFocusItem(editWidget1);
    //set the completing time interval to 0, avoid wait
    (static_cast<MCompleterPrivate *>(completer1->d_ptr))->completerTimer->setInterval(0);


    QString prefix = "tom";
    editWidget1->setText(prefix);
    completer1->complete();
    while (qApp->hasPendingEvents()) {
        qApp->processEvents();
    }

    QCOMPARE(completer1->completionPrefix(), prefix);
    QCOMPARE(completer1->matchedCandidateModel()->rowCount(), 3);
    prefix = "tomas";
    editWidget1->setText(prefix);
    completer1->complete();
    while (qApp->hasPendingEvents()) {
        qApp->processEvents();
    }
    QCOMPARE(completer1->completionPrefix(), prefix);
    QCOMPARE(completer1->matchedCandidateModel()->rowCount(), 1);
    QVariant var = completer1->matchedCandidateModel()->data(completer1->matchedCandidateModel()->index(0, 0));
    QVERIFY(var.toString().indexOf("<tomas.j@example.com>") > 0);

    //after confirm, the left and right delimiters will be trimed
    completer1->confirm();
    QCOMPARE(editWidget1->text(), QString("tomas.j@example.com"));

    //if the number of matched candidates is larger than 10, then complete won't get all candidates
    //call queryAll() can get all
    prefix = "com";
    editWidget1->setText(prefix);
    completer1->complete();
    while (qApp->hasPendingEvents()) {
        qApp->processEvents();
    }
    QCOMPARE(completer1->completionPrefix(), prefix);
    QVERIFY(completer1->matchedCandidateModel()->rowCount() < 12);
    completer1->queryAll();
    QVERIFY(completer1->matchedCandidateModel()->rowCount() == 12);

    delete editWidget1;
    delete completer1;
}

void Ut_MCompleter::checkTrim()
{
    //if don't trim
    m_subject->setCharactersToTrim("");
    QString prefix = "tomas";
    m_editWidget->setText(prefix);
    m_subject->complete();
    while (qApp->hasPendingEvents()) {
        qApp->processEvents();
    }
    QCOMPARE(m_subject->completionPrefix(), prefix);
    QCOMPARE(m_subject->matchedCandidateModel()->rowCount(), 1);
    m_subject->confirm();
    QCOMPARE(m_editWidget->text(), QString("<tomas.j@example.com>"));

    //if do trim
    m_subject->setCharactersToTrim("<>");
    //second confirm
    prefix = "nancy";
    m_editWidget->setText(prefix);
    m_subject->complete();
    while (qApp->hasPendingEvents()) {
        qApp->processEvents();
    }
    QCOMPARE(m_subject->completionPrefix(), prefix);
    m_subject->confirm();
    QCOMPARE(m_editWidget->text(), QString("nancy@example.com"));

}

QTEST_APPLESS_MAIN(Ut_MCompleter);

