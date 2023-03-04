#!/usr/bin/perl
# solve fixed substitution cyphers by having a word list "vote" on solutions
use warnings;
use strict;

## input plaintext

if (! @ARGV) {
  my @data;
  while (<>) {
    push @data, split ' ';
  }
  @ARGV = @data;
}
@ARGV = map uc, @ARGV;

## input words

my (@words);
{
  local @ARGV = ('../english/english.words');
  while (<>) {
    chomp;
    push @words, $_;
  }
}

## find words that match a letter pattern

my %votes;

for my $template (@ARGV) {
  $template =~ s/^"//;   $template =~ s/[",;\.\?]+$//;
  my @tmpl = split //, $template;
  my %tpos;
  my $re = '';
  my $repos = 0;
  my $refs;
  for my $l (@tmpl) {
    $l =~ /^[A-Z]$/ or $re .= $l, next;
    exists $tpos{$l} and $re .= "\\" . $tpos{$l}, ++$refs, next;
    $tpos{$l} = ++$repos;
    $re .= '([a-z])';
  }
  #next unless $refs;
  $re = qr/^$re$/;
  my @matching;
  for my $w (@words) {
    my (@m) = (lc($w) =~ /$re/) or next;
    my $f;
    $f = shift(@m), (grep($_ eq $f, @m) and next) while @m > 1;
    push @matching, lc $w;
  }
  next unless @matching;
  next if @matching > 10;

  foreach my $match (@matching) {
    my @match = split //, $match;
    my %voted;
    for my $i (0..$#match) {
      $voted{$match[$i]} and next;
      $votes{$tmpl[$i]}{$match[$i]} ||= 0;
      $votes{$tmpl[$i]}{$match[$i]} += (1/@matching);
      $voted{$match[$i]} = 1;
    }
  }
}

my %max_votes = map
  +($_ => ((sort {$b <=> $a} values %{$votes{$_}})[0])),
  keys %votes;

for my $c (sort {$max_votes{$b} <=> $max_votes{$a}} keys %votes) {
  print "$c:";
  my @lead = sort {$votes{$c}{$b} <=> $votes{$c}{$a}} keys %{$votes{$c}};
  my $dots = '';
  @lead > 8 and $dots = ' ...', @lead = @lead[0..7];
  for my $p (@lead) {
    printf "  %5.3f %s", $votes{$c}{$p}, $p;
  }
  print "$dots\n";
}
