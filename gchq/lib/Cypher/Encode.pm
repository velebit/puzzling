package Cypher::Encode;

use Cypher::Base;
our @ISA = qw( Cypher::Base );

sub _DOMAIN () { $Cypher::Base::atoz };
sub _RANGE ()  { $Cypher::Base::AtoZ };

# creation

sub new ( $$$ ) {
  my ($class, $plain, $cypher) = @_;
  # enforce "correct" case
  $plain  = lc $plain;
  $cypher = uc $cypher;
  # create and canonicalize
  $class->_new($plain, $cypher)->_canon;
}

# invert

sub invert ( $ ) {
  my ($self) = @_;
  require Cypher::Decode;
  Cypher::Decode->_inverted($self->[1], $self->[0]);
}

# explicit encode and decode

sub encode ( $$ )  { $_[0]->apply($_[1]); }
sub idecode ( $$ ) { $_[0]->invert->apply($_[1]); }
