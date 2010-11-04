#!/bin/env perl

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
