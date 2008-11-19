#!/usr/bin/perl
use warnings;
use strict;

# Problem: On a standard digital clock display, what is the minimum contiguous
#   area you have to see in order to uniquely determine the digit?

### data for displaying segments

my $DISP_TMPL = <<'EndOfDisp';
 aaaa |
b    c|
b    c|
 dddd |
e    f|
e    f|
 gggg |
EndOfDisp
$DISP_TMPL =~ s/\|//g;

my $DISP_CHAR = '#';

my @digits_ltr = ( 'abcefg', 'cf', 'acdeg', 'acdfg', 'bcdf',
                   'abdfg', 'abdefg', 'acf', 'abcdefg', 'abcdfg' );

### functions using the "a-g map" format for selecting segments

sub ltr2map ( $ ) {
    my ($letters) = @_;
    +{ map +( $_ => $DISP_CHAR ), split //, $letters };
}

sub map2disp ( $ ) {
    my ($map) = @_;
    my $text = $DISP_TMPL;
    $text =~ s/([a-g])/exists $map->{$1} ? $map->{$1} : ' '/ge;
    $text;
}

### functions using the bitmask format for selecting segments

sub map2bits ( $ ) {
    my ($map) = @_;
    my $bits = 0;
    $bits |= (1 << (ord($_)-ord('a'))) for keys %$map;
    $bits;
}

sub bits2map ( $ ) {
    my ($bits) = @_;
    my $map = {};
    ($bits & (1 << $_)) and $map->{ chr($_+ord('a')) } = $DISP_CHAR for 0..7;
    $map;
}

### various useful combinations

sub ltr2disp ( $ ) {
    my ($letters) = @_;
    map2disp ltr2map $letters;
}

sub bits2disp ( $ ) {
    my ($bits) = @_;
    map2disp bits2map $bits;
}

### bitmasks for selecting segments

my @digits_bits = map map2bits(ltr2map $_), @digits_ltr;

### helper functions for identifying minimal masks

sub are_unique ( @ ) {
    my (@data) = @_;
    my %tmp;
    ($tmp{$_} and return 0), $tmp{$_} = 1 for @data;
    1;
}

sub mask_num_bits ( $ ) {
    my ($mask) = @_;
    scalar grep $_ eq '1', split //, sprintf "%b", $mask;
}

sub mask_contains ( $$ ) {
    my ($mask1, $mask2) = @_;
    (($mask1 & $mask2) == $mask2) && ($mask1 != $mask2);
}

sub mask_contains_one_of ( $@ ) {
    my ($mask1, @masks) = @_;
    scalar grep mask_contains($mask1, $_), @masks;
}

sub mask_is_sufficient ( $ ) {
    my ($mask) = @_;
    are_unique map $mask & $_, @digits_bits;
}

sub what_collides ( $ ) {
    my ($mask) = @_;
    my %masked2orig;
    push @{ $masked2orig{$_ & $mask} }, $_ for @digits_bits;
    sort { @$b <=> @$a } grep @$_ > 1, values %masked2orig;
}

sub assert_equal ( $$$ ) {
    my ($v1, $v2, $text) = @_;
    warn "$text: $v1 vs $v2\n" unless $v1 == $v2;
}

### display helpers

sub glue ( @ ) {
    my (@elts) = @_;
    my @splat = map [ split /\n/, $_ ], @elts;
    my @lines;
    for my $r (0..$#{$splat[0]}) {
        $lines[$r] = join " ", map $_->[$r], @splat;
    }
    join '', map "$_\n", @lines;
}

my $BLANK = $DISP_TMPL;
$BLANK =~ s/\w/ /g;

my $ARROW = $DISP_TMPL;
$ARROW =~ s/(?<=d) />/;
$ARROW =~ s/d/-/g;
$ARROW =~ s/\w/ /g;

### tests

#print glue(map ltr2disp($_), @digits_ltr);
#printf "%8b\n", ltr2bits($_) for @digits_ltr;
#printf "%8b\n", $_ for @digits_bits;
#print glue(map bits2disp($_), @digits_bits);
#print are_unique(@digits_ltr), "\n";
#print are_unique(@digits_ltr, "cf"), "\n";
#print are_unique(@digits_bits), "\n";
#printf "%2d\n", mask_num_bits($_) for @digits_bits;

### production!

# Find masks that work.
my @masks0 = grep mask_is_sufficient($_), 0..127;

# Eliminate any mask that fully contains another mask.
my @masks = grep !mask_contains_one_of($_, @masks0), @masks0;

printf "%07b\n", $_ for @masks;
print glue map bits2disp($_), @masks;

print "\n*** collisions ***\n";
for my $n (0..6) {
    my $m = 127 & ~(1 << $n);
    my @coll = what_collides $m;
    assert_equal(!@coll, mask_is_sufficient($m), "inconsistency for $m");
    @coll or next;
    print glue bits2disp($m), $ARROW, map bits2disp($_), @{$coll[0]};
    print "\n";
}