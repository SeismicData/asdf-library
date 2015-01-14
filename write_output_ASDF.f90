
program write_ASDF

  use HDF5
  use ISO_C_BINDING

  implicit none

  character(len=12), parameter :: filename = "synthetic.h5"
  character(len=539) :: quakeml_string
  character(len=236) :: stationxml_string = "StationXML string !!"
  
  integer(hid_t) :: file, filetype
  integer(hid_t) :: space_scalar, space_array
  integer(hid_t) :: attr, attr2, dataset_Source, dataset_Receiver
  integer(hid_t) :: group, dcpl
  integer(hid_t) :: start_time
  integer(hsize_t), dimension(1) :: dims, maxdims
  integer :: hdferr
  real(kind=8) :: sampling_rate

  type(c_ptr) :: f_ptr
  type(c_ptr) :: f_ptr2
  start_time = 393838
  sampling_rate = 0.40
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

  dims = (/539/) ! Length of QuakeML string
  ! Create a dataset with unlimited dimensions
  call h5screate_simple_f(1, dims, space_array, hdferr, maxdims)
  ! Create the dataset creation property list and set the chunk size
  call h5pcreate_f(H5P_DATASET_CREATE_F, dcpl, hdferr)
  call h5pset_chunk_f(dcpl, 1, dims, hdferr)
  ! Create the QuakeML dataset
  call h5dcreate_f(file, "QuakeML", H5T_STD_I8LE, space_array, dataset_Source, hdferr, dcpl)
  ! Write QuakeML dataset
  call generateQuakeml(quakeml_string, hdferr)
  call h5dwrite_f(dataset_Source, H5T_STD_I8LE, quakeml_string, dims, hdferr) 

  ! Create group "Waveforms" in the root group
  call h5gcreate_f(file, "Waveforms", group, hdferr)
  
  ! Create SEED Group
  call h5gcreate_f(file, "/Waveforms/AF.CVNA", group, hdferr)

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

  CALL H5Awrite_f(attr, H5T_NATIVE_DOUBLE, f_ptr, hdferr)
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
  call h5gclose_f(group, hdferr)
  call h5fclose_f(file, hdferr)

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
