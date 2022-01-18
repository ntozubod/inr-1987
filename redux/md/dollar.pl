#!/usr/bin/env perl
use warnings;
use strict;
use utf8;

# my $s1 =
#    '<img src="https://render.githubusercontent.com/render/math?math=';
# my $s2 = '">';

my $s1 = '[[';
my $s2 = ']]';

open( AA, "< :encoding(UTF-8)", "work.md" );
open( BB, "> :encoding(UTF-8)", "expand.md" );
while ( my $aa = <AA> ) {
  $aa =~ s/\s*$//;
  my $bb = $aa;
  if ( $bb =~ /\$[^\$]+\$/ ) {
    if ( ! ( $bb =~ /\( \(S\$0\)-\(R\$0\) @@ S \)/ ) ) {
      if ( ! ( $bb =~ /`\$`/ ) ) {
        while ( $bb =~ s/\$\$([^\$]+)\$\$/$s1 . $1 . $s2/eg  ) { }
        while ( $bb =~ s/\$([^\$]+)\$/$s1 . $1 . $s2/eg  ) { }
      }
    }
  }
  print BB $bb, "\n";
}
close( AA );
close( BB );
