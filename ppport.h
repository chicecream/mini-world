
#if 0
<<'SKIP';
#endif
/*
----------------------------------------------------------------------

    ppport.h -- Perl/Pollution/Portability Version 3.28

    Automatically created by Devel::PPPort running under perl 5.008008.

    Do NOT edit this file directly! -- Edit PPPort_pm.PL and the
    includes in parts/inc/ instead.

    Use 'perldoc ppport.h' to view the documentation below.

----------------------------------------------------------------------

SKIP

=pod

=head1 NAME

ppport.h - Perl/Pollution/Portability version 3.28

=head1 SYNOPSIS

  perl ppport.h [options] [source files]

  Searches current directory for files if no [source files] are given

  --help                      show short help

  --version                   show version

  --patch=file                write one patch file with changes
  --copy=suffix               write changed copies with suffix
  --diff=program              use diff program and options

  --compat-version=version    provide compatibility with Perl version
  --cplusplus                 accept C++ comments

  --quiet                     don't output anything except fatal errors
  --nodiag                    don't show diagnostics
  --nohints                   don't show hints
  --nochanges                 don't suggest changes
  --nofilter                  don't filter input files

  --strip                     strip all script and doc functionality from
                              ppport.h

  --list-provided             list provided API
  --list-unsupported          list unsupported API
  --api-info=name             show Perl API portability information

=head1 COMPATIBILITY

This version of F<ppport.h> is designed to support operation with Perl
installations back to 5.003, and has been tested up to 5.20.

=head1 OPTIONS

=head2 --help

Display a brief usage summary.

=head2 --version

Display the version of F<ppport.h>.

=head2 --patch=I<file>

If this option is given, a single patch file will be created if
any changes are suggested. This requires a working diff program
to be installed on your system.

=head2 --copy=I<suffix>

If this option is given, a copy of each file will be saved with
the given suffix that contains the suggested changes. This does
not require any external programs. Note that this does not
automagically add a dot between the original filename and the
suffix. If you want the dot, you have to include it in the option
argument.

If neither C<--patch> or C<--copy> are given, the default is to
simply print the diffs for each file. This requires either
C<Text::Diff> or a C<diff> program to be installed.

=head2 --diff=I<program>

Manually set the diff program and options to use. The default
is to use C<Text::Diff>, when installed, and output unified
context diffs.

=head2 --compat-version=I<version>

Tell F<ppport.h> to check for compatibility with the given
Perl version. The default is to check for compatibility with Perl
version 5.003. You can use this option to reduce the output
of F<ppport.h> if you intend to be backward compatible only
down to a certain Perl version.

=head2 --cplusplus

Usually, F<ppport.h> will detect C++ style comments and
replace them with C style comments for portability reasons.
Using this option instructs F<ppport.h> to leave C++
comments untouched.

=head2 --quiet

Be quiet. Don't print anything except fatal errors.

=head2 --nodiag

Don't output any diagnostic messages. Only portability
alerts will be printed.

=head2 --nohints

Don't output any hints. Hints often contain useful portability
notes. Warnings will still be displayed.

=head2 --nochanges

Don't suggest any changes. Only give diagnostic output and hints
unless these are also deactivated.

=head2 --nofilter

Don't filter the list of input files. By default, files not looking
like source code (i.e. not *.xs, *.c, *.cc, *.cpp or *.h) are skipped.

=head2 --strip

Strip all script and documentation functionality from F<ppport.h>.
This reduces the size of F<ppport.h> dramatically and may be useful
if you want to include F<ppport.h> in smaller modules without
increasing their distribution size too much.

The stripped F<ppport.h> will have a C<--unstrip> option that allows
you to undo the stripping, but only if an appropriate C<Devel::PPPort>
module is installed.

=head2 --list-provided

Lists the API elements for which compatibility is provided by
F<ppport.h>. Also lists if it must be explicitly requested,
if it has dependencies, and if there are hints or warnings for it.

=head2 --list-unsupported

Lists the API elements that are known not to be supported by
F<ppport.h> and below which version of Perl they probably
won't be available or work.

=head2 --api-info=I<name>

Show portability information for API elements matching I<name>.
If I<name> is surrounded by slashes, it is interpreted as a regular
expression.

=head1 DESCRIPTION

In order for a Perl extension (XS) module to be as portable as possible
across differing versions of Perl itself, certain steps need to be taken.

=over 4

=item *

Including this header is the first major one. This alone will give you
access to a large part of the Perl API that hasn't been available in
earlier Perl releases. Use

    perl ppport.h --list-provided

to see which API elements are provided by ppport.h.

=item *

You should avoid using deprecated parts of the API. For example, using
global Perl variables without the C<PL_> prefix is deprecated. Also,
some API functions used to have a C<perl_> prefix. Using this form is
also deprecated. You can safely use the supported API, as F<ppport.h>
will provide wrappers for older Perl versions.

=item *

If you use one of a few functions or variables that were not present in
earlier versions of Perl, and that can't be provided using a macro, you
have to explicitly request support for these functions by adding one or
more C<#define>s in your source code before the inclusion of F<ppport.h>.

These functions or variables will be marked C<explicit> in the list shown
by C<--list-provided>.

Depending on whether you module has a single or multiple files that
use such functions or variables, you want either C<static> or global
variants.

For a C<static> function or variable (used only in a single source
file), use:

    #define NEED_function
    #define NEED_variable

For a global function or variable (used in multiple source files),
use:

    #define NEED_function_GLOBAL
    #define NEED_variable_GLOBAL

Note that you mustn't have more than one global request for the
same function or variable in your project.

    Function / Variable       Static Request               Global Request
    -----------------------------------------------------------------------------------------
    PL_parser                 NEED_PL_parser               NEED_PL_parser_GLOBAL
    PL_signals                NEED_PL_signals              NEED_PL_signals_GLOBAL
    caller_cx()               NEED_caller_cx               NEED_caller_cx_GLOBAL
    eval_pv()                 NEED_eval_pv                 NEED_eval_pv_GLOBAL
    grok_bin()                NEED_grok_bin                NEED_grok_bin_GLOBAL
    grok_hex()                NEED_grok_hex                NEED_grok_hex_GLOBAL
    grok_number()             NEED_grok_number             NEED_grok_number_GLOBAL
    grok_numeric_radix()      NEED_grok_numeric_radix      NEED_grok_numeric_radix_GLOBAL
    grok_oct()                NEED_grok_oct                NEED_grok_oct_GLOBAL
    load_module()             NEED_load_module             NEED_load_module_GLOBAL
    mg_findext()              NEED_mg_findext              NEED_mg_findext_GLOBAL
    my_snprintf()             NEED_my_snprintf             NEED_my_snprintf_GLOBAL
    my_sprintf()              NEED_my_sprintf              NEED_my_sprintf_GLOBAL
    my_strlcat()              NEED_my_strlcat              NEED_my_strlcat_GLOBAL
    my_strlcpy()              NEED_my_strlcpy              NEED_my_strlcpy_GLOBAL
    newCONSTSUB()             NEED_newCONSTSUB             NEED_newCONSTSUB_GLOBAL
    newRV_noinc()             NEED_newRV_noinc             NEED_newRV_noinc_GLOBAL
    newSV_type()              NEED_newSV_type              NEED_newSV_type_GLOBAL
    newSVpvn_flags()          NEED_newSVpvn_flags          NEED_newSVpvn_flags_GLOBAL
    newSVpvn_share()          NEED_newSVpvn_share          NEED_newSVpvn_share_GLOBAL
    pv_display()              NEED_pv_display              NEED_pv_display_GLOBAL
    pv_escape()               NEED_pv_escape               NEED_pv_escape_GLOBAL
    pv_pretty()               NEED_pv_pretty               NEED_pv_pretty_GLOBAL
    sv_2pv_flags()            NEED_sv_2pv_flags            NEED_sv_2pv_flags_GLOBAL
    sv_2pvbyte()              NEED_sv_2pvbyte              NEED_sv_2pvbyte_GLOBAL
    sv_catpvf_mg()            NEED_sv_catpvf_mg            NEED_sv_catpvf_mg_GLOBAL
    sv_catpvf_mg_nocontext()  NEED_sv_catpvf_mg_nocontext  NEED_sv_catpvf_mg_nocontext_GLOBAL
    sv_pvn_force_flags()      NEED_sv_pvn_force_flags      NEED_sv_pvn_force_flags_GLOBAL
    sv_setpvf_mg()            NEED_sv_setpvf_mg            NEED_sv_setpvf_mg_GLOBAL
    sv_setpvf_mg_nocontext()  NEED_sv_setpvf_mg_nocontext  NEED_sv_setpvf_mg_nocontext_GLOBAL
    sv_unmagicext()           NEED_sv_unmagicext           NEED_sv_unmagicext_GLOBAL
    vload_module()            NEED_vload_module            NEED_vload_module_GLOBAL
    vnewSVpvf()               NEED_vnewSVpvf               NEED_vnewSVpvf_GLOBAL
    warner()                  NEED_warner                  NEED_warner_GLOBAL

To avoid namespace conflicts, you can change the namespace of the
explicitly exported functions / variables using the C<DPPP_NAMESPACE>
macro. Just C<#define> the macro before including C<ppport.h>:

    #define DPPP_NAMESPACE MyOwnNamespace_
    #include "ppport.h"

The default namespace is C<DPPP_>.

=back

The good thing is that most of the above can be checked by running
F<ppport.h> on your source code. See the next section for
details.

=head1 EXAMPLES

To verify whether F<ppport.h> is needed for your module, whether you
should make any changes to your code, and whether any special defines
should be used, F<ppport.h> can be run as a Perl script to check your
source code. Simply say:

    perl ppport.h

The result will usually be a list of patches suggesting changes
that should at least be acceptable, if not necessarily the most
efficient solution, or a fix for all possible problems.

If you know that your XS module uses features only available in
newer Perl releases, if you're aware that it uses C++ comments,
and if you want all suggestions as a single patch file, you could
use something like this:

    perl ppport.h --compat-version=5.6.0 --cplusplus --patch=test.diff

If you only want your code to be scanned without any suggestions
for changes, use:

    perl ppport.h --nochanges

You can specify a different C<diff> program or options, using
the C<--diff> option:

    perl ppport.h --diff='diff -C 10'

This would output context diffs with 10 lines of context.

If you want to create patched copies of your files instead, use:

    perl ppport.h --copy=.new

To display portability information for the C<newSVpvn> function,
use:

    perl ppport.h --api-info=newSVpvn

Since the argument to C<--api-info> can be a regular expression,
you can use

    perl ppport.h --api-info=/_nomg$/

to display portability information for all C<_nomg> functions or

    perl ppport.h --api-info=/./

to display information for all known API elements.

=head1 BUGS

If this version of F<ppport.h> is causing failure during
the compilation of this module, please check if newer versions
of either this module or C<Devel::PPPort> are available on CPAN
before sending a bug report.

If F<ppport.h> was generated using the latest version of
C<Devel::PPPort> and is causing failure of this module, please
file a bug report here: L<https://github.com/mhx/Devel-PPPort/issues/>

Please include the following information:

=over 4

=item 1.

The complete output from running "perl -V"

=item 2.

This file.

=item 3.

The name and version of the module you were trying to build.

=item 4.

A full log of the build that failed.

=item 5.

Any other information that you think could be relevant.

=back

For the latest version of this code, please get the C<Devel::PPPort>
module from CPAN.

=head1 COPYRIGHT

Version 3.x, Copyright (c) 2004-2013, Marcus Holland-Moritz.

Version 2.x, Copyright (C) 2001, Paul Marquess.

Version 1.x, Copyright (C) 1999, Kenneth Albanowski.

This program is free software; you can redistribute it and/or
modify it under the same terms as Perl itself.

=head1 SEE ALSO

See L<Devel::PPPort>.

=cut

use strict;

# Disable broken TRIE-optimization
BEGIN { eval '${^RE_TRIE_MAXBUF} = -1' if $] >= 5.009004 && $] <= 5.009005 }

my $VERSION = 3.28;

my %opt = (
  quiet     => 0,
  diag      => 1,
  hints     => 1,
  changes   => 1,
  cplusplus => 0,
  filter    => 1,
  strip     => 0,
  version   => 0,
);

my($ppport) = $0 =~ /([\w.]+)$/;
my $LF = '(?:\r\n|[\r\n])';   # line feed
my $HS = "[ \t]";             # horizontal whitespace

# Never use C comments in this file!
my $ccs  = '/'.'*';
my $cce  = '*'.'/';
my $rccs = quotemeta $ccs;
my $rcce = quotemeta $cce;

eval {
  require Getopt::Long;
  Getopt::Long::GetOptions(\%opt, qw(
    help quiet diag! filter! hints! changes! cplusplus strip version
    patch=s copy=s diff=s compat-version=s
    list-provided list-unsupported api-info=s
  )) or usage();
};

if ($@ and grep /^-/, @ARGV) {
  usage() if "@ARGV" =~ /^--?h(?:elp)?$/;
  die "Getopt::Long not found. Please don't use any options.\n";
}

if ($opt{version}) {
  print "This is $0 $VERSION.\n";
  exit 0;
}

usage() if $opt{help};
strip() if $opt{strip};

if (exists $opt{'compat-version'}) {
  my($r,$v,$s) = eval { parse_version($opt{'compat-version'}) };
  if ($@) {
    die "Invalid version number format: '$opt{'compat-version'}'\n";
  }
  die "Only Perl 5 is supported\n" if $r != 5;
  die "Invalid version number: $opt{'compat-version'}\n" if $v >= 1000 || $s >= 1000;
  $opt{'compat-version'} = sprintf "%d.%03d%03d", $r, $v, $s;
}
else {
  $opt{'compat-version'} = 5;
}

my %API = map { /^(\w+)\|([^|]*)\|([^|]*)\|(\w*)$/
                ? ( $1 => {
                      ($2                  ? ( base     => $2 ) : ()),
                      ($3                  ? ( todo     => $3 ) : ()),
                      (index($4, 'v') >= 0 ? ( varargs  => 1  ) : ()),
                      (index($4, 'p') >= 0 ? ( provided => 1  ) : ()),
                      (index($4, 'n') >= 0 ? ( nothxarg => 1  ) : ()),
                    } )
                : die "invalid spec: $_" } qw(
ASCII_TO_NEED||5.007001|n
AvFILLp|5.004050||p
AvFILL|||
BhkDISABLE||5.021008|
BhkENABLE||5.021008|
BhkENTRY_set||5.021008|
BhkENTRY|||
BhkFLAGS|||
CALL_BLOCK_HOOKS|||
CLASS|||n
CPERLscope|5.005000||p
CX_CURPAD_SAVE|||
CX_CURPAD_SV|||
CopFILEAV|5.006000||p
CopFILEGV_set|5.006000||p
CopFILEGV|5.006000||p
CopFILESV|5.006000||p
CopFILE_set|5.006000||p
CopFILE|5.006000||p
CopSTASHPV_set|5.006000||p
CopSTASHPV|5.006000||p
CopSTASH_eq|5.006000||p
CopSTASH_set|5.006000||p
CopSTASH|5.006000||p
CopyD|5.009002|5.004050|p
Copy|||
CvPADLIST||5.008001|
CvSTASH|||
CvWEAKOUTSIDE|||
DEFSV_set|5.010001||p
DEFSV|5.004050||p
END_EXTERN_C|5.005000||p
ENTER|||
ERRSV|5.004050||p
EXTEND|||
EXTERN_C|5.005000||p
F0convert|||n
FREETMPS|||
GIMME_V||5.004000|n
GIMME|||n
GROK_NUMERIC_RADIX|5.007002||p
G_ARRAY|||
G_DISCARD|||
G_EVAL|||
G_METHOD|5.006001||p
G_NOARGS|||
G_SCALAR|||
G_VOID||5.004000|
GetVars|||
GvAV|||
GvCV|||
GvHV|||
GvSVn|5.009003||p
GvSV|||
Gv_AMupdate||5.011000|
HEf_SVKEY|5.003070||p
HeHASH||5.003070|
HeKEY||5.003070|
HeKLEN||5.003070|
HePV||5.004000|
HeSVKEY_force||5.003070|
HeSVKEY_set||5.004000|
HeSVKEY||5.003070|
HeUTF8|5.010001|5.008000|p
HeVAL||5.003070|
HvENAMELEN||5.015004|
HvENAMEUTF8||5.015004|
HvENAME||5.013007|
HvNAMELEN_get|5.009003||p
HvNAMELEN||5.015004|
HvNAMEUTF8||5.015004|
HvNAME_get|5.009003||p
HvNAME|||
INT2PTR|5.006000||p
IN_LOCALE_COMPILETIME|5.007002||p
IN_LOCALE_RUNTIME|5.007002||p
IN_LOCALE|5.007002||p
IN_PERL_COMPILETIME|5.008001||p
IS_NUMBER_GREATER_THAN_UV_MAX|5.007002||p
IS_NUMBER_INFINITY|5.007002||p
IS_NUMBER_IN_UV|5.007002||p
IS_NUMBER_NAN|5.007003||p
IS_NUMBER_NEG|5.007002||p
IS_NUMBER_NOT_INT|5.007002||p
IVSIZE|5.006000||p
IVTYPE|5.006000||p
IVdf|5.006000||p
LEAVE|||
LINKLIST||5.013006|
LVRET|||
MARK|||
MULTICALL||5.021008|
MUTABLE_PTR|5.010001||p
MUTABLE_SV|5.010001||p
MY_CXT_CLONE|5.009002||p
MY_CXT_INIT|5.007003||p
MY_CXT|5.007003||p
MoveD|5.009002|5.004050|p
Move|||
NATIVE_TO_NEED||5.007001|n
NOOP|5.005000||p
NUM2PTR|5.006000||p
NVTYPE|5.006000||p
NVef|5.006001||p
NVff|5.006001||p
NVgf|5.006001||p
Newxc|5.009003||p
Newxz|5.009003||p
Newx|5.009003||p
Nullav|||
Nullch|||
Nullcv|||
Nullhv|||
Nullsv|||
OP_CLASS||5.013007|
OP_DESC||5.007003|
OP_NAME||5.007003|
OP_TYPE_IS_OR_WAS||5.019010|
OP_TYPE_IS||5.019007|
ORIGMARK|||
OpHAS_SIBLING||5.021007|
OpSIBLING_set||5.021007|
OpSIBLING||5.021007|
PAD_BASE_SV|||
PAD_CLONE_VARS|||
PAD_COMPNAME_FLAGS|||
PAD_COMPNAME_GEN_set|||
PAD_COMPNAME_GEN|||
PAD_COMPNAME_OURSTASH|||
PAD_COMPNAME_PV|||
PAD_COMPNAME_TYPE|||
PAD_RESTORE_LOCAL|||
PAD_SAVE_LOCAL|||
PAD_SAVE_SETNULLPAD|||
PAD_SETSV|||
PAD_SET_CUR_NOSAVE|||
PAD_SET_CUR|||
PAD_SVl|||
PAD_SV|||
PERLIO_FUNCS_CAST|5.009003||p
PERLIO_FUNCS_DECL|5.009003||p
PERL_ABS|5.008001||p
PERL_BCDVERSION|5.021008||p
PERL_GCC_BRACE_GROUPS_FORBIDDEN|5.008001||p
PERL_HASH|5.003070||p
PERL_INT_MAX|5.003070||p
PERL_INT_MIN|5.003070||p
PERL_LONG_MAX|5.003070||p
PERL_LONG_MIN|5.003070||p
PERL_MAGIC_arylen|5.007002||p
PERL_MAGIC_backref|5.007002||p
PERL_MAGIC_bm|5.007002||p
PERL_MAGIC_collxfrm|5.007002||p
PERL_MAGIC_dbfile|5.007002||p
PERL_MAGIC_dbline|5.007002||p
PERL_MAGIC_defelem|5.007002||p
PERL_MAGIC_envelem|5.007002||p
PERL_MAGIC_env|5.007002||p
PERL_MAGIC_ext|5.007002||p
PERL_MAGIC_fm|5.007002||p
PERL_MAGIC_glob|5.021008||p
PERL_MAGIC_isaelem|5.007002||p
PERL_MAGIC_isa|5.007002||p
PERL_MAGIC_mutex|5.021008||p
PERL_MAGIC_nkeys|5.007002||p
PERL_MAGIC_overload_elem|5.021008||p
PERL_MAGIC_overload_table|5.007002||p
PERL_MAGIC_overload|5.021008||p
PERL_MAGIC_pos|5.007002||p
PERL_MAGIC_qr|5.007002||p
PERL_MAGIC_regdata|5.007002||p
PERL_MAGIC_regdatum|5.007002||p
PERL_MAGIC_regex_global|5.007002||p
PERL_MAGIC_shared_scalar|5.007003||p
PERL_MAGIC_shared|5.007003||p
PERL_MAGIC_sigelem|5.007002||p
PERL_MAGIC_sig|5.007002||p
PERL_MAGIC_substr|5.007002||p
PERL_MAGIC_sv|5.007002||p
PERL_MAGIC_taint|5.007002||p
PERL_MAGIC_tiedelem|5.007002||p
PERL_MAGIC_tiedscalar|5.007002||p
PERL_MAGIC_tied|5.007002||p
PERL_MAGIC_utf8|5.008001||p
PERL_MAGIC_uvar_elem|5.007003||p
PERL_MAGIC_uvar|5.007002||p
PERL_MAGIC_vec|5.007002||p
PERL_MAGIC_vstring|5.008001||p
PERL_PV_ESCAPE_ALL|5.009004||p
PERL_PV_ESCAPE_FIRSTCHAR|5.009004||p
PERL_PV_ESCAPE_NOBACKSLASH|5.009004||p
PERL_PV_ESCAPE_NOCLEAR|5.009004||p
PERL_PV_ESCAPE_QUOTE|5.009004||p
PERL_PV_ESCAPE_RE|5.009005||p
PERL_PV_ESCAPE_UNI_DETECT|5.009004||p
PERL_PV_ESCAPE_UNI|5.009004||p
PERL_PV_PRETTY_DUMP|5.009004||p
PERL_PV_PRETTY_ELLIPSES|5.010000||p
PERL_PV_PRETTY_LTGT|5.009004||p
PERL_PV_PRETTY_NOCLEAR|5.010000||p
PERL_PV_PRETTY_QUOTE|5.009004||p
PERL_PV_PRETTY_REGPROP|5.009004||p
PERL_QUAD_MAX|5.003070||p
PERL_QUAD_MIN|5.003070||p
PERL_REVISION|5.006000||p
PERL_SCAN_ALLOW_UNDERSCORES|5.007003||p
PERL_SCAN_DISALLOW_PREFIX|5.007003||p
PERL_SCAN_GREATER_THAN_UV_MAX|5.007003||p
PERL_SCAN_SILENT_ILLDIGIT|5.008001||p
PERL_SHORT_MAX|5.003070||p
PERL_SHORT_MIN|5.003070||p
PERL_SIGNALS_UNSAFE_FLAG|5.008001||p
PERL_SUBVERSION|5.006000||p
PERL_SYS_INIT3||5.006000|
PERL_SYS_INIT|||
PERL_SYS_TERM||5.021008|
PERL_UCHAR_MAX|5.003070||p
PERL_UCHAR_MIN|5.003070||p
PERL_UINT_MAX|5.003070||p
PERL_UINT_MIN|5.003070||p
PERL_ULONG_MAX|5.003070||p
PERL_ULONG_MIN|5.003070||p
PERL_UNUSED_ARG|5.009003||p
PERL_UNUSED_CONTEXT|5.009004||p
PERL_UNUSED_DECL|5.007002||p
PERL_UNUSED_VAR|5.007002||p
PERL_UQUAD_MAX|5.003070||p
PERL_UQUAD_MIN|5.003070||p
PERL_USE_GCC_BRACE_GROUPS|5.009004||p
PERL_USHORT_MAX|5.003070||p
PERL_USHORT_MIN|5.003070||p
PERL_VERSION|5.006000||p
PL_DBsignal|5.005000||p
PL_DBsingle|||pn
PL_DBsub|||pn
PL_DBtrace|||pn
PL_Sv|5.005000||p
PL_bufend|5.021008||p
PL_bufptr|5.021008||p
PL_check||5.006000|
PL_compiling|5.004050||p
PL_comppad_name||5.017004|
PL_comppad||5.008001|
PL_copline|5.021008||p
PL_curcop|5.004050||p
PL_curpad||5.005000|
PL_curstash|5.004050||p
PL_debstash|5.004050||p
PL_defgv|5.004050||p
PL_diehook|5.004050||p
PL_dirty|5.004050||p
PL_dowarn|||pn
PL_errgv|5.004050||p
PL_error_count|5.021008||p
PL_expect|5.021008||p
PL_hexdigit|5.005000||p
PL_hints|5.005000||p
PL_in_my_stash|5.021008||p
PL_in_my|5.021008||p
PL_keyword_plugin||5.011002|
PL_last_in_gv|||n
PL_laststatval|5.005000||p
PL_lex_state|5.021008||p
PL_lex_stuff|5.021008||p
PL_linestr|5.021008||p
PL_modglobal||5.005000|n
PL_na|5.004050||pn
PL_no_modify|5.006000||p
PL_ofsgv|||n
PL_opfreehook||5.011000|n
PL_parser|5.009005||p
PL_peepp||5.007003|n
PL_perl_destruct_level|5.004050||p
PL_perldb|5.004050||p
PL_ppaddr|5.006000||p
PL_rpeepp||5.013005|n
PL_rsfp_filters|5.021008||p
PL_rsfp|5.021008||p
PL_rs|||n
PL_signals|5.008001||p
PL_stack_base|5.004050||p
PL_stack_sp|5.004050||p
PL_statcache|5.005000||p
PL_stdingv|5.004050||p
PL_sv_arenaroot|5.004050||p
PL_sv_no|5.004050||pn
PL_sv_undef|5.004050||pn
PL_sv_yes|5.004050||pn
PL_tainted|5.004050||p
PL_tainting|5.004050||p
PL_tokenbuf|5.021008||p
POP_MULTICALL||5.021008|
POPi|||n
POPl|||n
POPn|||n
POPpbytex||5.007001|n
POPpx||5.005030|n
POPp|||n
POPs|||n
PTR2IV|5.006000||p
PTR2NV|5.006000||p
PTR2UV|5.006000||p
PTR2nat|5.009003||p
PTR2ul|5.007001||p
PTRV|5.006000||p
PUSHMARK|||
PUSH_MULTICALL||5.021008|
PUSHi|||
PUSHmortal|5.009002||p
PUSHn|||
PUSHp|||
PUSHs|||
PUSHu|5.004000||p
PUTBACK|||
PadARRAY||5.021008|
PadMAX||5.021008|
PadlistARRAY||5.021008|
PadlistMAX||5.021008|
PadlistNAMESARRAY||5.021008|
PadlistNAMESMAX||5.021008|
PadlistNAMES||5.021008|
PadlistREFCNT||5.017004|
PadnameIsOUR|||
PadnameIsSTATE|||
PadnameLEN||5.021008|
PadnameOURSTASH|||
PadnameOUTER|||
PadnamePV||5.021008|
PadnameREFCNT_dec||5.021008|
PadnameREFCNT||5.021008|
PadnameSV||5.021008|
PadnameTYPE|||
PadnameUTF8||5.021007|
PadnamelistARRAY||5.021008|
PadnamelistMAX||5.021008|
PadnamelistREFCNT_dec||5.021008|
PadnamelistREFCNT||5.021008|
PerlIO_clearerr||5.007003|
PerlIO_close||5.007003|
PerlIO_context_layers||5.009004|
PerlIO_eof||5.007003|
PerlIO_error||5.007003|
PerlIO_fileno||5.007003|
PerlIO_fill||5.007003|
PerlIO_flush||5.007003|
PerlIO_get_base||5.007003|
PerlIO_get_bufsiz||5.007003|
PerlIO_get_cnt||5.007003|
PerlIO_get_ptr||5.007003|
PerlIO_read||5.007003|
PerlIO_restore_errno|||
PerlIO_save_errno|||
PerlIO_seek||5.007003|
PerlIO_set_cnt||5.007003|
PerlIO_set_ptrcnt||5.007003|
PerlIO_setlinebuf||5.007003|
PerlIO_stderr||5.007003|
PerlIO_stdin||5.007003|
PerlIO_stdout||5.007003|
PerlIO_tell||5.007003|
PerlIO_unread||5.007003|
PerlIO_write||5.007003|
Perl_signbit||5.009005|n
PoisonFree|5.009004||p
PoisonNew|5.009004||p
PoisonWith|5.009004||p
Poison|5.008000||p
READ_XDIGIT||5.017006|
RETVAL|||n
Renewc|||
Renew|||
SAVECLEARSV|||
SAVECOMPPAD|||
SAVEPADSV|||
SAVETMPS|||
SAVE_DEFSV|5.004050||p
SPAGAIN|||
SP|||
START_EXTERN_C|5.005000||p
START_MY_CXT|5.007003||p
STMT_END|||p
STMT_START|||p
STR_WITH_LEN|5.009003||p
ST|||
SV_CONST_RETURN|5.009003||p
SV_COW_DROP_PV|5.008001||p
SV_COW_SHARED_HASH_KEYS|5.009005||p
SV_GMAGIC|5.007002||p
SV_HAS_TRAILING_NUL|5.009004||p
SV_IMMEDIATE_UNREF|5.007001||p
SV_MUTABLE_RETURN|5.009003||p
SV_NOSTEAL|5.009002||p
SV_SMAGIC|5.009003||p
SV_UTF8_NO_ENCODING|5.008001||p
SVfARG|5.009005||p
SVf_UTF8|5.006000||p
SVf|5.006000||p
SVt_INVLIST||5.019002|
SVt_IV|||
SVt_NULL|||
SVt_NV|||
SVt_PVAV|||
SVt_PVCV|||
SVt_PVFM|||
SVt_PVGV|||
SVt_PVHV|||
SVt_PVIO|||
SVt_PVIV|||
SVt_PVLV|||
SVt_PVMG|||
SVt_PVNV|||
SVt_PV|||
SVt_REGEXP||5.011000|
Safefree|||
Slab_Alloc|||
Slab_Free|||
Slab_to_ro|||
Slab_to_rw|||
StructCopy|||
SvCUR_set|||
SvCUR|||
SvEND|||
SvGAMAGIC||5.006001|
SvGETMAGIC|5.004050||p
SvGROW|||
SvIOK_UV||5.006000|
SvIOK_notUV||5.006000|
SvIOK_off|||
SvIOK_only_UV||5.006000|
SvIOK_only|||
SvIOK_on|||
SvIOKp|||
SvIOK|||
SvIVX|||
SvIV_nomg|5.009001||p
SvIV_set|||
SvIVx|||
SvIV|||
SvIsCOW_shared_hash||5.008003|
SvIsCOW||5.008003|
SvLEN_set|||
SvLEN|||
SvLOCK||5.007003|
SvMAGIC_set|5.009003||p
SvNIOK_off|||
SvNIOKp|||
SvNIOK|||
SvNOK_off|||
SvNOK_only|||
SvNOK_on|||
SvNOKp|||
SvNOK|||
SvNVX|||
SvNV_nomg||5.013002|
SvNV_set|||
SvNVx|||
SvNV|||
SvOK|||
SvOOK_offset||5.011000|
SvOOK|||
SvPOK_off|||
SvPOK_only_UTF8||5.006000|
SvPOK_only|||
SvPOK_on|||
SvPOKp|||
SvPOK|||
SvPVX_const|5.009003||p
SvPVX_mutable|5.009003||p
SvPVX|||
SvPV_const|5.009003||p
SvPV_flags_const_nolen|5.009003||p
SvPV_flags_const|5.009003||p
SvPV_flags_mutable|5.009003||p
SvPV_flags|5.007002||p
SvPV_force_flags_mutable|5.009003||p
SvPV_force_flags_nolen|5.009003||p
SvPV_force_flags|5.007002||p
SvPV_force_mutable|5.009003||p
SvPV_force_nolen|5.009003||p
SvPV_force_nomg_nolen|5.009003||p
SvPV_force_nomg|5.007002||p
SvPV_force|||p
SvPV_mutable|5.009003||p
SvPV_nolen_const|5.009003||p
SvPV_nolen|5.006000||p
SvPV_nomg_const_nolen|5.009003||p
SvPV_nomg_const|5.009003||p
SvPV_nomg_nolen|5.013007||p
SvPV_nomg|5.007002||p
SvPV_renew|5.009003||p
SvPV_set|||
SvPVbyte_force||5.009002|
SvPVbyte_nolen||5.006000|
SvPVbytex_force||5.006000|
SvPVbytex||5.006000|
SvPVbyte|5.006000||p
SvPVutf8_force||5.006000|
SvPVutf8_nolen||5.006000|
SvPVutf8x_force||5.006000|
SvPVutf8x||5.006000|
SvPVutf8||5.006000|
SvPVx|||
SvPV|||
SvREFCNT_dec_NN||5.017007|
SvREFCNT_dec|||
SvREFCNT_inc_NN|5.009004||p
SvREFCNT_inc_simple_NN|5.009004||p
SvREFCNT_inc_simple_void_NN|5.009004||p
SvREFCNT_inc_simple_void|5.009004||p
SvREFCNT_inc_simple|5.009004||p
SvREFCNT_inc_void_NN|5.009004||p
SvREFCNT_inc_void|5.009004||p
SvREFCNT_inc|||p
SvREFCNT|||
SvROK_off|||
SvROK_on|||
SvROK|||
SvRV_set|5.009003||p
SvRV|||
SvRXOK||5.009005|
SvRX||5.009005|
SvSETMAGIC|||
SvSHARED_HASH|5.009003||p
SvSHARE||5.007003|
SvSTASH_set|5.009003||p
SvSTASH|||
SvSetMagicSV_nosteal||5.004000|
SvSetMagicSV||5.004000|
SvSetSV_nosteal||5.004000|
SvSetSV|||
SvTAINTED_off||5.004000|
SvTAINTED_on||5.004000|
SvTAINTED||5.004000|
SvTAINT|||
SvTHINKFIRST|||
SvTRUE_nomg||5.013006|
SvTRUE|||
SvTYPE|||
SvUNLOCK||5.007003|
SvUOK|5.007001|5.006000|p
SvUPGRADE|||
SvUTF8_off||5.006000|
SvUTF8_on||5.006000|
SvUTF8||5.006000|
SvUVXx|5.004000||p
SvUVX|5.004000||p
SvUV_nomg|5.009001||p
SvUV_set|5.009003||p
SvUVx|5.004000||p
SvUV|5.004000||p
SvVOK||5.008001|
SvVSTRING_mg|5.009004||p
THIS|||n
UNDERBAR|5.009002||p
UTF8_MAXBYTES|5.009002||p
UVSIZE|5.006000||p
UVTYPE|5.006000||p
UVXf|5.007001||p
UVof|5.006000||p
UVuf|5.006000||p
UVxf|5.006000||p
WARN_ALL|5.006000||p
WARN_AMBIGUOUS|5.006000||p
WARN_ASSERTIONS|5.021008||p
WARN_BAREWORD|5.006000||p
WARN_CLOSED|5.006000||p
WARN_CLOSURE|5.006000||p
WARN_DEBUGGING|5.006000||p
WARN_DEPRECATED|5.006000||p
WARN_DIGIT|5.006000||p
WARN_EXEC|5.006000||p
WARN_EXITING|5.006000||p
WARN_GLOB|5.006000||p
WARN_INPLACE|5.006000||p
WARN_INTERNAL|5.006000||p
WARN_IO|5.006000||p
WARN_LAYER|5.008000||p
WARN_MALLOC|5.006000||p
WARN_MISC|5.006000||p
WARN_NEWLINE|5.006000||p
WARN_NUMERIC|5.006000||p
WARN_ONCE|5.006000||p
WARN_OVERFLOW|5.006000||p
WARN_PACK|5.006000||p
WARN_PARENTHESIS|5.006000||p
WARN_PIPE|5.006000||p
WARN_PORTABLE|5.006000||p
WARN_PRECEDENCE|5.006000||p
WARN_PRINTF|5.006000||p
WARN_PROTOTYPE|5.006000||p
WARN_QW|5.006000||p
WARN_RECURSION|5.006000||p
WARN_REDEFINE|5.006000||p
WARN_REGEXP|5.006000||p
WARN_RESERVED|5.006000||p
WARN_SEMICOLON|5.006000||p
WARN_SEVERE|5.006000||p
WARN_SIGNAL|5.006000||p
WARN_SUBSTR|5.006000||p
WARN_SYNTAX|5.006000||p
WARN_TAINT|5.006000||p
WARN_THREADS|5.008000||p
WARN_UNINITIALIZED|5.006000||p
WARN_UNOPENED|5.006000||p
WARN_UNPACK|5.006000||p
WARN_UNTIE|5.006000||p
WARN_UTF8|5.006000||p
WARN_VOID|5.006000||p
WIDEST_UTYPE|5.015004||p
XCPT_CATCH|5.009002||p
XCPT_RETHROW|5.009002||p
XCPT_TRY_END|5.009002||p
XCPT_TRY_START|5.009002||p
XPUSHi|||
XPUSHmortal|5.009002||p
XPUSHn|||
XPUSHp|||
XPUSHs|||
XPUSHu|5.004000||p
XSPROTO|5.010000||p
XSRETURN_EMPTY|||
XSRETURN_IV|||
XSRETURN_NO|||
XSRETURN_NV|||
XSRETURN_PV|||
XSRETURN_UNDEF|||
XSRETURN_UV|5.008001||p
XSRETURN_YES|||
XSRETURN|||p
XST_mIV|||
XST_mNO|||
XST_mNV|||
XST_mPV|||
XST_mUNDEF|||
XST_mUV|5.008001||p
XST_mYES|||
XS_APIVERSION_BOOTCHECK||5.021008|
XS_EXTERNAL||5.021008|
XS_INTERNAL||5.021008|
XS_VERSION_BOOTCHECK||5.021008|
XS_VERSION|||
XSprePUSH|5.006000||p
XS|||
XopDISABLE||5.021008|
XopENABLE||5.021008|
XopENTRYCUSTOM||5.021008|
XopENTRY_set||5.021008|
XopENTRY||5.021008|
XopFLAGS||5.013007|
ZeroD|5.009002||p
Zero|||
_aMY_CXT|5.007003||p
_add_range_to_invlist|||
_append_range_to_invlist|||
_core_swash_init|||
_get_encoding|||
_get_regclass_nonbitmap_data|||
_get_swash_invlist|||
_invlist_array_init|||n
_invlist_contains_cp|||n
_invlist_contents|||
_invlist_dump|||
_invlist_intersection_maybe_complement_2nd|||
_invlist_intersection|||
_invlist_invert|||
_invlist_len|||n
_invlist_populate_swatch|||n
_invlist_search|||n
_invlist_subtract|||
_invlist_union_maybe_complement_2nd|||
_invlist_union|||
_is_cur_LC_category_utf8|||
_is_in_locale_category||5.021001|
_is_uni_FOO||5.017008|
_is_uni_perl_idcont||5.017008|
_is_uni_perl_idstart||5.017007|
_is_utf8_FOO||5.017008|
_is_utf8_char_slow||5.021001|n
_is_utf8_idcont||5.021001|
_is_utf8_idstart||5.021001|
_is_utf8_mark||5.017008|
_is_utf8_perl_idcont||5.017008|
_is_utf8_perl_idstart||5.017007|
_is_utf8_xidcont||5.021001|
_is_utf8_xidstart||5.021001|
_load_PL_utf8_foldclosures|||
_make_exactf_invlist|||
_new_invlist_C_array|||
_new_invlist|||
_pMY_CXT|5.007003||p
_setup_canned_invlist|||
_swash_inversion_hash|||
_swash_to_invlist|||
_to_fold_latin1|||
_to_uni_fold_flags||5.014000|
_to_upper_title_latin1|||
_to_utf8_fold_flags||5.019009|
_to_utf8_lower_flags||5.019009|
_to_utf8_title_flags||5.019009|
_to_utf8_upper_flags||5.019009|
aMY_CXT_|5.007003||p
aMY_CXT|5.007003||p
aTHXR_|5.021008||p
aTHXR|5.021008||p
aTHX_|5.006000||p
aTHX|5.006000||p
aassign_common_vars|||
add_above_Latin1_folds|||
add_cp_to_invlist|||
add_data|||n
add_multi_match|||
add_utf16_textfilter|||
adjust_size_and_find_bucket|||n
alloc_maybe_populate_EXACT|||
alloccopstash|||
allocmy|||
amagic_call|||
amagic_cmp_locale|||
amagic_cmp|||