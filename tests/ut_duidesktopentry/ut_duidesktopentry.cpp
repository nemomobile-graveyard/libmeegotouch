/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#include <QObject>
#include <QGraphicsSceneMouseEvent>

#include <DuiLocale>
#include <duidesktopentry.h>
#include "duidesktopentry_p.h"
#include "ut_duidesktopentry.h"
#include "duilocale_stub.h"

QString gTranslationCatalog = "";

// qtTrId() stub
QString qtTrId(const char *id, int n)
{
    Q_UNUSED(n);
    if (gTranslationCatalog.isEmpty()) {
        return id;
    } else {
        if (gDefaultDuiLocaleStub.stubLastCallTo("installTrCatalog").parameter<const QString>(0) == gTranslationCatalog) {
            return QString("translated_foobar");
        } else {
            return "";
        }
    }
}

// A string pair type. Used with key/value pairs in this test
typedef QPair<QString, QString> Value;

/*
 * A simple container for Values
 */
class ValueList : public QList<Value>
{
public:
    /*
     * Tests if there is a Value with the specified key.
     */
    bool contains(const QString &key) const {
        for (const_iterator it = constBegin(); it != constEnd(); it++) {
            if (it->first == key) {
                return true;
            }
        }

        return false;
    }

    /*
     * Appends a value to the container
     */
    ValueList &operator<<(const Value &value) {
        append(value);
        return *this;
    }
};

typedef QPair<QString, ValueList> ValueGroup;
typedef QList<ValueGroup> ValueGroupList;

// These key/value pairs are added to the desktop entry by default
// under the main (i.e. Desktop Entry) group.
const static ValueList defaultValues = (ValueList() <<
                                        Value("Type", "Application") <<
                                        Value("GenericName", "") <<
                                        Value("Exec", "Executable") <<
                                        Value("Name", "Unlocalized name"));

// DuiDesktopEntry mock to be used as DuiDesktopEntry
class MockDuiDesktopEntry: public DuiDesktopEntry
{
public:
    MockDuiDesktopEntry(const ValueList &additionalValues = ValueList(),
                        const ValueGroupList &additionalGroups = ValueGroupList(),
                        bool includeDefaults = true) :
        DuiDesktopEntry("fakefile.desktop") {
        QByteArray byteArray;

        if (includeDefaults || !additionalValues.isEmpty()) {
            byteArray = "[Desktop Entry]\n";
        }

        if (includeDefaults) {
            // Put the default values to the desktop entry
            foreach(const Value & value, defaultValues) {
                if (!additionalValues.contains(value.first)) {  // If a key is overwritten, don't add the default value
                    byteArray.append(value.first + "=" + value.second + "\n");
                }
            }
        }

        // Add user supplied additional values to the desktop entry
        foreach(const Value & value, additionalValues) {
            byteArray.append(value.first + "=" + value.second + "\n");
        }

        // Add user supplied additional groups and values to the desktop entry
        foreach(const ValueGroup & group, additionalGroups) {
            byteArray.append("[" + group.first + "]\n");
            foreach(const Value & value, group.second) {
                byteArray.append(value.first + "=" + value.second + "\n");
            }
        }

        // Parse the desktop entry
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::ReadOnly);
        d_ptr->readDesktopFile(buffer, d_ptr->desktopEntriesMap);

        // FIXME modify the production code so that this part isn't needed in the unit test
        QString TranslationCatalogKey = "Desktop Entry/X-DUI-translation-catalog";
        if (contains(TranslationCatalogKey)) {
            DuiLocale locale;
            // Load the catalog from disk if it's not yet loaded
            locale.installTrCatalog(value(TranslationCatalogKey));
        }
    }
};

// QFile stubs used by DuiDesktopEntry
bool QFile::exists() const
{
    return false;
}

void Ut_DuiDesktopEntry::initTestCase()
{
    m_subject = NULL;
}

void Ut_DuiDesktopEntry::init()
{
    gDefaultDuiLocaleStub.stubReset();
    gDefaultDuiLocaleStub.stubSetReturnValue("language", QString("fi"));
    gDefaultDuiLocaleStub.stubSetReturnValue("country", QString("FI"));
}

void Ut_DuiDesktopEntry::cleanup()
{
    delete m_subject;
    m_subject = 0;

    gTranslationCatalog.clear();
}

