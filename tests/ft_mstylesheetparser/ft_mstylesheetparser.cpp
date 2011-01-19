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

const int NUMBER_OF_LOOPS = 10000;

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

const int SELECTOR_COUNT = 29;


void Ft_MStyleSheetParser::test_load()
{
    QMap<QByteArray, QByteArray> attributes;
    attributes.insert(MStyleSheetAttribute::attributeNameToPropertyName("attribute-integer"), "10");
    attributes.insert(MStyleSheetAttribute::attributeNameToPropertyName("attribute-real"), "10.0");
    attributes.insert(MStyleSheetAttribute::attributeNameToPropertyName("attribute-string"), "\"string;\"");
    attributes.insert(MStyleSheetAttribute::attributeNameToPropertyName("attribute-bool"), "true");
    attributes.insert(MStyleSheetAttribute::attributeNameToPropertyName("attribute-point"), "10px 10px");
    attributes.insert(MStyleSheetAttribute::attributeNameToPropertyName("attribute-font"), "sans 10px");
    attributes.insert(MStyleSheetAttribute::attributeNameToPropertyName("attribute-color"), "#ffffff");

    QMap<QByteArray, QByteArray> parentAttributes;
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

    QSharedPointer<MStyleSheetParser::StylesheetFileInfo> fi = m_subject->fileInfoList().value(0);




    // Check that there's correct amount of selectors
    QCOMPARE(fi->selectors.count() + fi->parentSelectors.count(), SELECTOR_COUNT);
    int selectorId = 0;
    int parentSelectorId = 0;

    QCOMPARE(fi->selectors[selectorId]->parentName(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId]->className(), QByteArray("TestObject"));
    QCOMPARE(fi->selectors[selectorId]->objectName(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId]->classType(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId]->orientation(), MStyleSheetSelector::UndefinedOrientation);
    QCOMPARE(fi->selectors[selectorId]->mode(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId++]->attributes()->count(), 7);
    //check that the attributes contain right values
    MAttributeList::const_iterator i;
    for (i = fi->selectors[0]->attributes()->constBegin(); i != fi->selectors[0]->attributes()->constEnd(); ++i) {
        QVERIFY(attributes.contains((*i)->getName()));
        QCOMPARE((*i)->getValue(), attributes.value((*i)->getName()));
    }

    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentName(), QByteArray("ParentName"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->className(), QByteArray("TestObject"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->objectName(), QByteArray(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->classType(), QByteArray(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->orientation(), MStyleSheetSelector::UndefinedOrientation);
    QCOMPARE(fi->parentSelectors[parentSelectorId]->mode(), QByteArray(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId++]->attributes()->count(), 7);
    //check that the attributes contain right values
    for (i = fi->parentSelectors[0]->attributes()->constBegin(); i != fi->parentSelectors[0]->attributes()->constEnd(); ++i) {
        QVERIFY(parentAttributes.contains((*i)->getName()));
        QCOMPARE((*i)->getValue(), parentAttributes.value((*i)->getName()));
    }



   QCOMPARE(fi->selectors[selectorId]->parentName(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId]->className(), QByteArray("TestObject"));
    QCOMPARE(fi->selectors[selectorId]->objectName(), QByteArray("Specialized"));
    QCOMPARE(fi->selectors[selectorId]->classType(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId]->orientation(), MStyleSheetSelector::UndefinedOrientation);
    QCOMPARE(fi->selectors[selectorId]->mode(), QByteArray(""));
    QVERIFY((fi->selectors[selectorId]->flags() & MStyleSheetSelector::Child)  == 0);
    QCOMPARE(fi->selectors[selectorId++]->attributes()->count(), 3);

    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentName(), QByteArray("ParentName"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->className(), QByteArray("TestObject"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->objectName(), QByteArray("Specialized"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->classType(), QByteArray(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->orientation(), MStyleSheetSelector::UndefinedOrientation);
    QCOMPARE(fi->parentSelectors[parentSelectorId]->mode(), QByteArray(""));
    QVERIFY((fi->parentSelectors[parentSelectorId]->flags() & MStyleSheetSelector::Child)  == 0);
    QCOMPARE(fi->parentSelectors[parentSelectorId++]->attributes()->count(), 3);



    QCOMPARE(fi->selectors[selectorId]->parentName(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId]->className(), QByteArray("TestObject"));
    QCOMPARE(fi->selectors[selectorId]->objectName(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId]->classType(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId]->orientation(), MStyleSheetSelector::UndefinedOrientation);
    QCOMPARE(fi->selectors[selectorId]->mode(), QByteArray("Disabled"));
    QCOMPARE(fi->selectors[selectorId++]->attributes()->count(), 1);

    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentName(), QByteArray("ParentName"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->className(), QByteArray("TestObject"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->objectName(), QByteArray(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->classType(), QByteArray(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->orientation(), MStyleSheetSelector::UndefinedOrientation);
    QCOMPARE(fi->parentSelectors[parentSelectorId]->mode(), QByteArray("Disabled"));
    QCOMPARE(fi->parentSelectors[parentSelectorId++]->attributes()->count(), 1);



    QCOMPARE(fi->selectors[selectorId]->parentName(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId]->className(), QByteArray("TestObject"));
    QCOMPARE(fi->selectors[selectorId]->objectName(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId]->classType(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId]->orientation(), MStyleSheetSelector::UndefinedOrientation);
    QCOMPARE(fi->selectors[selectorId]->mode(), QByteArray("Active"));
    QCOMPARE(fi->selectors[selectorId++]->attributes()->count(), 1);

    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentName(), QByteArray("ParentName"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->className(), QByteArray("TestObject"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->objectName(), QByteArray(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->classType(), QByteArray(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->orientation(), MStyleSheetSelector::UndefinedOrientation);
    QCOMPARE(fi->parentSelectors[parentSelectorId]->mode(), QByteArray("Active"));
    QCOMPARE(fi->parentSelectors[parentSelectorId++]->attributes()->count(), 1);



    QCOMPARE(fi->selectors[selectorId]->parentName(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId]->className(), QByteArray("TestObject"));
    QCOMPARE(fi->selectors[selectorId]->objectName(), QByteArray("Specialized"));
    QCOMPARE(fi->selectors[selectorId]->classType(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId]->orientation(), MStyleSheetSelector::UndefinedOrientation);
    QCOMPARE(fi->selectors[selectorId]->mode(), QByteArray("Active"));
    QCOMPARE(fi->selectors[selectorId++]->attributes()->count(), 1);

    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentName(), QByteArray("ParentName"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->className(), QByteArray("TestObject"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->objectName(), QByteArray("Specialized"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->classType(), QByteArray(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->orientation(), MStyleSheetSelector::UndefinedOrientation);
    QCOMPARE(fi->parentSelectors[parentSelectorId]->mode(), QByteArray("Active"));
    QCOMPARE(fi->parentSelectors[parentSelectorId++]->attributes()->count(), 1);



    QCOMPARE(fi->selectors[selectorId]->parentName(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId]->className(), QByteArray("TestObject"));
    QCOMPARE(fi->selectors[selectorId]->objectName(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId]->classType(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId]->orientation(), MStyleSheetSelector::PortraitOrientation);
    QCOMPARE(fi->selectors[selectorId]->mode(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId++]->attributes()->count(), 1);

    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentName(), QByteArray("ParentName"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->className(), QByteArray("TestObject"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->objectName(), QByteArray(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->classType(), QByteArray(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->orientation(), MStyleSheetSelector::PortraitOrientation);
    QCOMPARE(fi->parentSelectors[parentSelectorId]->mode(), QByteArray(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId++]->attributes()->count(), 1);



    QCOMPARE(fi->selectors[selectorId]->parentName(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId]->className(), QByteArray("TestObject"));
    QCOMPARE(fi->selectors[selectorId]->objectName(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId]->classType(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId]->orientation(), MStyleSheetSelector::LandscapeOrientation);
    QCOMPARE(fi->selectors[selectorId]->mode(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId++]->attributes()->count(), 1);

    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentName(), QByteArray("ParentName"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->className(), QByteArray("TestObject"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->objectName(), QByteArray(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->classType(), QByteArray(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->orientation(), MStyleSheetSelector::LandscapeOrientation);
    QCOMPARE(fi->parentSelectors[parentSelectorId]->mode(), QByteArray(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId++]->attributes()->count(), 1);



    QCOMPARE(fi->selectors[selectorId]->parentName(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId]->className(), QByteArray("TestObject"));
    QCOMPARE(fi->selectors[selectorId]->objectName(), QByteArray("Specialized"));
    QCOMPARE(fi->selectors[selectorId]->classType(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId]->orientation(), MStyleSheetSelector::PortraitOrientation);
    QCOMPARE(fi->selectors[selectorId]->mode(), QByteArray("Active"));
    QCOMPARE(fi->selectors[selectorId++]->attributes()->count(), 2);

    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentName(), QByteArray("ParentName"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->className(), QByteArray("TestObject"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->objectName(), QByteArray("Specialized"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->classType(), QByteArray(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->orientation(), MStyleSheetSelector::PortraitOrientation);
    QCOMPARE(fi->parentSelectors[parentSelectorId]->mode(), QByteArray("Active"));
    QCOMPARE(fi->parentSelectors[parentSelectorId++]->attributes()->count(), 2);



    QCOMPARE(fi->selectors[selectorId]->parentName(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId]->className(), QByteArray("TestObject"));
    QCOMPARE(fi->selectors[selectorId]->objectName(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId]->classType(), QByteArray("icon"));
    QCOMPARE(fi->selectors[selectorId]->orientation(), MStyleSheetSelector::UndefinedOrientation);
    QCOMPARE(fi->selectors[selectorId]->mode(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId++]->attributes()->count(), 1);

    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentName(), QByteArray("ParentName"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->className(), QByteArray("TestObject"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->objectName(), QByteArray(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->classType(), QByteArray("icon"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->orientation(), MStyleSheetSelector::UndefinedOrientation);
    QCOMPARE(fi->parentSelectors[parentSelectorId]->mode(), QByteArray(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId++]->attributes()->count(), 1);



    QCOMPARE(fi->selectors[selectorId]->parentName(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId]->className(), QByteArray("TestObject"));
    QCOMPARE(fi->selectors[selectorId]->objectName(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId]->classType(), QByteArray("fancy"));
    QCOMPARE(fi->selectors[selectorId]->orientation(), MStyleSheetSelector::UndefinedOrientation);
    QCOMPARE(fi->selectors[selectorId]->mode(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId++]->attributes()->count(), 1);

    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentName(), QByteArray("ParentName"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->className(), QByteArray("TestObject"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->objectName(), QByteArray(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->classType(), QByteArray("fancy"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->orientation(), MStyleSheetSelector::UndefinedOrientation);
    QCOMPARE(fi->parentSelectors[parentSelectorId]->mode(), QByteArray(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId++]->attributes()->count(), 1);



    QCOMPARE(fi->selectors[selectorId]->parentName(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId]->className(), QByteArray("TestObject"));
    QCOMPARE(fi->selectors[selectorId]->objectName(), QByteArray("Specialized"));
    QCOMPARE(fi->selectors[selectorId]->classType(), QByteArray("icon"));
    QCOMPARE(fi->selectors[selectorId]->orientation(), MStyleSheetSelector::UndefinedOrientation);
    QCOMPARE(fi->selectors[selectorId]->mode(), QByteArray(""));
    QCOMPARE(fi->selectors[selectorId++]->attributes()->count(), 1);

    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentName(), QByteArray("ParentName"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->className(), QByteArray("TestObject"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->objectName(), QByteArray("Specialized"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->classType(), QByteArray("icon"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->orientation(), MStyleSheetSelector::UndefinedOrientation);
    QCOMPARE(fi->parentSelectors[parentSelectorId]->mode(), QByteArray(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId++]->attributes()->count(), 1);

    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentName(), QByteArray("ParentName"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->className(), QByteArray("TestObject"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->objectName(), QByteArray("Specialized"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->classType(), QByteArray("icon"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->orientation(), MStyleSheetSelector::UndefinedOrientation);
    QCOMPARE(fi->parentSelectors[parentSelectorId]->mode(), QByteArray(""));
    QVERIFY((fi->parentSelectors[parentSelectorId]->flags() & MStyleSheetSelector::Child) != 0);
    QCOMPARE(fi->parentSelectors[parentSelectorId++]->attributes()->count(), 1);

    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentName(), QByteArray("ParentName"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentObjectName(), QByteArray("Specialized"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->className(), QByteArray("TestObject"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->objectName(), QByteArray(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->classType(), QByteArray(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->orientation(), MStyleSheetSelector::UndefinedOrientation);
    QCOMPARE(fi->parentSelectors[parentSelectorId]->mode(), QByteArray(""));
    QVERIFY((fi->parentSelectors[parentSelectorId]->flags() & MStyleSheetSelector::Child) != 0);
    QCOMPARE(fi->parentSelectors[parentSelectorId++]->attributes()->count(), 1);

    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentName(), QByteArray("ParentName"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentObjectName(), QByteArray("Specialized"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->className(), QByteArray(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->objectName(), QByteArray("Specialized"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->classType(), QByteArray(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->orientation(), MStyleSheetSelector::UndefinedOrientation);
    QCOMPARE(fi->parentSelectors[parentSelectorId]->mode(), QByteArray(""));
    QVERIFY((fi->parentSelectors[parentSelectorId]->flags() & MStyleSheetSelector::Child) != 0);
    QCOMPARE(fi->parentSelectors[parentSelectorId++]->attributes()->count(), 1);

    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentName(), QByteArray("ParentName"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentObjectName(), QByteArray("Specialized"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->className(), QByteArray("TestObject"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->objectName(), QByteArray(""));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->classType(), QByteArray("icon"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->orientation(), MStyleSheetSelector::UndefinedOrientation);
    QCOMPARE(fi->parentSelectors[parentSelectorId]->mode(), QByteArray(""));
    QVERIFY((fi->parentSelectors[parentSelectorId]->flags() & MStyleSheetSelector::Child) != 0);
    QCOMPARE(fi->parentSelectors[parentSelectorId++]->attributes()->count(), 1);

    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentName(), QByteArray("ParentName"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentObjectName(), QByteArray("Specialized"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->className(), QByteArray("TestObject"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->objectName(), QByteArray("Specialized"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->classType(), QByteArray("icon"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->orientation(), MStyleSheetSelector::UndefinedOrientation);
    QCOMPARE(fi->parentSelectors[parentSelectorId]->mode(), QByteArray(""));
    QVERIFY((fi->parentSelectors[parentSelectorId]->flags() & MStyleSheetSelector::Child) != 0);
    QCOMPARE(fi->parentSelectors[parentSelectorId++]->attributes()->count(), 1);

    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentName(), QByteArray("ParentName"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentObjectName(), QByteArray("Specialized"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->className(), QByteArray("TestObject"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->objectName(), QByteArray("Specialized"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->classType(), QByteArray("icon"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->orientation(), MStyleSheetSelector::PortraitOrientation);
    QCOMPARE(fi->parentSelectors[parentSelectorId]->mode(), QByteArray(""));
    QVERIFY((fi->parentSelectors[parentSelectorId]->flags() & MStyleSheetSelector::Child) != 0);
    QCOMPARE(fi->parentSelectors[parentSelectorId++]->attributes()->count(), 1);

    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentName(), QByteArray("ParentName"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->parentObjectName(), QByteArray("Specialized"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->className(), QByteArray("TestObject"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->objectName(), QByteArray("Specialized"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->classType(), QByteArray("icon"));
    QCOMPARE(fi->parentSelectors[parentSelectorId]->orientation(), MStyleSheetSelector::PortraitOrientation);
    QCOMPARE(fi->parentSelectors[parentSelectorId]->mode(), QByteArray("Active"));
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
    for (QList<QSharedPointer<MStyleSheetParser::StylesheetFileInfo> >::iterator fi = m_subject->fileInfoList().begin();
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

    QSharedPointer<MStyleSheetParser::StylesheetFileInfo> info = m_subject->fileInfoList()[0];
    QCOMPARE(info->constants.count(), 5);
    QCOMPARE(info->constants["cWidth"], QByteArray("10px"));
    QCOMPARE(info->constants["cHeight"], QByteArray("15px"));
    QCOMPARE(info->constants["cFontFamily"], QByteArray("sans"));
    QCOMPARE(info->constants["cFontSize"], QByteArray("12px"));
    QCOMPARE(info->constants["cColor"], QByteArray("#0abba0"));
    info = m_subject->fileInfoList()[1];
    QCOMPARE(info->constants.count(), 0);
    info = m_subject->fileInfoList()[2];
    QCOMPARE(info->constants.count(), 7);
    QCOMPARE(info->constants["cInt"], QByteArray("10"));
    QCOMPARE(info->constants["cReal"], QByteArray("1.0"));
    QCOMPARE(info->constants["cName"], QByteArray("\"name\""));
    QCOMPARE(info->constants["cColor"], QByteArray("#0dead0"));
    QCOMPARE(info->constants["cBool"], QByteArray("true"));
    QCOMPARE(info->constants["cPoint"], QByteArray("17px 11px"));
    QCOMPARE(info->constants["cFont"], QByteArray("arial 10px"));

    //check that there are right number of attributes
    info = m_subject->fileInfoList()[0];
    QCOMPARE(info->selectors.count(), 0);
    info = m_subject->fileInfoList()[1];
    QCOMPARE(info->selectors.count(), 1);
    QCOMPARE(info->selectors[0]->attributes()->count(), 1);
    info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex("attr-width"), NULL);
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-width")), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-width")))->getName(), QByteArray(MStyleSheetAttribute::attributeNameToPropertyName("attr-width")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-width")))->getValue(), QByteArray("10px"));
    info = m_subject->fileInfoList()[2];
    QCOMPARE(info->selectors.count(), 1);
    QCOMPARE(info->selectors[0]->attributes()->count(), 13);
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-int")), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-int")))->getName(), QByteArray(MStyleSheetAttribute::attributeNameToPropertyName("attr-int")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-int")))->getValue(), QByteArray("10"));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-string1")), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-string1")))->getName(), QByteArray(MStyleSheetAttribute::attributeNameToPropertyName("attr-string1")));

    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-string1")))->getValue(), QByteArray("\"name\""));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-string2")), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-string2")))->getName(), QByteArray(MStyleSheetAttribute::attributeNameToPropertyName("attr-string2")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-string2")))->getValue(), QByteArray("\"this is a string with constant \"name\"\" 10 1.0"));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-bool")), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-bool")))->getName(), QByteArray(MStyleSheetAttribute::attributeNameToPropertyName("attr-bool")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-bool")))->getValue(), QByteArray("true"));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos1")), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos1")))->getName(), QByteArray(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos1")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos1")))->getValue(), QByteArray("10px 15px"));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos2")), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos2")))->getName(), QByteArray(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos2")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos2")))->getValue(), QByteArray("17px 11px"));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-font1")), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-font1")))->getName(), QByteArray(MStyleSheetAttribute::attributeNameToPropertyName("attr-font1")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-font1")))->getValue(), QByteArray("sans 12px"));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-font2")), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-font2")))->getName(), QByteArray(MStyleSheetAttribute::attributeNameToPropertyName("attr-font2")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-font2")))->getValue(), QByteArray("arial 10px"));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-color")), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-color")))->getName(), QByteArray(MStyleSheetAttribute::attributeNameToPropertyName("attr-color")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-color")))->getValue(), QByteArray("#0abba0"));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-invalid")), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-invalid")))->getName(), QByteArray(MStyleSheetAttribute::attributeNameToPropertyName("attr-invalid")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-invalid")))->getValue(), QByteArray(""));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-void")), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-void")))->getName(), QByteArray(MStyleSheetAttribute::attributeNameToPropertyName("attr-void")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-void")))->getValue(), QByteArray(""));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-logical-black")))->getName(), QByteArray(MStyleSheetAttribute::attributeNameToPropertyName("attr-logical-black")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-logical-black")))->getValue(), QByteArray("#000000"));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-logical-green")))->getName(), QByteArray(MStyleSheetAttribute::attributeNameToPropertyName("attr-logical-green")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-logical-green")))->getValue(), QByteArray("#00ff00"));
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

    QSharedPointer<MStyleSheetParser::StylesheetFileInfo> info = m_subject->fileInfoList()[0];
    // the binary files do not contain any constants
    QCOMPARE(info->constants.count(), 0);

    //check that there are right number of attributes
    info = m_subject->fileInfoList()[0];
    QCOMPARE(info->selectors.count(), 0);
    info = m_subject->fileInfoList()[1];
    QCOMPARE(info->selectors.count(), 1);
    QCOMPARE(info->selectors[0]->attributes()->count(), 1);
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-width")), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-width")))->getName(), QByteArray(MStyleSheetAttribute::attributeNameToPropertyName("attr-width")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-width")))->getValue(), QByteArray("10px"));
    info = m_subject->fileInfoList()[2];
    QCOMPARE(info->selectors.count(), 1);
    QCOMPARE(info->selectors[0]->attributes()->count(), 13);
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-int")), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-int")))->getName(), QByteArray(MStyleSheetAttribute::attributeNameToPropertyName("attr-int")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-int")))->getValue(), QByteArray("10"));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-string1")), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-string1")))->getName(), QByteArray(MStyleSheetAttribute::attributeNameToPropertyName("attr-string1")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-string1")))->getValue(), QByteArray("\"name\""));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-string2")), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-string2")))->getName(), QByteArray(MStyleSheetAttribute::attributeNameToPropertyName("attr-string2")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-string2")))->getValue(), QByteArray("\"this is a string with constant \"name\"\" 10 1.0"));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-bool")), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-bool")))->getName(), QByteArray(MStyleSheetAttribute::attributeNameToPropertyName("attr-bool")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-bool")))->getValue(), QByteArray("true"));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos1")), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos1")))->getName(), QByteArray(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos1")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos1")))->getValue(), QByteArray("10px 15px"));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos2")), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos2")))->getName(), QByteArray(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos2")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-pos2")))->getValue(), QByteArray("17px 11px"));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-font1")), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-font1")))->getName(), QByteArray(MStyleSheetAttribute::attributeNameToPropertyName("attr-font1")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-font1")))->getValue(), QByteArray("sans 12px"));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-font2")), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-font2")))->getName(), QByteArray(MStyleSheetAttribute::attributeNameToPropertyName("attr-font2")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-font2")))->getValue(), QByteArray("arial 10px"));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-color")), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-color")))->getName(), QByteArray(MStyleSheetAttribute::attributeNameToPropertyName("attr-color")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-color")))->getValue(), QByteArray("#0abba0"));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-invalid")), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-invalid")))->getName(), QByteArray(MStyleSheetAttribute::attributeNameToPropertyName("attr-invalid")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-invalid")))->getValue(), QByteArray(""));
    QVERIFY(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-void")), NULL));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-void")))->getName(), QByteArray(MStyleSheetAttribute::attributeNameToPropertyName("attr-void")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-void")))->getValue(), QByteArray(""));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-logical-black")))->getName(), QByteArray(MStyleSheetAttribute::attributeNameToPropertyName("attr-logical-black")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-logical-black")))->getValue(), QByteArray("#000000"));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-logical-green")))->getName(), QByteArray(MStyleSheetAttribute::attributeNameToPropertyName("attr-logical-green")));
    QCOMPARE(info->selectors[0]->attributes()->value(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(MStyleSheetAttribute::attributeNameToPropertyName("attr-logical-green")))->getValue(), QByteArray("#00ff00"));
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

    QList<QSharedPointer<MStyleSheetParser::StylesheetFileInfo> >::iterator parserFi = parser.fileInfoList().begin();
    QList<QSharedPointer<MStyleSheetParser::StylesheetFileInfo> >::iterator binaryFi = binary.fileInfoList().begin();

    for (int i = 0; i < parser.fileInfoList().count(); i++) {
        // NOTE: (*parserFi)->constants.count() will differ from (*binaryFi)->constants.count()
        // as the binary files do not contain any constants

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
            QCOMPARE(selector0->parentObjectName(), selector1->parentObjectName());
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
                QCOMPARE(selector0Attributes.value()->getName(), selector1Attributes.value()->getName());
                QCOMPARE(selector0Attributes.value()->getValue(), selector1Attributes.value()->getValue());

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

void Ft_MStyleSheetParser::test_set_binary_file_directory()
{
    MStyleSheetParser parser;
    QString binaryPath = qApp->applicationDirPath();
    QString cssPath = qApp->applicationDirPath() + QDir::separator() + "ft_mstylesheetparser_test.css";

    parser.setBinaryFileGenerationEnabled(true);
    parser.setBinaryFileDirectory(binaryPath);
    parser.load(cssPath);

    QString binaryFilename = cssPath;
    binaryFilename.replace(QLatin1Char('_'), "__");
    binaryFilename.replace(QLatin1Char('/'), "_.");

    /* Check that binary file was created in the right dir */
    QVERIFY(QFile::exists(binaryPath + QDir::separator() + binaryFilename));

    /* remove the created file */
    QFile::remove(binaryPath + QDir::separator() + binaryFilename);
}
QTEST_APPLESS_MAIN(Ft_MStyleSheetParser)
