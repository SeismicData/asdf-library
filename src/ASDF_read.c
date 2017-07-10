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
 * @file ASDF_read.c
 * @author Matthieu Lefebvre
 */

#include <stdlib.h>
#include <string.h>

#define H5Dopen_vers 2
#include <hdf5.h>

#include "ASDF_read.h"
#include "ASDF_common.h"

hid_t ASDF_open_read_only(const char *filename, MPI_Comm comm) {
   hid_t plist_id, file_id;

   CHK_H5(plist_id = H5Pcreate (H5P_FILE_ACCESS));
   CHK_H5(H5Pset_fapl_mpio(plist_id, comm, MPI_INFO_NULL));
   CHK_H5(file_id = H5Fopen (filename, H5F_ACC_RDONLY, plist_id));
   CHK_H5(H5Pclose(plist_id));

   return file_id;
}

hid_t ASDF_open(const char *filename, MPI_Comm comm) {
   hid_t plist_id, file_id;

   // MPI_Info info;
   // MPI_Info_create(&info);
   // MPI_Info_set(info, "striping_unit", "8388608") ;
   //   /* or whatever your GPFS block size actually is*/

   CHK_H5(plist_id = H5Pcreate (H5P_FILE_ACCESS));
   // H5Pset_driver(plist_id,H5FD_MPIO);

   CHK_H5(H5Pset_fapl_mpio(plist_id, comm, MPI_INFO_NULL));
   // CHK_H5(H5Pset_fapl_mpio(plist_id, comm, info));

   CHK_H5(file_id = H5Fopen (filename, H5F_ACC_RDWR, plist_id));
   CHK_H5(H5Pclose(plist_id));

   return file_id;
}

hid_t ASDF_open_serial(const char *filename) {
   hid_t plist_id, file_id;

   CHK_H5(plist_id = H5Pcreate (H5P_FILE_ACCESS));
   CHK_H5(file_id = H5Fopen (filename, H5F_ACC_RDWR, plist_id));
   CHK_H5(H5Pclose(plist_id));

   return file_id;
}

int ASDF_read_str_attr(hid_t file_id, const char *grp_name,
                       const char *attr_name, char **attr_value) {
    hid_t attr_id, type;
    size_t size;
    int success = 0;

    CHK_H5(attr_id = H5Aopen_by_name(file_id, grp_name, attr_name, H5P_DEFAULT, H5P_DEFAULT));
    CHK_H5(type = H5Aget_type(attr_id));
    CHK_H5(size = H5Tget_size(type));
    size++;  // make a space for null terminator
    *attr_value= (char *) malloc(size* sizeof(char));
    CHK_H5(type = H5Tcopy(H5T_C_S1));
    CHK_H5(H5Tset_size(type, size));
    if (H5Aread(attr_id, type, *attr_value) >= 0)
      success = -1;
    else
      free(*attr_value);
    CHK_H5(H5Tclose(type));
    CHK_H5(H5Aclose(attr_id));
    if (!success)
        *attr_value= NULL;
    return success;
}

int ASDF_get_num_elements_dataset(hid_t dataset_id) {
    hid_t dspace;

    dspace = H5Dget_space(dataset_id);

    // get the number of dimensions
    const int ndims = H5Sget_simple_extent_ndims(dspace);
    hsize_t dims[ndims];

    // get the size of each dimension
    H5Sget_simple_extent_dims(dspace, dims, NULL);

    // returns the size of the first dimension
    return (int) dims[0];
}

int ASDF_get_num_elements_from_path(hid_t file_id, const char *path) {
    hid_t dataset_id;
    int num_elems;

    CHK_H5(dataset_id = H5Dopen(file_id, path, H5P_DEFAULT));
    CHK_H5(num_elems = ASDF_get_num_elements_dataset(dataset_id));

    CHK_H5(H5Dclose(dataset_id));
    return num_elems;
}

int ASDF_read_full_waveform(hid_t file_id, const char *path,
                            float * const waveform) {
    hid_t dataset_id;
    CHK_H5(dataset_id = H5Dopen(file_id, path, H5P_DEFAULT));

    H5Dread(dataset_id, H5T_IEEE_F32LE, H5S_ALL, H5S_ALL, H5P_DEFAULT, waveform);
    CHK_H5(H5Dclose(dataset_id));

    return 0;
}

