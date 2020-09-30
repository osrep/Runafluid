#!/bin/bash

function update_submodule {
    mod=$1
    update=$2

    git_major_version=$(git --version | cut -d ' ' -f 3 | cut -d '.' -f 1)
    if [[ $git_major_version < 2 ]]; then
        echo "Please use at least git 2, usually 'module load git'"
        exit 1
    fi

    # Initialize the submodule. This just copies .gitmodules config to .git/config, so always do this
    git submodule init $mod
    # Check if the url was indeed added to .git/config
    url=$(git config --get submodule.$mod.url)
    if [ ! $? -eq 0 ]; then
        echo "Could not find url of $mod" >&2
        return 1
    fi

    # Check the status of the submodule. We are happy if the checked out hashes are the same
    stat=$(git -C $mod rev-parse HEAD)
    mod_head=$(git submodule status -- $mod | grep -oP '^.\K\w+')
    if [[ "$stat" == "$mod_head" ]]; then
        submodules_avail+=($mod)
        return 0
    fi

    # Update if the user requested submodules to be updated (SUBMODULE_UPDATE=true or unset)
    if [[ "$update" = "true" ]]; then
        # Check access to the submodule repository
        git ls-remote $url > /dev/null 2>&1
        if  [ ! $? -eq 0 ]; then
            echo "Could not access $url" >&2
            if [[ $url == git* ]]; then
                echo "Did you set up SSH keys and requested source code access?" >&2
            else
                echo "Did you request source code access?" >&2
            fi
            return 1
        fi
        # Run submodule update command and check result
        git submodule update $mod
        if  [ ! $? -eq 0 ]; then
            echo "Problem updating $mod"
            return 1
        else
            submodules_avail+=($mod)
        fi
    else
        echo "WARNING! SUBMODULE_UPDATE=false, but $mod is out of sync. I hope you know what you are doing!"
    fi
}

function update_submodules {
    submodules=$1
    update=$2
    submodules_avail=()
    for mod in ${submodules}; do
        update_submodule $mod $update
    done
    IFS=' ' read -r -a submodule_arr <<< $submodules
    equal=true
    for i in $(seq 1 ${#submodule_arr[@]}); do
      if [[ ! ${submodule_arr[i-1]} = ${submodules_avail[i-1]} ]]; then
          equal=false
          break
      fi
    done
    if [[ "$equal" = "false" ]]; then
        echo "WARNING! Not all requested submodules were updated"
        return 1
    fi
    echo Available submodules: $(for i in ${submodules_avail[@]}; do echo $i; done)
}

function check_submodule_avail {
    submodules=$1
    transport_modules=$2
    IFS=',' read -r -a transport_arr <<< $transport_modules
    IFS=',' read -r -a submodule_arr <<< $submodules
    exit_code=0
    for trans in ${transport_arr[@]}; do
        for mod in ${submodule_arr[@]}; do
            if [[ $trans = 'QLK' ]]; then
                break 2
            fi
            echo "WARNING! $trans requested, but not available via submodules"
            exit_code=1
        done
    done
    return $exit_code
}
