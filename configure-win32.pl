#!/bin/env perl
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

use Cwd;
use File::Basename;
use File::Path;

$inpath = Cwd::abs_path(dirname($0));
$outpath = Cwd::getcwd;

#makes sure the windows path are set correctly and compliant with windows standard
$inpath  =~ s/^\/([a-z])/$1:/i;
$outpath =~ s/^\/([a-z])/$1:/i;

print "inpath : $inpath\n";
print "outpath: $outpath\n";

open( CACHE, ">$outpath/.qmake.cache" );
print CACHE "M_BUILD_TREE = \$\$quote($outpath)\n";
print CACHE "M_SOURCE_TREE = \$\$quote($inpath)\n";
close( CACHE );

$specpath = "$outpath/mkspecs/features";

if ( ! -d "$specpath" )
{
    mkpath( "$specpath" )
}

open( INF, "<$inpath/mkspecs/features/meegotouch_defines.prf.in" );
open( OUTF, ">$specpath/meegotouch_defines.prf" );

while ( <INF> )
{
    chomp;
    my $line = $_;

    # do variable substitution here if needed

    print OUTF "$line\n";
}

close( INF );
close( OUTF );
