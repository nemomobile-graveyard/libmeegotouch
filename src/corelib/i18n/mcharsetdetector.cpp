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

#include "mcharsetdetector.h"
#include "mcharsetdetector_p.h"

#include "mcharsetmatch.h"

#include <unicode/utypes.h>
#include <unicode/uversion.h>
#if (U_ICU_VERSION_MAJOR_NUM > 4) || (U_ICU_VERSION_MAJOR_NUM == 4 && U_ICU_VERSION_MINOR_NUM >=4)
#include <unicode/localpointer.h>
#endif
#include <unicode/uenum.h>
#include <unicode/ucsdet.h>

#include <QString>
#include <QStringList>
#include <QTextCodec>
#include <QDebug>

MCharsetDetectorPrivate::MCharsetDetectorPrivate()
    : _status(U_ZERO_ERROR),
      _uCharsetDetector(0),
      q_ptr(0)
{
    _uCharsetDetector = ucsdet_open(&_status);
    if(hasError())
        qWarning() << __PRETTY_FUNCTION__ << errorString();
}

MCharsetDetectorPrivate::~MCharsetDetectorPrivate()
{
    ucsdet_close(_uCharsetDetector);
}

bool MCharsetDetectorPrivate::hasError() const
{
    if(U_SUCCESS(_status))
        return false;
    else
        return true;
}

void MCharsetDetectorPrivate::clearError()
{
    _status = U_ZERO_ERROR;
}

QString MCharsetDetectorPrivate::errorString() const
{
    return QString(u_errorName(_status));
}

MCharsetDetector::MCharsetDetector()
    : d_ptr(new MCharsetDetectorPrivate)
{
    Q_D(MCharsetDetector);
    d->q_ptr = this;
}

MCharsetDetector::MCharsetDetector(const QByteArray &ba)
    : d_ptr (new MCharsetDetectorPrivate)
{
    Q_D(MCharsetDetector);
    d->q_ptr = this;
    setText(ba);
}

MCharsetDetector::MCharsetDetector(const char *str)
    : d_ptr(new MCharsetDetectorPrivate)
{
    Q_D(MCharsetDetector);
    d->q_ptr = this;
    setText(QByteArray(str));
}

MCharsetDetector::MCharsetDetector(const char *data, int size)
    : d_ptr(new MCharsetDetectorPrivate)
{
    Q_D(MCharsetDetector);
    d->q_ptr = this;
    setText(QByteArray(data, size));
}

MCharsetDetector::~MCharsetDetector()
{
    delete d_ptr;
}

bool MCharsetDetector::hasError() const
{
    Q_D(const MCharsetDetector);
    return d->hasError();
}

void MCharsetDetector::clearError()
{
    Q_D(MCharsetDetector);
    d->clearError();
}

QString MCharsetDetector::errorString() const
{
    Q_D(const MCharsetDetector);
    return d->errorString();
}

void MCharsetDetector::setText(const QByteArray &ba)
{
    Q_D(MCharsetDetector);
    clearError();
    d->_ba = ba;
    d->_baExtended = ba;
    while (d->_baExtended.size() < 50)
        d->_baExtended += d->_ba;
    ucsdet_setText(d->_uCharsetDetector, d->_baExtended.constData(), int32_t(-1), &(d->_status));
    if(hasError())
        qWarning() << __PRETTY_FUNCTION__ << errorString();
}

