#include <hdf5.h>
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "ASDF_init.h"
#include "ASDF_write.h"

#define FAKE_NUM_SAMPLES 30
#define MAX_STRING_LENGTH 256


int main(int argc, char *argv[]) {
  int i;

  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  /******************************************************
   *   Begin Fake Data                                  *
   ******************************************************/
  char *filename = "synthetic.h5";
  char *event_name = "event0123456789";
  char *quakeml_string = "quakemlstringstring";
    /*
      "quakemlstring = '<quakeml>\\n<event unique_id=\"EV_01\">\\n<location"
      "main=\"true\" unique_id=\"LOC_01\" analysis-type=\"M\">\\n<origin-date"
      "timezone=\"00:00\">\\n<year\\>2004</year>\\n<month\\>09</month\\n"
      "<day\\>28</day>\\n<hour\\>17</hour>\\n<minute\\>15</minute>\\n"
      "<seconds\\>24.0</seconds>\\n</origin-date>\\n<latitude error=\"0\">"
      "35.8</latitude>\\n<longitude error=\"0\">-120.4</longitude>\\n<depth"
      "unit=\"km\" error=\"0\">7</depth>\\n<magnitude unit=\"M\""
      "error=\"0\">6.0</magnitude>\\n<region>CENTRAL"
      "CALIFORNIA</region>\\n<author>SPECFEM3D_GLOBE</author>\\n</location>\\n"
      "</event>\\n</quakeml>";
    */
  int start_time = 393838;
  double sampling_rate = 0.4*(rank+1);
  int num_waveforms = size;
  int nsamples = FAKE_NUM_SAMPLES;
  char **waveform_names = (char **) malloc(num_waveforms*sizeof(char *));
  for (i = 0; i < num_waveforms; ++i) {
    waveform_names[i] = (char *) malloc(MAX_STRING_LENGTH*sizeof(char));
    sprintf(waveform_names[i], "Waveforms/AF.CVNA_%d", i);
  }
  float **waveforms = (float **) malloc(num_waveforms*sizeof(float *));
  for (i = 0; i < num_waveforms; ++i) {
    waveforms[i] = (float *) malloc(nsamples*sizeof(float));
    int j;
    for (j = 0; j < nsamples; ++j) {
      waveforms[i][j] = rank + 1.0;
    }
  }


  /******************************************************
   *   End   Fake Data                                  *
   ******************************************************/

  hid_t file_id;
  hid_t groups[num_waveforms];
  hid_t data_id[num_waveforms];
  
  ASDF_initialize_hdf5();
 
  file_id = ASDF_create_new_file(filename);

  /*------------------------------------*/
  ASDF_write_string_attribute(file_id, "file_format", "ASDF");
  ASDF_write_string_attribute(file_id, "file_version", "0.0.1b");
  /*------------------------------------*/
  ASDF_write_auxiliary_data(file_id);
  ASDF_write_provenance_data(file_id);
  /*------------------------------------*/
  ASDF_write_quakeml(file_id, quakeml_string);
  /*------------------------------------*/

  ASDF_define_waveforms(file_id, num_waveforms, nsamples, 
                        event_name, waveform_names,
                        groups, data_id);

  assert(nsamples);
  int write_size = nsamples / 4;
  int num_steps = (nsamples - 1) / write_size + 1;

  int j;
  for (j = 0; j < num_steps; ++j) {
    int offset, nsamples_to_write;
    offset = write_size * j; 
    if (j < num_steps-1) {
      nsamples_to_write = write_size;
    } else {
      nsamples_to_write = nsamples - offset;
    }
    
    for (i = 0; i < num_waveforms; ++i) {
      if (i == rank) {
        /*ASDF_write_full_waveform(data_id[i], waveforms[i]);*/
        ASDF_write_partial_waveform(data_id[i], waveforms[i], 
                                    offset, nsamples_to_write);
      }
    }
  }

  // TODO: write waveforms. test within a loop, each step writting partial
  //       data. Should probably be done using hyperslabs.

  for (i = 0; i < num_waveforms; ++i) {
    H5Gclose(groups[i]);
    H5Dclose(data_id[i]);
  }

  /*********************************
   * Clean fake data               *
   * *******************************/
  for (i = 0; i < num_waveforms; ++i) {
    free(waveform_names[i]);
    free(waveforms[i]);
  }
  free(waveform_names);
  free(waveforms);
 
  /*------------------------------------*/
  H5Fclose(file_id);

  ASDF_finalize_hdf5();

  MPI_Finalize();
  return EXIT_SUCCESS;
}
