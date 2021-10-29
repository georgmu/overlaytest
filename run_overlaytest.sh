#!/bin/bash

P=1 # number of parallel instances

set -e
if [ "$1" ]; then
  P=$(($1))
fi

SCRIPT=$(readlink -f "$0")
SCRIPT_DIR=$(dirname "${SCRIPT}")

# compile test program
gcc "$SCRIPT_DIR/tmpfiletest.c" -o "$SCRIPT_DIR/tmpfiletest"

BASEDIR="$PWD"
mkdir -p lower upper overlay work

mount -t overlay overlay -o "lowerdir=$BASEDIR/lower,upperdir=$BASEDIR/upper,workdir=$BASEDIR/work" "$BASEDIR/overlay"
cd "$BASEDIR/overlay"

for i in $(seq 1 $P); do
	"${SCRIPT_DIR}/tmpfiletest" &
done

wait $(jobs -rp)

cd "$BASEDIR"
umount "${BASEDIR}/overlay"

rmdir lower upper overlay work/work work
