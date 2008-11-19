#!/usr/bin/perl
use warnings;
use strict;
use Number::Fraction;

# Problem:
#   "Here's how Monty's deal works, in the math problem, anyway. (On
#   the real show it was a bit messier.) He shows you three closed
#   doors, with a car behind one and a goat behind each of the
#   others. If you open the one with the car, you win it. You start by
#   picking a door, but before it's opened Monty will always open
#   another door to reveal a goat. Then he'll let you open either
#   remaining door.
#
#   Suppose you start by picking Door 1, and Monty opens Door 3 to
#   reveal a goat. Now what should you do? Stick with Door 1 or switch
#   to Door 2?"
#
# Description taken from:
#   http://www.nytimes.com/2008/04/08/science/08tier.html
# See also:
#   http://query.nytimes.com/gst/fullpage.html?res=9D0CEFDD1E3FF932A15754C0A967958260&sec=&spon=&pagewanted=all

my @DOORS = 1..3;
my $PICKED_DOOR = 1;   # BSOMP =) (English: WLOG we can assume)

# initialize the possible cases: it could be any door, p=1/3
my @cases = map +{ p => Number::Fraction->new(1, scalar @DOORS),
		   firstPickedDoor => $PICKED_DOOR, actualDoor => $_ }, @DOORS;

# in each case, open a door with nothing/goat behind it
sub pickAnotherDoor ( @ ) {
  my @doors = @_;
  my $d;
  my @left = grep +($d = $_, !grep $d == $_, @doors), @DOORS;
}

sub openDoor ( $ ) {
  my ($case) = @_;
  # choose a door that isn't the picked one and doesn't have the prize
  my @left = pickAnotherDoor $case->{firstPickedDoor}, $case->{actualDoor};
  my @split = map +{ %$case, openedDoor => $_ }, @left;
  $_->{p} /= @split foreach @split;
  @split;
}

@cases = map openDoor $_, @cases;

# determine which door we can switch *to*
$_->{switchWouldBeDoor} = (pickAnotherDoor($_->{firstPickedDoor},
					   $_->{openedDoor}))[0]
  foreach @cases;

# gather results
my $totalP = 0;
$totalP += $_->{p} foreach @cases;
print "Total probability from all cases = $totalP\n";

my $stayP = 0;
($_->{actualDoor} == $_->{firstPickedDoor}) and $stayP += $_->{p}
  foreach @cases;
print "Probability of win if staying    = $stayP (@{[$stayP->to_num]})\n";

my $switchP = 0;
($_->{actualDoor} == $_->{switchWouldBeDoor}) and $switchP += $_->{p}
  foreach @cases;
print "Probability of win if switching  = $switchP (@{[$switchP->to_num]})\n";

my $ratio = $switchP / $stayP;
print "Probability ratio                = $ratio (@{[$ratio->to_num]})\n";

#use Data::Dumper; print Dumper \@cases;
