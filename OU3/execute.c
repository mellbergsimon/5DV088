#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "execute.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

/* Duplicate a pipe to a standard I/O file descriptor
 * Arguments:	pip	the pipe
 *		end	tells which end of the pipe shold be dup'ed; it can be
 *			one of READ_END or WRITE_END
 *		destfd	the standard I/O file descriptor to be replaced
 * Returns:	-1 on error, else destfd
 */
int dupPipe(int pip[2], int end, int destfd) {
	if (dup2(pip[end], destfd) < 0) {
		return -1;
	}

	if (close(pip[WRITE_END]) < 0) {
		return -1;
	}

	if (close(pip[READ_END]) < 0) {
		return -1;
	}

	return destfd;
}
/* Redirect a standard I/O file descriptor to a file
 * Arguments:	filename	the file to/from which the standard I/O file
 * 				descriptor should be redirected
 * 		flags	indicates whether the file should be opened for reading
 * 			or writing
 * 		destfd	the standard I/O file descriptor which shall be
 *			redirected
 * Returns:	-1 on error, else destfd
 */
int redirect(char *filename, int flags, int destfd) {

	int fd;

	if ((fd = open(filename, flags, S_IRUSR | S_IWUSR)) < 0) {
		return -1;
	}

	if (dup2(fd, destfd) < 0) {
		return -1;
	}

	if (close(fd) < 0) {
		return -1;
	}

	return destfd;
}