#ifndef _ASDF_WRITE_H_
#define _ASDF_WRITE_H_

#include <mpi.h>

#define CHK_H5(X) if ((X) < 0) return X;

hid_t ASDF_create_new_file(char *filename, MPI_Comm comm);

void ASDF_write_string_attribute(hid_t dataset_id, 
                                 char *attr_name, char *attr_value);

void ASDF_write_auxiliary_data(hid_t loc_id);

void ASDF_write_provenance_data(hid_t loc_id);

void ASDF_write_quakeml(hid_t loc_id, char *quakeml_string);

void ASDF_define_waveforms(hid_t file_id, int num_waveforms, int nsamples, int start_time, double sampling_rate, 
                           char *event_name, char **waveform_names,
                           int *groups, int *data_id);

void ASDF_write_full_waveform(hid_t data_id, float *waveform);

void ASDF_write_partial_waveform(hid_t data_id, float *waveform,
                                 int offset, int nsamples, int start_time, double sampling_rate);

#endif
