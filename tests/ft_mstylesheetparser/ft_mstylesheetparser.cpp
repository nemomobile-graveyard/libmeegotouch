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

#include "ft_mstylesheetparser.h"
#include "mapplication.h"
#include <time.h>
#include <QtTest>
#include <mstylesheetselector.h>
#include "../../src/corelib/style/mstylesheetattribute.h"
#include "../../src/corelib/theme/mlogicalvalues.h"

// include this to get theme profiling support
//#include "../../src/corelib/theme/mtheme_p.h"

const int NUMBER_OF_LOOPS = 100;

Ft_MStyleSheetParser::Ft_MStyleSheetParser()
  : m_logicalValues(NULL),
    m_subject(NULL)
{
}

void Ft_MStyleSheetParser::init()
{
    m_logicalValues = new MLogicalValues;
    m_logicalValues->append(qApp->applicationDirPath() + QDir::separator() + "ft_mstylesheetparser_logicalvalues.ini");
    m_subject = new MStyleSheetParser(m_logicalValues);
}

void Ft_MStyleSheetParser::cleanup()
{
    delete m_subject;
    delete m_logicalValues;
}

MApplication *app;

void Ft_MStyleSheetParser::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ft_mstylesheetparser" };
    app = new MApplication(argc, app_name);
}

void Ft_MStyleSheetParser::cleanupTestCase()
{
    delete app;
}

const int SELECTOR_COUNT = 23;


