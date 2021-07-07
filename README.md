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

# put skip index in the list pattern
des [3 => $w, $x, -2 => $y, $z] = [1..9];
# got ($w, $x, $y, $z) = (4, 5, 8, 9)

# use names of the variables in a hash pattern as keys when not assigned
# use previously used key for a sub-pattern when not assigned
des {$x, $A::B, $Y, [$a, $b]} = {x => 1, Y => [9, 8], B => 3};
# got ($x, $Y, $A::B, $a, $b) = (1, [9,8], 3, 9, 8);