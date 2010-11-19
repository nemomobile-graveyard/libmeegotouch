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

#if QT_VERSION >= 0x040600
    EASINGCURVE_TYPE,
#endif

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
#if QT_VERSION >= 0x040600
    QByteArray("QEasingCurve")
#endif
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

Q_DECLARE_METATYPE(const QPixmap *)
Q_DECLARE_METATYPE(const MScalableImage *)
Q_DECLARE_METATYPE(MBackgroundTiles)
Q_DECLARE_METATYPE(QTextCharFormat::UnderlineStyle)
Q_DECLARE_METATYPE(Qt::PenStyle)
Q_DECLARE_METATYPE(Qt::Axis)
Q_DECLARE_METATYPE(QPoint)
Q_DECLARE_METATYPE(QPointF)
Q_DECLARE_METATYPE(QSize)
Q_DECLARE_METATYPE(QSizeF)
Q_DECLARE_METATYPE(MFeedback)

#if QT_VERSION >= 0x040600
#include <QEasingCurve>
Q_DECLARE_METATYPE(QEasingCurve)
#endif

//! \internal
class QtDatatypeConverter
{
public:
    QMap<QByteArray, Qt::Alignment> ALIGNMENTS;
    QMap<QByteArray, Qt::Orientation> ORIENTATIONS;
    QMap<QByteArray, QTextCharFormat::UnderlineStyle> UNDERLINESTYLES;
    QMap<QByteArray, Qt::PenStyle> PENSTYLES;
    QMap<QByteArray, Qt::Axis> AXES;
    QMap<QByteArray, QFont::Weight> WEIGHTS;
    QMap<QByteArray, QFont::Capitalization> CAPITALIZATION;
#if QT_VERSION >= 0x040600
    QMap<QByteArray, QEasingCurve::Type> EASINGCURVETYPES;
#endif

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

#if QT_VERSION >= 0x040600
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
#endif
    }
};

// global array with hashes for already parsed variants in landscape and portrait
// the first has contains values as keys, the second one the variant types
namespace {
    static QHash<QByteArray, QHash<QByteArray, QVariant> > variantCache[2];
}
//! \internal_end

static QtDatatypeConverter DataTypeConverter;

MStyleSheetAttribute::MStyleSheetAttribute(const QByteArray& name, const QByteArray& value, qint64 position)
    : name(name), value(value), position(position)
{
}

MStyleSheetAttribute::MStyleSheetAttribute(const MStyleSheetAttribute &other)
{
    this->name = other.name;
    this->value = other.value;
    this->position = position;
}

QByteArray MStyleSheetAttribute::getName()
{
    return name;
}

QByteArray MStyleSheetAttribute::getValue()
{
    return value;
}

