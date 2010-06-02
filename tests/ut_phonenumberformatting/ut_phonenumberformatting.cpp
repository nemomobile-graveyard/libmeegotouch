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
#include "ut_phonenumberformatting.h"

#include "mlocale.h"

void Ut_PhoneNumberFormatting::testFormatting_data()
{
    QTest::addColumn<QString>("rawPhoneNumber");
    QTest::addColumn<QString>("formattedPhoneNumber");
    QTest::addColumn<bool>("doGrouping");

    // test empty string
    QTest::newRow( "" ) << QString()
			<< QString()
			<< false;

    // test splitting of country code without grouping
    QTest::newRow( "" ) << QString( "+3581234567" )
			<< QString( "+358 1234567" )
			<< false;


    QTest::newRow( "" ) << QString( "+123456789" )
			<< QString( "+1 23456789" )
			<< false;

    QTest::newRow( "" ) << QString( "+9876543210" )
			<< QString( "+98 76543210" )
			<< false;

    QTest::newRow( "" ) << QString( "+3581234567" )
			<< QString( "+358 1234567" )
			<< false;

    QTest::newRow( "" ) << QString( "00123456789" )
			<< QString( "00 1 23456789" )
			<< false;

    QTest::newRow( "" ) << QString( "009876543210" )
			<< QString( "00 98 76543210" )
			<< false;

    QTest::newRow( "" ) << QString( "003581234567" )
			<< QString( "00 358 1234567" )
			<< false;

    QTest::newRow( "" ) << QString( "011123456789" )
			<< QString( "011 1 23456789" )
			<< false;

    QTest::newRow( "" ) << QString( "0119876543210" )
			<< QString( "011 98 76543210" )
			<< false;

    QTest::newRow( "" ) << QString( "0113581234567" )
			<< QString( "011 358 1234567" )
			<< false;


    // test american number splitting without country code
    QTest::newRow( "" ) << QString( "9" )
			<< QString( "9" )
			<< true;

    QTest::newRow( "" ) << QString( "98" )
			<< QString( "98" )
			<< true;

    QTest::newRow( "" ) << QString( "987" )
			<< QString( "987" )
			<< true;

    QTest::newRow( "" ) << QString( "9876" )
			<< QString( "987-6" )
			<< true;

    QTest::newRow( "" ) << QString( "98765" )
			<< QString( "987-65" )
			<< true;

    QTest::newRow( "" ) << QString( "987654" )
			<< QString( "987-654" )
			<< true;

    QTest::newRow( "" ) << QString( "9876543" )
			<< QString( "987-6543" )
			<< true;

    QTest::newRow( "" ) << QString( "98765432" )
			<< QString( "(987) 654-32" )
			<< true;

    QTest::newRow( "" ) << QString( "987654321" )
                        //<< QString( "(987) 654- 321" ) typo ? check...
			<< QString( "(987) 654-321" )
			<< true;

    QTest::newRow( "" ) << QString( "9876543210" )
			<< QString( "(987) 654-3210" )
			<< true;

    QTest::newRow( "" ) << QString( "98765432109" )
			<< QString( "98765432109" )
			<< true;

    // grouping on, numbers starting with 1
    QTest::newRow( "" ) << QString( "1" )
			<< QString( "1" )
			<< true;

    QTest::newRow( "" ) << QString( "12" )
			<< QString( "1 (2  )" )
			<< true;

    QTest::newRow( "" ) << QString( "123" )
			<< QString( "1 (23 )" )
			<< true;

    QTest::newRow( "" ) << QString( "1234" )
			<< QString( "1 (234)" )
			<< true;

    QTest::newRow( "" ) << QString( "12345" )
			<< QString( "1 (234) 5" )
			<< true;

    QTest::newRow( "" ) << QString( "123456" )
			<< QString( "1 (234) 56" )
			<< true;

    QTest::newRow( "" ) << QString( "1234567" )
			<< QString( "1 (234) 567" )
			<< true;

    QTest::newRow( "" ) << QString( "12345678" )
			<< QString( "1 (234) 567-8" )
			<< true;

    QTest::newRow( "" ) << QString( "123456789" )
			<< QString( "1 (234) 567-89" )
			<< true;

    QTest::newRow( "" ) << QString( "1234567890" )
			<< QString( "1 (234) 567-890" )
			<< true;

    QTest::newRow( "" ) << QString( "12345678901" )
			<< QString( "1 (234) 567-8901" )
			<< true;

    QTest::newRow( "" ) << QString( "123456789012" )
			<< QString( "123456789012" )
			<< true;

    // check that no formatting is done for numbers starting with 11
    QTest::newRow( "" ) << QString( "11" )
			<< QString( "11" )
			<< true;

    QTest::newRow( "" ) << QString( "113" )
			<< QString( "113" )
			<< true;

    QTest::newRow( "" ) << QString( "1134" )
			<< QString( "1134" )
			<< true;

    QTest::newRow( "" ) << QString( "11345" )
			<< QString( "11345" )
			<< true;

    QTest::newRow( "" ) << QString( "113456" )
			<< QString( "113456" )
			<< true;

    QTest::newRow( "" ) << QString( "1134567" )
			<< QString( "1134567" )
			<< true;

    QTest::newRow( "" ) << QString( "11345678" )
			<< QString( "11345678" )
			<< true;

    QTest::newRow( "" ) << QString( "113456789" )
			<< QString( "113456789" )
			<< true;

    QTest::newRow( "" ) << QString( "1134567890" )
			<< QString( "1134567890" )
			<< true;

    QTest::newRow( "" ) << QString( "11345678901" )
			<< QString( "11345678901" )
			<< true;

    QTest::newRow( "" ) << QString( "113456789012" )
			<< QString( "113456789012" )
			<< true;

    // test that no formatting is done for numbers starting with
    // *, #, p or w
    QTest::newRow( "" ) << QString( "*987654" )
			<< QString( "*987654" )
			<< true;

    QTest::newRow( "" ) << QString( "#987654" )
			<< QString( "#987654" )
			<< true;

    QTest::newRow( "" ) << QString( "p987654" )
			<< QString( "p987654" )
			<< true;

    QTest::newRow( "" ) << QString( "w987654" )
			<< QString( "w987654" )
			<< true;

    // test combined effect of grouping of country code
    // and north american grouping
    QTest::newRow( "" ) << QString( "+123456789" )
			<< QString( "+1 (234) 567-89" )
			<< true;

    QTest::newRow( "" ) << QString( "+9876543210" )
			<< QString( "+98 (765) 432-10" )
			<< true;

    QTest::newRow( "" ) << QString( "+3581234567" )
			<< QString( "+358 (123) 456-7" )
			<< true;

    QTest::newRow( "" ) << QString( "00123456789" )
			<< QString( "00123456789" )
			<< true;

    QTest::newRow( "" ) << QString( "009876543210" )
			<< QString( "009876543210" )
			<< true;

    QTest::newRow( "" ) << QString( "003581234567" )
			<< QString( "003581234567" )
			<< true;

    QTest::newRow( "" ) << QString( "011123456789" )
			<< QString( "011 1 (234) 567-89" )
			<< true;

    QTest::newRow( "" ) << QString( "0119876543210" )
			<< QString( "011 98 (765) 432-10" )
			<< true;

    QTest::newRow( "" ) << QString( "0113581234567" )
			<< QString( "011 358 (123) 456-7" )
			<< true;
}

void Ut_PhoneNumberFormatting::testFormatting()
{
    QTest::addColumn<QString>("rawPhoneNumber");
    QTest::addColumn<QString>("formattedPhoneNumber");
    QTest::addColumn<bool>("doGrouping");

    QFETCH( QString, rawPhoneNumber );
    QFETCH( QString, formattedPhoneNumber );
    QFETCH( bool, doGrouping );

    Q_UNUSED(doGrouping);

    MLocale locale;

    QString myGroupedPhoneNumber = locale.formatPhoneNumber(
        rawPhoneNumber, doGrouping ?
	    MLocale::NorthAmericanGrouping :
	    MLocale::NoGrouping );

    QCOMPARE( myGroupedPhoneNumber, formattedPhoneNumber );
}


QTEST_APPLESS_MAIN(Ut_PhoneNumberFormatting);
