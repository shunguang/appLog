#!/bin/bash
usage(){
    echo "Usages:"
    echo "$0 build"
    echo "$0 build --install_dir=path/to/intall/location"
    echo "$0 clean"
}


if [[ ($# -ne 1) && ($# -ne 2) ]]; then
   usage
   exit 1
fi

shopt -s nocasematch


cmd="$1"
install_dir=""
# parse cmd line: Loop through the arguments
while [[ "$#" -gt 0 ]]; do
    case $1 in
        --install_dir=*) install_dir="${1#*=}";;
        *) ;;
    esac
    shift
done

# Print the extracted values if wants
if [[ 0 -ne 0 ]]; then
  echo "cmd: $cmd"
  echo "install_dir: $install_dir"
fi

case "$cmd" in
build)
(
    log_dir=$PWD    
    if [[ -z "${install_dir}" ]];then 
      install_dir=${log_dir}/build
    fi
    echo "install_dir = ${install_dir}"
    
    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${install_dir} ../
    cmake --build . -j$(nproc)
    cmake --install .
)
;;
clean)
(
    rm -rfv build
)
;;
*)
(
    usage
    exit 1
)
;;
esac

exit 0