void Ut_DuiDesktopEntry::testFileNameMethod()
{
    m_subject = new DuiDesktopEntry("foo.desktop");
    QCOMPARE(m_subject->fileName(), QString("foo.desktop"));
}

void Ut_DuiDesktopEntry::testMissingType()
{
    m_subject = new MockDuiDesktopEntry(ValueList() <<
                                        Value("GenericName", "") <<
                                        Value("Exec", "Executable") <<
                                        Value("Name", "Unlocalized name"),
                                        ValueGroupList(), false);
    QCOMPARE(m_subject->isValid(), false);
}

void Ut_DuiDesktopEntry::testMissingName()
{
    m_subject = new MockDuiDesktopEntry(ValueList() <<
                                        Value("Type", "Type") <<
                                        Value("Exec", "Executable"),
                                        ValueGroupList(), false);
    QCOMPARE(m_subject->isValid(), false);
}

void Ut_DuiDesktopEntry::testIsHidden()
{
    m_subject = new MockDuiDesktopEntry();
    QCOMPARE(m_subject->hidden(), false);
    delete m_subject;

    m_subject = new MockDuiDesktopEntry(ValueList() << Value("Hidden", "true"));
    QCOMPARE(m_subject->hidden(), true);
}

void Ut_DuiDesktopEntry::testMissingExec()
{
    m_subject = new MockDuiDesktopEntry(ValueList() <<
                                        Value("Type", "Application") <<
                                        Value("GenericName", "") <<
                                        Value("Name", "Unlocalized name"),
                                        ValueGroupList(), false);
    QCOMPARE(m_subject->isValid(), false);
}

void Ut_DuiDesktopEntry::testApplication()
{
    m_subject = new MockDuiDesktopEntry();
    QCOMPARE(m_subject->isValid(), true);
}

void Ut_DuiDesktopEntry::testLink()
{
    m_subject = new MockDuiDesktopEntry(ValueList() << Value("Type", "Link") << Value("URL", "http://www.foobar.org"));
    QCOMPARE(m_subject->isValid(), true);
    QCOMPARE(m_subject->type(), QString("Link"));
    delete m_subject;

    // Type Link requires an URL key
    m_subject = new MockDuiDesktopEntry(ValueList() << Value("Type", "Link"));
    QCOMPARE(m_subject->isValid(), false);
}

void Ut_DuiDesktopEntry::testVersion()
{
    m_subject = new MockDuiDesktopEntry(ValueList() << Value("Version", ""));
    QCOMPARE(m_subject->version(), QString(""));
    delete m_subject;

    m_subject = new MockDuiDesktopEntry(ValueList() << Value("Version", "1.0"));
    QCOMPARE(m_subject->version(), QString("1.0"));
}

void Ut_DuiDesktopEntry::testGenericName()
{
    m_subject = new MockDuiDesktopEntry();
    QCOMPARE(m_subject->genericName(), QString(""));
    delete m_subject;

    m_subject = new MockDuiDesktopEntry(ValueList() << Value("GenericName", "foo"));
    QCOMPARE(m_subject->genericName(), QString("foo"));
}

void Ut_DuiDesktopEntry::testMisc()
{
    m_subject = new MockDuiDesktopEntry(ValueList() <<
                                        Value("Comment", "foo comment") <<
                                        Value("Icon", "foo icon") <<
                                        Value("TryExec", "foo tryexec") <<
                                        Value("Path", "foo path"));
    QCOMPARE(m_subject->comment(), QString("foo comment"));
    QCOMPARE(m_subject->icon(), QString("foo icon"));
    QCOMPARE(m_subject->tryExec(), QString("foo tryexec"));
    QCOMPARE(m_subject->path(), QString("foo path"));
}

void Ut_DuiDesktopEntry::testNoDisplay()
{
    m_subject = new MockDuiDesktopEntry();
    QCOMPARE(m_subject->noDisplay(), false);
    delete m_subject;

    m_subject = new MockDuiDesktopEntry(ValueList() << Value("NoDisplay", "true"));
    QCOMPARE(m_subject->noDisplay(), true);
}

void Ut_DuiDesktopEntry::testTerminal()
{
    m_subject = new MockDuiDesktopEntry();
    QCOMPARE(m_subject->terminal(), false);
    delete m_subject;

    m_subject = new MockDuiDesktopEntry(ValueList() << Value("Terminal", "true"));
    QCOMPARE(m_subject->terminal(), true);
}

