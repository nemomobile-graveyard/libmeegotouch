#!/usr/bin/perl
# This script is useful to convert from TWIKI page into Doxygen page
# just save your raw wiki text into a txt file and
# do something like: cat twiki.txt > page.dox

while (<STDIN>)
{
    $_ =~ s/<verbatim>/\\code/g;
    $_ =~ s/<\/verbatim>/\\endcode/g;
    $_ =~ s/^---\+ <!--(.*)--> (.*)/\/*! \\page \1 \2/g;
    $_ =~ s/^---\+\+ <!--(.*)--> (.*)/\\section \1 \2/g;
    $_ =~ s/^---\+\+\+ <!--(.*)--> (.*)/\\subsection \1 \2/g;
    $_ =~ s/^---\+\+\+\+ <!--(.*)--> (.*)/\\subsubsection \1 \2/g;
    $_ =~ s/^---\+\+\+\+\+ (.*)/\\b \1 \\par/g;

    $_ =~ s/^---\+ (.*)/\/*! \\page \1 \1/g;
    $_ =~ s/^---\+\+ (.*)/\\section \1 \1/g;
    $_ =~ s/^---\+\+\+ (.*)/\\subsection \1 \1/g;
    $_ =~ s/^---\+\+\+\+ (.*)/\\subsubsection \1 \1/g;

    $_ =~ s/=([a-zA-Z0-9_:\(\)\/-]+)=/\\c \1 /g; # fixed width font
    $_ =~ s/`([a-zA-Z0-9_:\(\)-]+)`/\\c \1 /g;
    $_ =~ s/%ATTACHURL%\/(.*).png/\\image html $1.png "" /g;
    $_ =~ s/   \* /- /g;
    $_ =~ s/(?<!\w)!(?=\w)//g;
    $_ =~ s/\[\[(.+)\]\[(.+)\]\]/<a href="\1">\2<\/a>/g; # html links
    print $_;
}
print "*/\n";
