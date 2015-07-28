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
 * @file write_ASDF.c
 * @brief
 * @author Matthieu Lefebvre
 */

#include <hdf5.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
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
  char *station_name = "AF.CVNA";
  char *station_xml = "station_xml_string";
  char *provenance_string = "provenance_string";
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
  /***********************************************************
   * Allocate memory and define waveform names               *
   ***********************************************************/
  for (i = 0; i < num_waveforms; ++i) {
    waveform_names[i] = (char *) malloc(MAX_STRING_LENGTH*sizeof(char));
    sprintf(waveform_names[i], "AF.CVNA.s3.long.strange.name_%d", i);
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
  hid_t data_id[num_waveforms];

  ASDF_initialize_hdf5();

  file_id = ASDF_create_new_file(filename, MPI_COMM_WORLD);

  /*------------------------------------*/
  ASDF_write_string_attribute(file_id, "file_format", "ASDF");
  ASDF_write_string_attribute(file_id, "file_version", "0.0.1b");
  /*------------------------------------*/
  ASDF_write_auxiliary_data(file_id, "test", "test2");
  ASDF_write_provenance_data(file_id, provenance_string);
  /*------------------------------------*/
  ASDF_write_quakeml(file_id, quakeml_string);
  /*------------------------------------*/

  hid_t waveforms_grp = ASDF_create_waveforms_group(file_id);

  hid_t station_grp = ASDF_create_stations_group(waveforms_grp, station_name, station_xml);

  ASDF_define_waveforms(station_grp, num_waveforms, nsamples, start_time, sampling_rate,
                        event_name, waveform_names,
                        data_id);

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

  for (i = 0; i < num_waveforms; ++i) {
    H5Dclose(data_id[i]);
  }

  ASDF_close_group(station_grp);
  ASDF_close_group(waveforms_grp);

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
