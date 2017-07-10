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
 * @file ASDF_f.c
 * @author Matthieu Lefebvre
 */

#include <mpi.h>
#include <hdf5.h>

#include "ASDF_init.h"
#include "ASDF_write.h"
#include "ASDF_read.h"
#include "ASDF_provenance.h"

/**********************************************************
 *  Wrappers for ASDF_init                                *
 **********************************************************/

void asdf_initialize_hdf5_f_(int *err) {
  *err = ASDF_initialize_hdf5();
}

void asdf_finalize_hdf5_f_(int *err) {
  *err = ASDF_finalize_hdf5();
}

void asdf_close_file_f_(hid_t *file_id, int* err) {
  *err = ASDF_close_file(*file_id);
}

/**********************************************************
 *  Wrappers for ASDF_write                               *
 **********************************************************/

void asdf_create_new_file_f_(char *filename, MPI_Fint *f_comm, int *file_id) {
  /* Convert MPI communicator from Fortran to C. */
  MPI_Comm comm= MPI_Comm_f2c(*f_comm);
  *file_id = ASDF_create_new_file(filename, comm);
}

void asdf_create_new_file_serial_f_(char *filename, int *file_id) {
  *file_id = ASDF_create_new_file_serial(filename);
}

void asdf_write_string_attribute_f_(hid_t *dataset_id, char *attr_name,
                                    char *attr_value, int *err) {
  *err = ASDF_write_string_attribute(*dataset_id, attr_name, attr_value);
}

void asdf_write_auxiliary_data_f_(hid_t *loc_id, char *sf_constants_file, char *sf_Parfile, int *err) {
  *err = ASDF_write_auxiliary_data(*loc_id, sf_constants_file, sf_Parfile);
}

void asdf_write_provenance_data_f_(hid_t *loc_id, char *provenance_string, int *err) {
  *err = ASDF_write_provenance_data(*loc_id, provenance_string);
}

void asdf_write_quakeml_f_(hid_t *loc_id, char *quakeml_string, int *err) {
  *err = ASDF_write_quakeml(*loc_id, quakeml_string);
}

void asdf_create_waveforms_group_f_(hid_t *loc_id, hid_t *group_id) {
  *group_id = ASDF_create_waveforms_group(*loc_id);
}

void asdf_create_stations_group_f_(hid_t *loc_id, char *station_name,
                                   hid_t *group_id) {
  *group_id = ASDF_create_stations_group(*loc_id, station_name);
}

void asdf_define_station_xml_f_(hid_t *group_id, int *StationXML_length,
                                   hid_t *data_id) {
  *data_id = ASDF_define_station_xml(*group_id, *StationXML_length);
}

void asdf_define_waveform_f_(hid_t *loc_id, int *nsamples,
                          long long int *start_time, double *sampling_rate,
                          char *event_name, char *waveform_name,
                          int *data_id) {
  *data_id = ASDF_define_waveform(*loc_id, *nsamples, *start_time,
                                  *sampling_rate, event_name, waveform_name);
}


void asdf_define_waveforms_f_(hid_t *loc_id, int *num_waveforms, int *nsamples,
                              long long int *start_time, double *sampling_rate,
                              char *event_name, char **waveform_names,
                              int *data_id, int *err) {
  *err = ASDF_define_waveforms(*loc_id, *num_waveforms, *nsamples,
                               *start_time, *sampling_rate,
                               event_name, waveform_names,
                               data_id);
}

void asdf_write_station_xml_f_(hid_t *data_id, char *station_xml, int *err) {
  *err = ASDF_write_station_xml(*data_id, station_xml);
}

void asdf_write_full_waveform_f_(hid_t *data_id, float *waveform, int *err) {
  *err = ASDF_write_full_waveform(*data_id, waveform);
}

void asdf_write_partial_waveform_f_(hid_t *data_id, float *waveform,
                                    int *offset, int *nsamples, int *err) {
  *err = ASDF_write_partial_waveform(*data_id, waveform,
                                   *offset, *nsamples);
}