MCharsetMatch MCharsetDetector::detect()
{
    // Just call MCharsetDetector::detectAll() and take the first
    // match here instead of using ucsdet_detect() to get only a
    // single match. The list returned by ucsdet_detectAll() maybe
    // tweaked a bit in MCharsetDetector::detectAll() to improve
    // the quality of the detection. Therefore, the first element
    // of the list returned by MCharsetDetector::detectAll() may
    // differ from the single match returned by ucsdet_detect().
    Q_D(MCharsetDetector);
    QList<MCharsetMatch> mCharsetMatchList = detectAll();
    if(hasError()) {
        qWarning() << __PRETTY_FUNCTION__ << errorString();
        return MCharsetMatch();
    }
    if (mCharsetMatchList.isEmpty()) {
        // should never happen, because detectAll() already sets an
        // error if no matches are found which the previous
        // if(hasError()) should detect.
        d->_status = U_CE_NOT_FOUND_ERROR;
        qWarning() << __PRETTY_FUNCTION__
                   << "no matches found at all" << errorString();
        return MCharsetMatch();
    }
    return mCharsetMatchList.first();
}

QList<MCharsetMatch> MCharsetDetector::detectAll()
{
    Q_D(MCharsetDetector);
    clearError();
    // get list of matches from ICU:
    qint32 matchesFound;
    const UCharsetMatch **uCharsetMatch
        = ucsdet_detectAll(d->_uCharsetDetector, &matchesFound, &(d->_status));
    if(hasError()) {
        qWarning() << __PRETTY_FUNCTION__ << errorString();
        return QList<MCharsetMatch>();
    }
    // sometimes the number of matches found by ucsdet_detectAll()
    // maybe 0 (matchesFound == 0) but d->_status has no error. Do not
    // return here with an error if this happens because the fine
    // tuning below may add more matches.  Better check whether no
    // matches were found at all *after* the fine tuning.

    // fill list of matches into a QList<MCharsetMatch>:
    QList<MCharsetMatch> mCharsetMatchList;
    for (qint32 i = 0; i < matchesFound; ++i) {
        MCharsetMatch mCharsetMatch;
        mCharsetMatch.setName(
            QString::fromAscii(ucsdet_getName(uCharsetMatch[i], &(d->_status))));
        if(hasError()) {
            qWarning() << __PRETTY_FUNCTION__ << errorString();
            return QList<MCharsetMatch>();
        }
        mCharsetMatch.setConfidence(
            static_cast<qint32>(ucsdet_getConfidence (uCharsetMatch[i], &(d->_status))));
        if(hasError()) {
            qWarning() << __PRETTY_FUNCTION__ << errorString();
            return QList<MCharsetMatch>();
        }
        mCharsetMatch.setLanguage(
            QString::fromAscii(ucsdet_getLanguage(uCharsetMatch[i], &(d->_status))));
        if(hasError()) {
            qWarning() << __PRETTY_FUNCTION__ << errorString();
            return QList<MCharsetMatch>();
        }
        mCharsetMatchList << mCharsetMatch;
    }
    if(d->_allDetectableCharsets.isEmpty())
        getAllDetectableCharsets();
    // libicu sometimes does not detect single byte encodings at all
    // even if they can encode the input without error. This seems to
    // contradict the documentation on
    // http://icu-project.org/apiref/icu4c/ucsdet_8h.html which says:
    //
    //     A confidence value of ten does have a general meaning - it is
    //     used for charsets that can represent the input data, but for
    //     which there is no other indication that suggests that the
    //     charset is the correct one. Pure 7 bit ASCII data, for example,
    //     is compatible with a great many charsets, most of which will
    //     appear as possible matches with a confidence of 10.
    //
    // But if such a single byte encoding has been set as the declared
    // encoding, it should at least be tried, therefore add it here to
    // the list of matches with the confidence value of 10. If it
    // cannot encode the complete input, the iteration over the list
    // of matches will detect that and remove it again.
    if(!d->_declaredEncoding.isEmpty()
        && (d->_declaredEncoding.startsWith(QLatin1String("ISO-8859-"))
            || d->_declaredEncoding.startsWith(QLatin1String("windows-12"))
            || d->_declaredEncoding.startsWith(QLatin1String("KOI8"))))
            mCharsetMatchList << MCharsetMatch(d->_declaredEncoding, "", 10);
    // Similar as for declaredEncoding, when declaredLocale is used
    // and it is a locale where the legacy encoding is a single byte
    // encoding, it should at least be tried, therefore add the legacy
    // single byte encoding for the declared locale here.  If it
    // cannot encode the complete input, it will be removed again
    // later.  Multibyte encodings like Shift_JIS, EUC-JP, Big5,
    // etc. ...  do not need to be added, contrary to the single byte
    // encodings I could find no case where the matches returned by
    // libicu did omit a multibyte encoding when it should have been
    // included.
    if(!d->_declaredLocale.isEmpty()) {
        QString language = d->_declaredLocale.left(2);
        if(language ==  QLatin1String("ru")) {
            mCharsetMatchList << MCharsetMatch("KOI8-R", language, 10);
            mCharsetMatchList << MCharsetMatch("windows-1251", language, 10);
            mCharsetMatchList << MCharsetMatch("ISO-8859-5", language, 10);
        }
        else if(language == QLatin1String("tr"))
            mCharsetMatchList << MCharsetMatch("ISO-8859-9", language, 10);
        else if(language == QLatin1String("el"))
            mCharsetMatchList << MCharsetMatch("ISO-8859-7", language, 10);
        else if(language == QLatin1String("en")
                || language == QLatin1String("da")
                || language == QLatin1String("de")
                || language == QLatin1String("es")
                || language == QLatin1String("fr")
                || language == QLatin1String("it")
                || language == QLatin1String("nl")
                || language == QLatin1String("no")
                || language == QLatin1String("nn")
                || language == QLatin1String("nb")
                || language == QLatin1String("pt")
                || language == QLatin1String("sv"))
            mCharsetMatchList << MCharsetMatch("ISO-8859-1", language, 10);
        else if(language == QLatin1String("cs")
                || language == QLatin1String("hu")
                || language == QLatin1String("pl")
                || language == QLatin1String("ro"))
            mCharsetMatchList << MCharsetMatch("ISO-8859-1", language, 10);
        else if(language == QLatin1String("ar")
                || language == QLatin1String("fa")
                || language == QLatin1String("ur"))
            mCharsetMatchList << MCharsetMatch("ISO-8859-6", language, 10);
        else if(language == QLatin1String("he"))
            mCharsetMatchList << MCharsetMatch("ISO-8859-8", language, 10);
    }
    // iterate over the detected matches and do some fine tuning:
    bool sortNeeded = false;
    QList<MCharsetMatch>::iterator it = mCharsetMatchList.begin();
    while(it != mCharsetMatchList.end()) {
        if((*it).name() == QLatin1String("ISO-2022-JP")) {
            // non-Japanese text in ISO-2022-JP encoding is possible
            // but very unlikely:
            (*it).setLanguage("ja");
        }
        if((*it).name() == QLatin1String("UTF-8")
           && (*it).confidence() >= 80 && (*it).confidence() < 99) {
            // Actually libicu currently only returns confidence
            // values of 100, 80, 25, and 10 for UTF-8.  A value of 80
            // can mean two things:
            //
            // 1)  (hasBOM && numValid > numInvalid*10)
            // 2)  (numValid > 0 && numInvalid == 0)
            //
            // If it is case 1), the match will be removed anyway by
            // the check below which tests whether the complete input
            // can be encoded. I.e. we don’t need to care about this.
            //
            // If it is case 2) *and* the check below whether the
            // complete input can be encoded does not remove it, we
            // have valid UTF-8 and it is very unlikely that it is
            // anything else, therefore I think the confidence of 80
            // is too low and should be increased.
            // With a confidence of only 80, a longer ASCII text with
            // less than 4 UTF-8 characters will detect as ISO-8859-1
            // which is most certainly wrong.
            (*it).setConfidence(99);
            sortNeeded = true;
        }
        if(!d->_declaredEncoding.isEmpty()
           && (*it).name() == d->_declaredEncoding
           && (*it).confidence() == 10) {
            // A confidence value of 10 means the charset can
            // represent the input data, but there is no other
            // indication that suggests that the charset is the
            // correct one. But if the user has set this to be the
            // declared encoding, it should be preferred over the
            // other encodings which also got confidence 10 (there are
            // often many with confidence 10). Do not increase the
            // confidence too much though in order not to override
            // real evidence that the input does really use something
            // different than the declared encoding.
            (*it).setConfidence(40);
            sortNeeded = true;
        }
        if(!d->_declaredLocale.isEmpty()
           && d->_declaredLocale.startsWith((*it).language())
           && (*it).confidence() == 10) {
            // A confidence value of 10 means the charset can
            // represent the input data, but there is no other
            // indication that suggests that the charset is the
            // correct one. But if the detected language for this
            // charset matches the language declared by the user, this
            // charset should be preferred over the others which also
            // got confidence 10 (there are often many with confidence
            // 10). Do not increase the confidence too much though in
            // order not to override real evidence that the input does
            // really use something different than the declared
            // encoding.  Use a slightly lower value than for the
            // declared encoding. Setting the declared encoding
            // is more precise and should have somewhat higher priority
            if((d->_declaredLocale.contains("TW")
                || d->_declaredLocale.contains("HK")
                || d->_declaredLocale.contains("MO"))
               && (*it).name() == QLatin1String("Big5"))
                (*it).setConfidence(39); // trad. Chinese, Big5 more likely
            else if((d->_declaredLocale.contains("CN")
                     || d->_declaredLocale.contains("SG"))
                    && (*it).name() == QLatin1String("GB18030"))
                (*it).setConfidence(39); // simp. Chinese, GB18030/GB2312 more likely
            else {
                (*it).setConfidence(38);
            }
            sortNeeded = true;
        }
        if(!d->_allDetectableCharsets.contains((*it).name())) {
            // remove matches for charsets not supported by QTextCodec
            // then it is probably some weird charset we cannot use anyway
            it = mCharsetMatchList.erase(it);
        }
        else {
            // test whether the complete input text can be encoded
            // using this match, if not remove the match
            clearError();
            text(*it);
            if(hasError()) {
                // qDebug() << __PRETTY_FUNCTION__
                //          << "removing match" << (*it).name()
                //          << "because it cannot encode the complete input"
                //          << errorString();
                it = mCharsetMatchList.erase(it);
                clearError();
            }
            else
                ++it;
        }
    }
    // sort the list of matches again if confidences have been changed:
    if(sortNeeded)
        qSort(mCharsetMatchList.begin(), mCharsetMatchList.end(),
              qGreater<MCharsetMatch>());
    if(mCharsetMatchList.isEmpty()) {
        // is there any better status to describe this case?
        d->_status = U_CE_NOT_FOUND_ERROR;
        qWarning() << __PRETTY_FUNCTION__
                 << "number of matches found=0"
                 << errorString();
        return QList<MCharsetMatch>();
    }
    return mCharsetMatchList;
}

