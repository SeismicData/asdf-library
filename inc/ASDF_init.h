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
 * @file ASDF_init.h
 * @brief
 * @author Matthieu Lefebvre
 */

#ifndef _ASDF_INIT_H_
#define _ASDF_INIT_H_

#include <hdf5.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize ASDF by initializing HDF5.
 *
 * @return 0 for success, -1 for failure.
 */
int ASDF_initialize_hdf5(void);

/**
 * @brief Finalize ASDF by finalizing HDF5.
 *
 * @return 0 for success, -1 for failure.
 */
int ASDF_finalize_hdf5(void);

/**
 * @brief Close a given HDF5 file
 *
 * @param file_id The file to be closed.
 *
 * @return 0 for success.
 */

int ASDF_close_file(hid_t file_id);

#ifdef __cplusplus
}
#endif

#endif
