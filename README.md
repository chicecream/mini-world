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

    Named parameters are good when the number of parameters is large (more than 4).
    With this mod, you can do:

    ```perl
    sub f {
      des {my($id, $title, $x, $y, $width, $height)} = \@_;
      # The order is not important.
      ...
    }

    f(
      id => 1,
      title => 'Untitled',
      x => 10, y => 10,
      width => 200, height => 150,
    );
    ```

- enhance the readability by pointing out all the elements you might touch at the begining of each subroutine

    It's a good habit to name parameters instead of access @\_ directly
    (except you want to modify caller's arguments).
    This mod extend the ability to name parameters in the deep structure.
    You can explicitly list all the elements you might touch in the subroutine.

    ```perl
    sub f {
      des [my $x, { id => my $id, amount => my $amount }] = \@_;
      # or use des_alias, if you need to modify the passed parameters.
      des_alias [my $x, { id => my $id, amount => my $amount }] = \@_;
    }
    ```

    Even if you want to modify caller's arguments, you can still use "des\_alias" to name them.

    ```perl
    sub add {
      des_alias [my($a, $b, $sum)] = \@_;
      $sum = $a + $b;
    }

    my($a, $b, $c) = (1, 2, 0);
    add($a, $b, $c);
    # $c = 3
    ```

- enhance the performance by avoiding repeatedly digging into complex data structures

    Suppose we have data structures like this:

    ```perl
    my $player1 = {
      id => 25,
      hp => 8100,
      armor => {
        body => {
          id => 21,
          name => 'iron suit',
          protect => 10,
          durability => 100,
        },
        hand => {
          id => 29,
          name => 'iron sword',
          attack => 15,
          durability => 100,
        },
      },
    };
    my $player2 = ...;
    ```

    Instead of

    ```perl
    while( $player1->{hp}>0 && $player2->{hp}>0 ) {
      my $hit1 =
          ($player1->{armor}{hand}{durability} && $player1->{armor}{hand}{attack}) -
          ($player2->{armor}{body}{durability} && $player2->{armor}{body}{protect});
      my $hit2 =
          ($player2->{armor}{hand}{durability} && $player2->{armor}{hand}{attack}) -
          ($player1->{armor}{body}{durability} && $player1->{armor}{body}{protect});
      $hit1 = 1 if( $hit1 <= 0 );
      $hit2 = 1 if( $hit2 <= 0 );

      $player1->{hp} -= $hit2;
      $player2->{hp} -= $hit1;

      --$player1->{armor}{hand}{durability} if( $player1->{armor}{hand}{durability} );
      --$player1->{armor}{body}{durability} if( $player1->{armor}{body}{durability} );
   