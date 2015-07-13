/******************************************************************************
 * Copyright 2015 ASDF developers
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/
/**
 * @file ASDF_init.c
 * @author Matthieu Lefebvre
 */

#include <hdf5.h>

#include "ASDF_init.h"

int ASDF_initialize_hdf5(void) {
  return H5open();
}

int ASDF_finalize_hdf5(void) {
  return H5close();
}

int ASDF_close_file(hid_t file_id) {
  return H5Fclose(file_id);
}
