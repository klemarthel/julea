#include <julea-config.h>
#include "../fuse/julea-fuse.h"
#include <errno.h>
#include <glib.h>
#include <string.h>
#include "../fuse/file.h"

static void
test_fuse_jfs_write(void)
{
	
}

void
test_fuse_write(void)
{
	g_test_add_func("/fuse/write", test_fuse_jfs_write);
}
