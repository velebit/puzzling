#!/usr/bin/perl
use warnings;
use strict;

## input

@ARGV = ('cyphertext-200406');
my (@data);
local ($/) = '';
while (<>) {
  s/.*\n//m;
  s/\n{2,}$/\n/;
  push @data, $_;
}

unshift @data, join "\n", @data;

## decoding!

sub decode ( $$ ) {
  my ($sec, $text) = @_;
  if ($sec == 6) {
    $text =~ tr/QAZQSCX/october/;
    $text =~ tr/UCBPCYBET/wednesday/;
    $text =~ tr/ZIC_NQXPJPG_QD/the_morning_of/;
    $text =~ tr/WFM/qul/;
  }
  if ($sec == 4) {
    $text =~ tr/PIR_DOZ_NL_F_WQ_MKSUH/the_nda_si_o_yu_grfwp/;
    $text =~ tr/CEB_GA/mcl_bk/;
  }
  if ($sec == 1) {
    $text =~ tr/IWT_VD_PCS_A_HLB_UNX/the_go_and_l_swm_fyi/;
    $text =~ tr/RGEKYJ/crpvju/;
  }
  if ($sec == 2) {
    $text =~ tr/XHAAHUB_UCDHSCQ/william_ancient/;
    $text =~ tr/ZTLEP_NF_IVR/yhrds_go_fub/;
  }
  if ($sec == 5) {
    $text =~ tr/SBLAMULPRBPQ/headquarters/;
    $text =~ tr/JIZVCGW_KFDHT_N/obcvinw_pmlgf_k/;
  }
  if ($sec == 3) {
    $text =~ tr/GZJNHYPSZJSW_KRBR_ADC_UT/particularly_sene_mod_wh/;
    $text =~ tr/MOFQ_E/bfgv_k/;
  }
  $text;
}

print "text:\n";
for my $sec (1..$#data) {
  print "\n";
  for (split /(?<=\n)/, $data[$sec]) {
    my $d = decode($sec, $_);
    $d =~ s/[A-Z]/_/g;
    print "$sec: ", $_, ($d !~ /[a-z]/ || $d eq $_ ? () : ("   ", $d)), "\n";
  }
}

## letter frequencies

sub squote ( $ ) { $_[0] =~ "'" ? qq["$_[0]"] : qq['$_[0]']; }
sub dquote ( $ ) { $_[0] =~ '"' ? qq['$_[0]'] : qq["$_[0]"]; }
sub pquote ( $ ) { qq[($_[0])]; }

my (%sec_freq, %sec_freq_order);

for my $sec (1..$#data) {
  (my $text = $data[$sec]) =~ s/\s+/ /gs;
  $sec_freq{$sec}{$_}++ foreach map split(//), $text;
  my @order = sort {$sec_freq{$sec}{$b} <=> $sec_freq{$sec}{$a}}
    keys %{$sec_freq{$sec}};
  $sec_freq_order{$sec} = \@order;
  my $cols = 4;
  my $rows = int((@order + $cols - 1) / $cols);
  printf "letter frequencies for %s (%d total chars):",
    ($sec ? "section $sec" : "all sections"), scalar @order;
  for my $r (0..($rows-1)) {
    for my $c (0..($cols-1)) {
      print($c ? "   " : "\n  ");
      my $p = $c*$rows + $r;
      $p >= @order and next;
      my $cyp = $order[$p];
      my $dec = decode ($sec,$cyp) if $sec;
      if (!$dec or $dec eq $cyp) {
	printf "%3d %3s    ", $sec_freq{$sec}{$cyp}, squote $cyp;
      } else {
	printf "%3d %3s %s", $sec_freq{$sec}{$cyp}, squote $cyp,
	  pquote $dec;
      }
    }
  }
  print "\n";
}

## word frequencies

my (%sec_wfreq, %sec_wfreq_order);

print "\n";
for my $sec (1..$#data) {
  (my $text = $data[$sec]) =~ s/\s+/ /gs;

  s/^"//, s/[",;\.\?]+$//, $sec_wfreq{$sec}{$_}++
    foreach map split(/\s+/), $text;
  my @order = sort {$sec_wfreq{$sec}{$b} <=> $sec_wfreq{$sec}{$a}
		      or $a cmp $b}
    keys %{$sec_wfreq{$sec}};
  $sec_wfreq_order{$sec} = \@order;
  my $cols = 2;
  my $rows = int((@order + $cols - 1) / $cols);
  printf "word frequencies for %s (%d total words):",
    ($sec ? "section $sec" : "all sections"), scalar @order;
  for my $r (0..($rows-1)) {
    for my $c (0..($cols-1)) {
      print($c ? "   " : "\n  ");
      my $p = $c*$rows + $r;
      $p >= @order and next;
      my $cyp = $order[$p];
      my $dec = decode ($sec,$cyp) if $sec;
      if (! $dec or $dec !~ /[a-z]/) {
	printf "%1d %-15s %-17s", $sec_wfreq{$sec}{$cyp}, $cyp, '';
      } else {
	printf "%1d %-15s %-17s", $sec_wfreq{$sec}{$cyp}, $cyp,
	  pquote $dec;
      }
    }
  }
  print "\n";
}

## word frequencies, munged words

my (%munged, %munged_score, %sec_munged_order);

print "\n";
for my $sec (1..$#data) {
  foreach my $cyp (keys %{$sec_wfreq{$sec}}) {
    my @cletters = split //, $cyp;
    my %ccount;
    ++$ccount{$_} for @cletters;
    $munged{$cyp} = join '',
      sort {$ccount{$b} <=> $ccount{$a} or $a cmp $b} @cletters;
    my $score = 0;
    $score += ($_-1)*($_-1) for values %ccount;
    $munged_score{$cyp} = $score;
  }
  my @order = sort {$munged_score{$b} <=> $munged_score{$a}
		      or length $a <=> length $b
		      or $munged{$a} cmp $munged{$b}}
    keys %{$sec_wfreq{$sec}};
  $sec_munged_order{$sec} = \@order;
  my $cols = 2;
  my $rows = int((@order + $cols - 1) / $cols);
  printf "sorted-letter words for %s (%d total words):",
    ($sec ? "section $sec" : "all sections"), scalar @order;
  for my $r (0..($rows-1)) {
    for my $c (0..($cols-1)) {
      print($c ? "   " : "\n  ");
      my $p = $c*$rows + $r;
      $p >= @order and next;
      my $cyp = $order[$p];
      my @lcyp = split //, $cyp;
      my %ccyp;
      ++$ccyp{$_} for @lcyp;
      my $scy = join '', sort {$ccyp{$b} <=> $ccyp{$a} or $a cmp $b} @lcyp;
      printf "%1d %-15s %-17s", $sec_wfreq{$sec}{$cyp}, $scy,
	pquote $cyp;
    }
  }
  print "\n";
}

## decrypt mappings

my $BASE = join '', ('A'..'Z');

sub encode ( $$ ) {
  my ($sec, $text) = @_;
  my $d = decode($sec, $BASE);
  eval "\$text =~ tr/$d/$BASE/";
  $text;
}

print "\nkeys:\n";
for my $sec (1..$#data) {
  my $d = decode($sec, $BASE);  $d =~ s/[A-Z]/ /g;
  print "$sec: $BASE\n   $d\n"   if $d =~ /\S/;
  my $base = lc $BASE;
  my $e = encode($sec, $base);  $e =~ s/[a-z]/ /g;
  print "$sec: $base\n   $e\n\n" if $e =~ /\S/;
}
