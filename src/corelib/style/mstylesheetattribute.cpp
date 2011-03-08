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

#include <QString>
#include <QMetaObject>
#include <QDataStream>
#include <QMetaProperty>
#include <QVariant>
#include <QFont>
#include <QDebug>
#include <QTextCharFormat>
#include <QEasingCurve>
#include <QTextOption>

#include "mstylesheet.h"
#include "mstyle.h"
#include "mdeviceprofile.h"
#include "mtheme.h"
#include "mfeedback.h"
#include "mapplication.h"
#include "mapplicationwindow.h"
#include "mstylesheetparser.h"
#include "mshareddata.h"
#include "mbackgroundtiles.h"
#include "mscalableimage.h"
#include "movershotbeziereasingcurve.h"
#include "mmetatypes.h"
#include "mnamespace.h"

// internal enum that defines the attribute type
typedef enum {
    BOOL_TYPE,
    INT_TYPE,
    COLOR_TYPE,
    REAL_TYPE,
    CONST_PIXMAP_TYPE,
    CONST_SCALABLE_TYPE,
    SCALABLE_IMAGE_TILES_TYPE,
    SIZE_TYPE,
    SIZEF_TYPE,
    POINT_TYPE,
    POINTF_TYPE,
    FONT_TYPE,
    STRING_TYPE,
    CHAR_TYPE,
    ALIGNMENT_TYPE,
    ORIENTATION_TYPE,
    UNDERLINESTYLE_TYPE,
    PENSTYLE_TYPE,
    AXIS_TYPE,
    FEEDBACK_TYPE,

    EASINGCURVE_TYPE,

    WRAPMODE_TYPE,

    CONST_PIXMAP_TYPE_0,
    CONST_PIXMAP_TYPE_1,
    CONST_PIXMAP_TYPE_3,

    CONST_SCALABLE_TYPE_0,
    CONST_SCALABLE_TYPE_1,
    CONST_SCALABLE_TYPE_5,

    NUM_TYPES

} MContainerType;

// types of attribute values in CSS file
enum {
    COLOR_VALUE,
    IMAGE_VALUE,
    BOX_VALUE,
    SCALABLE_VALUE,
    SIZE_VALUE,
    POS_VALUE,
    FONT_VALUE,

    NUM_VALUES
};

// measurement unit types
enum {
    PIXELS_UNIT,
    MM_UNIT,
    PT_UNIT,
    PERCENT_UNIT,

    NUM_UNITS
};

static const QByteArray units[NUM_UNITS] = {
    QByteArray("px"),
    QByteArray("mm"),
    QByteArray("pt"),
    QByteArray("%")
};

static const QByteArray types[NUM_TYPES] = {
    QByteArray("bool"),
    QByteArray("int"),
    QByteArray("QColor"),
    QByteArray("qreal"),
    QByteArray("const QPixmap*"),
    QByteArray("const MScalableImage*"),
    QByteArray("MBackgroundTiles"),
    QByteArray("QSize"),
    QByteArray("QSizeF"),
    QByteArray("QPoint"),
    QByteArray("QPointF"),
    QByteArray("QFont"),
    QByteArray("QString"),
    QByteArray("QChar"),
    QByteArray("Qt::Alignment"),
    QByteArray("Qt::Orientation"),
    QByteArray("QTextCharFormat::UnderlineStyle"),
    QByteArray("Qt::PenStyle"),
    QByteArray("Qt::Axis"),
    QByteArray("MFeedback"),
    QByteArray("QEasingCurve"),
    QByteArray("QTextOption::WrapMode")
};

// These should be always in lower case
static const QByteArray values[NUM_VALUES] = {
    QByteArray("#"),
    QByteArray("image("),
    QByteArray("box("),
    QByteArray("scalable("),
    QByteArray("size("),
    QByteArray("pos("),
    QByteArray("font("),
};

enum CustomEasingCurveTypes {
    FirstCustomType = QEasingCurve::NCurveTypes + 1,
    OvershotBezier = FirstCustomType
};

//! \internal
class QtDatatypeConverter
{
public:
    QHash<QByteArray, Qt::Alignment> ALIGNMENTS;
    QHash<QByteArray, Qt::Orientation> ORIENTATIONS;
    QHash<QByteArray, QTextCharFormat::UnderlineStyle> UNDERLINESTYLES;
    QHash<QByteArray, Qt::PenStyle> PENSTYLES;
    QHash<QByteArray, Qt::Axis> AXES;
    QHash<QByteArray, QFont::Weight> WEIGHTS;
    QHash<QByteArray, QFont::Capitalization> CAPITALIZATION;
    QHash<QByteArray, int> EASINGCURVETYPES;
    QHash<QByteArray, QTextOption::WrapMode> WRAPMODES;


