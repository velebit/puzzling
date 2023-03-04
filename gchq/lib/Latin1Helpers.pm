package Latin1Helpers;
use Exporter ();
our (@ISA, $VERSION, @EXPORT, @EXPORT_OK);
@ISA         = qw(Exporter);

$VERSION     = 1.00;

@EXPORT      = qw( latin1_to_ascii );
@EXPORT_OK   = qw( latin1_to_ascii );

## helpers to read word dictionaries

sub latin1_to_ascii ( $ ) {
  my ($str) = @_;
  # Language characters:
  $str =~ s/[\xC0-\xC5]/A/g;
  $str =~ s/\xC6/AE/g;
  $str =~ s/\xC7/C/g;
  $str =~ s/[\xC8-\xCB]/E/g;
  $str =~ s/[\xCC-\xCF]/I/g;
  $str =~ s/\xD0/D/g;             # well, sort of-- this is the eth (Dj)
  $str =~ s/\xD1/N/g;
  $str =~ s/[\xD2-\xD6\xD8]/O/g;
  $str =~ s/[\xD9-\xDC]/U/g;
  $str =~ s/\xDD/Y/g;
  $str =~ s/\xDE/TH/g;            # well, sort of-- this is the thorn
  $str =~ s/\xDF/ss/g;
  $str =~ s/[\xE0-\xE5]/a/g;
  $str =~ s/\xE6/ae/g;
  $str =~ s/\xE7/c/g;
  $str =~ s/[\xE8-\xEB]/e/g;
  $str =~ s/[\xEC-\xEF]/i/g;
  $str =~ s/\xF0/d/g;             # well, sort of-- this is the eth (Dj)
  $str =~ s/\xF1/n/g;
  $str =~ s/[\xF2-\xF6\xF8]/o/g;
  $str =~ s/[\xF9-\xFC]/u/g;
  $str =~ s/\xFD/y/g;
  $str =~ s/\xFE/th/g;            # well, sort of-- this is the thorn
  $str =~ s/\xFF/y/g;
  # other characters:
  $str =~ s/\xA0/ /g;
  $str =~ s/\xD7/ x /g;           # multiplication symbol
  $str =~ s,\xF7, / ,g;           # division symbol
  $str =~ s/\xB2/^2/g;            # squared
  $str =~ s/\xB3/^3/g;            # cubed
  $str;
}

1;
