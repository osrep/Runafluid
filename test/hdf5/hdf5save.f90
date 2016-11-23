program diagnostic

    use euitm_schemas
    use euitm_routines
    use write_structures
    implicit none

  TYPE (TYPE_COREPROF), pointer :: COREPROF(:)
  TYPE (TYPE_EQUILIBRIUM), pointer :: EQUILIBRIUM(:)
  TYPE (TYPE_CORESOURCE), pointer :: CORESOURCE(:)
  TYPE (TYPE_CORETRANSP), pointer :: CORETRANSP(:)
  TYPE (TYPE_EDGE), pointer :: EDGE(:)
  TYPE (TYPE_DISTRIBUTION), pointer :: DISTRIBUTION(:)
  integer idx, idx3, i, cposize
  integer :: shotnumber,runnumber

    integer :: idx2
    type (type_equilibrium), pointer :: equilibrium2(:)

    Integer x

    call euitm_open('euitm', 28906, 7, idx2)
    
    call euitm_get(idx2, 'equilibrium', equilibrium2)

    write (*,*) "Size of CPO: ", size(equilibrium2)

    write (*,*) "Value of r: ", equilibrium2(1)%eqgeometry%boundary(1)%r(6)
    write (*,*) "Value of z: ", equilibrium2(1)%eqgeometry%boundary(1)%z(6)

    call euitm_close(idx2)


  read (*,*) shotnumber,runnumber


  write(*,*) 'Reading data'
  call euitm_open('euitm',28906, 1007,idx)

  call euitm_get(idx, 'equilibrium', equilibrium)
  cposize=size(equilibrium)
  call euitm_get(idx, 'coreprof', coreprof)
  call euitm_get(idx, 'distribution', distribution)
  !call euitm_get(idx, 'coresource', coresource)
  !call euitm_get(idx, 'coretransp', coretransp)
  !call euitm_get(idx, 'edge', edge)
  call euitm_close(idx)
  write(*,*) 'Read data'

  call euitm_create_hdf5('euitm',9,9998,0,0,idx3)
  call euitm_put(idx3,"coreprof",coreprof)
  call euitm_put(idx3,"equilibrium",equilibrium)
  call euitm_put(idx3,"distribution",distribution)


  call euitm_close(idx3)



end program

