### (a) 
```
music.c:5:5: error: call to undeclared library function 'printf' with type 'int (const char *, ...)'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
    printf("Hello The Audio Programming Book\n");
    ^
music.c:5:5: note: include the header <stdio.h> or explicitly provide a declaration for 'printf'
1 error generated.
```
---
### (b)
```
music.c:1:10: error: expected "FILENAME" or <FILENAME>
#include stdio.h
         ^
music.c:5:5: error: call to undeclared library function 'printf' with type 'int (const char *, ...)'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
    printf("Hello The Audio Programming Book\n");
    ^
music.c:5:5: note: include the header <stdio.h> or explicitly provide a declaration for 'printf'
2 errors generated`
```
---
### (c)
```
music.c:3:1: warning: variable named 'main' with external linkage has undefined behavior [-Wmain]
int main{}
^
music.c:3:9: error: expected ';' after top level declarator
int main{}
        ^
        ;
1 warning and 1 error generated.
```
---
### (d)
```
music.c:5:49: error: expected ';' after expression
    printf("Hello The Audio Programming Book\n")
                                                ^
                                                ;
music.c:6:13: error: expected ';' after return statement
    return 0
            ^
            ;
2 errors generated.
```
---
### (e)
```
music.c:6:5: error: expected expression
    return 0;
    ^
1 error generated.
```
---
### (f)
No compiler output
---
### (g)
```
music.c:6:14: error: expected '}'
    return 0;
             ^
music.c:4:1: note: to match this '{'
{
^
1 error generated.
```

