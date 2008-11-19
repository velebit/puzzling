#!/usr/bin/perl
# solves the "tower door" puzzle in KoL (www.kingdomofloathing.com)

use warnings;
use strict;

#### Here is what the three guards claim.  Edit to match your output.

# Example: if South claims that the 1st digit is 8, we record that as
#   [ S => '1st',   8 ]
# For ranges (e.g. "definitely more than 1"), use array refs:
#   [ E => '2nd',   [ 2 .. 9 ] ]
# 
# There are 4 possible claim types: 'tired' (the guard who spoke first is or
# isn't tired of people asking for the door combination), '1st' (the 1st digit
# of the combo), '2nd' (the 2nd digit) and '3rd' (the 3rd digit).
#
# The order of claims must match what KoL gave you, of course!

our @claims = (
    [ S => 'tired', 'yes' ],
    [ N => 'tired', 'no' ],
    [ E => 'tired', 'yes' ],
    [ S => '1st',   8 ],
    [ E => '1st',   5 ],
    [ N => '1st',   3 ],
    [ S => '2nd',   7 ],
    [ N => '2nd',   2 ],
    [ E => '2nd',   [ 2 .. 9 ] ],
    [ N => '2nd',   4 ],
    [ N => '3rd',   9 ],
    [ S => '3rd',   0 ],
    [ E => '3rd',   9 ],    
);

#### Solving the puzzle

# The code that follows needs the last field to always be an array ref.  But
# for simplicity, above we allow scalars in place of one-element arrays.
ref( $_->[2] ) or $_->[2] = [ $_->[2] ] for @claims;

#use Data::Dumper; print Dumper(\@claims);

## Collect what claims were made by each guard

our %claimsBy;
push @{ $claimsBy{ $_->[0] } }, $_ for @claims;

#use Data::Dumper; print Dumper(\%claimsBy);

## Build a list of possibilities for each claim type

# Temporarily, build $possible{TYPE}{VAL} = <number of times claimed> 
our %possible;
for my $c (@claims) {
    ++$possible{ $c->[1] }{$_} for @{ $c->[2] };
}
# Now convert to a list: $possible{TYPE} = [ <VAL1>, ... ]
# (The sort is here only to assist human-readable debugging.)
$possible{$_} = [ sort keys %{ $possible{$_} } ] for keys %possible;

#use Data::Dumper; print Dumper(\%possible);


## Build a brute-force list of all potentially possible combinations of values

# Each list element will be { tired => ?, 1st => ?, 2nd => ?, 3rd => ? }, with
# some set of possible values inserted in place of the question marks.
our @combinations = ( {} );
for my $k ( keys %possible ) {
    my @new;
    for my $p (@combinations) {
        push @new, map +{ %$p, $k => $_ }, @{ $possible{$k} };
    }
    @combinations = @new;
}

#use Data::Dumper; print Dumper(\@combinations);

## Helper: given a hypothetical combination, would a claim be true or false?

# Given a claim, and a combination (which we assume is actually "the truth"
# for the time being), would the claim be true or false?
# Returns 't' for true, 'f' for false.
sub claim_is_true ( $$ ) {
    my ( $claim, $comb ) = @_;
    my $true = grep $comb->{ $claim->[1] } eq $_, @{ $claim->[2] };
    $true ? 't' : 'f';
}

## Canonical names for telling the truth, lying, and switching every time

use constant TRUTH => 'telling the truth';
use constant LYING => 'lying';
use constant FLIP =>  'switching';

## Evaluate all combinations and see what fits

# Check each combination to see if it works.
COMB:
for my $comb (@combinations) {
    my %type;

    # For each person, check all of their claims (in order).
  WHO:
    for my $who ( keys %claimsBy ) {
        # Create a string consisting of 't' and 'f' characters, depending on
        # whether an individual claim is true or false.
        my $tf = join '', map claim_is_true( $_, $comb ), @{ $claimsBy{$who} };
        $tf or die "T/F string is empty!?!";

        # A truth-teller's string will be all 't's, and a liar's string will
        # be all 'f's.  A flipper will have either 'tftf...' or 'ftft...'.
        $tf =~ /^t+$/          and push(@{$type{+TRUTH}}, $who), next WHO; 
        $tf =~ /^f+$/          and push(@{$type{+LYING}}, $who), next WHO; 
        $tf =~ /^t?(?:ft)*f?$/ and push(@{$type{+FLIP}},  $who), next WHO;

        # Oops, irregular string; this combination isn't consistent.
        next COMB;
    }

    # If we're here, it means that for this combination, each person was found
    # to be either a truth-teller or a liar or a flipper.  In general, we
    # should also check that there was exactly one of each, but that hasn't
    # proven necessary in the past.

    ## Print the results for this (valid) combination.

    # state of each person
    print join(', ', map "$_: @{$type{$_}}", TRUTH, LYING, FLIP), "\n";
    # actual value for each digit etc 
    print join(', ', map "$_: $comb->{$_}", sort keys %$comb), "\n";

    print "\n";
}

