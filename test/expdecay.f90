subroutine expdecay(coreprof,equilibrium,coresource)

use euitm_schemas   !use euITM_routines ! Needed only if using euitm_deallocate and euitm_copy functions
implicit none
integer,parameter :: DP=kind(1.0D0)
type (type_coreprof),pointer :: coreprof(:)
type (type_equilibrium),pointer :: equilibrium(:)
type (type_coresource),pointer :: coresource(:)
real(DP),pointer :: time(:)

return
end subroutine



