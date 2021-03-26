#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

const char* WARN 	= "\e[93m[LAYT_WARN]>\e[39m";
const char* INFO 	= "\e[34m[LAYT_INFO]>\e[39m";

// Print system error
void layt_error(const char* err_msg) {
	char* error_str = strerror(errno);
	printf("\e[31m[LAYT_ERR]> \e[39m%s: %s\n", err_msg, error_str);
}

// Print info
void layt_info(const char* info_msg) {
	printf("\e[34m[LAYT_INFO]>\e[39m %s\n", info_msg);
}

void layt_info_file(const char* info_msg, const char* source) {
	printf("\e[34m[LAYT_INFO]>\e[39m %s %s\n", info_msg, source);
}

int main(int argc, char** argv)
{
	if ( argc != 3 ) {
		layt_error("Argument error\n");
		printf("Usage: ./layt <layout file> <source file>\n");
		exit(1);
	}

	layt_info("Welcome to LAYT!");
	layt_info_file("\e[96mLayout file:\e[39m", argv[1]);
	layt_info_file("\e[96mSource file:\e[39m", argv[2]);

	// Create pipe between processes
	int fd_pipe[2];
	if ( pipe(fd_pipe) < 0 ) {
		layt_error("Pipe: ");
		exit(1);
	}

	// fork and pipe child into parent
	pid_t pid = fork();
	
	// execute the preprocessor
	if ( pid == 0 ) { 
		close(fd_pipe[0]); 		// no need to read from pipe in preprocessor
		close(0);				// close stdin and read from file
		open(argv[1], O_RDONLY);
		close(1);
		dup(fd_pipe[1]);
		execlp("bin/layt-preproc", "bin/layt-preproc", (char *) NULL);
		layt_error("Execlp preprocessor error");
		exit(1);
	}
	// read from pipe to standard input and evaluate source with the instruction layout
	else {
		close(fd_pipe[1]);		// no need to write to pipe in main
		close(0);				// redirect stdin to the pipe
		dup(fd_pipe[0]);
		pid_t pid_main = fork();
		// execute the layout and source parser combo
		if ( pid_main == 0 ) {
			execlp("bin/layt-main", "bin/layt-main", argv[1], argv[2], (char*) NULL);
			layt_error("Execlp preprocessor error");
			exit(1);
		}
		// wait for the parsers to finish
		else {
			if ( waitpid(pid_main, NULL, 0) < 0 ) layt_error("Wait error");
		}
	}

	return 0;
}