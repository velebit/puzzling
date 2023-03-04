package LetterPattern;
use Exporter ();
our (@ISA, $VERSION, @EXPORT, @EXPORT_OK, %EXPORT_TAGS);
@ISA         = qw(Exporter);

$VERSION     = 0.50;

@EXPORT      = qw( read_prefixed_words read_words template_find );

@EXPORT_OK   = qw( read_prefixed_words_into read_prefixed_words
                   read_words_into read_words
                   template_regexp all_are_distinct template_find );

# for "use LetterPattern ':read';":
%EXPORT_TAGS = ( read => [ qw( read_prefixed_words_into read_prefixed_words
                               read_words_into read_words) ],
		 template => [ qw( template_find ) ],
                 all => \@EXPORT_OK );

## helpers to read word dictionaries

sub read_prefixed_words_into ( $$@ ) {
  my ($result, $prefix_re, @files) = @_;
  local (@ARGV) = @files;
  while (<>) {
    chomp;
    !defined $prefix_re or s/^$prefix_re// or next;
    push @$result, $_;
  }
  $result;
}

sub read_prefixed_words ( $@ ) {
  my ($prefix_re, @files) = @_;
  read_prefixed_words_into [], $prefix_re, @files;
}

sub read_words_into ( $@ ) {
  my ($result, @files) = @_;
  read_prefixed_words_into $result, undef, @files;
}

sub read_words ( @ ) {
  my (@files) = @_;
  read_prefixed_words_into [], undef, @files;
}

## find words that match a letter pattern

sub template_regexp ( $ ) {
  my ($template) = @_;
  my @tmpl = split //, $template;
  my %tpos;
  my $re = '';
  my $repos = 0;
  my $refs;
  for my $l (@tmpl) {
    $l =~ /^[A-Z]$/i or $re .= $l, next;
    exists $tpos{$l} and $re .= "\\" . $tpos{$l}, ++$refs, next;
    $tpos{$l} = ++$repos;
    $l eq lc $l and $re .= "($l)", next;
    $re .= '([a-z])';
  }
  qr/^$re$/i;
}

sub all_are_distinct ( @ ) {
  my (@vals) = @_;
  my $f;
  $f = shift(@vals), (grep($_ eq $f, @vals) and return) while @vals > 1;
  1;
}

sub template_find ( $@ ) {
  my ($template, @words) = @_;
  my $words = \@words;
  ($words) = @words if @words == 1 and ref($words[0])
    and UNIVERSAL::isa($words[0], 'ARRAY');
  my $re = template_regexp $template;
  my @matching;
  for my $w (@$words) {
    my (@m) = (lc($w) =~ /$re/) or next;
    all_are_distinct(@m) or next;
    push @matching, $w;
  }
  return unless @matching;
  \@matching;
}

1;
