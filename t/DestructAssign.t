# Before 'make install' is performed this script should be runnable with
# 'make test'. After 'make install' it should work as 'perl DestructAssign.t'

#########################

# change 'tests => 1' to 'tests => last_test_to_print';

use strict;
use warnings;

use Test::More tests => 48;
BEGIN {
    use_ok('DestructAssign');
    DestructAssign->import('des', 'des_alias');
};

#########################

# Insert your test code below, the Test::More module is use()ed here so read
# its man page ( perldoc Test::More ) for help writing this test script.

# Synopsis
{
  my($w, $x, $y, $z);
  des [$x, [undef, {y => $y}, undef, $w], $z] = [2, [25, {x => 'x', y => 3}, 26, 1], 4];
  is_deeply [$w, $x, $y, $z], [1, 2, 3, 4], 'Synopsis1';
  # (use undef as the skipping placeholder)

  # put skip index in the list pattern
  des [3 => $w, $x, -2 => $y, $z] = [1..9];
  is_deeply [$w, $x, $y, $z], [4, 5, 8, 9], 'Synopsis2';

  # put @array or @hash in the list pattern to eat all the remaining element
  my(@array, %hash);
  des [3 => @array, -4 => %hash] = [1..8];
  is_deeply [\@array, \%hash], [[4..8], {5..8}], 'Synopsis3';

  # put the same index or hash key
  #  when you need to capture different granularity on the same data structure
  des {x => $x, x => [$y, $z]} = {x => [1, 2]};
  is_deeply [$x, $y, $z], [[1,2], 1, 2], 'Synopsis4';

  # use the alias semantics
  my $data = [1, 2, 3];
  des_alias [undef, $x] = $data;
  $x = 20;
  is_deeply $data, [1, 20, 3], 'Synopsis5';
}

# mix des with my/local
{
    my $a = 5;
    our $y = 7;
    {
        des [my $a] = [10];
        is($a, 10, 'new my');
        des [local $y] = [11];
        is($y, 11, 'new local');
    }
    is($a, 5, 'orig my');
    is($y, 7, 'orig our');
}

# mix des_alias with my/local
{
    my $data = [10];
    my $a = 5;
    our $y = 7;
    {
        des_alias [my $a] 