void Ut_DuiDesktopEntry::testStartupNotify()
{
    m_subject = new MockDuiDesktopEntry();
    QCOMPARE(m_subject->startupNotify(), false);
    delete m_subject;

    m_subject = new MockDuiDesktopEntry(ValueList() << Value("StartupNotify", "true"));
    QCOMPARE(m_subject->startupNotify(), true);
}

void Ut_DuiDesktopEntry::testLocalizedNameLogicalId()
{
    ValueList additional;
    additional << Value("X-DUI-logical-id", "foobar");
    m_subject = new MockDuiDesktopEntry(additional);
    QCOMPARE(m_subject->name(), QString("!! Unlocalized name"));
    delete m_subject;

    gTranslationCatalog = "test";
    additional << Value("X-DUI-translation-catalog", "test");
    m_subject = new MockDuiDesktopEntry(additional);
    QCOMPARE(m_subject->name(), QString("translated_foobar"));
}

void Ut_DuiDesktopEntry::testLocalizedNamePostfix()
{
    m_subject = new MockDuiDesktopEntry(ValueList() << Value("Name[fi_FI]", "testtext"));
    QCOMPARE(m_subject->name(), QString("testtext"));
    delete m_subject;

    gDefaultDuiLocaleStub.stubSetReturnValue("language", QString("en"));
    gDefaultDuiLocaleStub.stubSetReturnValue("country", QString("GB"));
    gDefaultDuiLocaleStub.stubSetReturnValue("variant", QString(""));

    m_subject = new MockDuiDesktopEntry(ValueList() << Value("Name[en_GB@unknown]", "foo"));
    QCOMPARE(m_subject->name(), QString("Unlocalized name"));
    delete m_subject;

    m_subject = new MockDuiDesktopEntry(ValueList() << Value("Name[en_GB]", "barfoo"));
    QCOMPARE(m_subject->name(), QString("barfoo"));
    delete m_subject;

    m_subject = new MockDuiDesktopEntry(ValueList() << Value("Name[en]", "barfoo"));
    QCOMPARE(m_subject->name(), QString("barfoo"));
    delete m_subject;

    gDefaultDuiLocaleStub.stubSetReturnValue("language", QString("en"));
    gDefaultDuiLocaleStub.stubSetReturnValue("country", QString("GB"));
    gDefaultDuiLocaleStub.stubSetReturnValue("variant", QString("piglatin"));

    m_subject = new MockDuiDesktopEntry(ValueList() << Value("Name[en_GB@piglatin]", "bar"));
    QCOMPARE(m_subject->name(), QString("bar"));
    delete m_subject;

    m_subject = new MockDuiDesktopEntry(ValueList() << Value("Name[en_GB]", "bar"));
    QCOMPARE(m_subject->name(), QString("bar"));
    delete m_subject;

    m_subject = new MockDuiDesktopEntry(ValueList() << Value("Name[en]", "bar"));
    QCOMPARE(m_subject->name(), QString("bar"));
    delete m_subject;

    m_subject = new MockDuiDesktopEntry(ValueList() << Value("Name[en@piglatin]", "bar"));
    QCOMPARE(m_subject->name(), QString("bar"));
    delete m_subject;

    m_subject = new MockDuiDesktopEntry(ValueList() << Value("Name[unknown]", "foobar"));
    QCOMPARE(m_subject->name(), QString("Unlocalized name"));
}

void Ut_DuiDesktopEntry::testMultipleValues()
{
    m_subject = new MockDuiDesktopEntry(ValueList() << Value("Categories", "cat1;;cat2\\;;ca\\;t3;"));
    QStringList categories = m_subject->categories();
    QCOMPARE(categories.count(), 4);
    QCOMPARE(categories.at(0), QString("cat1"));
    QCOMPARE(categories.at(1), QString(""));
    QCOMPARE(categories.at(2), QString("cat2;"));
    QCOMPARE(categories.at(3), QString("ca;t3"));
}

void Ut_DuiDesktopEntry::testMultipleValuesWithoutTrailingSemicolon()
{
    m_subject = new MockDuiDesktopEntry(ValueList() << Value("Categories", "category"));
    QCOMPARE(m_subject->isValid(), false);
}

