
use strict;
use warnings;

use B::Deparse;

use DestructAssign qw(des des_alias);
use Carp;
use Data::Dumper;
$Data::Dumper::Indent = 0;
$Carp::MaxArgLen = 0;
$Carp::MaxArgNums = 0;

BEGIN {
    $SIG{__DIE__} = sub { confess $_[0] };
}

our $y;

sub tt($) : lvalue {}

#{
#    my($x, $z, $q, $g, @remain, %hv, $o);
#    tt [[$x, 2 => $y, { a => $z, b => $A::o }, @A::X, %B::Y, \%hv, \$y, \$x, [$x]], {a => $q}];
#}

# 預期用法
# {
#    my($a, $b, $c);
#    des [$a, {b => $b, c => [undef, $c]] = [1, {a => 'a', b => [1,2,3], c => [5, 6]];
#    # 得到
#    # $a = 1
#    # $b = [1,2,3]
#    # $c = 6
# }
# array 加上 jump index
# {
#    my($a, $b, $c, $d);
#    des [$a, 2, $b, $c, 9, $d] = [0, 1, 2, 3, 4]
#    # 得到
#    # $a = 0
#    # $b = 2
#    # $c = 3
#    # $d = undef
# }
# 順便幫忙 my