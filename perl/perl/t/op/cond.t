#!./perl

# $RCSfile: cond.t,v $$Revision: 1.1.1.2 $$Date: 2000/03/31 05:12:39 $

print "1..4\n";

print 1 ? "ok 1\n" : "not ok 1\n";	# compile time
print 0 ? "not ok 2\n" : "ok 2\n";

$x = 1;
print $x ? "ok 3\n" : "not ok 3\n";	# run time
print !$x ? "not ok 4\n" : "ok 4\n";
