$prog="./retry" ;

sub runtest {
	my $testname=shift @_;
	my $expectrt=shift @_;
	my $cmd=$prog." ".join(' ',@_);
	my $out=`$cmd 2>&1`;
	my $rt=${^CHILD_ERROR_NATIVE};
	if ($rt!=$expectrt) {
		die "$testname failed with ".$rt.": $!";		
	}
	return $out;
}

my $d=`dirname $0`;
chomp $d;
chdir("$d");

runtest("Failed args",256,"1","0");
runtest("Basic test always fail",256,"1","0","/bin/false");
runtest("Basic test always success",0,"1","0","/bin/true");
runtest("Basic test sometimes fail",0,"5","1","perl","./testretry.pl");
my $output=runtest("Some output",0,"5","1","perl","./testretry.pl","HUUHAA");
sleep(1);
$output.=runtest("Some output",0,"5","1","perl","./testretry.pl","HUUHAA");
if ("$output" ne "HUUHAA\nSTDERR: HUUHAA\nHUUHAA\nSTDERR: HUUHAA\n") {
	die("Output test failed with $output\n");
}

print "$0: All tests passed ok\n";

