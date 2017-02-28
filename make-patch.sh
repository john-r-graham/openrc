#!/bin/sh

EBUILD_PATH="/usr/local/portage/local/sys-apps/openrc"
PV="openrc-0.22.4"

PATCHNO="$1"
if [ -z "${PATCHNO}" ] ; then
	PATCHNO=1
fi

echo "Copying patch #${PATCHNO} to the local overlay."
PATCHNAME="${PV}-cmd-args-patch${PATCHNO}.patch"

set -x
git diff master >/tmp/${PATCHNAME}
sudo chown root:root /tmp/${PATCHNAME}
sudo cp /tmp/${PATCHNAME} ${EBUILD_PATH}/files
sudo rm /tmp/${PATCHNAME}
sudo ebuild ${EBUILD_PATH}/${PV}.ebuild manifest
