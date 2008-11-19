package Cypher::Decode;

use Cypher::Base;
our @ISA = qw( Cypher::Base );

sub _DOMAIN () { $Cypher::Base::AtoZ };
sub _RANGE ()  { $Cypher::Base::atoz };

# creation

sub new ( $$$ ) {
  my ($class, $cypher, $plain) = @_;
  # enforce "correct" case
  $cypher = uc $cypher;
  $plain  = lc $plain;
  # create and canonicalize
  $class->_new($cypher, $plain)->_canon;
}

# invert

sub invert ( $ ) {
  my ($self) = @_;
  require Cypher::Encode;
  Cypher::Encode->_inverted($self->[1], $self->[0]);
}

# explicit encode and decode

sub decode ( $$ )  { $_[0]->apply($_[1]); }
sub iencode ( $$ ) { $_[0]->invert->apply($_[1]); }
