#!/bin/sh
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

set -e

cd $1

SHA1="unknown"
# check if we are working with a git checkout
if [ -e .git/logs/HEAD ]; then
  # if git is not installed extract sha1 manually
  if $(type -P git > /dev/null); then
    SHA1=$(git log --pretty=format:%H -n 1)
  else
    SHA1=$(awk 'END { print $2 }' .git/logs/HEAD)
  fi
fi

# extract debian version number
DEBIAN_VERSION="unknown"
if $(type -P dpkg-parsechangelog > /dev/null); then \
  DEBIAN_VERSION=$(dpkg-parsechangelog | grep '^Version: ' | sed 's/^Version: \(.*\)/\1/')
fi

echo "Git revision: ${SHA1} Debian Version: ${DEBIAN_VERSION}"