    QtDatatypeConverter() {
        ALIGNMENTS["left"] = Qt::AlignLeft;
        ALIGNMENTS["right"] = Qt::AlignRight;
        ALIGNMENTS["hcenter"] = Qt::AlignHCenter;
        ALIGNMENTS["justify"] = Qt::AlignJustify;
        ALIGNMENTS["top"] = Qt::AlignTop;
        ALIGNMENTS["bottom"] = Qt::AlignBottom;
        ALIGNMENTS["vcenter"] = Qt::AlignVCenter;
        ALIGNMENTS["center"] = Qt::AlignCenter;
        ALIGNMENTS["absolute"] = Qt::AlignAbsolute;

        ORIENTATIONS["vertical"] = Qt::Vertical;
        ORIENTATIONS["horizontal"] = Qt::Horizontal;

        UNDERLINESTYLES["nounderline"] = QTextCharFormat::NoUnderline;
        UNDERLINESTYLES["singleunderline"] = QTextCharFormat::SingleUnderline;
        UNDERLINESTYLES["dashunderline"] = QTextCharFormat::DashUnderline;
        UNDERLINESTYLES["dotline"] = QTextCharFormat::DotLine;
        UNDERLINESTYLES["dashdotline"] = QTextCharFormat::DashDotLine;
        UNDERLINESTYLES["dashdotdotline"] = QTextCharFormat::DashDotDotLine;
        UNDERLINESTYLES["waveunderline"] = QTextCharFormat::WaveUnderline;

        PENSTYLES["nopen"] = Qt::NoPen;
        PENSTYLES["solidline"] = Qt::SolidLine;
        PENSTYLES["dashline"] = Qt::DashLine;
        PENSTYLES["dotline"] = Qt::DotLine;
        PENSTYLES["dashdotline"] = Qt::DashDotLine;
        PENSTYLES["dashdotdotline"] = Qt::DashDotDotLine;

        AXES["x"] = Qt::XAxis;
        AXES["y"] = Qt::YAxis;
        AXES["z"] = Qt::ZAxis;

        WEIGHTS["light"] = QFont::Light;
        WEIGHTS["normal"] = QFont::Normal;
        WEIGHTS["demibold"] = QFont::DemiBold;
        WEIGHTS["bold"] = QFont::Bold;
        WEIGHTS["black"] = QFont::Black;

        CAPITALIZATION["mixedcase"] = QFont::MixedCase;
        CAPITALIZATION["uppercase"] = QFont::AllUppercase;
        CAPITALIZATION["lowercase"] = QFont::AllLowercase;
        CAPITALIZATION["smallcaps"] = QFont::SmallCaps;
        CAPITALIZATION["capitalize"] = QFont::Capitalize;

        EASINGCURVETYPES["linear"] = QEasingCurve::Linear;
        EASINGCURVETYPES["inquad"] = QEasingCurve::InQuad;
        EASINGCURVETYPES["outquad"] = QEasingCurve::OutQuad;
        EASINGCURVETYPES["inoutquad"] = QEasingCurve::InOutQuad;
        EASINGCURVETYPES["outinquad"] = QEasingCurve::OutInQuad;
        EASINGCURVETYPES["incubic"] = QEasingCurve::InCubic;
        EASINGCURVETYPES["outcubic"] = QEasingCurve::OutCubic;
        EASINGCURVETYPES["inoutcubic"] = QEasingCurve::InOutCubic;
        EASINGCURVETYPES["outincubic"] = QEasingCurve::OutInCubic;
        EASINGCURVETYPES["inquart"] = QEasingCurve::InQuart;
        EASINGCURVETYPES["outquart"] = QEasingCurve::OutQuart;
        EASINGCURVETYPES["inoutquart"] = QEasingCurve::InOutQuart;
        EASINGCURVETYPES["outinquart"] = QEasingCurve::OutInQuart;
        EASINGCURVETYPES["inquint"] = QEasingCurve::InQuint;
        EASINGCURVETYPES["outquint"] = QEasingCurve::OutQuint;
        EASINGCURVETYPES["inoutquint"] = QEasingCurve::InOutQuint;
        EASINGCURVETYPES["outinquint"] = QEasingCurve::OutInQuint;
        EASINGCURVETYPES["insine"] = QEasingCurve::InSine;
        EASINGCURVETYPES["outsine"] = QEasingCurve::OutSine;
        EASINGCURVETYPES["inoutsine"] = QEasingCurve::InOutSine;
        EASINGCURVETYPES["outinsine"] = QEasingCurve::OutInSine;
        EASINGCURVETYPES["inexpo"] = QEasingCurve::InExpo;
        EASINGCURVETYPES["outexpo"] = QEasingCurve::OutExpo;
        EASINGCURVETYPES["inoutexpo"] = QEasingCurve::InOutExpo;
        EASINGCURVETYPES["outinexpo"] = QEasingCurve::OutInExpo;
        EASINGCURVETYPES["incirc"] = QEasingCurve::InCirc;
        EASINGCURVETYPES["outcirc"] = QEasingCurve::OutCirc;
        EASINGCURVETYPES["inoutcirc"] = QEasingCurve::InOutCirc;
        EASINGCURVETYPES["outincirc"] = QEasingCurve::OutInCirc;
        EASINGCURVETYPES["inelastic"] = QEasingCurve::InElastic;
        EASINGCURVETYPES["outelastic"] = QEasingCurve::OutElastic;
        EASINGCURVETYPES["inoutelastic"] = QEasingCurve::InOutElastic;
        EASINGCURVETYPES["outinelastic"] = QEasingCurve::OutInElastic;
        EASINGCURVETYPES["inback"] = QEasingCurve::InBack;
        EASINGCURVETYPES["outback"] = QEasingCurve::OutBack;
        EASINGCURVETYPES["inoutback"] = QEasingCurve::InOutBack;
        EASINGCURVETYPES["outinback"] = QEasingCurve::OutInBack;
        EASINGCURVETYPES["inbounce"] = QEasingCurve::InBounce;
        EASINGCURVETYPES["outbounce"] = QEasingCurve::OutBounce;
        EASINGCURVETYPES["inoutbounce"] = QEasingCurve::InOutBounce;
        EASINGCURVETYPES["outinbounce"] = QEasingCurve::OutInBounce;
        EASINGCURVETYPES["overshotbezier"] = OvershotBezier;

        WRAPMODES[""] = QTextOption::NoWrap;
        WRAPMODES["nowrap"] = QTextOption::NoWrap;
        WRAPMODES["wordwrap"] = QTextOption::WordWrap;
        WRAPMODES["wrapanywhere"] = QTextOption::WrapAnywhere;
        WRAPMODES["wrapatwordboundaryoranywhere"] =QTextOption::WrapAtWordBoundaryOrAnywhere;

        qRegisterMetaType<const QPixmap *>();
        qRegisterMetaType<QTextCharFormat::UnderlineStyle>();
        qRegisterMetaType<Qt::Alignment>();
        qRegisterMetaType<Qt::Orientation>();
        qRegisterMetaType<Qt::PenStyle>();
        qRegisterMetaType<Qt::Axis>();

        qRegisterMetaType<MFeedback>();
        qRegisterMetaType<const MScalableImage *>();
        qRegisterMetaType<MBackgroundTiles>();

        qRegisterMetaType<QTextOption::WrapMode>();
    }
};