QString MCharsetDetector::text(const MCharsetMatch &charsetMatch)
{
    Q_D(MCharsetDetector);
    clearError();
    QTextCodec *codec
        = QTextCodec::codecForName(charsetMatch.name().toAscii());
    if (codec == NULL) { // there is no codec matching the name
        d->_status = U_ILLEGAL_ARGUMENT_ERROR;
        qWarning() << __PRETTY_FUNCTION__
                 << "no codec for the name" << charsetMatch.name()
                 << errorString();
        // return empty string to indicate that no conversion is possible:
        return QString();
    }
    else {
        QTextCodec::ConverterState state;
        QString text =
            codec->toUnicode(d->_ba.constData(), d->_ba.size(), &state);
        if (state.invalidChars > 0)
            d->_status = U_INVALID_CHAR_FOUND;
        return text;
    }
}

void MCharsetDetector::setDeclaredLocale(const QString &locale)
{
    Q_D(MCharsetDetector);
    clearError();
    d->_declaredLocale = locale;
}

void MCharsetDetector::setDeclaredEncoding(const QString &encoding)
{
    Q_D(MCharsetDetector);
    clearError();
    d->_declaredEncoding = encoding;
    if (d->_declaredEncoding == QLatin1String("GB2312"))
        d->_declaredEncoding = QLatin1String("GB18030");
    ucsdet_setDeclaredEncoding(d->_uCharsetDetector,
                               d->_declaredEncoding.toAscii().constData(),
                               int32_t(-1),
                               &(d->_status));
    if(hasError())
        qWarning() << __PRETTY_FUNCTION__ << errorString();
}

