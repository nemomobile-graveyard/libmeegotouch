#/***************************************************************************
#**
#** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
#** All rights reserved.
#** Contact: Nokia Corporation (directui@nokia.com)
#**
#** This file is part of libmeegotouch.
#**
#** If you have questions regarding the use of this file, please contact
#** Nokia at directui@nokia.com.
#**
#** This library is free software; you can redistribute it and/or
#** modify it under the terms of the GNU Lesser General Public
#** License version 2.1 as published by the Free Software Foundation
#** and appearing in the file LICENSE.LGPL included in the packaging
#** of this file.
#**
#****************************************************************************/
#!/bin/bash

XSLT_TOOL=

which xsltproc > /dev/null
if [ "$?" -ne "0" ] ; then
  which xmlstarlet > /dev/null
  if [ "$?" -ne "0" ] ; then
    echo "No XSLT tool found. Try installing either 'xsltproc' or 'xmlstarlet'"
    # Let's return 0 for now although this is an error case so "make" doesn't stop here
    exit 0
  else
    XSLT_TOOL=xmlstarlet
  fi
else
  XSLT_TOOL=xsltproc
fi

# Change to the directory of the script
cd `dirname "$0"`

OUTPUT=./html/settingslanguageschema.html
if [ "$1" != "" ]; then
  OUTPUT="$1/$OUTPUT"
fi

XSL=./rng-to-html.xsl
RELAXNG=../tools/settingslanguage/settings.rng

if [ "$XSLT_TOOL" = "xsltproc" ] ; then
  xsltproc --output $OUTPUT $XSL $RELAXNG
fi

if [ "$XSLT_TOOL" = "xmlstarlet" ] ; then
  xmlstarlet tr $XSL $RELAXNG > $OUTPUT
fi

# Return to the previous directory
cd - >/dev/null

exit 0
