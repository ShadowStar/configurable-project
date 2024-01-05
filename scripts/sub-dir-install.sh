#!/bin/bash -
#===============================================================================
#
#          FILE: sub-dir-install.sh
#
#         USAGE: ./sub-dir-install.sh
#
#   DESCRIPTION:
#
#       OPTIONS: ---
#  REQUIREMENTS: ---
#          BUGS: ---
#         NOTES: ---
#        AUTHOR: ShadowStar, <orphen.leiliu@gmail.com>
#  ORGANIZATION:Gmail
#       CREATED: 01/05/2024 15:51:03
#   LAST CHANGE:01/05/2024 18:09:07
#      REVISION:  ---
#===============================================================================

set -eu -o pipefail  # (e) Exit immediately if a command exits with a non-zero status.
# (u) Treat unset variables as an error when substituting.
# (x) Print commands and their arguments as they are executed.
# (pipefail) The return value of a pipeline is the status of the last
# command to exit with a non-zero status, or zero if no
# command exited with a non-zero status

export LC_ALL=POSIX
export LANG=POSIX

usage () {
	echo "Usage: $0 <dir>/<sub-dir>"
	exit -1
}

[ $# -ge 1 ] || usage

DIR=$(echo "$1" | cut -d "/" -f1)
BASE=$(basename $1)

[ "${DIR}/${BASE}" == "$1" ] || [ "${DIR}/${BASE}/" == "$1" ] || (echo "Install 2 level directory ONLY" && usage)

[ -d "${DIR}/${BASE}" ] && echo "Directory ${DIR}/${BASE} already exist" && exit

mkdir -p ${DIR}/${BASE}
cp scripts/Makefile.template ${DIR}/${BASE}/Makefile

if [ -f "${DIR}"/sub-dir.in ]; then
	sed -e "s/^if ${DIR}\$/if ${DIR}\n\tconfig ${DIR}_${BASE}\n\tbool \"Enable ${BASE}\"\n/" ${DIR}/sub-dir.in > .tmp-${DIR}_${BASE} && \
		mv .tmp-${DIR}_${BASE} ${DIR}/sub-dir.in
else
	sed -e "s/@@dir_name@@/${DIR}/g" -e "s/@@module_name@@/${BASE}/" scripts/sub-dir.in.template > ${DIR}/sub-dir.in
fi

if [ ! -f ${DIR}/sub-dir.mk ]; then
	cp scripts/sub-dir.mk.template ${DIR}/sub-dir.mk
fi
