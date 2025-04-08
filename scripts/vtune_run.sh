#!/bin/bash

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
    *)   return ;;
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

EXEC_CMD="$1"
[[ ! -e $EXEC_CMD ]] && { echo "\$1 doesn't exist."; exit 1; }
[[ ! -x $EXEC_CMD ]] && { echo "\$1 unexecuteable."; exit 1; }

COLLECT_TARGET=$(collect_target_mapper "$2")
COLLECT_TARGET="${COLLECT_TARGET:-performance-snapshot}"

RESULT_DIR_PREFIX="./vtune_result"

KNOB=""
SAMPLING_INTERVAL="0.5"
#KNOB+=" -knob sampling-mode=hw "
#KNOB+=" -knob sampling-interval=$SAMPLING_INTERVAL "

KNOB_TEXT=${knob// /_}
KNOB_TEXT=${KNOB_TEXT##_} # prefix
KNOB_TEXT=${KNOB_TEXT%%_}

RESULT_DIR="$RESULT_DIR_PREFIX/$(basename $EXEC_CMD .exe)_collect_${COLLECT_TARGET}_$KNOB_TEXT"
RESULT_DIR=${RESULT_DIR##_} # prefix
RESULT_DIR=${RESULT_DIR%%_}





VTUNE_CMD="vtune -collect $COLLECT_TARGET -r $RESULT_DIR $KNOB $EXEC_CMD"
VTUNE_GUI_CMD="vtune-gui $RESULT_DIR"





check_and_delete_dir "$(realpath $RESULT_DIR)"

echo "$VTUNE_CMD"
echo "$VTUNE_GUI_CMD"