void Ft_MStyleSheetParser::test_load()
{
    QMap<QString, QString> attributes;
    attributes.insert(MStyleSheetAttribute::attributeNameToPropertyName("attribute-integer"), "10");
    attributes.insert(MStyleSheetAttribute::attributeNameToPropertyName("attribute-real"), "10.0");
    attributes.insert(MStyleSheetAttribute::attributeNameToPropertyName("attribute-string"), "\"string;\"");
    attributes.insert(MStyleSheetAttribute::attributeNameToPropertyName("attribute-bool"), "true");
    attributes.insert(MStyleSheetAttribute::attributeNameToPropertyName("attribute-point"), "10px 10px");
    attributes.insert(MStyleSheetAttribute::attributeNameToPropertyName("attribute-font"), "sans 10px");
    attributes.insert(MStyleSheetAttribute::attributeNameToPropertyName("attribute-color"), "#ffffff");

    QMap<QString, QString> parentAttributes;
    parentAttributes.insert(MStyleSheetAttribute::attributeNameToPropertyName("attribute-integer"), "10");
    parentAttributes.insert(MStyleSheetAttribute::attributeNameToPropertyName("attribute-real"), "10.0");
    parentAttributes.insert(MStyleSheetAttribute::attributeNameToPropertyName("attribute-string"), "\"string\"");
    parentAttributes.insert(MStyleSheetAttribute::attributeNameToPropertyName("attribute-bool"), "true");
    parentAttributes.insert(MStyleSheetAttribute::attributeNameToPropertyName("attribute-point"), "10px 10px");
    parentAttributes.insert(MStyleSheetAttribute::attributeNameToPropertyName("attribute-font"), "sans 10px");
    parentAttributes.insert(MStyleSheetAttribute::attributeNameToPropertyName("attribute-color"), "#ffffff");

    m_subject->setBinaryFileGenerationEnabled(false);

    // Test that the load fails if file doesn't exist
    QCOMPARE(m_subject->load("non-existing-file.css"), false);

    // Open test file
    QCOMPARE(m_subject->load(qApp->applicationDirPath() + "/ft_mstylesheetparser_test.css"), true);
    QCOMPARE(m_subject->fileInfoList().count(), 1);

    MStyleSheetParser::StylesheetFileInfo *fi = m_subject->fileInfoList().value(0);




    // Check that there's correct amount of selectors
    QCOMPARE(fi->selectors.count() + fi->parentSelectors.count(), SELECTOR_COUNT);
    int selectorId = 0;
    int parentSelectorId = 0;

    QCOMPARE(fi->selectors[selectorId]->parentName(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->className(), QString("TestObject"));
    QCOMPARE(fi->selectors[selectorId]->objectName(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->classType(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->orientation(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->mode(), QString(""));
    QCOMPARE(fi->selectors[selectorId++]->attributes()->count(), 7);
    //check that the attributes contain right values
    MAttributeList::const_iterator i;
    for (i = fi->selectors[0]->attributes()->constBegin(); i != fi->selectors[0]->attributes()->constEnd(); ++i) {
        QVERIFY(attributes.contains((*i)->name));
        QCOMPARE((*i)->value, attributes.value((*i)->name));
    }

    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentName(), QString("ParentName"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->className(), QString("TestObject"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->objectName(), QString(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->classType(), QString(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->orientation(), QString(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->mode(), QString(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId++]->attributes()->count(), 7);
    //check that the attributes contain right values
    for (i = fi->parentSelectors[0]->attributes()->constBegin(); i != fi->parentSelectors[0]->attributes()->constEnd(); ++i) {
        QVERIFY(parentAttributes.contains((*i)->name));
        QCOMPARE((*i)->value, parentAttributes.value((*i)->name));
    }



    QCOMPARE(fi->selectors[selectorId]->parentName(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->className(), QString("TestObject"));
    QCOMPARE(fi->selectors[selectorId]->objectName(), QString("Specialized"));
    QCOMPARE(fi->selectors[selectorId]->classType(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->orientation(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->mode(), QString(""));
    QVERIFY((fi->selectors[selectorId]->flags() & MStyleSheetSelector::Child)  == 0);
    QCOMPARE(fi->selectors[selectorId++]->attributes()->count(), 3);

    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentName(), QString("ParentName"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->className(), QString("TestObject"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->objectName(), QString("Specialized"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->classType(), QString(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->orientation(), QString(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->mode(), QString(""));
    QVERIFY((fi->parentSelectors[parentSelectorId]->flags() & MStyleSheetSelector::Child)  == 0);
    QCOMPARE(fi->parentSelectors[parentSelectorId++]->attributes()->count(), 3);



    QCOMPARE(fi->selectors[selectorId]->parentName(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->className(), QString("TestObject"));
    QCOMPARE(fi->selectors[selectorId]->objectName(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->classType(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->orientation(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->mode(), QString("Disabled"));
    QCOMPARE(fi->selectors[selectorId++]->attributes()->count(), 1);

    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentName(), QString("ParentName"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->className(), QString("TestObject"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->objectName(), QString(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->classType(), QString(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->orientation(), QString(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->mode(), QString("Disabled"));
    QCOMPARE(fi->parentSelectors[parentSelectorId++]->attributes()->count(), 1);



    QCOMPARE(fi->selectors[selectorId]->parentName(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->className(), QString("TestObject"));
    QCOMPARE(fi->selectors[selectorId]->objectName(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->classType(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->orientation(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->mode(), QString("Active"));
    QCOMPARE(fi->selectors[selectorId++]->attributes()->count(), 1);

    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentName(), QString("ParentName"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->className(), QString("TestObject"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->objectName(), QString(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->classType(), QString(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->orientation(), QString(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->mode(), QString("Active"));
    QCOMPARE(fi->parentSelectors[parentSelectorId++]->attributes()->count(), 1);



    QCOMPARE(fi->selectors[selectorId]->parentName(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->className(), QString("TestObject"));
    QCOMPARE(fi->selectors[selectorId]->objectName(), QString("Specialized"));
    QCOMPARE(fi->selectors[selectorId]->classType(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->orientation(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->mode(), QString("Active"));
    QCOMPARE(fi->selectors[selectorId++]->attributes()->count(), 1);

    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentName(), QString("ParentName"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->className(), QString("TestObject"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->objectName(), QString("Specialized"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->classType(), QString(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->orientation(), QString(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->mode(), QString("Active"));
    QCOMPARE(fi->parentSelectors[parentSelectorId++]->attributes()->count(), 1);



    QCOMPARE(fi->selectors[selectorId]->parentName(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->className(), QString("TestObject"));
    QCOMPARE(fi->selectors[selectorId]->objectName(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->classType(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->orientation(), QString("Portrait"));
    QCOMPARE(fi->selectors[selectorId]->mode(), QString(""));
    QCOMPARE(fi->selectors[selectorId++]->attributes()->count(), 1);

    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentName(), QString("ParentName"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->className(), QString("TestObject"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->objectName(), QString(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->classType(), QString(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->orientation(), QString("Portrait"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->mode(), QString(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId++]->attributes()->count(), 1);



    QCOMPARE(fi->selectors[selectorId]->parentName(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->className(), QString("TestObject"));
    QCOMPARE(fi->selectors[selectorId]->objectName(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->classType(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->orientation(), QString("Landscape"));
    QCOMPARE(fi->selectors[selectorId]->mode(), QString(""));
    QCOMPARE(fi->selectors[selectorId++]->attributes()->count(), 1);

    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentName(), QString("ParentName"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->className(), QString("TestObject"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->objectName(), QString(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->classType(), QString(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->orientation(), QString("Landscape"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->mode(), QString(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId++]->attributes()->count(), 1);



    QCOMPARE(fi->selectors[selectorId]->parentName(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->className(), QString("TestObject"));
    QCOMPARE(fi->selectors[selectorId]->objectName(), QString("Specialized"));
    QCOMPARE(fi->selectors[selectorId]->classType(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->orientation(), QString("Portrait"));
    QCOMPARE(fi->selectors[selectorId]->mode(), QString("Active"));
    QCOMPARE(fi->selectors[selectorId++]->attributes()->count(), 2);

    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentName(), QString("ParentName"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->className(), QString("TestObject"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->objectName(), QString("Specialized"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->classType(), QString(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->orientation(), QString("Portrait"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->mode(), QString("Active"));
    QCOMPARE(fi->parentSelectors[parentSelectorId++]->attributes()->count(), 2);



    QCOMPARE(fi->selectors[selectorId]->parentName(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->className(), QString("TestObject"));
    QCOMPARE(fi->selectors[selectorId]->objectName(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->classType(), QString("icon"));
    QCOMPARE(fi->selectors[selectorId]->orientation(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->mode(), QString(""));
    QCOMPARE(fi->selectors[selectorId++]->attributes()->count(), 1);

    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentName(), QString("ParentName"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->className(), QString("TestObject"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->objectName(), QString(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->classType(), QString("icon"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->orientation(), QString(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->mode(), QString(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId++]->attributes()->count(), 1);



    QCOMPARE(fi->selectors[selectorId]->parentName(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->className(), QString("TestObject"));
    QCOMPARE(fi->selectors[selectorId]->objectName(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->classType(), QString("fancy"));
    QCOMPARE(fi->selectors[selectorId]->orientation(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->mode(), QString(""));
    QCOMPARE(fi->selectors[selectorId++]->attributes()->count(), 1);

    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentName(), QString("ParentName"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->className(), QString("TestObject"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->objectName(), QString(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->classType(), QString("fancy"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->orientation(), QString(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->mode(), QString(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId++]->attributes()->count(), 1);



    QCOMPARE(fi->selectors[selectorId]->parentName(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->className(), QString("TestObject"));
    QCOMPARE(fi->selectors[selectorId]->objectName(), QString("Specialized"));
    QCOMPARE(fi->selectors[selectorId]->classType(), QString("icon"));
    QCOMPARE(fi->selectors[selectorId]->orientation(), QString(""));
    QCOMPARE(fi->selectors[selectorId]->mode(), QString(""));
    QCOMPARE(fi->selectors[selectorId++]->attributes()->count(), 1);

    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentName(), QString("ParentName"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->className(), QString("TestObject"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->objectName(), QString("Specialized"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->classType(), QString("icon"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->orientation(), QString(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->mode(), QString(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId++]->attributes()->count(), 1);

    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentName(), QString("ParentName"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->className(), QString("TestObject"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->objectName(), QString("Specialized"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->classType(), QString("icon"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->orientation(), QString(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->mode(), QString(""));
    QVERIFY((fi->parentSelectors[parentSelectorId]->flags() & MStyleSheetSelector::Child) != 0);
    QCOMPARE(fi->parentSelectors[parentSelectorId++]->attributes()->count(), 1);
}

void Ft_MStyleSheetParser::test_import()
{
    m_subject->setBinaryFileGenerationEnabled(false);

    QList<QString> importedFiles;
    importedFiles.append("ft_mstylesheetparser_import.css");
    importedFiles.append("ft_mstylesheetparser_import1.css");
    importedFiles.append("ft_mstylesheetparser_import2.css");
    importedFiles.append("ft_mstylesheetparser_constants1.css");
    importedFiles.append("ft_mstylesheetparser_test.css");

    QCOMPARE(m_subject->load(qApp->applicationDirPath() + "/ft_mstylesheetparser_import.css"), true);
    QCOMPARE(m_subject->fileInfoList().count(), 5);
    int numberOfSelectors = 0;
    for (QList<MStyleSheetParser::StylesheetFileInfo *>::iterator fi = m_subject->fileInfoList().begin();
            fi != m_subject->fileInfoList().end();
            fi++) {
        QFileInfo fileinfo((*fi)->filename);
        QVERIFY(importedFiles.indexOf(fileinfo.fileName()) != -1);
        numberOfSelectors += (*fi)->selectors.count();
        numberOfSelectors += (*fi)->parentSelectors.count();
    }
    QCOMPARE(numberOfSelectors, SELECTOR_COUNT);
}

void Ft_MStyleSheetParser::test_constants()
{
    m_subject->setBinaryFileGenerationEnabled(false);

    // Open test file
    QCOMPARE(m_subject->load(qApp->applicationDirPath() + "/ft_mstylesheetparser_constants.css"), true);

    //check that file count is correct
    QCOMPARE(m_subject->fileInfoList().count(), 3);

    MStyleSheetParser::StylesheetFileInfo *info = m_subject->fileInfoList()[0];
    QCOMPARE(info->constants.count(), 5);
    QCOMPARE(info->constants["cWidth"], QString("10px"));
    QCOMPARE(info->constants["cHeight"], QString("15px"));
    QCOMPARE(info->constants["cFontFamily"], QString("sans"));
    QCOMPARE(info->constants["cFontSize"], QString("12px"));
    QCOMPARE(info->constants["cColor"], QString("#0abba0"));
    info = m_subject->fileInfoList()[1];
    QCOMPARE(info->constants.count(), 0);
    info = m_subject->fileInfoList()[2];
    QCOMPARE(info->constants.count(), 7);
    QCOMPARE(info->constants["cInt"], QString("10"));
    QCOMPARE(info->constants["cReal"], QString("1.0"));
    QCOMPARE(info->constants["cName"], QString("\"name\""));
    QCOMPARE(info->constants["cColor"], QString("#0dead0"));
    QCOMPARE(info->constants["cBool"], QString("true"));
    QCOMPARE(info->constants["cPoint"], QString("17px 11px"));
    QCOMPARE(info->constants["cFont"], QString("arial 10px"));

    //check that there are right number of attributes
    info = m_subject->fileInfoList()[0];
    QCOMPARE(info->selectors.count(), 0);
    info = m_subject->fileInfoList()[1];
    QCOMPARE(info->selectors.count(), 1);
    QCOMPARE(info->selectors[0]->attributes()->count(), 1);
    info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-width"), NULL);
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-width"), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-width"))->name, QString(MStyleSheetAttribute::attributeNameToPropertyName("attr-width")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-width"))->value, QString("10px"));
    info = m_subject->fileInfoList()[2];
    QCOMPARE(info->selectors.count(), 1);
    QCOMPARE(info->selectors[0]->attributes()->count(), 13);
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-int"), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-int"))->name, QString(MStyleSheetAttribute::attributeNameToPropertyName("attr-int")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-int"))->value, QString("10"));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-string1"), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-string1"))->name, QString(MStyleSheetAttribute::attributeNameToPropertyName("attr-string1")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-string1"))->value, QString("\"name\""));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-string2"), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-string2"))->name, QString(MStyleSheetAttribute::attributeNameToPropertyName("attr-string2")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-string2"))->value, QString("\"this is a string with constant $cName\""));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-bool"), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-bool"))->name, QString(MStyleSheetAttribute::attributeNameToPropertyName("attr-bool")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-bool"))->value, QString("true"));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos1"), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos1"))->name, QString(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos1")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos1"))->value, QString("10px 15px"));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos2"), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos2"))->name, QString(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos2")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos2"))->value, QString("17px 11px"));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-font1"), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-font1"))->name, QString(MStyleSheetAttribute::attributeNameToPropertyName("attr-font1")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-font1"))->value, QString("sans 12px"));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-font2"), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-font2"))->name, QString(MStyleSheetAttribute::attributeNameToPropertyName("attr-font2")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-font2"))->value, QString("arial 10px"));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-color"), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-color"))->name, QString(MStyleSheetAttribute::attributeNameToPropertyName("attr-color")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-color"))->value, QString("#0abba0"));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-invalid"), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-invalid"))->name, QString(MStyleSheetAttribute::attributeNameToPropertyName("attr-invalid")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-invalid"))->value, QString(""));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-void"), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-void"))->name, QString(MStyleSheetAttribute::attributeNameToPropertyName("attr-void")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-void"))->value, QString(""));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-logical-black"))->name, QString(MStyleSheetAttribute::attributeNameToPropertyName("attr-logical-black")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-logical-black"))->value, QString("#000000"));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-logical-green"))->name, QString(MStyleSheetAttribute::attributeNameToPropertyName("attr-logical-green")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-logical-green"))->value, QString("#00ff00"));
}

void Ft_MStyleSheetParser::test_constants_binary()
{
    //read css from binary
    m_subject->setBinaryFileGenerationEnabled(true);
    QCOMPARE(m_subject->load(qApp->applicationDirPath() + "/ft_mstylesheetparser_constants.css"), true);
    delete m_subject;

    m_subject = new MStyleSheetParser(m_logicalValues);
    QCOMPARE(m_subject->load(qApp->applicationDirPath() + "/ft_mstylesheetparser_constants.css"), true);

    //check that file count is correct
    QCOMPARE(m_subject->fileInfoList().count(), 3);

    MStyleSheetParser::StylesheetFileInfo *info = m_subject->fileInfoList()[0];
    QCOMPARE(info->constants.count(), 5);
    QCOMPARE(info->constants["cWidth"], QString("10px"));
    QCOMPARE(info->constants["cHeight"], QString("15px"));
    QCOMPARE(info->constants["cFontFamily"], QString("sans"));
    QCOMPARE(info->constants["cFontSize"], QString("12px"));
    QCOMPARE(info->constants["cColor"], QString("#0abba0"));
    info = m_subject->fileInfoList()[1];
    QCOMPARE(info->constants.count(), 0);
    info = m_subject->fileInfoList()[2];
    QCOMPARE(info->constants.count(), 7);
    QCOMPARE(info->constants["cInt"], QString("10"));
    QCOMPARE(info->constants["cReal"], QString("1.0"));
    QCOMPARE(info->constants["cName"], QString("\"name\""));
    QCOMPARE(info->constants["cColor"], QString("#0dead0"));
    QCOMPARE(info->constants["cBool"], QString("true"));
    QCOMPARE(info->constants["cPoint"], QString("17px 11px"));
    QCOMPARE(info->constants["cFont"], QString("arial 10px"));

    //check that there are right number of attributes
    info = m_subject->fileInfoList()[0];
    QCOMPARE(info->selectors.count(), 0);
    info = m_subject->fileInfoList()[1];
    QCOMPARE(info->selectors.count(), 1);
    QCOMPARE(info->selectors[0]->attributes()->count(), 1);
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-width"), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-width"))->name, QString(MStyleSheetAttribute::attributeNameToPropertyName("attr-width")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-width"))->value, QString("10px"));
    info = m_subject->fileInfoList()[2];
    QCOMPARE(info->selectors.count(), 1);
    QCOMPARE(info->selectors[0]->attributes()->count(), 13);
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-int"), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-int"))->name, QString(MStyleSheetAttribute::attributeNameToPropertyName("attr-int")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-int"))->value, QString("10"));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-string1"), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-string1"))->name, QString(MStyleSheetAttribute::attributeNameToPropertyName("attr-string1")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-string1"))->value, QString("\"name\""));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-string2"), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-string2"))->name, QString(MStyleSheetAttribute::attributeNameToPropertyName("attr-string2")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-string2"))->value, QString("\"this is a string with constant $cName\""));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-bool"), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-bool"))->name, QString(MStyleSheetAttribute::attributeNameToPropertyName("attr-bool")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-bool"))->value, QString("true"));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos1"), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos1"))->name, QString(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos1")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos1"))->value, QString("10px 15px"));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos2"), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos2"))->name, QString(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos2")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos2"))->value, QString("17px 11px"));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-font1"), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-font1"))->name, QString(MStyleSheetAttribute::attributeNameToPropertyName("attr-font1")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-font1"))->value, QString("sans 12px"));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-font2"), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-font2"))->name, QString(MStyleSheetAttribute::attributeNameToPropertyName("attr-font2")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-font2"))->value, QString("arial 10px"));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-color"), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-color"))->name, QString(MStyleSheetAttribute::attributeNameToPropertyName("attr-color")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-color"))->value, QString("#0abba0"));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-invalid"), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-invalid"))->name, QString(MStyleSheetAttribute::attributeNameToPropertyName("attr-invalid")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-invalid"))->value, QString(""));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-void"), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-void"))->name, QString(MStyleSheetAttribute::attributeNameToPropertyName("attr-void")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-void"))->value, QString(""));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-logical-black"))->name, QString(MStyleSheetAttribute::attributeNameToPropertyName("attr-logical-black")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-logical-black"))->value, QString("#000000"));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-logical-green"))->name, QString(MStyleSheetAttribute::attributeNameToPropertyName("attr-logical-green")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetAttribute::attributeNameToPropertyName("attr-logical-green"))->value, QString("#00ff00"));
}

void Ft_MStyleSheetParser::test_binary_equality()
{
    // parse test css file, dumping it to file
    MStyleSheetParser parser;
    parser.setBinaryFileGenerationEnabled(true);
    QCOMPARE(parser.load(qApp->applicationDirPath() + "/ft_mstylesheetparser_test.css"), true);

//    // load binary file, see that it exists
//    QVERIFY(QFile::exists(QDir::tempPath() + QDir::separator() + "ft_mstylesheetparser_test.css.bin"));

    MStyleSheetParser binary;
    QCOMPARE(binary.load(qApp->applicationDirPath() + "/ft_mstylesheetparser_test.css"), true);

    // check that there is equal count of file infos
    QCOMPARE(parser.fileInfoList().count(), binary.fileInfoList().count());

    QList<MStyleSheetParser::StylesheetFileInfo *>::iterator parserFi = parser.fileInfoList().begin();
    QList<MStyleSheetParser::StylesheetFileInfo *>::iterator binaryFi = binary.fileInfoList().begin();

    for (int i = 0; i < parser.fileInfoList().count(); i++) {
        // check that there is equal count of constants
        QCOMPARE((*parserFi)->constants.count(), (*binaryFi)->constants.count());

        // check that there is equal count of selectors
        QCOMPARE((*parserFi)->selectors.count(), (*binaryFi)->selectors.count());

        QList<MStyleSheetSelector *>::iterator parserSelectors = (*parserFi)->selectors.begin();
        QList<MStyleSheetSelector *>::iterator binarySelectors = (*binaryFi)->selectors.begin();

        // loop through all the selectors, check that they are equal
        for (int i = 0; i < (*parserFi)->selectors.count(); i++) {
            // Get selectors
            MStyleSheetSelector *selector0 = *parserSelectors;
            MStyleSheetSelector *selector1 = *binarySelectors;

            // check that selector properties are equal
            QCOMPARE(selector0->parentName(), selector1->parentName());
            QCOMPARE(selector0->objectName(), selector1->objectName());
            QCOMPARE(selector0->className(), selector1->className());
            QCOMPARE(selector0->orientation(), selector1->orientation());
            QCOMPARE(selector0->mode(), selector1->mode());

            // check that amount of attributes is equal
            QCOMPARE(selector0->attributes()->count(), selector1->attributes()->count());

            // loop through all attributes, check that they are equal
            MAttributeList::iterator selector0Attributes = selector0->attributes()->begin();
            MAttributeList::iterator selector1Attributes = selector1->attributes()->begin();
            for (int attributeIndex = 0; attributeIndex < selector0->attributes()->count(); attributeIndex++) {
                QCOMPARE(selector0Attributes.key(), selector1Attributes.key());
                QCOMPARE(selector0Attributes.value()->name, selector1Attributes.value()->name);
                QCOMPARE(selector0Attributes.value()->value, selector1Attributes.value()->value);

                selector0Attributes++;
                selector1Attributes++;
            }

            parserSelectors++;
            binarySelectors++;
        }

        parserFi++;
        binaryFi++;
    }
}

void Ft_MStyleSheetParser::test_parser_speed()
{
    unsigned long int TIMERS[NUMBER_OF_LOOPS];
    unsigned long int TOTAL_TIME = 0;

    clock_t test_start, time_start;
    clock_t time_end;

    qDebug() << "Testing speed of CSS parser, reading ft_mstylesheetparser_test.css" << NUMBER_OF_LOOPS << "times";

    test_start = clock();

    for (int i = 0; i < NUMBER_OF_LOOPS; i++) {
        MStyleSheetParser p;
        p.setBinaryFileGenerationEnabled(false);

        // Start clocking
        time_start = clock();
        // Open test file
        QCOMPARE(p.load(qApp->applicationDirPath() + "/ft_mstylesheetparser_test.css"), true);
        // End clocking
        time_end = clock();

        // Store result & cleanup
        TIMERS[i] = time_end - time_start;
    }

    TOTAL_TIME = time_end - test_start;

    long average = 0;
    for (int i = 0; i < NUMBER_OF_LOOPS; i++) {
        average += TIMERS[i];
    }
    average /= NUMBER_OF_LOOPS;

    qDebug() << "Average reading time:" << average << "microseconds (" << average / 1000000.0 << "seconds)";
    qDebug() << "Total reading time:" << TOTAL_TIME << "microseconds (" << TOTAL_TIME / 1000000.0 << "seconds)";
}

void Ft_MStyleSheetParser::test_binary_speed()
{
    unsigned long int TIMERS[NUMBER_OF_LOOPS];
    unsigned long int TOTAL_TIME = 0;

    clock_t test_start, time_start;
    clock_t time_end;

    // create binary file, if it doesn't exist
    MStyleSheetParser tmp;
    tmp.setBinaryFileGenerationEnabled(true);
    QCOMPARE(tmp.load(qApp->applicationDirPath() + "/ft_mstylesheetparser_test.css"), true);

    qDebug() << "Testing speed of binary css files, reading ft_mstylesheetparser_test.css" << NUMBER_OF_LOOPS << "times";

    test_start = clock();

    for (int i = 0; i < NUMBER_OF_LOOPS; i++) {
        MStyleSheetParser p;
        p.setBinaryFileGenerationEnabled(true);

        // Start clocking
        time_start = clock();
        // Open test file
        QCOMPARE(p.load(qApp->applicationDirPath() + "/ft_mstylesheetparser_test.css"), true);
        // End clocking
        time_end = clock();

        // Store result & cleanup
        TIMERS[i] = time_end - time_start;
    }

    TOTAL_TIME = time_end - test_start;

    long average = 0;
    for (int i = 0; i < NUMBER_OF_LOOPS; i++) {
        average += TIMERS[i];
    }
    average /= NUMBER_OF_LOOPS;

    qDebug() << "Average reading time:" << average << "microseconds (" << average / 1000000.0 << "seconds)";
    qDebug() << "Total reading time:" << TOTAL_TIME << "microseconds (" << TOTAL_TIME / 1000000.0 << "seconds)";
}

QTEST_APPLESS_MAIN(Ft_MStyleSheetParser)
