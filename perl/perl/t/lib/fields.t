#!./perl -w

my $w;

BEGIN {
   chdir 't' if -d 't';
   unshift @INC, '../lib' if -d '../lib';
   $SIG{__WARN__} = sub {
       if ($_[0] =~ /^Hides field 'b1' in base class/) {
           $w++;
           return;
       }
       print $_[0];
   };
}

use strict;
use warnings;
use vars qw($DEBUG);

package B1;
use fields qw(b1 b2 b3);

package B2;
use fields '_b1';
use fields qw(b1 _b2 b2);

sub new { bless [], shift }

package D1;
use base 'B1';
use fields qw(d1 d2 d3);

package D2;
use base 'B1';
use fields qw(_d1 _d2);
use fields qw(d1 d2);

package D3;
use base 'B2';
use fields qw(b1 d1 _b1 _d1);  # hide b1

package D4;
use base 'D3';
use fields qw(_d3 d3);

package M;
sub m {}

package D5;
use base qw(M B2);

package Foo::Bar;
use base 'B1';

package Foo::Bar::Baz;
use base 'Foo::Bar';
use fields qw(foo bar baz);

# Test repeatability for when modules get reloaded.
package B1;
use fields qw(b1 b2 b3);

package D3;
use base 'B2';
use fields qw(b1 d1 _b1 _d1);  # hide b1

package main;

sub fstr {
   my $h = shift;
   my @tmp;
   for my $k (sort {$h->{$a} <=> $h->{$b}} keys %$h) {
	my $v = $h->{$k};
        push(@tmp, "$k:$v");
   }
   my $str = join(",", @tmp);
   print "$h => $str\n" if $DEBUG;
   $str;
}

my %expect = (
    B1 => "b1:1,b2:2,b3:3",
    B2 => "_b1:1,b1:2,_b2:3,b2:4",
    D1 => "b1:1,b2:2,b3:3,d1:4,d2:5,d3:6",
    D2 => "b1:1,b2:2,b3:3,_d1:4,_d2:5,d1:6,d2:7",
    D3 => "b2:4,b1:5,d1:6,_b1:7,_d1:8",
    D4 => "b2:4,b1:5,d1:6,_d3:9,d3:10",
    D5 => "b1:2,b2:4",
    'Foo::Bar::Baz' => 'b1:1,b2:2,b3:3,foo:4,bar:5,baz:6',
);

print "1..", int(keys %expect)+13, "\n";
my $testno = 0;
while (my($class, $exp) = each %expect) {
   no strict 'refs';
   my $fstr = fstr(\%{$class."::FIELDS"});
   print "EXP: $exp\nGOT: $fstr\nnot " unless $fstr eq $exp;
   print "ok ", ++$testno, "\n";
}

# Did we get the appropriate amount of warnings?
print "not " unless $w == 1;
print "ok ", ++$testno, "\n";

# A simple object creation and AVHV attribute access test
my B2 $obj1 = D3->new;
$obj1->{b1} = "B2";
my D3 $obj2 = $obj1;
$obj2->{b1} = "D3";

print "not " unless $obj1->[2] eq "B2" && $obj1->[5] eq "D3";
print "ok ", ++$testno, "\n";

# We should get compile time failures field name typos
eval q(my D3 $obj3 = $obj2; $obj3->{notthere} = "");
print "not " unless $@ && $@ =~ /^No such pseudo-hash field "notthere"/;
print "ok ", ++$testno, "\n";

# Slices
@$obj1{"_b1", "b1"} = (17, 29);
print "not " unless "@$obj1[1,2]" eq "17 29";
print "ok ", ++$testno, "\n";
@$obj1[1,2] = (44,28);
print "not " unless "@$obj1{'b1','_b1','b1'}" eq "28 44 28";
print "ok ", ++$testno, "\n";

my $ph = fields::phash(a => 1, b => 2, c => 3);
print "not " unless fstr($ph) eq 'a:1,b:2,c:3';
print "ok ", ++$testno, "\n";

$ph = fields::phash([qw/a b c/], [1, 2, 3]);
print "not " unless fstr($ph) eq 'a:1,b:2,c:3';
print "ok ", ++$testno, "\n";

$ph = fields::phash([qw/a b c/], [1]);
print "not " if exists $ph->{b} or exists $ph->{c} or !exists $ph->{a};
print "ok ", ++$testno, "\n";

eval '$ph = fields::phash("odd")';
print "not " unless $@ && $@ =~ /^Odd number of/;
print "ok ", ++$testno, "\n";

#fields::_dump();

# check if fields autovivify
{
    package Foo;
    use fields qw(foo bar);
    sub new { bless [], $_[0]; }

    package main;
    my Foo $a = Foo->new();
    $a->{foo} = ['a', 'ok ' . ++$testno, 'c'];
    $a->{bar} = { A => 'ok ' . ++$testno };
    print $a->{foo}[1], "\n";
    print $a->{bar}->{A}, "\n";
}

# check if fields autovivify
{
    package Bar;
    use fields qw(foo bar);
    sub new { return fields::new($_[0]) }

    package main;
    my Bar $a = Bar::->new();
    $a->{foo} = ['a', 'ok ' . ++$testno, 'c'];
    $a->{bar} = { A => 'ok ' . ++$testno };
    print $a->{foo}[1], "\n";
    print $a->{bar}->{A}, "\n";
}
