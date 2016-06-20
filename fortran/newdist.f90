subroutine equ2dist(coreprofin,distributionout)

use euITM_schemas
!use euITM_routines ! Needed only if using euitm_deallocate and euitm_copy functions

implicit none

integer,parameter :: DP=kind(1.0D0)
!
! Always describe cpo as array 
! In case of time slice, the size of the input cpo is 1
!
type (type_coreprof),pointer :: coreprofin(:)
type (type_distribution),pointer :: distributionout(:)

integer :: i,j,k,l


write(0,*) 'Entering subroutine coreprof2mhd'

write(0,*) 'size of input CPO = ',size(coreprofin)

! The output CPO must be allocated with its number of time slices (1 for a single time slice physics module)
! Here we allocate the output CPO to the same size as the input CPO (but this is not a general rule)
allocate(distributionout(size(coreprofin)))

! Fill in the output CPO (Physical data)
do i=1,1
   ! Time : copy from input CPO
   write(0,*) 'Received input time from Coreprof : ', coreprofin(i)%time
   distributionout(i)%time = coreprofin(i)%time  ! THE TIME FIELD MUST BE FILLED (MANDATORY) in case of multiple time slice mode for the CPO; NB this information is overwritten by the wrapper in case of single slice mode for the CPO
enddo

allocate(distributionout(1)%codeparam%codename(1))   ! For a string of 132 characters max.
distributionout(1)%codeparam%codename(1)   = 'coreprof2distribution'
allocate(distributionout(1)%codeparam%codeversion(1))   ! For a string of 132 characters max.
distributionout(1)%codeparam%codeversion(1)   = '1.0'
allocate(distributionout(1)%codeparam%parameters(1))   ! For a string of 132 characters max.
distributionout(1)%codeparam%parameters(1) = 'my_code_specific_parameters'
allocate(distributionout(1)%codeparam%output_diag(1))   ! For a string of 132 characters max.
distributionout(1)%codeparam%output_diag(1) = 'my_output_diag'
distributionout(1)%codeparam%output_flag = 0   ! Integer output flag, 0 means the run was successful and can be used in the rest of the workflow, <0 means failure

write(0,*) 'End Subroutine'

return
end subroutine