// global array with hashes for already parsed variants in landscape and portrait
// the first has contains values as keys, the second one the variant types
namespace {
    static QHash<MUniqueStringCache::Index, QHash<int, QVariant> > variantCache[3];
}
//! \internal_end

static QtDatatypeConverter DataTypeConverter;

MStyleSheetAttribute::MStyleSheetAttribute(MUniqueStringCache::Index name, MUniqueStringCache::Index value, qint64 position)
    : name(name), value(value), position(position)
{
}

QLatin1String MStyleSheetAttribute::getName() const
{
    return MStyleSheetParser::stringCacheWithReverseLookup()->indexToString(name);
}

MUniqueStringCache::Index MStyleSheetAttribute::getNameID() const
{
    return name;
}

QLatin1String MStyleSheetAttribute::getValue() const
{
    return MStyleSheetParser::stringCacheWithoutReverseLookup()->indexToString(value);
}

MUniqueStringCache::Index MStyleSheetAttribute::getValueID() const
{
    return value;
}

qint64 MStyleSheetAttribute::getPosition() const
{
    return position;
}

int MStyleSheetAttribute::maxValueForDevice(SizeAttributeType type, M::Orientation orientation)
{
    // Percentages must be based on the devices native orientation, so
    // here we are returning the correct sizes based on the device AND the
    // requested orientation.
    //
    // Another way of putting is, for portrait native devices:
    //   M::landscape == MDeviceProfile::instance()->resolution().height()
    // While, for landscape native devices:
    //   M::landscape == MDeviceProfile::instance()->resolution().width()

    if (MDeviceProfile::instance()->orientationFromAngle(M::Angle0) == M::Landscape) {
        // native display orientation is landscape
        if (orientation == M::Landscape) {
            if (type == WidthAttribute)
                return MDeviceProfile::instance()->resolution().width();
            else
                return MDeviceProfile::instance()->resolution().height();
        } else {
            if (type == WidthAttribute)
                return MDeviceProfile::instance()->resolution().height();
            else
                return MDeviceProfile::instance()->resolution().width();
        }
    } else {
        // native display orientation is portrait
        if (orientation == M::Landscape) {
            if (type == WidthAttribute)
                return MDeviceProfile::instance()->resolution().height();
            else
                return MDeviceProfile::instance()->resolution().width();
        } else {
            if (type == WidthAttribute)
                return MDeviceProfile::instance()->resolution().width();
            else
                return MDeviceProfile::instance()->resolution().height();
        }
    }
}

