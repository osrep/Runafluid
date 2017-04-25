subroutine expdecay(coreprof,equilibrium,coresource)

use euitm_schemas   !use euITM_routines ! Needed only if using euitm_deallocate and euitm_copy functions
use euitm_routines  !allocate_deallocate



      USE  EUITM_SCHEMAS
      USE  EUITM_ROUTINES
      USE  ALLOCATE_DEALLOCATE
      USE  ITM_TYPES
      USE  COPY_STRUCTURES
      USE  DEALLOCATE_STRUCTURES
      
implicit none
integer,parameter :: DP=kind(1.0D0)
type (type_coreprof),pointer :: coreprof(:)
type (type_equilibrium),pointer :: equilibrium(:)
type (type_coresource),pointer :: coresource(:)



! +++ Local variables:
      REAL(R8)                          :: TIME
      REAL(R8),            ALLOCATABLE  :: AMN(:)
      REAL(R8),            ALLOCATABLE  :: ZN(:)
      REAL(R8),            ALLOCATABLE  :: ZION(:)
      REAL(R8),            ALLOCATABLE  :: RHO(:)
      REAL(R8),            ALLOCATABLE  :: VPRIME(:)
      REAL(R8),            ALLOCATABLE  :: JNI(:)
      REAL(R8),            ALLOCATABLE  :: QEL(:)
      REAL(R8),            ALLOCATABLE  :: QION(:,:)
      REAL(R8),            ALLOCATABLE  :: SEL(:)
      REAL(R8),            ALLOCATABLE  :: SION(:,:)
      REAL(R8),            ALLOCATABLE  :: UION(:,:)
      REAL(R8)                          :: R0                  


! +++ Dimensions:
      INTEGER,               PARAMETER  :: NSLICE = 1           
      INTEGER                           :: NPSI                 
      INTEGER                           :: NRHO                
      INTEGER                           :: NNUCL              
      INTEGER                           :: NION               
      INTEGER                           :: NIMP                
      INTEGER,             ALLOCATABLE  :: NZIMP(:)           
      INTEGER                           :: NNEUT                 
      INTEGER,             ALLOCATABLE  :: NCOMP(:)           
      INTEGER,             ALLOCATABLE  :: NTYPE(:)           

      INTEGER                           :: NION2               
      INTEGER                           :: INUCL               
      INTEGER                           :: IION1               
      INTEGER                           :: IION2               


! +++ Set dimensions:
      NRHO                   = SIZE (COREPROF(1)%rho_tor, DIM=1)
      NPSI                   = SIZE (EQUILIBRIUM(1)%profiles_1d%rho_tor, DIM=1)
      NION2                  = 100 
      
      
      CALL GET_COMP_DIMENSIONS      (COREPROF(1)%COMPOSITIONS, NNUCL, NION,  NIMP,  NZIMP, NNEUT, NTYPE, NCOMP)

! +++ Allocate output CPO:
      CALL ALLOCATE_CORESOURCE_CPO  (NSLICE,      NRHO, NNUCL, NION,  NIMP,  NZIMP, NNEUT, NTYPE, NCOMP, CORESOURCE)
      CALL DEALLOCATE_CPO           (CORESOURCE(1)%COMPOSITIONS)
      CALL COPY_CPO                 (COREPROF(1)%COMPOSITIONS, CORESOURCE(1)%COMPOSITIONS)



allocate(coresource(1))
allocate(coresource(1)%codeparam%codename(1))   ! For a string of 132 characters max.
coresource(1)%codeparam%codename(1)   = 'expdecay'
allocate(coresource(1)%codeparam%codeversion(1))   ! For a string of 132 characters max.
coresource(1)%codeparam%codeversion(1)   = '1.0'

return
end subroutine



