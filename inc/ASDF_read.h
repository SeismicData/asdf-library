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
 * @file ASDF_read.h
 * @brief Routines to read values from ASDF files.
 * @author Matthieu Lefebvre
 */


#ifndef _ASDF_READ_H_
#define _ASDF_READ_H_

#include <mpi.h>
#include <hdf5.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Open an existing ASDF file in read-only mode.
 *
 * @param filename Name of the ASDF file to open
 * @param comm The communicator in which the file will be opened.
 *
 * @return The id of the opened file.
 */
hid_t ASDF_open_read_only(const char *filename, MPI_Comm comm);

/**
 * @brief Read the value of \p attr_value of a string attribute \p attr_name
 *        within the HDF5 group \p grp_name.
 *
 * @param file_id Id of the file to read the attribute from.
 * @param grp_name Path / name of the HDF5 group containing the attribute.
 * @param attr_name Name of the attribute
 * @param attr_value Value read
 *
 * @return 0 for success
 *
 * @note \p attr_value will be allocated within this routine. Users are
 *       responsible for deallocating it.
 */
int ASDF_read_str_attr(hid_t file_id, const char *grp_name,
                       const char *attr_name, char **attr_value);

/**
 * @brief Get the number of elements of a dataset (i.e. array)
 *
 * @param dataset_id Id of the dataset to get the the number of element.
 *
 * @return Number of the elements in the dataset
 */
int ASDF_get_num_elements_dataset(hid_t dataset_id);

/**
 * @brief Get the number of elements of a dataset from its path.
 *
 * @param file_id Id of the file containing the dataset.
 * @param path Path of the dataset in the HDF5 file.
 *
 * @return 0 for success
 */
int ASDF_get_num_elements_from_path(hid_t file_id, const char *path);

/**
 * @brief Read a complete waveform from a dataset
 * 
 * @param file_id Id of the file containing the waveform.
 * @param path The path to the waveform
 * @param waveform The array to read the waveform in.
 *
 * @return 0 for success
 *
 * @note \p waveform should be preallocated and large enough to store
 *       the data in full.
 */
int ASDF_read_full_waveform(hid_t file_id, const char *path,
							float * const waveform);

/**
 * @brief Read a partial waveform from a dataset
 * 
 * @param file_id Id of the file containing the waveform.
 * @param path The path to the waveform
 * @param offset Where to start reading in the dataset
 * @param nsamples How many elements to read 
 * @param waveform The array to read the waveform in.
 *
 * @return 0 for success
 *
 * @note \p waveform should be preallocated and large enough to store
 *       the nsamples of data.
 */
int ASDF_read_partial_waveform(hid_t file_id, const char *path, int offset,
                               int nsamples, float * const waveform);

/**
 * @brief Build a string "path/name"
 *
 * @param path The base path
 * @param name The name to append to path
 *
 * @return A pointer to the "path/name" string.
 *
 * @note Allocates the space needed to build the string. Should be
 *       deallocated by the user.
 */
char *ASDF_extend_path(const char *path, const char *name);

/** 
 * @brief Check if there is an object \p name under \p path in file \p file_id
 * 
 * @param file_id The identifier of the file to look in
 * @param path The path where name is look for.
 * @param name The name being looked for
 * 
 * @return > 0 if present, 0 if not, < 0 in case of failure
 */
int ASDF_exists_in_path(hid_t file_id, const char *path, const char *name);

/** 
 * @brief Check if a station called \p name exists in file \p file_id
 * 
 * @param file_id The identifier of the file to look in
 * @param name The station name being checked.
 * 
 * @return  
 *
 * @note It is looking for a variable `"/Waveforms/<name>"`
 */
int ASDF_station_exists(hid_t file_id, const char *name);

/** 
 * @brief Check if a waveform \p waveform_name is present for station
 *        \p station_name in file \p file_id
 * 
 * @param file_id
 * @param station_name
 * @param waveform_name
 * 
 * @return > 0 if present, 0 if not, < 0 in case of failure
 */

int ASDF_waveform_exists(hid_t file_id, const char *station_name, 
                         const char *waveform_name);

#ifdef __cplusplus
}
#endif

#endif
