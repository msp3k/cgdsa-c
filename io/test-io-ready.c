#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "io-ready.h"

int main(int argc, char ** argv)
{
	int fd[2] = { 0, 0 };
	char buf[4096] = { 0 };

	assert(pipe(fd) == 0);

	assert(write_ready(fd[0]) == false);
	assert(read_ready(fd[0]) == false);
	assert(write_ready(fd[1]) == true);
	assert(read_ready(fd[1]) == false);

	assert(write(fd[1], "hello world", strlen("hello world")) == strlen("hello world"));

	assert(write_ready(fd[0]) == false);
	assert(read_ready(fd[0]) == true);
	assert(write_ready(fd[1]) == true);
	assert(read_ready(fd[1]) == false);

	assert(read(fd[0], buf, 4095) == strlen("hello world"));
	assert(strcmp(buf, "hello world") == 0);

	assert(write_ready(fd[0]) == false);
	assert(read_ready(fd[0]) == false);
	assert(write_ready(fd[1]) == true);
	assert(read_ready(fd[1]) == false);

	while (write_ready(fd[1]))
		write(fd[1], "hello world", strlen("hello world"));

	assert(write_ready(fd[0]) == false);
	assert(read_ready(fd[0]) == true);
	assert(write_ready(fd[1]) == false);
	assert(read_ready(fd[1]) == false);

	while (read_ready(fd[0]))
		read(fd[0], buf, 4095);

	assert(write_ready(fd[0]) == false);
	assert(read_ready(fd[0]) == false);
	assert(write_ready(fd[1]) == true);
	assert(read_ready(fd[1]) == false);

	return(0);
}
