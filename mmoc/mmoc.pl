#! /usr/bin/perl

use English;

# common regular expressions
# these have to be before exift command
# (otherwise they will not get any value)
$::spaces = "\\s*";
$::comma = ",";
$::colon = ":";
$::parenO = "\\(";
$::parenC = "\\)";
$::emptyParen = "\\(\\)";
$::angleO = "<";
$::angleC = ">";
$::braceO = "\\{";
$::nameSpace = "(?:\\w+::)";
$::typeName = "\\w+";
$::pointer = "\\*";                                                                                                            
$::templateName = "\\w+";
$::plainParam = "(\\w+)";
$::boolParam = "(true|false)";
$::anyParam = "(.+)";

$::QT_MOC_PATH = find_moc ();

if (! -x $::QT_MOC_PATH) {
        print "Unable to find moc, or is not executable\n";
        if ( "MSWin32" ne "$OSNAME" ) {
                exit (1);
        }
}

chomp( $::QT_MOC_PATH );

exit main( @ARGV );

sub find_moc
{
    my $mocpath;

    if ($ENV{"QTDIR"} && -x "$ENV{\"QTDIR\"}/bin/moc")
    {
        return "$ENV{\"QTDIR\"}/bin/moc";
    }

    # windows case
    if ($ENV{"QTDIR"} && -x "$ENV{\"QTDIR\"}/bin/moc.exe")
    {
        return "$ENV{\"QTDIR\"}/bin/moc.exe";
    }

    # unix case
    if ( "MSWin32" ne "$OSNAME" )
    {

        $mocpath = `which moc 2>/dev/null`;
        if ($? == 0) {
            chomp $mocpath;
            return $mocpath;
        }

        $mocpath = `which moc-qt4 2>/dev/null`;
        if ($? == 0) {
            chomp $mocpath;
            return $mocpath;
        }
    } else {
        return "moc";
    }
}

sub main
{
    my @argv = @_;

    my @commandLineParameters = ( $::QT_MOC_PATH );

    my $filename = "";
    my $type = "";

    for ( my $i=0; $i<@argv; ++$i ) {
        if ( $argv[$i] =~ /style.h$/ ) {
            $type = "Style";
            $filename = $argv[$i];
        } elsif ( $argv[$i] =~ /model.h$/ ) {
            $type = "Model";
            $filename = $argv[$i];
        } else {
            push @commandLineParameters, $argv[$i];
        }
    }

    if ( $filename eq "" ) {
        system( @commandLineParameters );
    } else {
        push @commandLineParameters, "-f".$filename;

        if ( $type eq "Model" ) {
            runModelMoc( $filename, @commandLineParameters );
        } elsif ( $type eq "Style" ) {
            runStyleMoc( $filename, @commandLineParameters );
        }
    }

    return 0;
}

sub runStyleMoc
{
    my ($header, @arguments) = @_;

    my $commandLine = join( " ", @arguments );

    open( INF, "<$header" ) || die( "Could not open $header for reading : $!" );

    open( MOC, "|$commandLine" ) || die( "Could not run command $commandLine : $!" );

    while ( <INF> ) {
        chomp;
        my $line = $_;

        $line =~ s/\s*M_STYLE_ATTRIBUTE\s*\(\s*(\w+\:*\w*)\s*,\s*(\w+)\s*,\s*(\w+)\s*\)\s*/    Q_PROPERTY($1 $2 READ $2 WRITE set$3)/;
        $line =~ s/\s*M_STYLE_PTR_ATTRIBUTE\s*\(\s*(\w+\:*\w*\s*\*+)\s*,\s*(\w+)\s*,\s*(\w+)\s*\)\s*/    Q_PROPERTY(const $1 $2 READ $2 WRITE set$3)/;

        print MOC "$line\n";
    }

    close( MOC );

    close( INF );
}

sub runModelMoc
{
    my ($header, @arguments) = @_;

    my $commandLine = join( " ", @arguments );

    my @pattern = (
        $::spaces.
        "M_MODEL_PROPERTY".
        $::spaces.$::parenO.$::spaces.
            "(".
                "(?:".
                    $::nameSpace."?".
                    $::typeName.
                    $::spaces.
                    $::pointer."?".
                ")".
                "|".
                "(?:".
                    $::templateName.
                    $::angleO.
                        $::spaces.
                        $::typeName.
                        $::spaces.
                        $::pointer."?".
                        $::spaces.
                    $::angleC.
                ")".
            ")".
                $::spaces.$::comma.$::spaces.
            $::plainParam.
                $::spaces.$::comma.$::spaces.
            $::plainParam.
                $::spaces.$::comma.$::spaces.
            $::plainParam.
                $::spaces.$::comma.$::spaces.
            $::anyParam.
        $::spaces.$::parenC.$::spaces,

        $::spaces.
        "M_MODEL_PTR_PROPERTY".
        $::spaces.$::parenO.$::spaces.
            "(".
                "(?:".
                    $::nameSpace."?".
                    $::typeName.
                    $::spaces.
                    $::pointer."?".
                    $::spaces.
                ")".
                "|".
                "(?:".
                    $::templateName.
                    $::angleO.
                        $::spaces.
                        $::typeName.
                        $::spaces.
                        $::pointer."?".
                        $::spaces.
                    $::angleC.
                ")".
            ")".
                $::spaces.$::comma.$::spaces.
            $::plainParam.
                $::spaces.$::comma.$::spaces.
            $::plainParam.
                $::spaces.$::comma.$::spaces.
            $::boolParam.
                $::spaces.$::comma.$::spaces.
            $::anyParam.
        $::spaces.$::parenC.$::spaces,

    );

    open( INF, "<$header" ) || die( "Could not open header file for reading : $!" );

    open( MOC, "|$commandLine" ) || die( "Could not run command $commandLine : $!" );

    while ( <INF> ) {
        chomp;
        my $line = $_;

        $line =~ s/$pattern[0]/    Q_PROPERTY($1 $2 READ $2 WRITE set$3)/;
        $line =~ s/$pattern[1]/    Q_PROPERTY($1 $2 READ $2 WRITE set$3)/;

        print MOC "$line\n";
    }

    close( MOC );

    close( INF );
}
