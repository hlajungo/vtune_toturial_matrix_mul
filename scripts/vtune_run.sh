#!/bin/bash

show_collect_target ()
{
cat << EOF
 ps)performance-snapshot
 h) hotspots
 ad)anomaly-detection
 mc)memory-consumption
 ue)uarch-exploration
 ma)memory-access
 t) threading
 hp)hpc-performance
 i) io
 eo)exp-offload
 go)gpu-offload
 gh)gpu-hotspots
 fi)fpga-interaction
 n) npu
 so)system-overview
 gr)graphics-rendering
 te)tsx-exploration
 th)tsx-hotspots
 sh)sgx-hotspots
 a) aps
EOF
}

show_help ()
{
cat << EOF
-h | --help Show help
-sct | --show_collect_target Show collectable targets
EOF
}


collect_target_mapper() {
  case "$1" in
    ps)  echo "performance-snapshot" ;;
    h)   echo "hotspots" ;;
    ad)  echo "anomaly-detection" ;;
    mc)  echo "memory-consumption" ;;
    ue)  echo "uarch-exploration" ;;
    ma)  echo "memory-access" ;;
    t)   echo "threading" ;;
    hp)  echo "hpc-performance" ;;
    i)   echo "io" ;;
    eo)  echo "exp-offload" ;;
    go)  echo "gpu-offload" ;;
    gh)  echo "gpu-hotspots" ;;
    fi)  echo "fpga-interaction" ;;
    n)   echo "npu" ;;
    so)  echo "system-overview" ;;
    gr)  echo "graphics-rendering" ;;
    te)  echo "tsx-exploration" ;;
    th)  echo "tsx-hotspots" ;;
    sh)  echo "sgx-hotspots" ;;
    a)   echo "aps" ;;
    *)   return 1;;
  esac
}

check_and_delete_dir()
{
  local TARGET_DIR="$1"  # 取得函數參數

  if [ "$(ls -A "$TARGET_DIR" 2>/dev/null)" ]; then
    # User Input
    read -p "Delete warning: $TARGET_DIR is not empty. Delete its contents? (y/yes to confirm) " confirmation
    if [[ "$confirmation" =~ ^(y|Y|yes|Yes)$ ]]; then
      echo "Deleting contents of $TARGET_DIR"
      if [[ -w $(dirname $TARGET_DIR) ]]; then
        rm -rf "$TARGET_DIR"
      else
        sudo rm -rf "$TARGET_DIR"
      fi
    else
      echo "Deletion aborted."
    fi
    #else
    # echo  "$TARGET_DIR is empty. No deletion needed."
  fi
}


ARGS=$(getopt -o \
h\
\
\
 --long \
help,sct\
\
 -n $(basename "$0") -- "$@")

eval set -- "$ARGS"

while true; do
  case "$1" in
    --show_collect_target | --sct )
      show_collect_target
      exit 0
      ;;
    -h | --help )
      show_help
      exit 0
      ;;
    --)
      shift
      break
      ;;
    *)
      echo "Internal error!" >&2
      exit 1
      ;;
	esac
done


COLLECT_TARGET=$(collect_target_mapper "$1")
[[ $? -eq 1 ]] && { echo error collect target.; exit 1; }
shift 1

#EXEC_CMD="$1"
#[[ ! -e $EXEC_CMD ]] && { echo "file doesn't exist."; exit 1; }
#[[ ! -x $EXEC_CMD ]] && { echo "file unexecuteable."; exit 1; }



RESULT_DIR_PREFIX="./vtune_result/$COLLECT_TARGET/"
mkdir -p $RESULT_DIR_PREFIX

#KNOB=""
#SAMPLING_INTERVAL="0.5"
#KNOB+=" -knob sampling-mode=hw "
#KNOB+=" -knob sampling-interval=$SAMPLING_INTERVAL "

#KNOB_TEXT=${knob// /_}
#KNOB_TEXT=${KNOB_TEXT##_} # prefix
#KNOB_TEXT=${KNOB_TEXT%%_}

#COLLECT_TARGET="${COLLECT_TARGET:-performance-snapshot}"
#
#RESULT_DIR="$RESULT_DIR_PREFIX/_${COLLECT_TARGET}_$KNOB_TEXT"
#RESULT_DIR=${RESULT_DIR##_} # prefix
#RESULT_DIR=${RESULT_DIR%%_}


RESULT_DIR="$RESULT_DIR_PREFIX"

VTUNE_CMD="vtune -collect $COLLECT_TARGET -r $RESULT_DIR $KNOB $EXEC_CMD"
VTUNE_GUI_CMD="vtune-gui $RESULT_DIR"

#check_and_delete_dir "$(realpath $RESULT_DIR)"

echo "$VTUNE_CMD"
#echo "$VTUNE_GUI_CMD"


