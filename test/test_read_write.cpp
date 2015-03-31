#include <unistd.h>
#include <iostream>

#include "mpi.h"
#include <hdf5.h>

#include "gtest/gtest.h"

extern "C" {
#include "ASDF_common.h"
#include "ASDF_init.h"
#include "ASDF_read.h"
#include "ASDF_write.h"
}

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
  char filename[17] = "/tmp/asdf.XXXXXX";
};

// Check if file is properly create (write mode).
TEST_F(ReadWrite, FileCreation) {
  hid_t file_id  = ASDF_create_new_file(filename, MPI_COMM_WORLD);
  ASSERT_GT(file_id, 0);
}

// Check if string attributes are properly written and read.
TEST_F(ReadWrite, StringAttribute) {
  char attr_name[6] = "Harry";
  char attr_init[23] = "Go ahead, make my day.";
  char *attr_final;

  // Write the attribute to file.
  {
  hid_t file_id  = ASDF_create_new_file(filename, MPI_COMM_WORLD);
  ASDF_write_string_attribute(file_id, attr_name, attr_init);
  ASDF_close_file(file_id);
  }
  // Read it back.
  {
  char group[2] = "/";  // Group at the file's root.
  hid_t file_id = ASDF_open_read_only(filename, MPI_COMM_WORLD);
  ASDF_read_str_attr(file_id, group, attr_name, &attr_final);
  ASDF_close_file(file_id);
  }
  // Check that was is read is what was written.
  ASSERT_EQ(strlen(attr_init), strlen(attr_final));
  ASSERT_EQ(strncmp(attr_init, attr_final, strlen(attr_init)), 0);
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