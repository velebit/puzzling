package Cypher::Base;

use overload ( '&{}' => 'make_sub' );

our $AtoZ = join '', ('A'..'Z');
our $atoz = join '', ('a'..'z');

# creation

sub _new ( $$$ ) {
  my ($class, $from, $to) = @_;
  $class = ref($class) || $class;
  length $from eq length $to
    or die "Inconsistent input length in '$from' -> '$to'";
  # check uniqueness in c->p mapping
  my $check = $from;
  eval "\$check =~ tr/$from/$to/";
  $check eq $to or die "Inconsistent decoding '$from' -> '$to' ($check)";
  # create
  bless [ $from, $to ], $class;
}

# default creation call for "inverted" cypher

sub _inverted ( $$$ ) {
  my ($class, $from, $to) = @_;
  $class->new($from, $to);
}

# canonicalize: make $cypher small and sorted (with $plain matching)

sub _canon ( $ ) {
  my ($self) = @_;
  my ($from, $to) = @$self;
  my ($dom, $rng) = ($self->_DOMAIN, $self->_RANGE);
  # find source characters that are currently not defined
  my $unused = $self->($dom);
  #eval "\$unused =~ tr/$rng//d";   # NB: not actually needed by later code
  # find source characters that are currently defined
  my $used = $dom;
  eval "\$used =~ tr/$unused//d";
  # put in canonical version
  my $mapped = $self->($used);
  $self->[0] = $used;   $self->[1] = $mapped;
  $self;
}

# create callback to apply cypher to text

sub make_sub ( $ ) {
  my ($self) = @_;
  my ($from, $to) = @$self;
  eval 'sub { my ($t) = @_; $t =~ tr/'."\Q$from".'/'."\Q$to".'/; $t }';
}

# apply cypher to text
# (uses make_sub to avoid duplicating code, though it's less efficient)

sub apply ( $$ ) {
  my ($self, $text) = @_;
  $text = $self->($text);
  # remove characters that didn't make it because the cypher is incomplete
  my $dom = $self->_DOMAIN;
  eval "\$text =~ tr/\Q$dom\E/_/";
  $text;
}

# format the cypher for printing on-screen

sub display ( $;$$ ) {
  my ($self, $pre1, $pre2) = @_;
  $pre1 = ''                 unless defined $pre1;
  $pre2 = ' ' x length $pre1 unless defined $pre2;
  my $dom  = $self->_DOMAIN;
  my $dest = $self->($dom);
  eval "\$dest =~ tr/\Q$dom\E/ /";
  "$pre1$dom\n$pre2$dest\n";
}

# see if two cyphers are compatible

sub compatible ( $$ ) {
  my ($self, $self2) = @_;
  my $dom = $self->_DOMAIN;
  return if $self2->_DOMAIN ne $dom;
  $self->($self2->($dom)) eq $self2->($self->($dom)) or return;
  my $self_i  = $self->invert;
  my $self2_i = $self2->invert;
  my $rng = $self->_RANGE;
  $self_i->($self2_i->($rng)) eq $self2_i->($self_i->($rng)) or return;
  1;
}

# combine two cyphers

sub combine_with ( $$ ) {
  my ($self, $self2) = @_;
  $self->compatible($self2)
    or die "Cannot combine incompatible cyphers";
  $self->[0] .= $self2->[0];   $self->[1] .= $self2->[1];
  $self->_canon;
}

# generate a hash mapping of characters corresponding to the cypher

sub hash ( $ ) {
  my ($self) = @_;
  my @from = split //, $self->[0];
  my @to   = split //, $self->[1];
  map +($from[$_] => $to[$_]), 0..$#from;
}
