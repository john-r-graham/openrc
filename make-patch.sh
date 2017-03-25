#!/bin/sh

EBUILD_PATH="/usr/local/portage/jrg/sys-apps/openrc"
PV="openrc-0.23.2_p1"

SUFFIX="$1"
if [ -z "${SUFFIX}" ] ; then
	echo "$0: Error: Must specify patch suffix."
	exit 1
fi

echo "Copying patch #${SUFFIX} to the local overlay."
PATCHNAME="${PV}-cmd-args-${SUFFIX}.patch"

set -x
git diff master >/tmp/${PATCHNAME}
sudo chown root:root /tmp/${PATCHNAME}
sudo cp /tmp/${PATCHNAME} ${EBUILD_PATH}/files
sudo rm /tmp/${PATCHNAME}
sudo ebuild ${EBUILD_PATH}/${PV}.ebuild manifest
