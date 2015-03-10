#include <hdf5.h>
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

#define FAKE_NUM_SAMPLES 30
#define MAX_STRING_LENGTH 256

void initialize_hdf5_asdf();
void finalize_hdf5_asdf();
hid_t ASDF_create_new_file(char *filename);
void ASDF_write_string_attribute(hid_t dataset_id, char *attr_name, char *attr_value);
void ASDF_write_auxiliary_data(hid_t loc_id);
void ASDF_write_provenance_data(hid_t loc_id);
void ASDF_write_quakeml(hid_t loc_id, char *quakeml_string);
void ASDF_define_waveforms(hid_t file_id, int num_waveforms, int nsamples, 
                           char *event_name, char **waveform_names,
                           int *groups, int *data_id);

int main(int argc, char *argv[]) {

  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

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
  
  initialize_hdf5_asdf();
 
  hid_t file_id;
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
  {
    hid_t group_id = H5Gcreate(file_id, "Waveforms", 
                               H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    H5Gclose(group_id);
  }
  /*------------------------------------*/
  {
    int i;
    int num_waveforms = size;
    int nsamples = FAKE_NUM_SAMPLES;
    char **waveform_names = (char **) malloc(num_waveforms*sizeof(*waveform_names));
    for (i = 0; i < num_waveforms; ++i) {
      waveform_names[i] = (char *) malloc(MAX_STRING_LENGTH*sizeof(char));
      sprintf(waveform_names[i], "Waveforms/AF.CVNA_%d", i);
    }
    hid_t groups[num_waveforms];
    hid_t data_id[num_waveforms];

    ASDF_define_waveforms(file_id, num_waveforms, nsamples, 
                          event_name, waveform_names,
                          groups, data_id);

    // TODO: write waveforms. test within a loop, each step writting partial
    //       data. Should probably be done using hyperslabs.

    for (i = 0; i < num_waveforms; ++i) {
      H5Gclose(groups[i]);
      H5Dclose(data_id[i]);
      free(waveform_names[i]);
    }
    free(waveform_names);
  }
  /*------------------------------------*/
  H5Fclose(file_id);

  finalize_hdf5_asdf();

  MPI_Finalize();
  return EXIT_SUCCESS;
}

void initialize_hdf5_asdf() {
  H5open();
}

void finalize_hdf5_asdf() {
  H5close();
}

void ASDF_write_string_attribute(hid_t dataset_id, char *attr_name, 
                                 char *attr_value) {
    hid_t space_id, type_id, attr_id;

    space_id  = H5Screate(H5S_SCALAR);
    type_id = H5Tcopy(H5T_C_S1);
    H5Tset_size(type_id, strlen(attr_value)+1);
    H5Tset_strpad(type_id,H5T_STR_NULLTERM);

    attr_id = H5Acreate(dataset_id, attr_name, type_id, space_id,
                        H5P_DEFAULT, H5P_DEFAULT);

    H5Awrite(attr_id, type_id, attr_value);

    H5Aclose(attr_id);
    H5Tclose(type_id);
    H5Sclose(space_id);
}

hid_t ASDF_create_new_file(char *filename) {
  hid_t plist_id = H5Pcreate(H5P_FILE_ACCESS);
  H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);
  /* Create the file collectively.*/
  hid_t file_id = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);
  H5Pclose(plist_id);

  return file_id;
}

void ASDF_write_auxiliary_data(hid_t loc_id) {
    hid_t group_id = H5Gcreate(loc_id, "AuxiliaryData", 
                               H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    /* Fill up with whatever AuxiliaryData contains. */
    H5Gclose(group_id);
}

void ASDF_write_provenance_data(hid_t loc_id) {
    hid_t group_id = H5Gcreate(loc_id, "Provenance", 
                               H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    /* Fill up with whatever Provenance contains. */
    H5Gclose(group_id);
}

void ASDF_write_quakeml(hid_t loc_id, char *quakeml_string) {
    hsize_t dims[1] = {strlen(quakeml_string)+1};
    hid_t space_id = H5Screate_simple(1, dims, NULL);

    hid_t array_id = H5Dcreate(loc_id, "/QuakeML", H5T_STD_I8LE, space_id, 
                               H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    H5Dwrite(array_id, H5T_STD_I8LE, H5S_ALL, H5S_ALL, 
            H5P_DEFAULT, quakeml_string);

    H5Dclose(array_id);
    H5Sclose(space_id);
}

void ASDF_define_waveforms(hid_t loc_id, int num_waveforms, int nsamples, 
                           char *event_name, char **waveform_names,
                           int *groups, int *data_id) {
  int i;
  for (i = 0; i < num_waveforms; ++i) {
    char char_buf[256];

    groups[i] = H5Gcreate(loc_id, waveform_names[i],
                          H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    hsize_t dims[1] = {nsamples}; // Length of waveform
    hid_t space_id= H5Screate_simple(1, dims, NULL);
    hid_t dcpl = H5Pcreate(H5P_DATASET_CREATE);

    sprintf(char_buf, "Synthetics_number_%d", i);
    data_id[i] = H5Dcreate(groups[i], char_buf, H5T_IEEE_F32LE, space_id, 
                           H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    ASDF_write_string_attribute(data_id[i], "event_id", event_name);

    H5Pclose(dcpl);
    H5Sclose(space_id);
  }
}

