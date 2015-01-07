
program write_ASDF

  use HDF5
  use ISO_C_BINDING

  implicit none

  character(len=12), parameter :: filename = "synthetic.h5"
  character(len=19) :: quakeml_string = "QuakeML string test"
  character(len=20) :: stationxml_string = "StationXML string !!"
  
  integer(hid_t) :: file, filetype
  integer(hid_t) :: space_scalar, space_array
  integer(hid_t) :: attr, dataset_Source, dataset_Receiver
  integer(hid_t) :: group, dcpl
  integer(hsize_t), dimension(1) :: dims, maxdims
  integer :: hdferr

  type(c_ptr) :: f_ptr

  ! Initalize FORTRAN interface
  call h5open_f(hdferr)
  if (hdferr .eq. -1) print *, "Error opening HDF5 Fortran interface."

  maxdims = (/H5S_UNLIMITED_F/) ! Must be done after opening the interface

  ! Create a new file using default properties.
  call h5fcreate_f(filename, H5F_ACC_TRUNC_F, file, hdferr)
  if (hdferr .eq. -1) print *, "Error creating HDF5 file."

  ! Create file and memory datatypes.
  call h5tcopy_f(h5t_string, filetype, hdferr)
  if (hdferr .eq. -1) print *, "Error creating file and memory datatypes."
 
  ! Create attribute and write string to it.
  call h5screate_f(H5S_SCALAR_F, space_scalar, hdferr)
  call h5acreate_f(file, "file_format", filetype, space_scalar, attr, hdferr)
  call h5acreate_f(file, "file_format_version", filetype, space_scalar, attr, hdferr)

  ! Create group "AuxiliaryData" and "Provenance" in the root group
  call h5gcreate_f(file, "AuxiliaryData", group, hdferr)
  call h5gcreate_f(file, "Provenance", group, hdferr)

  dims = (/19/) ! Length of QuakeML string
  ! Create a dataset with unlimited dimensions
  call h5screate_simple_f(1, dims, space_array, hdferr, maxdims)
  ! Create the dataset creation property list and set the chunk size
  call h5pcreate_f(H5P_DATASET_CREATE_F, dcpl, hdferr)
  call h5pset_chunk_f(dcpl, 1, dims, hdferr)
  ! Create the QuakeML dataset
  call h5dcreate_f(file, "QuakeML", H5T_STD_I8LE, space_array, dataset_Source, hdferr, dcpl)
  ! Write QuakeML dataset
  call h5dwrite_f(dataset_Source, H5T_STD_I8LE, quakeml_string, dims, hdferr) 

  ! Create group "Waveforms" in the root group
  call h5gcreate_f(file, "Waveforms", group, hdferr)
  
  ! Create SEED Group
  call h5gcreate_f(file, "/Waveforms/AF.CVNA", group, hdferr)

  dims = (/3000/)
  ! Create sample Seismogram dataspace inside the SEED group
  call h5screate_simple_f(1, dims, space_array, hdferr, maxdims)
  ! Create the dataset creation property list and set the chunk size
  call h5pcreate_f(H5P_DATASET_CREATE_F, dcpl, hdferr)
  call h5pset_chunk_f(dcpl, 1, dims, hdferr)
  ! Create the waveeform dataset
  call h5dcreate_f(group, "AF.CVNA.S3.MXE__2010-03-11T06:22:17__2010-03-11T08:02:45__synthetic",&
         H5T_IEEE_F32LE, space_array, dataset_Source, hdferr, dcpl)

  ! Now create the sampling rate and start time attributes
  call h5screate_f(H5S_SCALAR_F, space_scalar, hdferr)
  call h5acreate_f(dataset_Source, "event_id", filetype, space_scalar, attr, hdferr)
  call h5acreate_f(dataset_Source, "sampling_rate", H5T_IEEE_F64LE, space_scalar, attr, hdferr)
  call h5acreate_f(dataset_Source, "starttime", H5T_STD_I64LE, space_scalar, attr, hdferr)

  dims = (/20/) ! Length of StationXML string
  ! Finally create the StationXML dataset for the SEED Group
  call h5screate_simple_f(1, dims, space_array, hdferr, maxdims)
  ! Create the dataset creation property list and set the chunk size
  call h5pcreate_f(H5P_DATASET_CREATE_F, dcpl, hdferr)
  call h5pset_chunk_f(dcpl, 1, dims, hdferr)
  ! Create the StationXML dataset
  call h5dcreate_f(group, "StationXML", H5T_STD_I8LE, space_array, dataset_Receiver, hdferr, dcpl)
  ! Write StationXML dataset
  call h5dwrite_f(dataset_Receiver, H5T_STD_I8LE, stationxml_string, dims, hdferr) 

  ! Close and release resources
  call h5aclose_f(attr, hdferr)
  call h5sclose_f(space_scalar, hdferr)
  call h5sclose_f(space_array, hdferr)
  call H5Tclose_f(filetype, hdferr)
  call h5gclose_f(group, hdferr)
  call h5fclose_f(file, hdferr)

end program write_ASDF
