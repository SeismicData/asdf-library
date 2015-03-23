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

/**********************************************************
 *  Wrappers for ASDF_write                               *
 **********************************************************/

void asdf_create_new_file_f_(char *filename, MPI_Comm *comm, int *file_id) {
  *file_id = ASDF_create_new_file(filename, *comm);
}

void asdf_write_string_attribute_f_(hid_t *dataset_id, char *attr_name, 
                                    char *attr_value, int *err) {
  *err = ASDF_write_string_attribute(*dataset_id, attr_name, attr_value);
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

void asdf_define_waveforms_f_(hid_t *loc_id, int *num_waveforms, int *nsamples,
                              int *start_time, double *sampling_rate, 
                              char *event_name, char **waveform_names,
                              int *groups, int *data_id, int *err) {
  *err = ASDF_define_waveforms(*loc_id, *num_waveforms, *nsamples, 
                               *start_time, *sampling_rate, 
                               event_name, waveform_names,
                               groups, data_id);
}

void asdf_write_full_waveform_f_(hid_t *data_id, float *waveform, int *err) {
  *err = ASDF_write_full_waveform(*data_id, waveform);
}

void asdf_write_partial_waveform_f_(hid_t *data_id, float *waveform,
                                    int *offset, int *nsamples, int *err) {
  *err = ASDF_write_partial_waveform(*data_id, waveform,
                                   *offset, *nsamples);
}
