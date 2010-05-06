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

const QString THEME_1_LIBM_CSS = \
                                   "MWidgetStyle {\n"\
                                   "    show-animation: \"none\";\n"\
                                   "    hide-animation: \"none\";\n"\
                                   "    margin-left:   0;\n"\
                                   "    margin-top:    0;\n"\
                                   "    margin-right:  0;\n"\
                                   "    margin-bottom: 0;\n"\
                                   "    padding-left:   0;\n"\
                                   "    padding-top:    0;\n"\
                                   "    padding-right:  0;\n"\
                                   "    padding-bottom: 0;\n"\
                                   "    reactive-margin-left:   0;\n"\
                                   "    reactive-margin-top:    0;\n"\
                                   "    reactive-margin-right:  0;\n"\
                                   "    reactive-margin-bottom: 0;\n"\
                                   "    background-image:;\n"\
                                   "    background-color: #ffffff;\n"\
                                   "    background-boxed-percent: 0;\n"\
                                   "    background-opacity: 1.0;\n"\
                                   "    preferred-size: -1 -1;\n"\
                                   "    minimum-size: -1 -1;\n"\
                                   "    maximum-size: -1 -1;\n"\
                                   "    press-feedback:;\n"\
                                   "    release-feedback:;\n"\
                                   "}\n";

const QString THEME_2_LIBM_CSS = \
                                   "MWidgetStyle {\n"\
                                   "    backgroundColor: #000000;\n"\
                                   "}\n";

const QString THEME_2_APP_CSS = \
                                "MWidgetStyle {\n"\
                                "    backgroundColor: #f0f0f0;\n"\
                                "}\n";

const QString LIBM_THEME_1_VIEW_CONFIGURATION = \
        "[MWidgetController]\n"\
        "default = MWidgetView\n";

const QString LIBM_THEME_2_VIEW_CONFIGURATION = \
        "[MWidgetController]\n"\
        "test = TestView2\n";

const QString TESTAPP_VIEW_CONFIGURATION = \
        "[MWidgetController]\n"\
        "default = TestView\n";

const QString THEME_1_CONSTANTS_INI = \
                                      "[Palette]\n"\
                                      "COLOR_FOREGROUND            = #000000\n"\
                                      "COLOR_SECONDARY_FOREGROUND  = #666666\n"\
                                      "COLOR_BACKGROUND            = #000000\n"\
                                      "COLOR_INVERTED_FOREGROUND           = #ffffff\n"\
                                      "COLOR_INVERTED_SECONDARY_FOREGROUND = #cccccc\n"\
                                      "COLOR_INVERTED_BACKGROUND           = #FFFFFF\n"\
                                      "COLOR_SELECT = #f5bf00\n"\
                                      "COLOR_WARNING       = #CC0000\n"\
                                      "COLOR_ATTENTION     = #CC9900\n"\
                                      "COLOR_NOTIFICATION  = #C3F500\n"\
                                      "COLOR_LINK          = #3465a4\n"\
                                      "COLOR_LINK_ACTIVE   = #f5bf00\n"\
                                      "COLOR_ACCENT1 = #aad400\n"\
                                      "COLOR_ACCENT2 = #69bfde\n"\
                                      "COLOR_ACCENT3 = #ff9955\n"\
                                      "COLOR_ACCENT4 = #de87cd\n"\
                                      "COLOR_ACCENT5 = #d8b427\n"\
                                      "[Fonts]\n"\
                                      "FONT_XLARGE     = Arial 32px\n"\
                                      "FONT_LARGE      = Arial 28px\n"\
                                      "FONT_DEFAULT    = Arial 24px\n"\
                                      "FONT_SMALL      = Arial 20px\n"\
                                      "FONT_XSMALL     = Arial 16px\n";


const QString THEME_2_CONSTANTS_INI = \
                                      "[Palette]\n"\
                                      "COLOR_FOREGROUND = #ffffff\n"\
                                      "COLOR_INVERTED_SECONDARY_FOREGROUND = #dddddd\n"\
                                      "[Fonts]\n"\
                                      "FONT_DEFAULT = Courier 24px\n";

const QString FINNISH_CONSTANTS_INI = \
                                      "[Fonts]\n"\
                                      "FONT_DEFAULT = System 24px\n";
