#!/usr/bin/perl
use warnings;
use strict;

use lib '../lib';
use Cypher::Decode;
use Cypher::Encode;

## set up test patterns

my @tests;
for my $l ('a'..'z') {
  push @tests, "$l$_" for 1..10;
}
#@tests = qw( ? );

my %tests = map +($_ => 1), @tests;

## read names, movies and cyphers from a file

our (%movie, %name, %cypher);

open my $S, '<', 'SOLUTION-200412' or die "open(<): $!";
while (<$S>) {
  chomp;
  my ($movie, $m_name, $w_name, $cyph, $year) = split /#/;
  my $info = [ $movie, $m_name, $w_name, $cyph, $year ];
  $movie{$movie}           = $info;
  $name{men}{$m_name}      = $info;
  $name{''}{$m_name}       = $info;
  $name{women}{$w_name}    = $info;
  $name{''}{$w_name}       = $info;
  $cypher{$cyph}           = $info;
  (my $m_name_l = $m_name) =~ s/^(.*?)\s+(\S+)$/$2, $1/;
  (my $w_name_l = $w_name) =~ s/^(.*?)\s+(\S+)$/$2, $1/;
  $name{lmen}{$m_name_l}   = $info;
  $name{l}{$m_name_l}      = $info;
  $name{lwomen}{$w_name_l} = $info;
  $name{l}{$w_name_l}      = $info;
}
close $S;

## read cyphertext and check consistency

@ARGV = ('cyphertext-200412');
my $key = '';
my (%data, @data, %cat);
while (<>) {
  s/[\r\n]+//g;
  /^\s+(.+)\s*$/ and $key = lc $1, next;
  /\S/ or next;
  push @{$data{$key}}, $_;
  push @data, $_;
  $cat{$_} = $key;
}
$data{''} = \@data;

join(':',
     map(Cypher::Encode->new($Cypher::Base::atoz, $_->[3])->(lc $_->[1]),
	 @{ $name{lmen} }{ sort keys %{$name{lmen}} } ),
     map(Cypher::Encode->new($Cypher::Base::atoz, $_->[3])->(lc $_->[2]),
	 @{ $name{lwomen} }{ sort keys %{$name{lwomen}} } ))
  eq join(':', @{ $data{men} }, @{ $data{women} })
  or die "sorted cyphertexts don't match";

## quotation helpers

sub slice_disp ( @ ) {
  my (@slice) = @_;
  my $sl  = join '', @slice;
  my $ssl = join '', sort @slice;
  my %fsl;
  ++$fsl{$_} for @slice;
  my $lsl = join '', sort keys %fsl;
  my $fssl = join '', sort {$fsl{$b} <=> $fsl{$a} or $a cmp $b} @slice;
  my $flsl = join '', sort {$fsl{$b} <=> $fsl{$a} or $a cmp $b} keys %fsl;
  sprintf "%-20s (%-20s -- %4s %s)",
    $sl, $fssl, '[' . length($flsl) . ']', $flsl;
}

## prep for common cypher test code

