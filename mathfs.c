#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

#include "mathfs.h"

Entry * entries[NUM_DIRECTS];
char * tokens[3];
char output[BUFF_SIZE];
int toks, choice;

// Fill the stbuf with attributes of the directory or file
// Return result of attempt to get attributes
static int mathfs_getattributes(const char *path, struct stat *stbuf)
{
	// Fill the entirety of stbuf with 0s
	memset(stbuf, 0, sizeof(struct stat));

	// ENOENT means no such file or directory
	int res = -ENOENT;

	// if path is '/', return a new directory
	// TODO: What does this mean
	if (strcmp(path, "/") == 0) {
		return new_dir(stbuf);
	}

	choice = -1;
	toks = tokenize(tokens, path);
	for(int i = 0 ; i < NUM_DIRECTS ; ++i){
		if(strcmp(tokens[0], entries[i]->dirName+1) == 0) {
			choice = i;
			if(toks == 1) {
				res = new_dir(stbuf);
			}
			else if(toks == 2){
				if(strcmp(tokens[1], "doc") == 0) {
					res = new_file(stbuf, strlen(documents[i]));
				}
				else if(is_number(tokens[1]) == 0){
					if(entries[choice]->argCount == 2) {
						res = new_dir(stbuf);
					}
					else {
						res = new_file(stbuf, BUFF_SIZE);
					}
				}
			}
			else if(toks == 3) {
				if(is_number(tokens[1]) == 0 && is_number(tokens[2]) == 0) {
					res = new_file(stbuf, BUFF_SIZE);
				}
			}
			break;
		}
	}
	return res;
}

// Fill buffer with contents of given directory path
//  Return result of attempt to read dir
static int mathfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
						  off_t offset, struct fuse_file_info *fi)
{
	(void) offset;
	(void) fi;
	if(strcmp(path, "/") == 0) {
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		for(int i = 0 ; i < NUM_DIRECTS ; ++i)
			filler(buf, entries[i]->dirName+1, NULL, 0);
		return 0;
	}
	for(int i = 0 ; i < NUM_DIRECTS ; ++i) {
		if(strcmp(path, entries[i]->dirName) == 0) {
			filler(buf, ".", NULL, 0);
			filler(buf, "..", NULL, 0);
			filler(buf, "doc", NULL, 0);
			return 0;
		}
	}
	return -ENOENT;
}

// Reads file into content_buffer
static int mathfs_read(const char *path, char *content_buffer, size_t size, off_t offset, struct fuse_file_info *fi)
{
	size_t length;
	(void) fi;
	char * res;
	double x, y = 0;

	toks = tokenize(tokens, path);
	length = 0;
	memset(&output[0], 0, BUFF_SIZE);

	if(strcmp(tokens[1], "doc") == 0){
		res = documents[choice];
	}else{
		sscanf(tokens[1], "%lf", &x);
		sscanf(tokens[2], "%lf", &y);
		res = (*entries[choice]->funcPtr)(&output[0], x, y);
	}

	if((length = strlen(res)) > 0 && res != 0){
		if (offset < length) {
			if (offset + size > length){
				size = length - offset;
				memcpy(content_buffer, res + offset, size);
			} else {
				size = 0;
			}
			return size;
		}
	}
	return -ENOENT;
}

static struct fuse_operations mathfs_oper = {
	.getattr	= mathfs_getattributes,
	.readdir	= mathfs_readdir,
	.read		= mathfs_read,
};

// driver
int main(int argc, char *argv[])
{
	tokens[2] = &output[0];
	fill_table(entries);

	fuse_main(argc, argv, &mathfs_oper, NULL);
	cleanup(entries);

	return 0;
}
