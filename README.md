
[![Build Status](https://travis-ci.org/SeismicData/asdf-library.svg)](https://travis-ci.org/SeismicData/asdf-library)

#### Instruction to install the hdf5 parallel library on mac.

Remove everything hdf5 and mpi related from conda

```
$ conda uninstall hdf5 h5py mpi4py
```
Install mpi and parallel hdf5 somehow - on mac i strongly recommend homebrew.

```
$ brew install open-mpi
$ brew tap homebrew/science
$ brew install hdf5 --with-mpi
```

Then - with the pip in the conda (environement)
Will make sure mpi4py is linked against your systems MPI

```
$ pip install mpi4py
```

Download h5py and unpack it and make sure the conda env is active

```
$ python setup.py configure --mpi
$ python setup.py build
$ python setup.py install
```

This will then install a parallel h5py version.

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

If you want to get cmake to find your local boost directory, try:

```bash
$ cmake -DBoost_NO_BOOST_CMAKE=TRUE \
    -DBoost_NO_SYSTEM_PATHS=TRUE \
    -DBOOST_ROOT:PATHNAME=$TARGET \
    -DBoost_LIBRARY_DIRS:FILEPATH=${TARGET}/lib
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

To install in a nonstandard location (e.g., a user directory without root
privileges), you can either set ``CMAKE_INSTALL_PREFIX`` during configuration
or ``DESTDIR`` during install, e.g.,

```bash
$ cmake -DCMAKE_INSTALL_PREFIX=/path/to/toplevel/install/directory <options> .
$ make
$ make install
```
or
```bash
$ cmake <options> .
$ make
$ make DESTDIR=/path/to/toplevel/install/directory install
```

Note that the files end up in ``$DESTDIR/$CMAKE_INSTALL_PREFIX/*``, so it is
only necessary to set one of these.


#### Titan compilation

##### modules
```
cmake/2.8.10 (or higher)
PrgEnv-gnu/5.2.40
cray-hdf5-parallel
boost/1.57.0 (or higher)
```
##### export hdf5
export PATH=$PATH:/opt/cray/hdf5-parallel/1.8.14/gnu/48
#####

##### configuration

```
cd asdf-library
Uncomment line 27 in CMakeLists.txt (Delete the # symbol)
mkdir build
cd build
cmake .. -DCMAKE_Fortran_COMPILER=ftn -DBoost_NO_SYSTEM_PATHS=TRUE
```
