#include <julea-config.h>
#include "../fuse/julea-fuse.h"
#include <errno.h>
#include "../fuse/file.h"

static void
test_fuse_jfs_rmdir(void)
{
	
}
void
test_fuse_rmdir(void)
{
	g_test_add_func("/fuse/rmdir", test_fuse_jfs_rmdir);
}