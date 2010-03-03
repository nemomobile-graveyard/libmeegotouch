#!/bin/env perl

use Cwd;
use File::Basename;
use File::Path;

$inpath = Cwd::abs_path(dirname($0));
$outpath = Cwd::getcwd;

print "inpath : $inpath\n";
print "outpath: $outpath\n";

open( CACHE, ">$outpath/.qmake.cache" );
print CACHE "DUI_BUILD_TREE = \$\$quote($outpath)\n";
close( CACHE );

$specpath = "$outpath/mkspecs/features";

if ( ! -d "$specpath" )
{
    mkpath( "$specpath" )
}

open( INF, "<$inpath/mkspecs/features/dui_defines.prf.in" );
open( OUTF, ">$specpath/dui_defines.prf" );

while ( <INF> )
{
    chomp;
    my $line = $_;

    # do variable substitution here if needed

    print OUTF "$line\n";
}

close( INF );
close( OUTF );
