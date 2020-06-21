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

program read_example
  use mpi
  use iso_c_binding, only: C_NULL_CHAR

  implicit none

  integer, parameter :: MAX_STRING_LENGTH = 256

  character(len=MAX_STRING_LENGTH) :: filename

  integer :: num_stations, num_channels_per_station
  integer :: num_waveforms  ! == num_stations * num_channels_per_station
  ! The number of channels per station is constant, as in SPECFEM
  integer :: nsamples  ! constant, as in SPECFEM
  ! Network names and station names are two different beast, as in SPECFEM
  ! network_names(i) is related to station_names(i)
  character(len=MAX_STRING_LENGTH), dimension(:), allocatable :: network_names
  character(len=MAX_STRING_LENGTH), dimension(:), allocatable :: station_names
  ! data. dimension = nsamples * num_channels_per_station * num_stations
  real, dimension(:, :, :), allocatable :: waveforms

  character(len=MAX_STRING_LENGTH) :: station_name, waveform_name, path

  !-- ASDF variables
  integer(kind=8) :: file_id   ! HDF5 file id, also root group "/"
  integer :: station_exists, waveform_exists
  integer :: nsamples_infered

  !--- MPI variables
  integer :: myrank, mysize, comm
  !--- Loop variables
  integer :: i, j
  !--- Error variable
  integer :: ier

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

  num_stations = 1 + myrank
  num_channels_per_station = 2
  nsamples = 20

  num_waveforms = num_stations * num_channels_per_station

  allocate(network_names(num_stations), stat=ier)
  allocate(station_names(num_stations), stat=ier)
  allocate(waveforms(nsamples, num_channels_per_station, num_stations), &
           stat=ier)

  do i = 1, num_stations
    write(network_names(i), '(a,i0.2)') "NTWK_", myrank
    write(station_names(i), '(a,i0.2,a,i0.2)') "STAT_", myrank, "_", i
  enddo

  !--------------------------------------------------------
  ! Read the ASDF file.
  !--------------------------------------------------------
  call ASDF_initialize_hdf5_f(ier);
  call ASDF_open_read_only_f(trim(filename) // C_NULL_CHAR, comm, file_id)

  do j = 1, num_stations
    station_name = trim(network_names(j)) // '.' // &
                   trim(station_names(j))
    call ASDF_station_exists_f(file_id, &
                               trim(station_name) // C_NULL_CHAR, &
                               station_exists)
    if (station_exists > 0) then
      do  i = 1, num_channels_per_station
        ! Generate dummy waveform names to match write_example.f90 output
        ! Note to fortran users:
        !   be sure to trim your strings and to append '\0' (i.e
        !   C_NULL_CHAR) to them.
        write(waveform_name, '(a,i0.2)') &
            trim(network_names(j)) // "." //      &
            trim(station_names(j)) // ".channel_", i
        call ASDF_waveform_exists_f(file_id, &
                                    trim(station_name) // C_NULL_CHAR, &
                                    trim(waveform_name) // C_NULL_CHAR, &
                                    waveform_exists)
        path = "/Waveforms/" // trim(station_name) // "/" &
            // trim(waveform_name) //  C_NULL_CHAR
        call ASDF_get_num_elements_from_path_f(file_id, path, nsamples_infered)
        if (nsamples_infered == nsamples) then
          call ASDF_read_full_waveform_f(file_id, &
                                         trim(path) // C_NULL_CHAR, &
                                         waveforms(:, i, j), ier)
          if (myrank == mysize-1) then
            print *, "-------------------------------------------------"
            print *, trim(waveform_name)
            print *, "-------------------------------------------------"
            print *, waveforms(:, i, j)
            print *, ""
            !call flush()
          endif
        endif
      enddo
    endif
  enddo

  !--------------------------------------------------------
  ! Clean up
  !--------------------------------------------------------
  call ASDF_close_file_f(file_id, ier)
  call ASDF_finalize_hdf5_f(ier);
  call MPI_Finalize(ier)

end program
