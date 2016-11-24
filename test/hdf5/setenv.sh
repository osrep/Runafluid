#!/bin/tcsh -fe

if ("$MDSPLUS_TREE_BASE_0" == "") then
	setenv $MDSPLUS_TREE_BASE_0 $HOME/public/itmdb/itm_trees/aug/4.10b/hdf5
	echo "MDSPLUS_TREE_BASE_0 set to "+$MDSPLUS_TREE_BASE_0 
endif

setenv euitm_path  $MDSPLUS_TREE_BASE_0
echo "euitm_path set to "+$euitm_path 