int MStyleSheetAttribute::attributeToInt(const QByteArray &attribute, bool *conversionOk)
{
    QByteArray value = attribute.trimmed();

    if (attribute.endsWith(units[PIXELS_UNIT])) {
        // strip "px" from the end
        value.truncate(value.length() - 2);
        return value.toInt(conversionOk);
    }

    if (attribute.endsWith(units[MM_UNIT])) {
        // strip "mm" from the end
        value.truncate(value.length() - 2);

        return MDeviceProfile::instance()->mmToPixels(value.toFloat(conversionOk));
    }

    if (attribute.endsWith(units[PT_UNIT])) {
        // strip "pt" from the end
        value.truncate(value.length() - 2);

        return MDeviceProfile::instance()->ptToPixels(value.toFloat(conversionOk));
    }

    return value.toInt(conversionOk);
}

int MStyleSheetAttribute::attributeToInt(const QByteArray &attribute, bool *conversionOk, SizeAttributeType type, M::Orientation orientation, CacheOrientationFlags *cacheOrientation)
{
    QByteArray value = attribute.trimmed();

    if (attribute.endsWith(units[PERCENT_UNIT])) {
        int maximumValue = maxValueForDevice(type, orientation);

        *cacheOrientation = (orientation==M::Landscape) ? LandscapeFlag : PortraitFlag;

        value.truncate(value.length() - 1);

        // convert to integer value
        qreal v = value.toFloat(conversionOk);
        return(v / 100.f * (float)maximumValue);
    } else {
        return attributeToInt(attribute, conversionOk);
    }
}

qreal MStyleSheetAttribute::attributeToFloat(const QByteArray &attribute, bool *conversionOk)
{
    QByteArray value = attribute.trimmed();

    if (attribute.endsWith(units[PIXELS_UNIT])) {
        // strip "px" from the end
        value.truncate(value.length() - 2);
        return value.toFloat(conversionOk);
    }

    if (attribute.endsWith(units[MM_UNIT])) {
        // strip "mm" from the end
        value.truncate(value.length() - 2);

        return MDeviceProfile::instance()->mmToPixelsF(value.toFloat(conversionOk));
    }

    if (attribute.endsWith(units[PT_UNIT])) {
        // strip "pt" from the end
        value.truncate(value.length() - 2);

        return MDeviceProfile::instance()->ptToPixelsF(value.toFloat(conversionOk));
    }

    return value.toFloat(conversionOk);
}

qreal MStyleSheetAttribute::attributeToFloat(const QByteArray &attribute, bool *conversionOk, SizeAttributeType type, M::Orientation orientation, CacheOrientationFlags *cacheOrientation)
{
    QByteArray value = attribute.trimmed();

    if (attribute.endsWith(units[PERCENT_UNIT])) {
        int maximumValue = maxValueForDevice(type, orientation);

        *cacheOrientation = (orientation==M::Landscape) ? LandscapeFlag : PortraitFlag;

        value.truncate(value.length() - 1);

        // convert to integer value
        qreal v = value.toFloat(conversionOk);//myAtof(value, conversionOk);
        return(v / 100.f * (float) maximumValue);
    } else {
        return attributeToFloat(attribute, conversionOk);
    }
}

bool MStyleSheetAttribute::booleanFromString(const QByteArray &string, bool *conversionOk)
{
    if (qstricmp(string, "true") == 0) {
        if (conversionOk)
            *conversionOk = true;
        return true;
    } else if (qstricmp(string, "false") == 0) {
        if (conversionOk)
            *conversionOk = true;
        return false;
    }

    if (conversionOk)
        *conversionOk = false;
    return bool();
}

QColor MStyleSheetAttribute::colorFromString(const QByteArray &string, bool *conversionOk)
{
    QColor color(string.data());
    if (conversionOk)
        *conversionOk = (string.length() > 0) ? color.isValid() : true;
    return color;
}

