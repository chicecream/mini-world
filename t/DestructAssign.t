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

