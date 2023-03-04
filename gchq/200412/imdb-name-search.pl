#!/usr/bin/perl
use warnings;
use strict;
use Getopt::Long;

use lib '../lib';
use LetterPattern;
use Cypher::Decode;
use Cypher::Encode;
use Latin1Helpers;

use LWP::UserAgent;
use HTML::TreeBuilder;

## flags

our ($JOB_RE);
GetOptions('job|j=s' => \$JOB_RE,
	  ) or die;

@ARGV or die "usage: $0 [flags] nameSearchTerm [namePattern...]\n";

my $search = shift @ARGV;

## input

my (%data, @data, %cat);
{
  local @ARGV = ('cyphertext-200412');
  my $key = '';
  while (<>) {
    s/[\r\n]+//g;
    /^\s+(.+)\s*$/ and $key = lc $1, next;
    /\S/ or next;
    push @{$data{$key}}, $_;
    push @data, $_;
    $cat{$_} = $key;
  }
}
$data{''} = \@data;

## make an IMDb search

my $search_uri = "http://www.imdb.com/find?q=\L$search\E;more=nm";

my $tree = HTML::TreeBuilder->new;
my $UA = LWP::UserAgent->new;
my $res = $UA->request(HTTP::Request->new(GET => $search_uri),
		       sub { $tree->parse($_[0]) });

my @links = $tree->look_down('_tag', 'a', 'href', qr!^/name/nm!);

my (%ndesc, %njob);
for my $person (@links) {
  my $name = latin1_to_ascii(lc $person->as_text);

  my $desc = $person->parent->as_text;
  $desc =~ s/(?<=\)\)).*//s or $desc =~ s/(,.*?\)).*/$1/s;

  my ($job) = ($desc =~ /\( ( [^(),]+ ) ,/x) or next;
  ! $JOB_RE or ($job =~ /^$JOB_RE$/i) or next;

  push @{ $ndesc{$name} }, $desc;
  ++$njob{$name}{lc $job};
}

my @names = sort keys %ndesc;

@ARGV = @data unless @ARGV;
foreach my $tmpl (@ARGV) {
  my $match = template_find uc($tmpl), \@names or next;
  #@$match == 1
  #  and printf("%-30s could be '%s'\n", "'$tmpl' ($cat{$tmpl})", $match->[0]),
  #    next;

  printf "%-30s may be:\n", "'$tmpl' ($cat{$tmpl})";
  print "        $_\n" for map @{$ndesc{$_}}, @$match;
}
