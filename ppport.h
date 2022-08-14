
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