qint64 MStyleSheetAttribute::getPosition()
{
    return position;
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

int MStyleSheetAttribute::attributeToInt(const QByteArray &attribute, bool *conversionOk, SizeAttributeType type, M::Orientation orientation)
{
    QByteArray value = attribute.trimmed();

    if (attribute.endsWith(units[PERCENT_UNIT])) {
        int maximumValue = 0;

        if (orientation == M::Landscape) {
            if (type == WidthAttribute)
                maximumValue = MDeviceProfile::instance()->resolution().width();
            else
                maximumValue = MDeviceProfile::instance()->resolution().height();
        } else {
            if (type == WidthAttribute)
                maximumValue = MDeviceProfile::instance()->resolution().height();
            else
                maximumValue = MDeviceProfile::instance()->resolution().width();
        }

        value.truncate(value.length() - 1);

        // convert to integer value
        qreal v = value.toFloat(conversionOk);
        return(v / 100.0 * (float)maximumValue);
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

qreal MStyleSheetAttribute::attributeToFloat(const QByteArray &attribute, bool *conversionOk, SizeAttributeType type, M::Orientation orientation)
{
    QByteArray value = attribute.trimmed();

    if (attribute.endsWith(units[PERCENT_UNIT])) {
        int maximumValue = 0;

        if (orientation == M::Landscape) {
            if (type == WidthAttribute)
                maximumValue = MDeviceProfile::instance()->resolution().width();
            else
                maximumValue = MDeviceProfile::instance()->resolution().height();
        } else {
            if (type == WidthAttribute)
                maximumValue = MDeviceProfile::instance()->resolution().height();
            else
                maximumValue = MDeviceProfile::instance()->resolution().width();
        }

        value.truncate(value.length() - 1);

        // convert to integer value
        qreal v = value.toFloat(conversionOk);//myAtof(value, conversionOk);
        return(v / 100.0 * (float) maximumValue);
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
        qreal pixelSize = attributeToFloat(list[1], conversionOk);
        if (*conversionOk) {
            QFont font(list[0]);
            font.setPixelSize(pixelSize);
            return font;
        }
    }
    //family + weight/italic/capitalization + font size
    else if (list.size() == 3) {
        qreal pixelSize = attributeToFloat(list[2], conversionOk);
        if (*conversionOk) {
            QFont font(list[0]);
            font.setPixelSize(pixelSize);
            if (DataTypeConverter.WEIGHTS.contains(list[1])) {
                font.setWeight(DataTypeConverter.WEIGHTS[list[1]]);
                return font;
            } else if (list[1] == "italic") {
                font.setItalic(true);
                return font;
            } else if (DataTypeConverter.CAPITALIZATION.contains(list[1])) {
                font.setCapitalization(DataTypeConverter.CAPITALIZATION[list[1]]);
                return font;
            } else {
                *conversionOk = false;
            }
        }
    }
    //family + weight/italic + italic/capitalization + font size
    else if (list.size() == 4) {
        qreal pixelSize = attributeToFloat(list[3], conversionOk);
        if (*conversionOk) {
            QFont font(list[0]);
            font.setPixelSize(pixelSize);
            if (DataTypeConverter.WEIGHTS.contains(list[1]) && list[2] == "italic") {
                font.setWeight(DataTypeConverter.WEIGHTS[list[1]]);
                font.setItalic(true);
                return font;
            } else if (DataTypeConverter.WEIGHTS.contains(list[1]) && DataTypeConverter.CAPITALIZATION.contains(list[2])) {
                font.setWeight(DataTypeConverter.WEIGHTS[list[1]]);
                font.setCapitalization(DataTypeConverter.CAPITALIZATION[list[2]]);
                return font;
            } else if (list[1] == "italic" && DataTypeConverter.CAPITALIZATION.contains(list[2])) {
                font.setItalic(true);
                font.setCapitalization(DataTypeConverter.CAPITALIZATION[list[2]]);
                return font;
            } else {
                *conversionOk = false;
            }
        }
    }
    //family + weight + italic + capitalization + font size
    else if (list.size() == 5) {
        qreal pixelSize = attributeToFloat(list[4], conversionOk);
        if (*conversionOk) {
            QFont font(list[0]);
            font.setPixelSize(pixelSize);
            if (DataTypeConverter.WEIGHTS.contains(list[1]) && list[2] == "italic" && DataTypeConverter.CAPITALIZATION.contains(list[3])) {
                font.setWeight(DataTypeConverter.WEIGHTS[list[1]]);
                font.setItalic(true);
                font.setCapitalization(DataTypeConverter.CAPITALIZATION[list[3]]);
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
                result += QChar(attributeName.at(i)).toUpper();
            }
        } else {
            result += attributeName.at(i);
        }
    }

    return result;
}

bool MStyleSheetAttribute::writeAttribute(const QString &filename,
        MStyle *style,
        const QMetaProperty &property,
        M::Orientation orientation)
{
    QVariant cachedVariant = variantCache[orientation][value][property.typeName()];
    if (cachedVariant.isValid()) {
        return property.write(style, cachedVariant);
    }

    bool conversionOK = false;
    // most types are the same in landscape and portrait
    CacheOrientationFlags cacheOrientation = CacheOrientationFlags(PortraitFlag | LandscapeFlag);

    const char *attributeType = property.typeName();
    if (attributeType == types[BOOL_TYPE]) {
        bool result = booleanFromString(value, &conversionOK);
        if (conversionOK) {
            return fillProperty(property, style, cacheOrientation, result);
        }
    } else if (attributeType == types[INT_TYPE]) {
        int integer = attributeToInt(value, &conversionOK);
        if (conversionOK) {
            return fillProperty(property, style, cacheOrientation, integer);
        }
    } else if (attributeType == types[COLOR_TYPE]) {
        if(value == "none")
            return fillProperty(property, style, cacheOrientation, QColor());

        QColor color = colorFromString(value, &conversionOK);
        if (conversionOK) {
            return fillProperty(property, style, cacheOrientation, color);
        }
    } else if (attributeType == types[REAL_TYPE]) {
        qreal real = attributeToFloat(value, &conversionOK);
        if (conversionOK) {
            return fillProperty(property, style, cacheOrientation, real);
        }
    } else if (attributeType == types[CONST_PIXMAP_TYPE]) {
        if(value == "none")
            return fillProperty(property, style, cacheOrientation, qVariantFromValue((const QPixmap *) NULL));

        //"image: image_id;"
        //"image: image_id 64px 64px;"
        //"image: "image id";"
        //"image: "image id" 64px 64px;"

        QList<QByteArray> list;
        if (value.startsWith('\"')) {
            //parse name inside quotes
            int idx = value.indexOf('\"', 1);
            if (idx != -1) {
                //get quoted image_id
                QByteArray imageid = value.mid(1, idx - 1);

                //split rest of the parameters
                QByteArray values = value.mid(idx + 1).trimmed();
                list = values.split(' ');
                list.removeAll("");

                //insert image_id as first parameter
                list.insert(0, imageid);
            }
        } else {
            //no quotes, just split the parameters
            list = value.split(' ');
            list.removeAll("");
        }

        //only image_id
        if (list.size() == 1) {
            const QPixmap *pixmap = MTheme::pixmap(list[0]);
            return fillProperty(property, style, cacheOrientation, qVariantFromValue(pixmap));
        }
        //image_id + width + height
        else if (list.size() == 3) {
            int width = attributeToInt(list[1], &conversionOK, WidthAttribute, orientation);
            int height = attributeToInt(list[2], &conversionOK, HeightAttribute, orientation);
            const QPixmap *pixmap = MTheme::pixmap(list[0], QSize(width, height));
            cacheOrientation = (orientation == M::Portrait) ? PortraitFlag : LandscapeFlag;
            return fillProperty(property, style, cacheOrientation, qVariantFromValue(pixmap));
        }
        //no parameters
        else if (list.size() == 0) {
            //init null pixmap which is ok if someone does not want to use it
            return fillProperty(property, style, cacheOrientation, qVariantFromValue((const QPixmap *) NULL));
        }
    } else if (attributeType == types[CONST_SCALABLE_TYPE] || attributeType == types[SCALABLE_IMAGE_TILES_TYPE]) {
        //"background: image_id left right top bottom;"
        //"background: image_id;"
        //"background: "image id" left right top bottom;"
        //"background: "image id";"

        QList<QByteArray> list;
        if (value.startsWith('\"')) {
            //parse name inside quotes
            int idx = value.indexOf('\"', 1);
            if (idx != -1) {
                //get quoted image_id
                QByteArray imageid = value.mid(1, idx - 1);

                //split rest of the parameters
                QByteArray values = value.mid(idx + 1).trimmed();
                list = values.split(' ');
                list.removeAll("");
                //insert image_id as first parameter
                list.insert(0, imageid);
            }
        } else {
            //no quotes, just split the parameters
            list = value.split(' ');
        }

        //no parameters
        if (value.isEmpty() || value == "none") {
            //init null image which is ok if someone does not want to use it
            if(attributeType == types[CONST_SCALABLE_TYPE])
                return fillProperty(property, style, cacheOrientation, qVariantFromValue((const MScalableImage *) NULL));
            else
                return fillProperty(property, style, cacheOrientation, QVariant::fromValue(MBackgroundTiles()), false);
        }
        //only image_id
        else if (list.size() == 1) {
            if(attributeType == types[CONST_SCALABLE_TYPE]) {
                const MScalableImage *image = MTheme::scalableImage(list[0], 0, 0, 0, 0);
                return fillProperty(property, style, cacheOrientation, qVariantFromValue(image), false);
            } else {
                return fillProperty(property, style, cacheOrientation, QVariant::fromValue(MBackgroundTiles(list[0], 0,0,0,0)), false);
            }
        }
        //image_id + border width paramaters
        else if (list.size() == 5) {
            //image_id and the border parameters
            if(attributeType == types[CONST_SCALABLE_TYPE]) {
                const MScalableImage *image = MTheme::scalableImage(list[0],
                                                attributeToInt(list[1], &conversionOK),
                                                attributeToInt(list[2], &conversionOK),
                                                attributeToInt(list[3], &conversionOK),
                                                attributeToInt(list[4], &conversionOK));
                return fillProperty(property, style, cacheOrientation, qVariantFromValue(image), false);
            } else {
                return fillProperty(property, style, cacheOrientation, QVariant::fromValue(MBackgroundTiles(list[0],
                                                                attributeToInt(list[1], &conversionOK),
                                                                attributeToInt(list[2], &conversionOK),
                                                                attributeToInt(list[3], &conversionOK),
                                                                attributeToInt(list[4], &conversionOK))), false);
            }
        }
    } else if (attributeType == types[SIZE_TYPE] || attributeType == types[SIZEF_TYPE]) {
        //size: 25px 25px;

        //just split into pieces and create QSize or QSizeF depending on the attributeType
        QList<QByteArray> list = value.split(' ');
        list.removeAll("");
        if (list.size() == 2) {
            cacheOrientation = (orientation == M::Portrait) ? PortraitFlag : LandscapeFlag;
            if (attributeType == types[SIZE_TYPE]) {
                int width = attributeToInt(list[0], &conversionOK, WidthAttribute, orientation);
                int height = attributeToInt(list[1], &conversionOK, HeightAttribute, orientation);
                return fillProperty(property, style, cacheOrientation, QSize(width, height));
            } else {
                qreal width = attributeToFloat(list[0], &conversionOK, WidthAttribute, orientation);
                qreal height = attributeToFloat(list[1], &conversionOK, HeightAttribute, orientation);
                return fillProperty(property, style, cacheOrientation, QSizeF(width, height));
            }
        }
    } else if (attributeType == types[POINT_TYPE] || attributeType == types[POINTF_TYPE]) {
        //"point: 256px 123px;

        //just split into pieces and create QPoint or QPointF depending on the attributeType
        QList<QByteArray> list = value.split(' ');
        list.removeAll("");
        if (list.size() == 2) {
            cacheOrientation = (orientation == M::Portrait) ? PortraitFlag : LandscapeFlag;
            if (attributeType == types[POINT_TYPE]) {
                int x = attributeToInt(list[0], &conversionOK, WidthAttribute, orientation);
                int y = attributeToInt(list[1], &conversionOK, HeightAttribute, orientation);
                return fillProperty(property, style, cacheOrientation, QPoint(x, y));
            } else {
                qreal x = attributeToFloat(list[0], &conversionOK, WidthAttribute, orientation);
                qreal y = attributeToFloat(list[1], &conversionOK, HeightAttribute, orientation);
                return fillProperty(property, style, cacheOrientation, QPointF(x, y));
            }
        }
    } else if (attributeType == types[FONT_TYPE]) {
        QFont font = fontFromString(value, &conversionOK);
        if (conversionOK) {
            return fillProperty(property, style, cacheOrientation, font);
        }
    } else if (attributeType == types[STRING_TYPE]) {
        if (value.length() >= 2) {
            if ((value.at(0) == 0x22) && (value.at(value.length()-1) == 0x22)) {
                return fillProperty(property, style, cacheOrientation, QString(value.mid(1, value.length() - 2)));
            }
        } else if (value.length() == 0) {
            return fillProperty(property, style, cacheOrientation, QString());
        }
    } else if (attributeType == types[CHAR_TYPE]) {
        if (value.length() == 3) {
            if ((value[0] == '\'') && (value[2] == '\'')) {
                return fillProperty(property, style, cacheOrientation, static_cast<QChar>(value[1]));
            }
        }
    } else if (attributeType == types[ALIGNMENT_TYPE]) {
        if (DataTypeConverter.ALIGNMENTS.contains(value)) {
            return fillProperty(property, style, cacheOrientation, QVariant(DataTypeConverter.ALIGNMENTS[value]));
        }
    } else if (attributeType == types[ORIENTATION_TYPE]) {
        if (DataTypeConverter.ORIENTATIONS.contains(value)) {
            return fillProperty(property, style, cacheOrientation, QVariant(DataTypeConverter.ORIENTATIONS[value]));
        }
    } else if (attributeType == types[UNDERLINESTYLE_TYPE]) {
        if (DataTypeConverter.UNDERLINESTYLES.contains(value)) {
            return fillProperty(property, style, cacheOrientation, qVariantFromValue(DataTypeConverter.UNDERLINESTYLES[value]));
        }
    } else if (attributeType == types[PENSTYLE_TYPE]) {
        if (DataTypeConverter.PENSTYLES.contains(value)) {
            return fillProperty(property, style, cacheOrientation, qVariantFromValue(DataTypeConverter.PENSTYLES[value]));
        }
    } else if (attributeType == types[AXIS_TYPE]) {
        if (DataTypeConverter.AXES.contains(value)) {
            return fillProperty(property, style, cacheOrientation, qVariantFromValue(DataTypeConverter.AXES[value]));
        }
    } else if (attributeType == types[FEEDBACK_TYPE]) {
        MFeedback feedback(value);
        return fillProperty(property, style, cacheOrientation, qVariantFromValue(feedback));
    }
#if QT_VERSION >= 0x040600
    else if (attributeType == types[EASINGCURVE_TYPE]) {
        QEasingCurve curve;
        // curve type
        QList<QByteArray> list = value.split(',');
        if (list.size() > 0) {
            if (DataTypeConverter.EASINGCURVETYPES.contains(list[0])) {
                curve.setType(DataTypeConverter.EASINGCURVETYPES[list[0]]);
                // curve amplitude
                if (list.size() > 1) {
                    curve.setAmplitude((qreal) list[1].toDouble());
                    // curve overshoot
                    if (list.size() > 2) {
                        curve.setOvershoot((qreal) list[2].toDouble());
                        // curve period
                        if (list.size() > 3) {
                            curve.setPeriod((qreal) list[3].toDouble());
                        }
                    }
                }
                return fillProperty(property, style, cacheOrientation, qVariantFromValue(curve));
            }
        }
    }
#else
    // This attribute type is supported in Qt4.6->
    return true;
#endif

    MStyleSheetParser::outputParseError(filename, "Not a valid attribute(" + QLatin1String(attributeType) + "): " + name + ": " + value, MStyleSheetParser::getLineNum(filename, position));
    return false;
}

bool MStyleSheetAttribute::fillProperty(const QMetaProperty &property, MStyle *style, CacheOrientationFlags cacheOrientation, const QVariant &variant, bool cache)
{
    if (cache && variant.isValid()) {
        // most variants will be cahced in landscape and portrait.
        // this should not really increase memory usage as QVariants are implicitly shared
        if (cacheOrientation & PortraitFlag) {
            variantCache[M::Portrait][value][property.typeName()] = variant;
        }
        if (cacheOrientation & LandscapeFlag) {
            variantCache[M::Landscape][value][property.typeName()] = variant;
        }
    }

    return property.write(style, variant);
}
