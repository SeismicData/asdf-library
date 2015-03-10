#include <hdf5.h>

#include "ASDF_init.h"

void ASDF_initialize_hdf5() {
  H5open();
}

void ASDF_finalize_hdf5() {
  H5close();
}
