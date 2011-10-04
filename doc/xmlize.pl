#!/usr/bin/perl
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

sub xml($) {
	$buffer = $_[0];
	$buffer =~ s/^File: (.*) \(row: (\d+)\) (.*)/\t\<Message file=\"$1\" row=\"$2\"\>\n\t\t$3\n\t\<\/Message\>\n/;
	return $buffer;
}

open(FIN,"doxygen.log");
open(FOUT,">doxygen.log.xml");

print FOUT "<Doxygen>\n";

$buffer = "";
while (<FIN>) {
	chop;
	$row = $_;

	$row =~ s/&/&#38;#38;/g;
	$row =~ s/</&#38;#60;/g;
	$row =~ s/>/&#x003E;/g;

	if ( $row =~ /^File: / ) {
		if (length($buffer)>0) {
			print FOUT xml($buffer);
			$buffer = "";
		} 
		$buffer .= $row;
	} else {
		$buffer .= $row;
	}

}
print FOUT "</Doxygen>\n";

close(FIN);
close(FOUT);



