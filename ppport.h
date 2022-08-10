
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