QFont MStyleSheetAttribute::fontFromString(const QByteArray string, bool *conversionOk)
{
    //font: "font family" 20px
    //font: "font family" bold 20px
    //font: "font family" italic 20px
    //font: "font family" uppercase 20px
    //font: "font family" bold italic 20px
    //font: "font family" bold uppercase 20px
    //font: "font family" italic uppercase 20px
    //font: "font family" bold italic uppercase 20px
    //font: font_family 20px
    //font: font_family bold 20px
    //font: font_family italic 20px
    //font: font_family uppercase 20px
    //font: font_family bold italic 20px
    //font: font_family bold uppercase 20px
    //font: font_family italic uppercase 20px
    //font: font_family bold italic uppercase 20px
    bool ok;
    if (!conversionOk)
        conversionOk = &ok;
    *conversionOk = false;

    QList<QByteArray> list;
    if (string.startsWith('\"')) {
        int idx = string.indexOf('\"', 1);
        if (idx != -1) {
            //get quoted font family name
            QByteArray family = string.mid(1, idx - 1);

            //split rest of the parameters
            QByteArray values = string.mid(idx + 1).trimmed();
            list = values.split(' ');
            list.removeAll("");

            //insert family as first parameter
            list.insert(0, family);
        }
    } else {
        //no quotes, just split the parameters
        list = string.split(' ');
    }

    //family + font size
    if (list.size() == 2) {
        qreal pixelSize = attributeToFloat(list.at(1), conversionOk);
        if (*conversionOk) {
            QFont font(list.at(0));
            font.setPixelSize(pixelSize);
            return font;
        }
    }
    //family + weight/italic/capitalization + font size
    else if (list.size() == 3) {
        qreal pixelSize = attributeToFloat(list.at(2), conversionOk);
        if (*conversionOk) {
            QFont font(list.at(0));
            font.setPixelSize(pixelSize);
            if (DataTypeConverter.WEIGHTS.contains(list.at(1))) {
                font.setWeight(DataTypeConverter.WEIGHTS[list.at(1)]);
                return font;
            } else if (list.at(1) == "italic") {
                font.setItalic(true);
                return font;
            } else if (DataTypeConverter.CAPITALIZATION.contains(list.at(1))) {
                font.setCapitalization(DataTypeConverter.CAPITALIZATION[list.at(1)]);
                return font;
            } else {
                *conversionOk = false;
            }
        }
    }
    //family + weight/italic + italic/capitalization + font size
    else if (list.size() == 4) {
        qreal pixelSize = attributeToFloat(list.at(3), conversionOk);
        if (*conversionOk) {
            QFont font(list.at(0));
            font.setPixelSize(pixelSize);
            if (DataTypeConverter.WEIGHTS.contains(list.at(1)) && list.at(2) == "italic") {
                font.setWeight(DataTypeConverter.WEIGHTS[list.at(1)]);
                font.setItalic(true);
                return font;
            } else if (DataTypeConverter.WEIGHTS.contains(list.at(1)) && DataTypeConverter.CAPITALIZATION.contains(list.at(2))) {
                font.setWeight(DataTypeConverter.WEIGHTS[list.at(1)]);
                font.setCapitalization(DataTypeConverter.CAPITALIZATION[list.at(2)]);
                return font;
            } else if (list.at(1) == "italic" && DataTypeConverter.CAPITALIZATION.contains(list.at(2))) {
                font.setItalic(true);
                font.setCapitalization(DataTypeConverter.CAPITALIZATION[list.at(2)]);
                return font;
            } else {
                *conversionOk = false;
            }
        }
    }
    //family + weight + italic + capitalization + font size
    else if (list.size() == 5) {
        qreal pixelSize = attributeToFloat(list.at(4), conversionOk);
        if (*conversionOk) {
            QFont font(list.at(0));
            font.setPixelSize(pixelSize);
            if (DataTypeConverter.WEIGHTS.contains(list.at(1)) && list.at(2) == "italic" && DataTypeConverter.CAPITALIZATION.contains(list.at(3))) {
                font.setWeight(DataTypeConverter.WEIGHTS[list.at(1)]);
                font.setItalic(true);
                font.setCapitalization(DataTypeConverter.CAPITALIZATION[list.at(3)]);
                return font;
            } else {
                *conversionOk = false;
            }
        }
    }

    return QFont();
}

QByteArray MStyleSheetAttribute::attributeNameToPropertyName(const QByteArray &attributeName) {
    QByteArray result;
    const int length = attributeName.length();
    for (int i = 0; i < length; ++i) {
        if (attributeName.at(i) == '-') {
            if (length > i + 1) {
                i++;
                result += toupper(attributeName.at(i));
            }
        } else {
            result += attributeName.at(i);
        }
    }

    return result;
}

