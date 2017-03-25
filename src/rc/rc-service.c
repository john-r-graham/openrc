/*
 * rc-service.c
 * Finds all OpenRC services
 */

/*
 * Copyright (c) 2008-2015 The OpenRC Authors.
 * See the Authors file at the top-level directory of this distribution and
 * https://github.com/OpenRC/openrc/blob/master/AUTHORS
 *
 * This file is part of OpenRC. It is subject to the license terms in
 * the LICENSE file found in the top-level directory of this
 * distribution and at https://github.com/OpenRC/openrc/blob/master/LICENSE
 * This file may not be copied, modified, propagated, or distributed
 *    except according to the terms contained in the LICENSE file.
 */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "einfo.h"
#include "queue.h"
#include "rc.h"
#include "rc-misc.h"
#include "_usage.h"

const char *applet = NULL;
const char *extraopts = NULL;
const char *getoptstring = "+iINae:r:l" getoptstring_COMMON;
const struct option longopts[] = {
	{ "ifexists",     0, NULL, 'i' },
	{ "ifinactive",   0, NULL, 'I' },
	{ "ifnotstarted", 0, NULL, 'N' },
	{ "args",         0, NULL, 'a' },
	{ "exists",       1, NULL, 'e' },
	{ "resolve",      1, NULL, 'r' },
	{ "list",         0, NULL, 'l' },
	longopts_COMMON
};
const char * const longopts_help[] = {
	"If the service exists then run the command",
	"If the service is inactive then run the command",
	"If the service is not started then run the command",
	"Service command is followed by args",
	"Tests if the service exists or not",
	"Resolve the service name to an init script",
	"List all available services",
	longopts_help_COMMON
};
const char *usagestring = ""
	"Usage: rc-service [options] [-i|-I|-N] <service> [<cmd>...]\n"
	"   or: rc-service [options] [-i|-I|-N] -a <service> [<cmd> [<arg>...]]\n"
	"   or: rc-service [options] -e <service>\n"
	"   or: rc-service [options] -r <service>\n"
	"   or: rc-service [options] -l\n";

int main(int argc, char **argv)
{
	int opt;
	char *service;
	RC_STRINGLIST *list;
	RC_STRING *s;
	RC_SERVICE state;
	bool if_exists = false;
	bool if_inactive = false;
	bool if_notstarted = false;
	bool has_args = false;

	applet = basename_c(argv[0]);
	/* Ensure that we are only quiet when explicitly told to be */
	unsetenv("EINFO_QUIET");

	while ((opt = getopt_long(argc, argv, getoptstring,
		    longopts, (int *) 0)) != -1)
	{
		switch (opt) {
		case 'e':
			service = rc_service_resolve(optarg);
			opt = service ? EXIT_SUCCESS : EXIT_FAILURE;
			free(service);
			return opt;
			/* NOTREACHED */
		case 'i':
			if_exists = true;
			break;
		case 'I':
			if_inactive = true;
			break;
		case 'N':
			if_notstarted = true;
			break;
		case 'l':
			list = rc_services_in_runlevel(NULL);
			if (TAILQ_FIRST(list) == NULL)
				return EXIT_FAILURE;
			rc_stringlist_sort(&list);
			TAILQ_FOREACH(s, list, entries)
			    printf("%s\n", s->value);
			rc_stringlist_free(list);
			return EXIT_SUCCESS;
			/* NOTREACHED */
		case 'r':
			service = rc_service_resolve(optarg);
			if (service == NULL)
				return EXIT_FAILURE;
			printf("%s\n", service);
			free(service);
			return EXIT_SUCCESS;
			/* NOTREACHED */
		case 'a':
			has_args = true;
			break;

		case_RC_COMMON_GETOPT
		}
	}

	argc -= optind;
	argv += optind;
	if (*argv == NULL)
		eerrorx("%s: you need to specify a service", applet);
	if ((service = rc_service_resolve(*argv)) == NULL) {
		if (if_exists)
			return 0;
		eerrorx("%s: service `%s' does not exist", applet, *argv);
	}
	state = rc_service_state(*argv);
	if (if_inactive && ! (state & RC_SERVICE_INACTIVE))
		return 0;
	if (if_notstarted && (state & RC_SERVICE_STARTED))
		return 0;
	if (has_args) {
		*argv-- = "-a";
		argc++;
	}
	*argv = service;
	execv(*argv, argv);
	eerrorx("%s: %s", applet, strerror(errno));
	/* NOTREACHED */
}
