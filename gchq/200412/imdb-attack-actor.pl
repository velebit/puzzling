#!/usr/bin/perl
use warnings;
use strict;
use Data::Dumper;

use lib '../lib';
use LetterPattern;
use Cypher::Decode;
use Cypher::Encode;
use Latin1Helpers;

use LWP::UserAgent;
use HTML::TreeBuilder;

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

## read old cypher relationships, if any

our $cyphers = {};
require 'cyphers-200412.data' if -f 'cyphers-200412.data';

## perform a IMDb download if needed

my $UA = LWP::UserAgent->new;
@ARGV or die;

my @files;
for my $entry (@ARGV) {
  # if it's a file, there's nothing to do.
  -f $entry and push(@files, $entry), next;

  # if it's a URL, download it and save to file for convenience.
  if ($entry =~ m!^(?:http|ftp)://!) {
    my $tree = HTML::TreeBuilder->new;
    my $tmpfile = 'tmpdata.html';
    $tmpfile = "html/$tmpfile" if -d 'html';
    my $res = $UA->request(HTTP::Request->new(GET => $entry),
			   $tmpfile);
    $res->is_success or die "request failed.\n";
    $tree->parse_file($tmpfile);

    my $file = $tree->look_down('_tag', 'title')->as_text or die;
    $file =~ s/[\(\)]//g;  $file =~ s/\s+$//;  $file =~ s/\W+/_/g;
    $file = lc($file) . '.html';
    $file = "html/$file" if -d 'html';
    rename $tmpfile, $file or die "rename: $!";
    push @files, $file;
    next;
  }

  die "Unknown argument '$entry'";
}

## process downloaded IMDb pages, extract movie data, find matching cyphers

for my $file (@files) {
  my $tree = HTML::TreeBuilder->new;
  $tree->parse_file($file);

  my $name = $tree->look_down('_tag', 'title')->as_text or die;
  $name =~ s/\s+ \( [ivx]+ \) $//xi;

  my $match = template_find uc($name), $data{''}
    or print("no possibilities for '$name'\n"), next;
  @$match == 1
    or print("multiple possibilities for '$name'\n"), next;
  my $code = $match->[0];

  printf "%-40s %s\n", "$name:", "($code)";

  my $movies = $tree->look_down('_tag', 'a', 'name', qr/^(?:actress|actor)$/)
    or die "no movies found\n";
  my (@movies, @titles, $right);
  $movies = $movies->parent
    until @movies = $movies->look_down('_tag', 'a', 'href', qr!^/title/!)
      or (ref($right = $movies->right)
	  and @movies = $right->look_down('_tag', 'a', 'href', qr!^/title/!));

  for my $m (@movies) {
    my $title = $m->as_text;
    $title = latin1_to_ascii $title;
    die "Funny characters in $title\n" if $title =~ /[\x7F-\xFF]/;
    push @titles, $title;
    for my $t ($m->right) {
      $t = $t->as_text if ref $t;
      $t = latin1_to_ascii $t;
      die "Funny characters in $t\n" if $t =~ /[\x7F-\xFF]/;
      $t =~ s/^ \s* \.\.\. \s* aka \s+//x or next;
      $t =~ s/\s+$//;
      $t =~ s/\s+ \( .* \) $//x;
      push @titles, $t;
    }
  }

  my $C = Cypher::Encode->new($name, $code);
  #print($C->invert->display('        -> '), $C->display('        <- '));
  my ($firstP, $firstC) = $C->hash;

  for my $t (@titles) {
    # Remove duplicate chars (e.g. 'The Terminator' -> 'THERMINAO')
    # Add all unused characters (e.g. 'THERMINAO' -> 'THERMINAOBCDFGJ...YZ')
    my @chars = grep /[A-Z]/, split //, uc $t;
    my (@xchars, %nchar);
    $nchar{$_}++ or push(@xchars, $_) for @chars, 'A' .. 'Z';
    my $blob = join '', @xchars;
    # "Rotate" the string so $firstC is the first character
    $blob =~ s/^(.*)(?=\Q$firstC\E)(.+)$/$2$1/;
    # Generate and "rotate" the letter sequence for plaintext
    my $plain = $Cypher::Base::atoz;
    $plain =~ s/^(.*)(?=\Q$firstP\E)(.+)$/$2$1/;
    # Make and check cypher.
    my $C2 = Cypher::Encode->new($plain, $blob);
    $C2->compatible($C) or next;
    (my $t0 = $t) =~ s/^(.{37}).{4,}/$1.../;
    #printf "  %-40s -> '%s'\n", $t0, $blob;
    print "  $t0\n";
    print($C2->invert->display('        -> '), $C2->display('        <- '));
    # record for poster(ior)ity
    my $keygen = $C2->($Cypher::Base::atoz);
    exists $cyphers->{$name} and $cyphers->{$name}[1] ne $keygen and die;
    $cyphers->{$name} = [ $t, $keygen ];
  }
}

## save cypher relationships

open my $DATA, '>', 'cyphers-200412.data' or die "open(>): $!";
print $DATA Data::Dumper->Dump([$cyphers], ['$cyphers']);
