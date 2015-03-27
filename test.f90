program test_asdf
  use mpi
  use iso_c_binding
  integer :: ier, myrank, mysize
  integer :: file_id

  call MPI_Init(ier)
  call MPI_Comm_rank(MPI_COMM_WORLD, myrank, ier)
  call MPI_Comm_size(MPI_COMM_WORLD, mysize, ier)

  call ASDF_initialize_hdf5_f(ier)

  call ASDF_create_new_file_f("synthetic_f.h5", MPI_COMM_WORLD, file_id)

  call ASDF_write_string_attribute_f(file_id, "file_format", "ASDF", ier)
  call ASDF_write_string_attribute_f(file_id, "file_version", "0.0.1b", ier)

  call ASDF_write_auxiliary_data_f(file_id, ier)

  call ASDF_finalize_hdf5_f(ier)

  call MPI_Finalize(ier)
end program test_asdf