QStringList MCharsetDetector::getAllDetectableCharsets()
{
    Q_D(MCharsetDetector);

    if (!d->_allDetectableCharsets.isEmpty())
        return d->_allDetectableCharsets;

    // Codecs supported by QTextCodec (Qt 4.7):
    //
    // ISO-2022-JP JIS7 EUC-KR GB2312 Big5 Big5-ETen CP950 GB18030
    // EUC-JP Shift_JIS SJIS MS_Kanji System UTF-8 ISO-8859-1 latin1
    // CP819 IBM819 iso-ir-100 csISOLatin1 ISO-8859-15 latin9 UTF-32LE
    // UTF-32BE UTF-32 UTF-16LE UTF-16BE UTF-16 mulelao-1 roman8
    // hp-roman8 csHPRoman8 TIS-620 ISO 8859-11 WINSAMI2 WS2 Apple
    // Roman macintosh MacRoman windows-1258 CP1258 windows-1257
    // CP1257 windows-1256 CP1256 windows-1255 CP1255 windows-1254
    // CP1254 windows-1253 CP1253 windows-1252 CP1252 windows-1251
    // CP1251 windows-1250 CP1250 IBM866 CP866 csIBM866 IBM874 CP874
    // IBM850 CP850 csPC850Multilingual ISO-8859-16 iso-ir-226 latin10
    // ISO-8859-14 iso-ir-199 latin8 iso-celtic ISO-8859-13
    // ISO-8859-10 iso-ir-157 latin6 ISO-8859-10:1992 csISOLatin6
    // ISO-8859-9 iso-ir-148 latin5 csISOLatin5 ISO-8859-8 ISO
    // 8859-8-I iso-ir-138 hebrew csISOLatinHebrew ISO-8859-7 ECMA-118
    // greek iso-ir-126 csISOLatinGreek ISO-8859-6 ISO-8859-6-I
    // ECMA-114 ASMO-708 arabic iso-ir-127 csISOLatinArabic ISO-8859-5
    // cyrillic iso-ir-144 csISOLatinCyrillic ISO-8859-4 latin4
    // iso-ir-110 csISOLatin4 ISO-8859-3 latin3 iso-ir-109 csISOLatin3
    // ISO-8859-2 latin2 iso-ir-101 csISOLatin2 KOI8-U KOI8-RU KOI8-R
    // csKOI8R Iscii-Mlm Iscii-Knd Iscii-Tlg Iscii-Tml Iscii-Ori
    // Iscii-Gjr Iscii-Pnj Iscii-Bng Iscii-Dev TSCII GBK gb2312.1980-0
    // gbk-0 CP936 MS936 windows-936 jisx0201*-0 jisx0208*-0
    // ksc5601.1987-0 cp949 Big5-HKSCS big5-0 big5hkscs-0

    QStringList availableCodecsQt;
    foreach(const QByteArray &ba, QTextCodec::availableCodecs())
        availableCodecsQt << QString(ba);

    // Charsets detectable by libicu 4.4.2:
    QStringList allDetectableCharsetsICU;
    allDetectableCharsetsICU
    << "UTF-8"
    << "UTF-16BE"
    << "UTF-16LE"
    << "UTF-32BE"
    << "UTF-32LE"
    << "ISO-8859-1"
    << "ISO-8859-2"
    << "ISO-8859-5"
    << "ISO-8859-6"
    << "ISO-8859-7"
    << "ISO-8859-8-I"
    << "ISO-8859-8"
    << "ISO-8859-9"
    << "KOI8-R"
    << "Shift_JIS"
    << "GB18030"
    << "EUC-JP"
    << "EUC-KR"
    << "Big5"
    << "ISO-2022-JP"
    << "ISO-2022-KR"
    << "ISO-2022-CN"
    << "IBM424_rtl"
    << "IBM424_ltr"
    << "IBM420_rtl"
    << "IBM420_ltr"
    << "windows-1250"
    << "windows-1251"
    << "windows-1252"
    << "windows-1253"
    << "windows-1255"
    << "windows-1256"
    << "windows-1254";

    // The charsets detectable by libicu can be determined by
    // ucsdet_getAllDetectableCharsets() and the documentation for
    // that function at
    // http://icu-project.org/apiref/icu4c/ucsdet_8h.html says:
    //
    //     “The state of the Charset detector that is passed in does
    //     not affect the result of this function, but requiring a
    //     valid, open charset detector as a parameter insures that
    //     the charset detection service has been safely initialized
    //     and that the required detection data is available.”
    //
    // but that does not seem to be completely true, in fact it
    // *does* depend on the state of the charset detector. For example
    // sometimes "windows-1250" *is* among the returned charsets.
    // This happens if some non-ASCII text
    // is in the detector and a detection is attempted and *then*
    // ucsdet_getAllDetectableCharsets() is called.
    // And sometimes "windows-1250" is *not* among the returned
    // charsets. This happens when an empty charset detector is created
    // and then ucsdet_getAllDetectableCharsets() is called.
    // If ucsdet_getAllDetectableCharsets() has been called once
    // the list of returned charsets never seems to change anymore,
    // even if the text in the detector is changed again and
    // another detection attempted which would result in a different
    // list if ucsdet_getAllDetectableCharsets() were called first
    // in that state.
    //
    // Sometimes ucsdet_getAllDetectableCharsets() reports charsets
    // multiple times depending on the number of languages it can
    // detect for that charsets, i.e. it may report ISO-8859-2 four
    // times because it can detect the languages “cs”, “hu”,
    // “pl”, and “ro” with that charset.
    //
    // This looks like a bug to me, to get a reliable list,
    // I have hardcoded the complete list of charsets which
    // ucsdet_getAllDetectableCharsets() can possibly return
    // for all states of the detector above.
    //
    // Therefore, the following code should not any extra charsets
    // anymore, at least not for libicu 4.4.2:
    clearError();
    UEnumeration *en =
        ucsdet_getAllDetectableCharsets(d->_uCharsetDetector, &(d->_status));
    if (!hasError()) {
        qint32 len;
        const UChar *uc;
        while ((uc = uenum_unext(en, &len, &(d->_status))) != NULL) {
            if(uc && !hasError())
                allDetectableCharsetsICU << QString::fromUtf16(uc, len);
        }
    }
    uenum_close(en);

    // remove all charsets not supported by QTextCodec and all duplicates:
    foreach(const QString &cs, allDetectableCharsetsICU) {
        if(availableCodecsQt.contains(cs) && !d->_allDetectableCharsets.contains(cs))
            d->_allDetectableCharsets << cs;
    }

    qSort(d->_allDetectableCharsets);

    return d->_allDetectableCharsets;
}

void MCharsetDetector::enableInputFilter(bool enable)
{
    Q_D(MCharsetDetector);
    clearError();
    ucsdet_enableInputFilter(d->_uCharsetDetector, UBool(enable));
}

bool MCharsetDetector::isInputFilterEnabled()
{
    Q_D(MCharsetDetector);
    clearError();
    return bool(ucsdet_isInputFilterEnabled(d->_uCharsetDetector));
}
