#!/bin/sh

#
# Put variable names here that openrc-run should collect:
#
SVC_INF_VARS="description allow_args"
#
# These three variable names define service script specific
# commands. These are special because a command description
# variable may be defined (and should be collected).
#
SVC_CMD_VARS="extra_commands extra_started_commands extra_stopped_commands"

conditional_emit_var() {
	VARN="$1"
	eval VARV=\$$VARN

	if [ -n "$VARV" ] ; then
		echo "$VARN=\"$VARV\""
	fi
}

if [ -z "$1" ] ; then
	exit 0
 fi

if ! rc-service -e "$1" ; then
	exit 0
fi

source $(rc-service --resolve "$1")

for i in $SVC_CMD_VARS $SVC_INF_VARS; do
	conditional_emit_var $i
done

for i in $SVC_CMD_VARS ; do
	eval VAR=\$$i
	for j in $VAR ; do
		conditional_emit_var description_$j
	done
done