bool MStyleSheetAttribute::writeAttribute(MUniqueStringCache::Index filename,
        MStyle *style,
        const QMetaProperty &property,
        M::Orientation orientation) const
{
    // first check if the attribute is cached orientation independent, if not found
    // check for the given orientation
    QVariant cachedVariant = variantCache[M::Landscape + 1][value][property.userType()];
    if (cachedVariant.isValid()) {
        return property.write(style, cachedVariant);
    } else {
        cachedVariant = variantCache[orientation][value][property.userType()];
        if (cachedVariant.isValid()) {
            style->setOrientationDependent(true);
            return property.write(style, cachedVariant);
        }
    }

    bool conversionOK = false;
    // most types are the same in landscape and portrait
    CacheOrientationFlags cacheOrientation = CacheOrientationFlags(PortraitFlag | LandscapeFlag);

    QLatin1String vs = MStyleSheetParser::stringCacheWithoutReverseLookup()->indexToString(value);
    QByteArray valueString = QByteArray::fromRawData(vs.latin1(), strlen(vs.latin1()));

    const int attributeType = property.userType();
    if (attributeType == QMetaType::Bool) {
        bool result = booleanFromString(valueString, &conversionOK);
        if (conversionOK) {
            return fillProperty(property, style, cacheOrientation, result);
        }
    } else if (attributeType == QMetaType::Int) {
        int integer = attributeToInt(valueString, &conversionOK);
        if (conversionOK) {
            return fillProperty(property, style, cacheOrientation, integer);
        }
    } else if (attributeType == QMetaType::QColor) {
        if(valueString == "none")
            return fillProperty(property, style, cacheOrientation, QColor());

        QColor color = colorFromString(valueString, &conversionOK);
        if (conversionOK) {
            return fillProperty(property, style, cacheOrientation, color);
        }
    } else if (attributeType == QMetaType::QReal) {
        qreal real = attributeToFloat(valueString, &conversionOK);
        if (conversionOK) {
            return fillProperty(property, style, cacheOrientation, real);
        }
    } else if (attributeType == qMetaTypeId<const QPixmap*>()) {
        if(valueString == "none")
            return fillProperty(property, style, cacheOrientation, qVariantFromValue((const QPixmap *) NULL));

        //"image: image_id;"
        //"image: image_id 64px 64px;"
        //"image: "image id";"
        //"image: "image id" 64px 64px;"

        QList<QByteArray> list;
        if (valueString.startsWith('\"')) {
            //parse name inside quotes
            int idx = valueString.indexOf('\"', 1);
            if (idx != -1) {
                //get quoted image_id
                QByteArray imageid = valueString.mid(1, idx - 1);

                //split rest of the parameters
                QByteArray values = valueString.mid(idx + 1).trimmed();
                list = values.split(' ');
                list.removeAll("");

                //insert image_id as first parameter
                list.insert(0, imageid);
            }
        } else {
            //no quotes, just split the parameters
            list = valueString.split(' ');
            list.removeAll("");
        }

        //only image_id
        if (list.size() == 1) {
            const QPixmap *pixmap = MTheme::pixmap(list.at(0));
            return fillProperty(property, style, cacheOrientation, qVariantFromValue(pixmap), false);
        }
        //image_id + width + height
        else if (list.size() == 3) {
            int width = attributeToInt(list.at(1), &conversionOK, WidthAttribute, orientation, &cacheOrientation);
            int height = attributeToInt(list.at(2), &conversionOK, HeightAttribute, orientation, &cacheOrientation);
            const QPixmap *pixmap = MTheme::pixmap(list.at(0), QSize(width, height));
            return fillProperty(property, style, cacheOrientation, qVariantFromValue(pixmap), false);
        }
        //no parameters
        else if (list.size() == 0) {
            //init null pixmap which is ok if someone does not want to use it
            return fillProperty(property, style, cacheOrientation, qVariantFromValue((const QPixmap *) NULL));
        }
    } else if (attributeType == qMetaTypeId<const MScalableImage*>() || attributeType == qMetaTypeId<MBackgroundTiles>()) {
        //"background: image_id left right top bottom;"
        //"background: image_id;"
        //"background: "image id" left right top bottom;"
        //"background: "image id";"

        QList<QByteArray> list;
        if (valueString.startsWith('\"')) {
            //parse name inside quotes
            int idx = valueString.indexOf('\"', 1);
            if (idx != -1) {
                //get quoted image_id
                QByteArray imageid = valueString.mid(1, idx - 1);

                //split rest of the parameters
                QByteArray values = valueString.mid(idx + 1).trimmed();
                list = values.split(' ');
                list.removeAll("");
                //insert image_id as first parameter
                list.insert(0, imageid);
            }
        } else {
            //no quotes, just split the parameters
            list = valueString.split(' ');
            list.removeAll("");
        }

        //no parameters
        if (valueString.isEmpty() || valueString == "none") {
            //init null image which is ok if someone does not want to use it
            if(attributeType == qMetaTypeId<const MScalableImage*>())
                return fillProperty(property, style, cacheOrientation, qVariantFromValue((const MScalableImage *) NULL));
            else
                return fillProperty(property, style, cacheOrientation, QVariant::fromValue(MBackgroundTiles()), false);
        }
        //only image_id
        else if (list.size() == 1) {
            if(attributeType == qMetaTypeId<const MScalableImage*>()) {
                const MScalableImage *image = MTheme::scalableImage(list.at(0), 0, 0, 0, 0);
                return fillProperty(property, style, cacheOrientation, qVariantFromValue(image), false);
            } else {
                return fillProperty(property, style, cacheOrientation, QVariant::fromValue(MBackgroundTiles(list.at(0), 0,0,0,0)), false);
            }
        }
        //image_id + border width paramaters
        else if (list.size() == 5) {
            //image_id and the border parameters
            if(attributeType == qMetaTypeId<const MScalableImage*>()) {
                const MScalableImage *image = MTheme::scalableImage(list.at(0),
                                                attributeToInt(list.at(1), &conversionOK),
                                                attributeToInt(list.at(2), &conversionOK),
                                                attributeToInt(list.at(3), &conversionOK),
                                                attributeToInt(list.at(4), &conversionOK));
                return fillProperty(property, style, cacheOrientation, qVariantFromValue(image), false);
            } else {
                return fillProperty(property, style, cacheOrientation, QVariant::fromValue(MBackgroundTiles(list.at(0),
                                                                attributeToInt(list.at(1), &conversionOK),
                                                                attributeToInt(list.at(2), &conversionOK),
                                                                attributeToInt(list.at(3), &conversionOK),
                                                                attributeToInt(list.at(4), &conversionOK))), false);
            }
        }
    } else if (attributeType == QMetaType::QSize || attributeType == QMetaType::QSizeF) {
        //size: 25px 25px;

        //just split into pieces and create QSize or QSizeF depending on the attributeType
        QList<QByteArray> list = valueString.split(' ');
        list.removeAll("");
        if (list.size() == 2) {
            if (attributeType == QMetaType::QSize) {
                int width = attributeToInt(list.at(0), &conversionOK, WidthAttribute, orientation, &cacheOrientation);
                int height = attributeToInt(list.at(1), &conversionOK, HeightAttribute, orientation, &cacheOrientation);
                return fillProperty(property, style, cacheOrientation, QSize(width, height));
            } else {
                qreal width = attributeToFloat(list.at(0), &conversionOK, WidthAttribute, orientation, &cacheOrientation);
                qreal height = attributeToFloat(list.at(1), &conversionOK, HeightAttribute, orientation, &cacheOrientation);
                return fillProperty(property, style, cacheOrientation, QSizeF(width, height));
            }
        }
    } else if (attributeType == QMetaType::QPoint || attributeType == QMetaType::QPointF) {
        //"point: 256px 123px;

        //just split into pieces and create QPoint or QPointF depending on the attributeType
        QList<QByteArray> list = valueString.split(' ');
        list.removeAll("");
        if (list.size() == 2) {
            if (attributeType == QMetaType::QPoint) {
                int x = attributeToInt(list.at(0), &conversionOK, WidthAttribute, orientation, &cacheOrientation);
                int y = attributeToInt(list.at(1), &conversionOK, HeightAttribute, orientation, &cacheOrientation);
                return fillProperty(property, style, cacheOrientation, QPoint(x, y));
            } else {
                qreal x = attributeToFloat(list.at(0), &conversionOK, WidthAttribute, orientation, &cacheOrientation);
                qreal y = attributeToFloat(list.at(1), &conversionOK, HeightAttribute, orientation, &cacheOrientation);
                return fillProperty(property, style, cacheOrientation, QPointF(x, y));
            }
        }
    } else if (attributeType == QMetaType::QFont) {
        QFont font = fontFromString(valueString, &conversionOK);
        if (conversionOK) {
            return fillProperty(property, style, cacheOrientation, font);
        }
    } else if (attributeType == QMetaType::QString) {
        if (valueString.length() >= 2) {
            if ((valueString.at(0) == 0x22) && (valueString.at(valueString.length()-1) == 0x22)) {
                return fillProperty(property, style, cacheOrientation, QString(valueString.mid(1, valueString.length() - 2)));
            }
        } else if (valueString.length() == 0) {
            return fillProperty(property, style, cacheOrientation, QString());
        }
    } else if (attributeType == QMetaType::QChar) {
        if (valueString.length() == 3) {
            if ((valueString.at(0) == '\'') && (valueString.at(2) == '\'')) {
                return fillProperty(property, style, cacheOrientation, static_cast<QChar>(valueString.at(1)));
            }
        }
    } else if (attributeType == qMetaTypeId<Qt::Alignment>()) {
        if (DataTypeConverter.ALIGNMENTS.contains(valueString)) {
            return fillProperty(property, style, cacheOrientation, qVariantFromValue(DataTypeConverter.ALIGNMENTS[valueString]));
        }
    } else if (attributeType == qMetaTypeId<Qt::Orientation>()) {
        if (DataTypeConverter.ORIENTATIONS.contains(valueString)) {
            return fillProperty(property, style, cacheOrientation, qVariantFromValue(DataTypeConverter.ORIENTATIONS[valueString]));
        }
    } else if (attributeType == qMetaTypeId<QTextCharFormat::UnderlineStyle>()) {
        if (DataTypeConverter.UNDERLINESTYLES.contains(valueString)) {
            return fillProperty(property, style, cacheOrientation, qVariantFromValue(DataTypeConverter.UNDERLINESTYLES[valueString]));
        }
    } else if (attributeType == qMetaTypeId<Qt::PenStyle>()) {
        if (DataTypeConverter.PENSTYLES.contains(valueString)) {
            return fillProperty(property, style, cacheOrientation, qVariantFromValue(DataTypeConverter.PENSTYLES[valueString]));
        }
    } else if (attributeType == qMetaTypeId<Qt::Axis>()) {
        if (DataTypeConverter.AXES.contains(valueString)) {
            return fillProperty(property, style, cacheOrientation, qVariantFromValue(DataTypeConverter.AXES[valueString]));
        }
    } else if (attributeType == qMetaTypeId<MFeedback>()) {
        MFeedback feedback(valueString);
        return fillProperty(property, style, cacheOrientation, qVariantFromValue(feedback));
    } else if (attributeType == QMetaType::QEasingCurve) {
        QEasingCurve curve;
        // curve type
        QList<QByteArray> list = valueString.split(',');
        if (list.size() > 0) {
            if (DataTypeConverter.EASINGCURVETYPES.contains(list.at(0))) {
                int type = DataTypeConverter.EASINGCURVETYPES[list.at(0)];
                if (type < FirstCustomType)
                    curve.setType(static_cast<QEasingCurve::Type>(type));
                else if (type == OvershotBezier)
                    curve = MOvershotBezierEasingCurve();
                // curve amplitude
                if (list.size() > 1) {
                    curve.setAmplitude((qreal) list.at(1).toDouble());
                    // curve overshoot
                    if (list.size() > 2) {
                        curve.setOvershoot((qreal) list.at(2).toDouble());
                        // curve period
                        if (list.size() > 3) {
                            curve.setPeriod((qreal) list.at(3).toDouble());
                        }
                    }
                }
                return fillProperty(property, style, cacheOrientation, qVariantFromValue(curve));
            }
        }
    } else if (attributeType == qMetaTypeId<QTextOption::WrapMode>()) {
        if (DataTypeConverter.WRAPMODES.contains(valueString)) {
            return fillProperty(property, style, cacheOrientation, qVariantFromValue(DataTypeConverter.WRAPMODES[valueString]));
        }
    }

    MStyleSheetParser::outputParseError(MStyleSheetParser::stringCacheWithReverseLookup()->indexToString(filename), "Not a valid attribute(" + QLatin1String(property.typeName()) + "): " + MStyleSheetParser::stringCacheWithoutReverseLookup()->indexToString(name) + " : " + valueString, MStyleSheetParser::getLineNum(MStyleSheetParser::stringCacheWithReverseLookup()->indexToString(filename), position));
    return false;
}

bool MStyleSheetAttribute::fillProperty(const QMetaProperty &property, MStyle *style, CacheOrientationFlags cacheOrientation, const QVariant &variant, bool cache) const
{
    if (cache && variant.isValid()) {
        if (cacheOrientation == (PortraitFlag | LandscapeFlag)) {
            variantCache[M::Landscape + 1][value][property.userType()] = variant;
        } else {
            style->setOrientationDependent(true);
            if (cacheOrientation & PortraitFlag) {
                variantCache[M::Portrait][value][property.userType()] = variant;
            } else if (cacheOrientation & LandscapeFlag) {
                variantCache[M::Landscape][value][property.userType()] = variant;
            }
        }
    }

    return property.write(style, variant);
}
