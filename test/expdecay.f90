subroutine expdecay(in_coreprof,in_equilibrium,out_coresource)

use euitm_schemas   !use euITM_routines ! Needed only if using euitm_deallocate and euitm_copy functions
implicit none
integer,parameter :: DP=kind(1.0D0)
type (type_coreprof),pointer :: in_coreprof(:)
type (type_equilibrium),pointer :: in_equilibrium(:)
type (type_coresource),pointer :: out_coresource(:)
real(DP),pointer :: time(:)
allocate(out_coresource(1))
allocate(out_coresource(1)%codeparam%codename(1))   ! For a string of 132 characters max.
out_coresource(1)%codeparam%codename(1)   = 'expdecay'
allocate(out_coresource(1)%codeparam%codeversion(1))   ! For a string of 132 characters max.
out_coresource(1)%codeparam%codeversion(1)   = '1.0'

return
end subroutine