/*
void asdf_write_partial_waveform_collective_f_(hid_t *data_id, float *waveform,
                                    int *offset, int *nsamples, int *err) {
  *err = ASDF_write_partial_waveform_collective(*data_id, waveform,
                                   *offset, *nsamples);
} 
*/

void asdf_open_waveforms_group_f_(hid_t *loc_id, hid_t *group_id) {
  *group_id = ASDF_open_waveforms_group(*loc_id);
}

void asdf_open_stations_group_f_(hid_t *loc_id, char *station_name,
                                   hid_t *group_id) {
  *group_id = ASDF_open_stations_group(*loc_id, station_name);
}

void asdf_open_waveform_f_(hid_t *loc_id, char *waveform_name,
                                   hid_t *data_id) {
  *data_id = ASDF_open_waveform(*loc_id, waveform_name);
}


void asdf_close_group_f_(hid_t *group_id, int *err) {
  *err = ASDF_close_group(*group_id);
}

void asdf_close_dataset_f_(hid_t *dataset_id, int *err) {
  *err = ASDF_close_dataset(*dataset_id);
}

/**********************************************************
 *  Wrappers for ASDF_read                                *
 **********************************************************/

void asdf_open_read_only_f_(char *filename, MPI_Fint *f_comm, int *file_id) {
  MPI_Comm comm= MPI_Comm_f2c(*f_comm);
  *file_id = ASDF_open_read_only(filename, comm);
}

void asdf_open_f_(char *filename, MPI_Fint *f_comm, int *file_id) {
  MPI_Comm comm= MPI_Comm_f2c(*f_comm);
  *file_id = ASDF_open(filename, comm);
}

void asdf_open_serial_f_(char *filename, int *file_id) {
  *file_id = ASDF_open_serial(filename);
}

void asdf_read_str_attr_f_(hid_t *file_id, char *grp_name,
                       char *attr_name, char **attr_value, int *err) {
  *err = ASDF_read_str_attr(*file_id, grp_name, attr_name, attr_value);
}

void asdf_get_num_elements_dataset_f_(hid_t *dataset_id, int *err) {
  *err = ASDF_get_num_elements_dataset(*dataset_id);
}

void asdf_get_num_elements_from_path_f_(hid_t *file_id, char *path, int *err) {
 *err = ASDF_get_num_elements_from_path(*file_id, path);
}

void asdf_read_full_waveform_f_(hid_t *file_id, char *path, float *waveform,
                             int *err) {
  *err = ASDF_read_full_waveform(*file_id, path, waveform);
}

void asdf_read_partial_waveform_f_(hid_t *file_id, char *path, int *offset,
                                   int *nsamples, float *waveform, int *err) {
  *err = ASDF_read_partial_waveform(*file_id, path, *offset,
                                    *nsamples, waveform);
}

// char *ASDF_extend_path(const char *path, const char *name);
// No fortran wrapper, it is a convenience function for the C API.
// Fortran users should use the '//' operator.

void asdf_exists_in_path_f_(hid_t *file_id, char *path, char *name,
                            int *exists) {
  *exists = ASDF_exists_in_path(*file_id, path, name);
}

void asdf_station_exists_f_(hid_t *file_id, char *name, int *exists) {
  *exists = ASDF_station_exists(*file_id, name);
}

void asdf_adjoint_source_exists_f_(hid_t *file_id, char *name, int *exists) {
  *exists = ASDF_adjoint_source_exists(*file_id, name);
}

void asdf_waveform_exists_f_(hid_t *file_id, char *station_name,
                             char *waveform_name, int *exists) {
  *exists = ASDF_waveform_exists(*file_id, station_name, waveform_name);
}

/**********************************************************
 *  Wrappers for ASDF_generate_provenance                 *
 **********************************************************/

// TODO: It is probably irrelevant to export every function used to generate
// provenance information. check ASDF_write_provenance_data.

void asdf_generate_sf_provenance_f_(char *startTime, char *endTime, char **provenance, int *length) {
  ASDF_generate_sf_provenance(startTime, endTime, provenance, length);
}

void asdf_clean_provenance_f_(char **provenance) {
  ASDF_clean_provenance(provenance);
}
