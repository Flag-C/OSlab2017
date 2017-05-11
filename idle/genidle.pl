#!/usr/bin/perl

open(SIG, $ARGV[0]) || die "open $ARGV[0]: $!";

$n = sysread(SIG, $buf, 10*1024);

if($n > 10 * 1024){
	  print STDERR "ERROR:Kernel binary file too large:\
	        $n bytes (max 10240)\n";
			  exit 1;
		  }

		  print STDERR "Kernel binary file is $n bytes (max 10240)\n";

		  $m = $n % (512);
		  $s = 512 - $m;
		  $p = 10240 - $n;

		  print STDERR "Idle binary file size n = $n\n";
		  print STDERR "Idle binary file sectors num m = $m\n";
		  print STDERR "Idle binary file added $p bytes s = $s\n";

		  $buf .= "\0" x $p;


		  open(SIG, ">$ARGV[0]") || die "open >$ARGV[0]: $!";
		  print SIG $buf;
		  close SIG;


		  open(SIG, $ARGV[0]) || die "open $ARGV[0]: $!";

		  $n = sysread(SIG, $buf, 100*1024);

		  if($n > 10 * 1024){
			    print STDERR "ERROR:Idle binary file large: $n bytes (max 102400)\n";
				  exit 1;
			  }

			  print STDERR "Idle binary file is $n bytes (max 10240)\n";
			  print STDERR "*********************************************\n";
			  print STDERR "*********************************************\n";
			  print STDERR "*********************************************\n";
			  close SIG;
