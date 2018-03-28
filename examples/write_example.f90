!==============================================================================
! Copyright 2015 ASDF developers
!
! Licensed under the Apache License, Version 2.0 (the "License");
! you may not use this file except in compliance with the License.
! You may obtain a copy of the License at
!
!    http://www.apache.org/licenses/LICENSE-2.0
!
! Unless required by applicable law or agreed to in writing, software
! distributed under the License is distributed on an "AS IS" BASIS,
! WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
! See the License for the specific language governing permissions and
! limitations under the License.
!==============================================================================

program write_example
  !========================================================
  ! This example mimics SPECFEM3D_GLOBE data organization
  !
  ! Note: There is no error checking. This, of course, should be done
  !       in code intended to be used in production
  !========================================================

  use mpi

  use iso_c_binding

  implicit none

  integer, parameter :: MAX_STRING_LENGTH = 256

  character(len=MAX_STRING_LENGTH) :: filename

  !--- Data to be written to the ASDF file
  character(len=MAX_STRING_LENGTH) :: event_name
  character(len=MAX_STRING_LENGTH) :: provenance
  character(len=MAX_STRING_LENGTH) :: quakeml
  ! character(len=MAX_STRING_LENGTH) :: station_xml
  ! TODO: check if a single station_xml is enough

  integer :: num_stations, num_channels_per_station
  integer :: num_waveforms  ! == num_stations * num_channels_per_station
  ! The number of channels per station is constant, as in SPECFEM

  integer :: start_time, nsamples  ! constant, as in SPECFEM
  double precision :: sampling_rate  ! idem

  ! Network names and station names are two different beast, as in SPECFEM
  ! network_names(i) is related to station_names(i)
  character(len=MAX_STRING_LENGTH), dimension(:), allocatable :: network_names
  character(len=MAX_STRING_LENGTH), dimension(:), allocatable :: station_names

  ! data. dimension = nsamples * num_channels_per_station * num_stations
  real, dimension(:, :, :), allocatable :: waveforms

  !-- ASDF variables
  !   These variables are used to know where further writes should be done.
  !   They have to be cleaned as soon as they become useless
  integer :: file_id   ! HDF5 file id, also root group "/"
  integer :: waveforms_grp  ! Group "/Waveforms/"
  integer, dimension(:, :, :), allocatable :: data_ids

  !--- MPI variables
  integer :: myrank, mysize, comm
  !--- Loop variables
  integer :: i, j, k
  !--- Error variable
  integer :: ier

  !--- 'allgather' arrays. Variables that needs to be known by everyone in
  !    order to define ASDF groups and datasets or write them as attributes.
  integer, dimension(:), allocatable :: num_stations_gather
  integer :: max_num_stations_gather
  character(len=MAX_STRING_LENGTH), dimension(:,:), allocatable :: &
      station_names_gather, network_names_gather
  integer, dimension(:,:), allocatable :: station_grps_gather
  integer, dimension(:), allocatable :: displs, rcounts

  ! temporary name built from network, station and channel names.
  character(len=MAX_STRING_LENGTH) :: waveform_name

  ! C/Fortran interop for C-allocated strings

  type(c_ptr) :: cptr
  character, pointer :: fptr(:)

  integer :: len

  !--------------------------------------------------------
  ! Init MPI
  !--------------------------------------------------------
  call MPI_Init(ier)
  call MPI_Comm_rank(MPI_COMM_WORLD, myrank, ier)
  call MPI_Comm_size(MPI_COMM_WORLD, mysize, ier)
  call MPI_Comm_dup(MPI_COMM_WORLD, comm, ier)

  !--------------------------------------------------------
  ! Setup data on each process.
  !--------------------------------------------------------
  filename = "synthetic.h5"
  event_name = "event0123456789"
  provenance = "<provenance>"
  quakeml = "<quakeml>"
  !station_xml = "<station_xml>"

  num_stations = 1 + myrank
  num_channels_per_station = 2
  sampling_rate = 0.1
  nsamples = 20
  start_time = 393838

  num_waveforms = num_stations * num_channels_per_station

  allocate(network_names(num_stations), stat=ier)
  allocate(station_names(num_stations), stat=ier)
  allocate(waveforms(nsamples, num_channels_per_station, num_stations), &
           stat=ier)

  do i = 1, num_stations
    write(network_names(i), '(a,i0.2)') "NTWK_", myrank
    write(station_names(i), '(a,i0.2,a,i0.2)') "STAT_", myrank, "_", i
  enddo

  ! -- We do not care about seeding.
  call random_number(waveforms)

  !--------------------------------------------------------
  ! ASDF variables
  !--------------------------------------------------------
  ! Find how many stations are managed by each allgatheress
  allocate(num_stations_gather(mysize))
  call MPI_Allgather(num_stations, 1, MPI_INTEGER, num_stations_gather, 1, &
                     MPI_INTEGER, MPI_COMM_WORLD, ier)
  ! find the largest number of stations per allgatheress
  max_num_stations_gather = maxval(num_stations_gather)

  allocate(displs(mysize))
  allocate(rcounts(mysize))

  ! Everyone should know about each and every station name
  allocate(station_names_gather(max_num_stations_gather, mysize))
  allocate(network_names_gather(max_num_stations_gather, mysize))

  ! The number of stations is not constant across processes
  do i = 1, mysize
    displs(i) = (i-1) * max_num_stations_gather * max_string_length
    rcounts(i) = num_stations_gather(i) * max_string_length
  enddo
  call MPI_Allgatherv(station_names, &
                      num_stations *  MAX_STRING_LENGTH, &
                      MPI_CHARACTER, &
                      station_names_gather, &
                      rcounts, &
                      displs, &
                      MPI_CHARACTER, &
                      MPI_COMM_WORLD, ier)
  call MPI_Allgatherv(network_names, &
                      num_stations *  MAX_STRING_LENGTH, &
                      MPI_CHARACTER, &
                      network_names_gather, &
                      rcounts, &
                      displs, &
                      MPI_CHARACTER, &
                      MPI_COMM_WORLD, ier)

  allocate(station_grps_gather(max_num_stations_gather, mysize))

  allocate(data_ids(num_channels_per_station, &
                    max_num_stations_gather, &
                    mysize))

  !--------------------------------------------------------
  ! write ASDF
  !--------------------------------------------------------
  call ASDF_initialize_hdf5_f(ier);
  call ASDF_create_new_file_f(trim(filename), comm, file_id)

  call ASDF_write_string_attribute_f(file_id, "file_format" // C_NULL_CHAR, &
                                     "ASDF" // C_NULL_CHAR, ier)
  call ASDF_write_string_attribute_f(file_id, "file_version" // C_NULL_CHAR, &
                                     "0.0.1.b" // C_NULL_CHAR, ier)

  call ASDF_write_auxiliary_data_f(file_id, "test1", "test2", ier)
  call ASDF_write_provenance_data_f(file_id, trim(provenance), ier)
  call ASDF_write_quakeml_f(file_id, trim(quakeml), ier)

  call ASDF_create_waveforms_group_f(file_id, waveforms_grp)

  do k = 1, mysize
    do j = 1, num_stations_gather(k)
      call ASDF_create_stations_group_f(waveforms_grp,   &
           trim(network_names_gather(j, k)) // "." //      &
           trim(station_names_gather(j,k)) // C_NULL_CHAR, &
           station_grps_gather(j, k))

      do  i = 1, num_channels_per_station
        ! Generate unique dummy waveform names
        write(waveform_name, '(a,i0.2)') &
           trim(network_names_gather(j,k)) // "." //      &
           trim(station_names_gather(j,k)) // ".channel_", i

        ! Create the dataset where waveform will be written later on.
        call ASDF_define_waveform_f(station_grps_gather(j,k), &
             nsamples, start_time, sampling_rate, &
             trim(event_name) // C_NULL_CHAR, &
             trim(waveform_name) // C_NULL_CHAR, &
             data_ids(i, j, k))
      enddo

    enddo
  enddo


  do j = 1, num_stations
    do i = 1, num_channels_per_station
      call ASDF_write_full_waveform_f(data_ids(i, j, myrank+1), &
                                      waveforms(:, i, j), ier)
    enddo
  enddo

  !--------------------------------------------------------
  ! Clean up
  !--------------------------------------------------------
  do k = 1, mysize
    do j = 1, num_stations_gather(k)
      call ASDF_close_group_f(station_grps_gather(j, k), ier)
      do i = 1, num_channels_per_station
        call ASDF_close_dataset_f(data_ids(i, j, k), ier)
      enddo
    enddo
  enddo

  call ASDF_close_group_f(waveforms_grp, ier)
  call ASDF_finalize_hdf5_f(ier);

  deallocate(data_ids)
  deallocate(station_grps_gather)
  deallocate(station_names_gather)
  deallocate(network_names_gather)
  deallocate(num_stations_gather)

  deallocate(waveforms)
  deallocate(station_names)
  deallocate(network_names)

  call MPI_Finalize(ier)

end program write_example
