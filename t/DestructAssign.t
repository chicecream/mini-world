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
  is_deeply [\@