#!/usr/bin/perl
use warnings;
use strict;
use Data::Dumper;

use lib '../lib';
use LetterPattern;
use Cypher::Decode;
use Cypher::Encode;
use Latin1Helpers;

# HACK: this entire script is just to avoid depending on HTML::TreeBuilder

## read old cypher relationships, if any

our $cyphers = {};
require 'cyphers-200412.data' if -f 'cyphers-200412.data';

## process downloaded IMDb pages

my (%years, %cm_years);

for my $file (@ARGV) {
  open my $F, '<', $file or die;
  while (<>) {
    my ($m, $y) = m!^<a href="/title/tt\d+/">([^<>]+)</a> \((\d{4})\)!
      or next;
    $m = latin1_to_ascii $m;
    die "Funny characters in $ARGV\n" if $m =~ /[\x7F-\xFF]/;
    $years{$m} = $y;
  }
}



for my $info (values %$cyphers) {
  my $year = $years{$info->[0]} or die "No year for '$info->[0]'";
  $info->[2] = $year;
  $cm_years{$info->[0]} = $year;
}

printf "%-35s (%s)\n", $_, $years{$_}
  for sort {$years{$a} <=> $years{$b}} keys %cm_years;

## save updated cypher relationships

open my $DATA, '>', 'cyphers-200412.data' or die "open(>): $!";
print {$DATA} Data::Dumper->Dump([$cyphers], ['$cyphers']);
