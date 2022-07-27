
package DestructAssign;

use 5.008009;
use strict;
use warnings;
use Carp;

require Exporter;
use AutoLoader;

our @ISA = qw(Exporter);

# Items to export into callers namespace by default. Note: do not export
# names by default without a very good reason. Use EXPORT_OK instead.
# Do not simply export all your public functions/methods/constants.

# This allows declaration	use DestructAssign ':all';
# If you do not need this, moving things directly into @EXPORT or @EXPORT_OK
# will save memory.
our %EXPORT_TAGS = ( 'all' => [ qw(
    des des_alias
) ] );

our @EXPORT_OK = ( @{ $EXPORT_TAGS{'all'} } );

our @EXPORT = qw(
	
);

our $VERSION = '0.002007';

sub des($) : lvalue { $_[0] }
sub des_alias($) : lvalue { $_[0] }

require XSLoader;
XSLoader::load('DestructAssign', $VERSION);

# Preloaded methods go here.

# Autoload methods go after =cut, and are processed by the autosplit program.

1;
__END__

=head1 NAME

DestructAssign - Destructuring assignment

=head1 SYNOPSIS

  use DestructAssign qw(des des_alias);

  my($w, $x, $y, $z);
  our($X, $Y, $Z);
  des [$x, [undef, {y => $y}, undef, $w], $z] = [2, [25, {x => 'x', y => 3}, 26, 1], 4];
  # got ($w, $x, $y, $z) = (1, 2, 3, 4)
  # (use undef as the skipping placeholder)

  # put skip index in the list pattern
  des [3 => $w, $x, -2 => $y, $z] = [1..9];
  # got ($w, $x, $y, $z) = (4, 5, 8, 9)

  # use names of the variables in a hash pattern as keys when not assigned
  # use previously used key for a sub-pattern when not assigned
  des {$x, $A::B, $Y, [$a, $b]} = {x => 1, Y => [9, 8], B => 3};
  # got ($x, $Y, $A::B, $a, $b) = (1, [9,8], 3, 9, 8);

  # use hash pattern to match against an array reference
  # So we can write:
  sub f {
    des {my($score, $name, $detail), {my($math, $english)}} = \@_;
    ...
  }
  f(
    name => 'Cindy',
    score => 95,
    detail => {math => 90, english => 30, bios => 60},
  );


  # put @array or @hash in the list pattern to eat all the remaining element
  my(@array, %hash);
  des [3 => @array, -4 => %hash] = [1..8];
  # got @array = (4..8), %hash = (5 => 6, 7 => 8)

  # put the same index or hash key
  #  when you need to capture different granularity on the same data structure
  #  (notice that you can use duplicated keys in the hash pattern)