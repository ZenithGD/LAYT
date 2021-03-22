 /*
	Basic preprocessor for LAYT files
	TODO: Allow splitting instruction layouts in multiple lines syntax
		  for more flexibility
 */
%{
	#include <stdio.h>
%}

%x COMM
%%
^(?:[\t ]*(?:\r?\n|\r))+
 												
\<								{ BEGIN COMM; }
<COMM>\>[\t \n]*				{ 
									BEGIN INITIAL; 
								}
<COMM>(.|\n)
^[ \t]+					
[ \t]+					{ printf(" "); }
.						{ printf("%s", yytext); }
%%