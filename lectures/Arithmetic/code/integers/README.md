This example tests whether you understand the integer promotion
rules in C – an area that I find is very poorly understood by many
developers.

The answer is that this program outputs “-20 > 6″. The
reason for this is that expressions involving signed and unsigned
types have all operands promoted to unsigned types. Thus -20 becomes
a very large positive integer and the expression evaluates to greater
than 6. 

If you get this one wrong, then you are perilously close to not
being hired.
