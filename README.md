
[![Build Status](https://travis-ci.org/SeismicData/ASDF_f90.svg)](https://travis-ci.org/SeismicData/ASDF_f90)

#### Usage

The following instructions are for in-source builds.

```bash
$ cmake .
$ make 
```

Finding the correct MPI / HDF5 libraries does not seem to be working all the 
same on different systems.

If the compilation complains about a missing ``use mpi``, try the following:

```bash
$ cmake . -DCMAKE_Fortran_COMPILER=mpif90
$ make 
```

If the compilation still complains about missing hdf5 libraries, try:

```bash
$ cmake . -DCMAKE_Fortran_COMPILER=h5pfc
$ make 
```

It should create a library in ``lib/``
examples programs are in ``test/`` and generated there.

If documentation needs to be generated:
```
cmake -DBUILD_DOCUMENTATION=ON
```
You should have doxygen installed. Documentation will be generated in ``doc/``.

For out-of source builds, start with:
```bash
$ mkdir build/
$ cd build/
```

and follow the previous instructions, replacing ``.`` with ``..``

Once build, you may run test with:
```bash
$ make test
```