int ASDF_read_partial_waveform(hid_t file_id, const char *path, int offset,
                               int nsamples, float * const waveform) {
  hid_t dataset_id;
  hid_t filespace_id; // map data from file
  hid_t memspace_id; // map data to buffer

  hsize_t start[1] = {offset};
  hsize_t count[1] = {nsamples};
  hsize_t offset_memspace_id[1] = {0};

  CHK_H5(dataset_id = H5Dopen(file_id, path, H5P_DEFAULT));
  CHK_H5(filespace_id = H5Dget_space(dataset_id));
  CHK_H5(memspace_id = H5Screate_simple(1, count, NULL));
  CHK_H5(H5Sselect_hyperslab(filespace_id, H5S_SELECT_SET, start,
                             NULL, count, NULL));
  CHK_H5(H5Sselect_hyperslab (memspace_id, H5S_SELECT_SET, 
		       offset_memspace_id, NULL, count, NULL));
  CHK_H5(H5Dread(dataset_id, H5T_IEEE_F32LE, memspace_id, filespace_id,
          H5P_DEFAULT, waveform));

  CHK_H5(H5Sclose(memspace_id));
  CHK_H5(H5Sclose(filespace_id));
  CHK_H5(H5Dclose(dataset_id));

  return 0;
}

char *ASDF_extend_path(const char *path, const char *name) {
  // length of full_path (/) => +1, ('\0') => +1
  char *full_path = malloc((strlen(name) + strlen(path) + 2) * sizeof(char));
  sprintf(full_path, "%s/%s", path, name);
  return full_path;
}

int ASDF_exists_in_path(hid_t file_id, const char *path, const char *name) {
  htri_t exists;

  char *full_path = ASDF_extend_path(path, name);
  exists = H5Lexists(file_id, full_path, H5P_DEFAULT);
  free(full_path);
  return (int) exists;
}


int ASDF_station_exists(hid_t file_id, const char *name) {
  return ASDF_exists_in_path(file_id, "Waveforms", name);
}

int ASDF_adjoint_source_exists(hid_t file_id, const char *name) {
  return ASDF_exists_in_path(file_id, "AuxiliaryData/AdjointSources", name);
}

int ASDF_waveform_exists(hid_t file_id, const char *station_name,
                         const char *waveform_name) {
  int station_exists;
  station_exists = ASDF_station_exists(file_id, station_name);

  if (station_exists > 0) {
    char *path = ASDF_extend_path("Waveforms", station_name);
    return ASDF_exists_in_path(file_id, path, waveform_name);
    free(path);
  } else {
    return station_exists;
  }
}

// BS BS
//

hid_t ASDF_open_waveforms_group(hid_t loc_id) {
  hid_t group_id;
  /* Open the group "/Waveforms" */
  CHK_H5(group_id = H5Gopen(loc_id, "Waveforms", H5P_DEFAULT));

  return group_id;
}

hid_t ASDF_open_stations_group(hid_t loc_id, const char *station_name) {
  hid_t group_id, accpl_id;
  hbool_t is_collective;

  /* Open the group "/Waveforms/<station_name>" */
  // CHK_H5(accpl_id = H5Pcreate(H5P_GROUP_ACCESS));
  // is_collective = 1; // true;
  // CHK_H5(H5Pset_all_coll_metadata_ops( accpl_id, is_collective ));

  // CHK_H5(group_id = H5Gopen(loc_id, station_name, accpl_id));
  CHK_H5(group_id = H5Gopen(loc_id, station_name, H5P_DEFAULT));

  // CHK_H5(H5Pclose(accpl_id));

  return group_id;
}

hid_t ASDF_open_waveform(hid_t loc_id, const char *waveform_name) {
  hid_t data_id, accpl_id;
  hbool_t is_collective;

  /* Open the group "/Waveforms/<station_name>/<waveform_name>" */
  // CHK_H5(accpl_id = H5Pcreate(H5P_GROUP_ACCESS));
  // is_collective = 1; // true;
  // CHK_H5(H5Pset_all_coll_metadata_ops( accpl_id, is_collective ));

  // CHK_H5(data_id = H5Dopen(loc_id, waveform_name, accpl_id));
  CHK_H5(data_id = H5Dopen(loc_id, waveform_name, H5P_DEFAULT));

  // CHK_H5(H5Pclose(accpl_id));

  return data_id;
}

hid_t ASDF_open_waveform_path(hid_t file_id, const char *station_name, const char *waveform_name) {
  hid_t data_id, accpl_id;
  hbool_t is_collective;
  int waveform_exists;

  /* Open the group "/Waveforms/<station_name>/<waveform_name>" */
  waveform_exists = ASDF_waveform_exists(file_id, station_name, waveform_name);

  if (waveform_exists > 0) {

    char *path = ASDF_extend_path("Waveforms", station_name);
    char *path2 = ASDF_extend_path(path, waveform_name);

    CHK_H5(accpl_id = H5Pcreate(H5P_GROUP_ACCESS));
    is_collective = 1; // true;
    // CHK_H5(H5Pset_all_coll_metadata_ops( accpl_id, is_collective ));

    // CHK_H5(data_id = H5Dopen(file_id, path2, accpl_id));
    CHK_H5(data_id = H5Dopen(file_id, path2, H5P_DEFAULT));

    free(path2);
    free(path);
    CHK_H5(H5Pclose(accpl_id));
    
    return data_id;

  } else {
      return waveform_exists;
  }

}



