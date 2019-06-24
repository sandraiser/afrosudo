#include <unistd.h>
#include <crypt.h>
#include <stdio.h>
#include <pwd.h>
#include <limits.h>
#include <shadow.h>
#include <errno.h>
#include <stdlib.h>
#include <grp.h>

void join_gid(gid_t group_to_add) {

	int num_groups = getgroups(0, NULL);
	gid_t *groups = calloc (num_groups + 1, sizeof (gid_t));
	getgroups (num_groups, groups + 1);

	// Add the new group.
	groups[0] = group_to_add;
	setgroups (num_groups + 1, groups);
	free(groups);
}

int main(int argc, char **argv) {
	int c;
	uid_t uid = -1;
	gid_t gid = -1;
	pid_t pid;

	if (argc < 2) {
		printf("Usage: %s [-u user] [-g group] cmd args...\n", argv[0]);
		exit(1);
	}
	int run_once = 1;
	while ((c = getopt (argc, argv, "-u:g:")) != -1) {
	switch (c) {
        	case 'u':
			uid = atoi(optarg);
			break;
		case 'g':
			gid = atoi(optarg);
			if (run_once) {
				setgroups(0, NULL);
				setgid(gid);
				run_once=0;
			}
			join_gid(gid);
			break;
		case '-':
			//break;
			goto exit_loop; //end parameter processing
		case '?':
			if (optopt == 'u' || optopt == 'g')
				fprintf (stderr, "Option -%c requires an argument.\n", optopt);
			else
				fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
			return 1;
	}
	}

exit_loop:

	if (setuid(uid) != 0) {
		fprintf(stderr, "setuid fail, current user != 0");
		exit(1);
	} else {
		execvp(argv[optind], &argv[optind]);
		exit(0);
	}
/*
	pid = fork();
	if (pid == -1) {
		fprintf(stderr, "fork\n");
	} else if (pid != 0) {
		execvp(argv[optind], &argv[optind]);
//		execvpe(argv[optind], &argv[optind], envp);
		fprintf(stderr, "execvpe\n");
	}
	exit(0);
*/
}
