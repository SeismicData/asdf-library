#### Usage

```bash
$ cmake .
$ make 
```

Should create a library in ```lib/``
examples programs are in test/ and generated there.

If documentation needs to be generated:
```
cmake -DBUILD_DOCUMENTATION=ON
```
You should have doxygen installed. Documentation will be generated in ```doc/```.
