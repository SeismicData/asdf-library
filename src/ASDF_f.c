#include <mpi.h>
#include <hdf5.h>

#include "ASDF_init.h"
#include "ASDF_write.h"

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

void asdf_write_string_attribute_f_(hid_t *dataset_id, char *attr_name, 
                                    char *attr_value, int *err) {
  ASDF_write_string_attribute(*dataset_id, attr_name, attr_value);
}

void asdf_write_auxiliary_data_f_(hid_t *loc_id, int *err) {
  *err = ASDF_write_auxiliary_data(*loc_id);
}

void asdf_write_provenance_data_f_(hid_t *loc_id, int *err) {
  *err = ASDF_write_provenance_data(*loc_id);
}

void asdf_write_quakeml_f_(hid_t *loc_id, char *quakeml_string, int *err) {
  *err = ASDF_write_quakeml(*loc_id, quakeml_string);
}

void asdf_create_waveforms_group_f_(hid_t *loc_id, hid_t *group_id) {
  *group_id = ASDF_create_waveforms_group(*loc_id);
}

void asdf_close_group_f_(hid_t *group_id, int *err) {
  *err = ASDF_close_group(*group_id);
}

void asdf_create_stations_group_f_(hid_t *loc_id, char *station_name,
                                  char *station_xml, hid_t *group_id) {
  *group_id = ASDF_create_stations_group(*loc_id, station_name, station_xml);
}

void asdf_define_waveform_f_(hid_t *loc_id, int *nsamples, 
                          int *start_time, double *sampling_rate,
                          char *event_name, char *waveform_name,
                          int *data_id) {
  *data_id = ASDF_define_waveform(*loc_id, *nsamples, *start_time,
                                  *sampling_rate, event_name, waveform_name);
}


void asdf_define_waveforms_f_(hid_t *loc_id, int *num_waveforms, int *nsamples,
                              int *start_time, double *sampling_rate, 
                              char *event_name, char **waveform_names,
                              int *data_id, int *err) {
  *err = ASDF_define_waveforms(*loc_id, *num_waveforms, *nsamples, 
                               *start_time, *sampling_rate, 
                               event_name, waveform_names,
                               data_id);
}

void asdf_write_full_waveform_f_(hid_t *data_id, float *waveform, int *err) {
  *err = ASDF_write_full_waveform(*data_id, waveform);
}

void asdf_write_partial_waveform_f_(hid_t *data_id, float *waveform,
                                    int *offset, int *nsamples, int *err) {
  *err = ASDF_write_partial_waveform(*data_id, waveform,
                                   *offset, *nsamples);
}
