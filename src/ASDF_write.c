#include <hdf5.h>

#include "ASDF_write.h"

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
  {
    // It might be made a separate function.
    hid_t group_id = H5Gcreate(loc_id, "Waveforms", 
                               H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    H5Gclose(group_id);
  }
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

void ASDF_write_full_waveform(hid_t data_id, float *waveform) {
  H5Dwrite(data_id, H5T_IEEE_F32LE, H5S_ALL, H5S_ALL, H5P_DEFAULT, waveform);
}

void ASDF_write_partial_waveform(hid_t data_id, float *waveform, 
                                 int offset, int nsamples) {
  hid_t space_id = H5Dget_space(data_id);
  hsize_t start[1] = {offset};
  hsize_t count[1] = {1};
  hsize_t block[1] = {nsamples};
  H5Sselect_hyperslab(space_id, H5S_SELECT_SET, start, NULL, count, block);
  hid_t slab_id = H5Screate_simple(1, block, NULL);
  H5Dwrite(data_id, H5T_IEEE_F32LE, slab_id, space_id, H5P_DEFAULT, waveform);
  H5Sclose(slab_id);
  H5Sclose(space_id);
}
