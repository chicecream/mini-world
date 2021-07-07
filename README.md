# NAME

DestructAssign - Destructuring assignment

# SYNOPSIS

```perl
use DestructAssign qw(des des_alias);

my($w, $x, $y, $z);
our($X, $Y, $Z);
des [$x, [undef, {y => $y}, undef, $w], $z] = [2, [25, {x => 'x', y => 3}, 26, 1], 4];
# got ($w, $x, $y, $z) = (1, 2, 3, 4)
# (use undef as the skipping placeholder)

# put skip index in the list patter