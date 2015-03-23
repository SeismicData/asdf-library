#include <hdf5.h>

#include "ASDF_init.h"

int ASDF_initialize_hdf5() {
  return H5open();
}

int ASDF_finalize_hdf5() {
  return H5close();
}

int ASDF_close_file(hid_t file_id) {
  return H5Fclose(file_id);
}
