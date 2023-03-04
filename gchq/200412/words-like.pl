#!/usr/bin/perl
use warnings;
use strict;
use Getopt::Long;

use lib '../lib';
use LetterPattern;

## cmd line

my (@WORD_FILES, $MAX_MATCHES, $WORD_PREFIX, $SHOW_NO_MATCHES, $LIST_ONLY);

GetOptions('word-files|w=s' => \@WORD_FILES,
	   'max|m=i'        => \$MAX_MATCHES,
	   'prefix|P=s'     => \$WORD_PREFIX,
	   'show-zero|z'    => \$SHOW_NO_MATCHES,
	   'list|L'         => \$LIST_ONLY,

	   'last'           => sub {
	     push @WORD_FILES, '../english/english.names',
	       '../english/misc.names', 'bert/extra.names.last';
	   },
	   'first'          => sub {
	     push @WORD_FILES, 'bert/english.names.first',
	       'bert/misc.names.first', 'bert/extra.names.first';
	     $WORD_PREFIX = '..';
	   },
	  ) or die "usage: $0 [-w FILE] [-m NUM] [-P REGEX]\n";

@WORD_FILES = ('../english/english.words') unless @WORD_FILES;
$MAX_MATCHES = 100 unless defined $MAX_MATCHES;

## read the dictionaries

my $words = read_prefixed_words $WORD_PREFIX, @WORD_FILES;

## find words that match a letter pattern

for my $template (@ARGV) {
  $template =~ s/^"//;   $template =~ s/[",;\.\?]+$//;
  my $matching = template_find $template, $words;
  $matching or ($SHOW_NO_MATCHES ? $matching = [] : next);
  if ($LIST_ONLY) { print "\L$_\n" for @$matching; next; }
  print(scalar(@$matching) . " words like '$template'.\n"), next
    if @$matching > $MAX_MATCHES or !@$matching;
  print scalar(@$matching) . " words like '$template':\n";
  print "  '$_'\n" for @$matching;
}
