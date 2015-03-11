#include <hdf5.h>

#include "ASDF_init.h"

int ASDF_initialize_hdf5() {
  return H5open();
}

int ASDF_finalize_hdf5() {
  return H5close();
}
