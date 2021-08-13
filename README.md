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
des {x => $x, x => [$y, $z]} = {x => [1, 2]};
# got $x = [1,2], $y = 1, $z = 2

# use the alias semantics
my $data = [1, 2, 3];
des_alias [undef, $x] = $data;
$x = 20;
# got $data = [1, 20, 3]

{
  # mixed with lexical variable introduction
  des [my($i, $j), { k => my $k }] = [1, 2, {k => 3}];
  # got my($i, $j, $k) = (1, 2, 3)
}
```

# DESCRIPTION

This mod provides destructuring assignment for Perl.
You can capture (by value) or bind (by alias) variables into
part of a potentially large and complex data structure.

I expect it to bring following benefits:

- provide named parameters more easily

    Named parameters are good when the number of parameters is la