sub uniq ( @ ) { my %seen;  map $seen{$_}++ ? () : $_, @_; }
sub letters ( $ ) { my ($l) = @_;  $l =~ s/\W+//g;  split //, $l; }

my %cyph2ml = map +( $movie{$_}[3] => [ letters uc $_ ]), keys %movie;
my %cyph2cl = map +( $movie{$_}[3] => [ uniq letters(uc $_), 'A'..'Z' ]),
  keys %movie;

my $ml_max = (sort {$a <=> $b} map $#$_, values %cyph2ml)[0];
my $cl_max = (sort {$a <=> $b} map $#$_, values %cyph2cl)[0];

## common cypher test code

# sequence generating arrays
use constant ENC_GEN => [ '', [ 'a' .. 'z' ] ];
use constant DEC_GEN => [ '', [ 'A' .. 'Z' ] ];

# cypher creation
use constant ENC_CYPH =>
  [ 'encode', sub {Cypher::Encode->new($Cypher::Base::atoz, $_[0])} ];
use constant DEC_CYPH =>
  [ 'decode', sub {Cypher::Decode->new($_[0], $Cypher::Base::atoz)} ];

# ordering of letters within each "slice"
sub BY_MOVIE () {
  [ 'sorted by movie', [ map $movie{$_}[3], sort keys %movie ] ];
}
sub BY_ACTOR () {
  [ 'sorted by actor',
    [ map $_->[3], @{ $name{lmen} }{sort keys %{ $name{lmen} }} ] ];
}
sub BY_ACTRESS () {
  [ 'sorted by actress',
    [ map $_->[3], @{ $name{lwomen} }{sort keys %{ $name{lwomen} }} ] ];
}
sub BY_YEAR () {
  [ 'sorted by year',
    [ sort {$cypher{$a}[4] <=> $cypher{$b}[4]} keys %cypher ] ];
}

# genericized test code
sub generic_test ( @ ) {
  my ($loop_gen_info, $enc_dec_info, $cyph_info) = @_;
  my $desc = $loop_gen_info->[0] || $cyph_info->[0];
  ref $desc and $desc = $desc->($cyph_info->[0]);
  printf "\n$enc_dec_info->[0] cypher search, $desc:\n";
  for my $gen (@{ $loop_gen_info->[1] }) {
    my @slice;
    for my $cpos (0 .. $#{ $cyph_info->[1] }) {
      my $cyph = $cyph_info->[1][$cpos];
      my $letter = ($loop_gen_info->[2]
		    ? $loop_gen_info->[2]->($gen, $cyph, $cpos)
		    : $gen)
	or last;
      my $C = $enc_dec_info->[1]->($cyph);
      push @slice, $C->($letter);
    }
    printf "%5s: %s\n", $gen, slice_disp @slice;
  }
}

# shorter aliases
sub enc_test ( $ ) { generic_test ENC_GEN, ENC_CYPH, @_; }
sub dec_test ( $ ) { generic_test DEC_GEN, DEC_CYPH, @_; }

## show cypher info

for my $gen ($tests{a1} ? BY_MOVIE : (), $tests{a2} ? BY_YEAR : (),
	     $tests{a3} ? BY_ACTOR : (), $tests{a4} ? BY_ACTRESS : ()) {
  printf "\ncypher keys $gen->[0]:\n";
  for my $cyph (@{ $gen->[1] }) {
    $cyph =~ s// /g;
    print "  $cyph\n";
  }
}

for my $gen ($tests{a5} ? BY_MOVIE : (), $tests{a6} ? BY_YEAR : (),
	     $tests{a7} ? BY_ACTOR : (), $tests{a8} ? BY_ACTRESS : ()) {
  printf "\nlook for 'the' in cypher keys $gen->[0]:\n";
  for my $cyph (@{ $gen->[1] }) {
    $cyph =~ tr/THEA-Z/the_/;  $cyph =~ s// /g;
    print "  $cyph\n";
  }
}

## try finding hidden quotation in encode/decode cyphers

enc_test BY_MOVIE if $tests{c1};
dec_test BY_MOVIE if $tests{c2};

## try finding hidden quotation using movie names

generic_test [ 'via movie name', [ 0..$ml_max ],
	       sub { uc $cyph2ml{$_[1]}[$_[0]]; } ],
  DEC_CYPH, BY_MOVIE if $tests{c3};
generic_test [ 'via movie name', [ 0..$ml_max ],
	       sub { lc $cyph2ml{$_[1]}[$_[0]]; } ],
  ENC_CYPH, BY_MOVIE if $tests{c4};

generic_test [ 'via "collapsed" movie name', [ 0..$cl_max ],
	       sub { uc $cyph2cl{$_[1]}[$_[0]]; } ],
  DEC_CYPH, BY_MOVIE if $tests{c5};
generic_test [ 'via "collapsed" movie name', [ 0..$cl_max ],
	       sub { lc $cyph2cl{$_[1]}[$_[0]]; } ],
  ENC_CYPH, BY_MOVIE if $tests{c6};

## try finding hidden quotation in encode/decode cyphers, by people

enc_test BY_ACTOR   if $tests{e1};
enc_test BY_ACTRESS if $tests{e2};

dec_test BY_ACTOR   if $tests{e3};
dec_test BY_ACTRESS if $tests{e4};

## try finding hidden quotation in encode/decode cyphers, by year

enc_test BY_YEAR    if $tests{f1};
dec_test BY_YEAR    if $tests{f2};

## diagonal search helpers

sub letter_add ( $$ ) {
  my ($letter, $n) = @_;
  $letter =~ tr/A-Za-z/B-ZAb-za/ for 1..$n;
  $letter =~ tr/A-Za-z/ZA-Yza-y/ for 1..-$n;
  $letter;
}

sub diagonalize ( $$ ) {
  my ($base, $slant) = @_;
  my ($label, $range, $letter_gen) = @$base;
  my @diag;
  if (! $label) {
    $diag[0] = sub { sprintf "%s (diag %+d)", $_[0], $slant; };
  } elsif (! ref $label) {
    $diag[0] = sub { sprintf "%s (diag %+d)", $label, $slant; };
  } else {
    print "RRR $label\n";
    $diag[0] = sub { sprintf "%s (diag %+d)", $label->($_[0]), $slant; };
  }
  $diag[1] = $range;
  if (! $letter_gen) {
    $diag[2] = sub { letter_add $_[0], $slant*$_[2]; };
  } else {
    $diag[2] = sub { letter_add $letter_gen->(@_), $slant*$_[2]; };
  }
  \@diag;
}

sub diag_enc_test ( $$ ) { generic_test diagonalize(ENC_GEN, $_[0]),
			     ENC_CYPH, $_[1]; }
sub diag_dec_test ( $$ ) { generic_test diagonalize(DEC_GEN, $_[0]),
			     DEC_CYPH, $_[1]; }

## diagonal tests

diag_enc_test +1, BY_MOVIE   if $tests{g1};
diag_enc_test +1, BY_ACTOR   if $tests{g2};
diag_enc_test +1, BY_ACTRESS if $tests{g3};
diag_enc_test +1, BY_YEAR    if $tests{g4};

diag_dec_test +1, BY_MOVIE   if $tests{g6};
diag_dec_test +1, BY_ACTOR   if $tests{g7};
diag_dec_test +1, BY_ACTRESS if $tests{g8};
diag_dec_test +1, BY_YEAR    if $tests{g9};

diag_enc_test +2, BY_MOVIE   if $tests{h1};
diag_enc_test +2, BY_ACTOR   if $tests{h2};
diag_enc_test +2, BY_ACTRESS if $tests{h3};
diag_enc_test +2, BY_YEAR    if $tests{h4};

diag_dec_test +2, BY_MOVIE   if $tests{h6};
diag_dec_test +2, BY_ACTOR   if $tests{h7};
diag_dec_test +2, BY_ACTRESS if $tests{h8};
diag_dec_test +2, BY_YEAR    if $tests{h9};

diag_enc_test -1, BY_MOVIE   if $tests{i1};
diag_enc_test -1, BY_ACTOR   if $tests{i2};
diag_enc_test -1, BY_ACTRESS if $tests{i3};
diag_enc_test -1, BY_YEAR    if $tests{i4};

diag_dec_test -1, BY_MOVIE   if $tests{i6};
diag_dec_test -1, BY_ACTOR   if $tests{i7};
diag_dec_test -1, BY_ACTRESS if $tests{i8};
diag_dec_test -1, BY_YEAR    if $tests{i9};

diag_enc_test -2, BY_MOVIE   if $tests{j1};
diag_enc_test -2, BY_ACTOR   if $tests{j2};
diag_enc_test -2, BY_ACTRESS if $tests{j3};
diag_enc_test -2, BY_YEAR    if $tests{j4};

diag_dec_test -2, BY_MOVIE   if $tests{j6};
diag_dec_test -2, BY_ACTOR   if $tests{j7};
diag_dec_test -2, BY_ACTRESS if $tests{j8};
diag_dec_test -2, BY_YEAR    if $tests{j9};

__END__
## quote helpers

sub squote ( $ ) { $_[0] =~ /'/ ? qq["$_[0]"] : qq['$_[0]']; }
sub dquote ( $ ) { $_[0] =~ /"/ ? qq['$_[0]'] : qq["$_[0]"]; }
sub pquote ( $ ) { qq[($_[0])]; }
sub bquote ( $ ) { qq<[$_[0]]>; }
