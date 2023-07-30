#include <julea-config.h>
#include "../fuse/julea-fuse.h"
#include <glib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "../fuse/file.h"
#include <fcntl.h>
#include <sys/stat.h>


static void
test_fuse_jfs_rename(void)
{
	
}
void
test_fuse_rename(void)
{
	g_test_add_func("/fuse/rename", test_fuse_jfs_rename);
}