void Ut_DuiDesktopEntry::testMultipleValuesWithEscapedTrailingSemicolon()
{
    m_subject = new MockDuiDesktopEntry(ValueList() << Value("Categories", "category\\;"));
    QCOMPARE(m_subject->isValid(), false);
}

void Ut_DuiDesktopEntry::testCommentedLine()
{
    m_subject = new MockDuiDesktopEntry(ValueList() << Value("#TestCommentedLine", "comment"));
    QCOMPARE(m_subject->contains("Desktop Entry/#TestCommentedLine"), false);
    QCOMPARE(m_subject->contains("Desktop Entry/TestCommentedLine"), false);
}

void Ut_DuiDesktopEntry::testMultipleKeys()
{
    // Note the space after the key name - that is the important part here
    m_subject = new MockDuiDesktopEntry(ValueList() << Value("Name ", "foo"));
    // Multiple similarly named keys doesn't render the entry invalid
    QCOMPARE(m_subject->isValid(), true);
    // The first key is used, the second is ignored
    QCOMPARE(m_subject->name(), QString("Unlocalized name"));
}

void Ut_DuiDesktopEntry::testEqualSignValues()
{
    m_subject = new MockDuiDesktopEntry(ValueList() << Value("ExampleKey", "Type1=Type2"));
    QCOMPARE(m_subject->value("Desktop Entry/ExampleKey"), QString("Type1=Type2"));
}

void Ut_DuiDesktopEntry::testKeyStartingWithWhitespaces()
{
    m_subject = new MockDuiDesktopEntry(ValueList() << Value("                 LineStartingWithSpace", "line with space"));
    QCOMPARE(m_subject->contains("Desktop Entry/LineStartingWithSpace"), true);
    QCOMPARE(m_subject->value("Desktop Entry/LineStartingWithSpace"), QString("line with space"));
}

void Ut_DuiDesktopEntry::testInvalidKeys()
{
    m_subject = new MockDuiDesktopEntry(ValueList() << Value(";", ";=") << Value("?", "?") << Value("ad!", "123"));
    // Invalid key names are just bypassed. Doesn't render the entry invalid
    QCOMPARE(m_subject->isValid(), true);
    QCOMPARE(m_subject->contains("Desktop Entry/;"), false);
    QCOMPARE(m_subject->contains("Desktop Entry/?"), false);
    QCOMPARE(m_subject->contains("Desktop Entry/ad!"), false);
}

void Ut_DuiDesktopEntry::testMultipleGroupNames()
{
    ValueGroupList additionalGroups;
    additionalGroups.append(ValueGroup("New Group", ValueList() << Value("StartupNotify", "true")));
    additionalGroups.append(ValueGroup("Desktop Entry", ValueList() << Value("StartupNotify", "true")));
    m_subject = new MockDuiDesktopEntry(ValueList(), additionalGroups);
    // Multiple similarly named groups renders the entry invalid
    QCOMPARE(m_subject->isValid(), false);
    QCOMPARE(m_subject->contains("Desktop Entry/StartupNotify"), true);
    QCOMPARE(m_subject->contains("New Group/StartupNotify"), true);
}

void Ut_DuiDesktopEntry::testGroupNameLocation()
{
    ValueGroupList additionalGroups;
    additionalGroups.append(ValueGroup("Test Group", ValueList() << Value("foo", "foo")));
    additionalGroups.append(ValueGroup("Desktop Entry", ValueList() << Value("bar", "bar")));
    m_subject = new MockDuiDesktopEntry(ValueList(), additionalGroups, false);
    QCOMPARE(m_subject->isValid(), false);
}

void Ut_DuiDesktopEntry::testInvalidGroupNames()
{
    {
        ValueGroupList additionalGroups;
        additionalGroups.append(ValueGroup("Desktop\n Entry", ValueList() << Value("bar", "bar")));
        m_subject = new MockDuiDesktopEntry(ValueList(), additionalGroups, false);
        QCOMPARE(m_subject->isValid(), false);
    }
    delete m_subject;

    {
        ValueGroupList additionalGroups;
        additionalGroups.append(ValueGroup("\tDesktop Entry", ValueList() << Value("foo", "foo")));
        m_subject = new MockDuiDesktopEntry(ValueList(), additionalGroups, false);
        QCOMPARE(m_subject->isValid(), false);
    }
}

QTEST_MAIN(Ut_DuiDesktopEntry)
