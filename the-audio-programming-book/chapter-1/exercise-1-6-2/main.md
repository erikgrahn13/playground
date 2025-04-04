### a)
```
iscale -m -i -m 19 60
```

- -m will be interpretet as the -m option
- -i will be interpreted as the -i option
- -m will be interpreted as the -m option 
- 19 will be interpreted as N
- 60 will be interpreted as startval
---
### b)
```
iscale -i - 19 60
```
- -i will be interpreted as -i option
- \- will be interpreted as an "unrecognized option"
---
### c)
```
iscale m 19 60
```
- m will be interpreted as N
- 19 will not be interpreted
- 60 will not be interpreted
---
### d)
```
iscale 19 data.txt
```
- 19 will be interpreted as N.
- data.txt will be interpreted as the startval