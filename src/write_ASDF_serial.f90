
program write_ASDF
  use hdf5
  use iso_c_binding

  implicit none

  integer :: i
  !----------------------------------------------------------------------------
  ! Variables to write down
  !----------------------------------------------------------------------------
  integer :: nsamples
  real, allocatable, dimension(:) :: waveform
  character(len=539) :: quakeml_string
  character(len=236) :: stationxml_string
  type(c_ptr), dimension(1:2), target :: wdata
  character(len=5, kind=c_char), dimension(1), target :: format = "ASDF"//C_NULL_CHAR
  character(len=7, kind=c_char), dimension(1), target :: version ="0.0.1b"//C_NULL_CHAR
  real(kind=8) :: sampling_rate
  !----------------------------------------------------------------------------
  ! HDF5 variables
  !----------------------------------------------------------------------------
  character(len=*), parameter :: filename = "synthetic.h5"
  integer(hid_t) :: file_id
  integer :: hdferr
  integer(hid_t) :: filetype
  integer(hid_t) :: space_scalar, space_array
  integer(hid_t) :: attr, attr2, dataset_Source, dataset_Receiver
  integer(hid_t) :: group, dcpl, memtype
  integer(hid_t) :: start_time
  integer(hsize_t), dimension(1) :: maxdims
  type(c_ptr) :: f_ptr
  type(c_ptr) :: f_ptr2
  ! Groups
  character(len=256) :: group_name
  integer(hid_t) :: groupAuxiliary, groupProvenance
  integer(hid_t), allocatable :: groups(:)
  ! Datasets in groups
  integer(hid_t) :: dataspace
  integer(hsize_t), dimension(1) :: dims
  character(len=256) :: dataset_name
  integer(hid_t), allocatable :: datasets(:)

  ! --- Allocate and set our variables
  wdata(1) = c_loc(format(1))
  wdata(2) = c_loc(version(1))
  start_time = 393838
  sampling_rate = 0.40

  ! Initalize FORTRAN interface
  call h5open_f(hdferr)
  if (hdferr .eq. -1) print *, "Error opening HDF5 Fortran interface."
  maxdims = (/H5S_UNLIMITED_F/) ! Must be done after opening the interface

  ! Create a new file using default properties.
  call h5fcreate_f (filename, H5F_ACC_TRUNC_F, file_id, hdferr)
  call h5tcopy_f(h5t_string, filetype, hdferr)

  ! Create attribute and write string to it.
  call h5screate_f(h5s_scalar_f, space_scalar, hdferr)
  call h5acreate_f(file_id, "file_format", filetype, space_scalar, attr, hdferr)
  f_ptr = C_LOC(wdata(1))
  call h5awrite_f(attr, h5t_string, f_ptr, hdferr)
  call h5acreate_f(file_id, "file_format_version", filetype, space_scalar, attr, hdferr)
  f_ptr = C_LOC(wdata(2))
  call h5awrite_f(attr, filetype, f_ptr, hdferr)
  call h5aclose_f(attr, hdferr)

  ! Create group "AuxiliaryData" and "Provenance" in the root group
  call h5gcreate_f(file_id, "AuxiliaryData", groupAuxiliary, hdferr)
  call h5gclose_f(groupAuxiliary, hdferr)
  call h5gcreate_f(file_id, "Provenance", groupProvenance, hdferr)
  call h5gclose_f(groupProvenance, hdferr)

  dims = (/539/) ! Length of QuakeML string
  ! Create a dataset with unlimited dimensions
  call h5screate_simple_f(1, dims, space_array, hdferr, maxdims)
  ! Create the dataset creation property list and set the chunk size
  call h5pcreate_f(H5P_DATASET_CREATE_F, dcpl, hdferr)
  call h5pset_chunk_f(dcpl, 1, dims, hdferr)
  ! Create the QuakeML dataset
  call h5dcreate_f(file_id, "QuakeML", H5T_STD_I8LE, space_array, dataset_Source, hdferr, dcpl)
  ! Write QuakeML dataset
  call generateQuakeml(quakeml_string, hdferr)
  call h5dwrite_f(dataset_Source, H5T_STD_I8LE, quakeml_string, dims, hdferr) 

  ! Create group "Waveforms" in the root group
  call h5gcreate_f(file_id, "Waveforms", group, hdferr)
  
  ! Create SEED Group
  call h5gcreate_f(file_id, "/Waveforms/AF.CVNA", group, hdferr)

  dims = (/3000/) ! Length of waveform
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
  ! Write the sampling rate and start time attributes
  f_ptr = C_LOC(sampling_rate)
  call h5awrite_f(attr, H5T_IEEE_F64LE, f_ptr, hdferr)
  call h5acreate_f(dataset_Source, "starttime", H5T_STD_I64LE, space_scalar, attr2, hdferr)
  f_ptr2 = C_LOC(start_time)
  call h5awrite_f(attr2, H5T_STD_I64LE, f_ptr2, hdferr)

  !CALL H5Awrite_f(attr, H5T_NATIVE_DOUBLE, f_ptr, hdferr)
  dims = (/236/) ! Length of StationXML string
  ! Finally create the StationXML dataset for the SEED Group
  call h5screate_simple_f(1, dims, space_array, hdferr, maxdims)
  ! Create the dataset creation property list and set the chunk size
  call h5pcreate_f(H5P_DATASET_CREATE_F, dcpl, hdferr)
  call h5pset_chunk_f(dcpl, 1, dims, hdferr)
  ! Create the StationXML dataset
  call h5dcreate_f(group, "StationXML", H5T_STD_I8LE, space_array, dataset_Receiver, hdferr, dcpl)
  ! Write StationXML dataset
  call generateStationXMLstrings(stationxml_string, hdferr)
  call h5dwrite_f(dataset_Receiver, H5T_STD_I8LE, stationxml_string, dims, hdferr) 

  ! Close and release resources
  call h5aclose_f(attr, hdferr)
  call h5sclose_f(space_scalar, hdferr)
  call h5sclose_f(space_array, hdferr)
  call H5Tclose_f(filetype, hdferr)
  call h5fclose_f(file_id, hdferr)


