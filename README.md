# Mylang-Interpreter
Interpreter for a simple custom language called Mylang using flex and bison.
Variables start with a lower case letter and always followed by exactly 2 decimal numbers. For example a11. The variables are all numeric type variables. If they start with a to n, they are treated as signed integer variables by default. 
For variables starting from o to z, are by default double precision floating points variables. 
The language has arithmetic propagation, binary operators( +, -, *, / and also exponential)
The first 4 are left associative, exponential is right associative 
a^b^c = a^(b^c)
Presidense: exp, (*,/), (+,-)
Mixed mode expression will happen as C

Automatic type conversion to one to which it is assigned. 
We will have logical operators like in C and ! For now. 

You also have if and if else like in C and you have a while loop like in c. 
For, Switch case is *optional*. 
All the comparison operators from C are there <,>, ≥ etc. 
{} Is present in the beginning and end of the entire program. 
; Is the statement Terminator. 

Anything else not mentioned, assume it's like C. 
Nothing called functions, nothing called declaration.
