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
 * @file test_read_write.cpp
 * @author Matthieu Lefebvre
 */

#include <unistd.h>
#include <iostream>
#include <string>

#include "mpi.h"

#include "gtest/gtest.h"

#include "ASDF_common.h"
#include "ASDF_init.h"
#include "ASDF_read.h"
#include "ASDF_write.h"

TEST(InitFinalize, ReturnSuccess) {
  ASSERT_EQ(ASDF_initialize_hdf5(), 0);
  ASSERT_EQ(ASDF_finalize_hdf5(), 0);
  SUCCEED();
}

// Fixture class to test read and writes.
// Manage temporary file names.
class ReadWrite : public ::testing::Test {
 protected:
  ReadWrite() {
    strcpy(filename, "/tmp/asdf.XXXXXX");
    file_ptr = mktemp(filename);
  }
  virtual ~ReadWrite() {
    if (file_ptr) {
      unlink(filename);
      file_ptr = nullptr;
    }
  }

  virtual void SetUp() {
    ASDF_initialize_hdf5();
  }
  virtual void TearDown() {
    ASDF_finalize_hdf5();
  }

  char *file_ptr; 
  char filename[17];
};

// Check if file is properly create (write mode).
TEST_F(ReadWrite, FileCreation) {
  hid_t file_id  = ASDF_create_new_file(filename, MPI_COMM_WORLD);
  ASSERT_GT(file_id, 0);
}

// Check if string attributes are properly written and read.
TEST_F(ReadWrite, StringAttribute) {
  char attr_name[] = "Harry";
  char attr_init[] = "Go ahead, make my day.";
  char *attr_final;

  // Write the attribute to file.
  {
  hid_t file_id  = ASDF_create_new_file(filename, MPI_COMM_WORLD);
  ASDF_write_string_attribute(file_id, attr_name, attr_init);
  ASDF_close_file(file_id);
  }
  // Read it back.
  {
  char group[] = "/";  // Group at the file's root.
  hid_t file_id = ASDF_open_read_only(filename, MPI_COMM_WORLD);
  ASDF_read_str_attr(file_id, group, attr_name, &attr_final);
  ASDF_close_file(file_id);
  }
  // Check that was is read is what was written.
  ASSERT_EQ(strlen(attr_init), strlen(attr_final));
  ASSERT_EQ(strncmp(attr_init, attr_final, strlen(attr_init)), 0);
}


TEST_F(ReadWrite, AuxiliaryData) {
  int exists;
  {
  hid_t file_id = ASDF_create_new_file(filename, MPI_COMM_WORLD);
  ASDF_write_auxiliary_data(file_id);
  ASDF_close_file(file_id);
  }
  {
  hid_t file_id = ASDF_open_read_only(filename, MPI_COMM_WORLD);
  exists = ASDF_exists_in_path(file_id, "/", "AuxiliaryData");
  ASDF_close_file(file_id);
  }
  ASSERT_GT(exists, 0);
}

TEST_F(ReadWrite, ProvenanceData) {
  int exists;
  {
  hid_t file_id = ASDF_create_new_file(filename, MPI_COMM_WORLD);
  ASDF_write_provenance_data(file_id);
  ASDF_close_file(file_id);
  }
  {
  hid_t file_id = ASDF_open_read_only(filename, MPI_COMM_WORLD);
  exists = ASDF_exists_in_path(file_id, "/", "Provenance");
  ASDF_close_file(file_id);
  }
  ASSERT_GT(exists, 0);
}

TEST_F(ReadWrite, SingleWaveform) {
  int nsamples = 10;
  int start_time = 5;
  double sampling_rate = 0.1;
  char event_name[] = "my_event";
  char station_name[] = "my_station";
  char station_xml[] = "<station_xml>";
  char waveform_name [] = "my_waveform";
  float *waveform = new float[nsamples];
  for (int i = 0; i < nsamples; ++i) {
    waveform[i] = random();
  }

  int nsamples_infered;
  //int nsamples_read, start_time_read;
  //double sampling_rate_read;
  float *waveform_read = new float[nsamples];

  int station_exists, waveform_exists;

  {
  hid_t file_id = ASDF_create_new_file(filename, MPI_COMM_WORLD);
  hid_t waveforms_group = ASDF_create_waveforms_group(file_id);
  hid_t station_group = ASDF_create_stations_group(waveforms_group, 
                                                   station_name, station_xml);
  hid_t data_id = ASDF_define_waveform(station_group, nsamples, start_time,
                                    sampling_rate, event_name, waveform_name);
  ASDF_write_full_waveform(data_id, waveform);
  ASDF_close_dataset(data_id);
  ASDF_close_group(station_group);
  ASDF_close_group(waveforms_group);
  ASDF_close_file(file_id);
  }
  {
  hid_t file_id = ASDF_open_read_only(filename, MPI_COMM_WORLD);
  station_exists = ASDF_station_exists(file_id, station_name);
  waveform_exists = ASDF_waveform_exists(file_id, station_name, 
                                         waveform_name);
  std::string path = "/Waveforms/" + std::string(station_name) 
                   + "/" + std::string(waveform_name);
  nsamples_infered = ASDF_get_num_elements_from_path(file_id, path.c_str());

  // TODO: check other values:
  //        * nsamples, start_time, sampling_rate

  ASDF_read_full_waveform(file_id, path.c_str(), waveform_read);
  ASDF_close_file(file_id);
  }
  ASSERT_GT(station_exists, 0);
  ASSERT_GT(waveform_exists, 0);
  ASSERT_EQ(nsamples_infered, nsamples);
  
  for (int i = 0; i < nsamples; ++i) {
    ASSERT_EQ(waveform[i], waveform_read[i]);
  }

  delete waveform;
  delete waveform_read;
}

///////////////////////////////////////////////////////////////////////////////
// Main routine. Should be the one calling MPI.
int main(int argc, char **argv) {
  int err;

  err = MPI_Init(&argc, &argv);
  ::testing::InitGoogleTest(&argc, argv);
  err |= RUN_ALL_TESTS();
  err |= MPI_Finalize();

  return err;
}
