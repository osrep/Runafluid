################################
# Testing actor before release #
#                              #
#       (c) Matyas Aradi, 2017 #
################################

#!/bin/tcsh

# environment preferences
setenv kepler_version "4.10b.10"
setenv runin_stable_version "1.6.0"
setenv runafluid_stable_version "1.3.1"

# help description
if ( $1  =~  {,-h,-help,--help} ) then
    printf "\nUsage:\n\t./release_actor.sh MODULE.\n\n"
    printf "This script is written for simplifying the actor release for Kepler.\n(c) Matyas Aradi, 2017\n\n"
    printf "Here is the list of possible arguments for MODULE:\n"
    printf "\trunin\tstable version of Runaway Indicator (RUNIN)\n"
    printf "\trunafluid\tstable version of Runaway Fluid (RUNAFLUID)\n"
    printf "\trunin_rc\trelease candidate version of Runaway Indicator (RUNIN)\n"
    printf "\trunafluid_rc\trelease candidate version of Runaway Fluid (RUNAFLUID)\n"
    printf "\n\t-h or --help\tdisplay this help and exit\n"
    printf "\n\nIf you would like to release other code, please set this endifle manually and add the description into the #help description section.\n\n"   
    exit 1
endif

# switch testing code
if ( $1 == "runin_rc" ) then
    setenv repo "https://gforge.efda-itm.eu/svn/runin/branches/release-branch"
    setenv actors "runaway_indicator"
else if ( $1 == "runafluid_rc" ) then
    setenv repo "https://gforge.efda-itm.eu/svn/runafluid/branches/release-candidate"
    setenv actors "runaway_fluid,runafluid_efieldEdit"
else if ( $1 == "runin" ) then
    setenv repo "https://gforge.efda-itm.eu/svn/runin/tags/"$kepler_version"_"$runin_stable_version"_runaway_indicator"
    setenv actors "runaway_indicator,runaway_postprocessor"
else if ( $1 == "runafluid" ) then
    setenv repo "https://gforge.efda-itm.eu/svn/runafluid/tags/"$kepler_version"_"$runafluid_stable_version"_runaway_fluid"
    setenv actors "runaway_fluid,runafluid_efieldEdit"
else
    printf "\nERROR:\n\tInvalid MODULE name: $1\n\nFor further info, use ./release_actor.sh --help.\n\n"
    exit 1
endif

# cleaning
mkdir -p $ITMWORK/release
rm -rf $ITMWORK/release/actor_tester_$1
rm -f $ITMWORK/release/actor_release_$1
rm -f $ITMWORK/release/TEST_SUMMARY_$1

# init
echo "$repo $actors" > $ITMWORK/release/actor_release_$1.txt

# release test
test_actor_build.sh $ITMWORK/release/actor_tester_$1 $ITMWORK/release/actor_release_$1.txt 


# endifnished
cp $ITMWORK/release/actor_tester_$1/TEST_SUMMARY $ITMWORK/release/TEST_SUMMARY_$1
printf "\nThe actor was tested from the following repository:\n\t"$repo"\n\nThe output saved into $ITMWORK/release/actor_release_$1.txt\n"
printf "The output saved into $ITMWORK/release/TEST_SUMMARY_$1\n\n"
