program diagnostic
    use euitm_schemas
    use euitm_routines
    use write_structures
    implicit none

    integer :: idx
    type (type_equilibrium), pointer :: equilibrium(:)

    Integer x

    call euitm_open('euitm', 12, 2, idx)

    call euitm_get(idx, 'equilibrium', equilibrium)

    write (*,*) "Size of CPO: ", size(equilibrium)

    write (*,*) "Value of r: ", equilibrium(1)%eqgeometry%boundary(1)%r(1)
    write (*,*) "Value of z: ", equilibrium(1)%eqgeometry%boundary(1)%z(1)

    call euitm_close(idx)
    call euitm_disconnect()
end program