end program write_ASDF

subroutine generateQuakeML(quakemlstring, quakeml_length)

  implicit none
  character(len=*) :: quakemlstring
  integer :: quakeml_length

  print *, "Generating QuakeML"

  quakemlstring = '<quakeml>\n<event unique_id=\"EV_01\">\n<location&
main=\"true\" unique_id=\"LOC_01\" analysis-type=\"M\">\n<origin-date&
timezone=\"00:00\">\n<year\>2004</year>\n<month\>09</month\n<day\>28</day>\n<hour\>17</hour>\n<minute\>15</minute>\n<seconds\>24.0</seconds>\n</origin-date>\n<latitude&
error="0">35.8</latitude>\n<longitude error="0">-120.4</longitude>\n<depth&
unit="km" error="0">7</depth>\n<magnitude unit="M"&
error="0">6.0</magnitude>\n<region>CENTRAL&
CALIFORNIA</region>\n<author>SPECFEM3D_GLOBE</author>\n</location>\n</event>\n</quakeml>'

end subroutine generateQuakeML

subroutine generateStationXMLstrings(stationxmlstrings, stationxmllengths)

  implicit none
  character(len=*) :: stationxmlstrings
  integer :: stationxmllengths

  print *, "Generating StationXML strings"

  stationxmlstrings = '<stationXML>\n<url>http://www.iris.edu/ws/station/query?net=II\&sta=AAK\&&
chan=BHZ,BHN,BHE,BH1,BH2\&loc=00,--\&level=chan</url>\n<refreshInterval>&
\n<unit>DAY</unit>\n<value>10</value>\n</refreshInterval>\n</stationXML>'

end subroutine generateStationXMLstrings
