#!/usr/bin/perl
#

print "XB-Testing-metadata:\n";
print " <testinfo>\n";
print '   <target package="libmeegotouch" version="${binary:Version}" />' . "\n";
open(META,"meta");
$closetag = "";
$counter = 0;

my $command = 'DISPLAY=:1 LD_LIBRARY_PATH=~/nokia/dui/lib find . -type f -perm +100 -name "?t_*" -print -exec  {} -functions \;';
@buffer = qx($command 2>&1);

foreach $row (@buffer) {
	chomp $row;
	if ( $row =~ /^\./) {
		($a,$b,$c) = split(/\//,$row);
		if ($counter > 0) {
     		        printf("     <case id=\"0\" name=\"cleanupTestCase\" />\n");
			print "   </set>\n";
		} 
   		printf("   <set type=\"unit\" id=\"0\" name=\"%s\" timeout=\"900000\">\n",$b);
     		printf("     <case id=\"0\" name=\"initTestCase\" />\n");
		$counter ++;
		# print "RIVI: " . $b. "\n";
	} else {
		chop $row; chop $row;
     		printf("     <case id=\"0\" name=\"%s\" />\n",$row);
	}

}
if ($counter>0) {
	printf("     <case id=\"0\" name=\"cleanupTestCase\" />\n");
	print "   </set>\n";
}
printf(" </testinfo>\n");
