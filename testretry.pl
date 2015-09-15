
# Just return 0 or 1 depending on time
if (scalar @ARGV>0) {
	print join(' ',@ARGV)."\n";
	print STDERR "STDERR: ".join(' ',@ARGV)."\n";
}
exit (time()&1) ;
