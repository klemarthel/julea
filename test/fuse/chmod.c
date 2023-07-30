#include <julea-config.h>
#include <errno.h>
#include "../fuse/file.h"
#include "../fuse/julea-fuse.h"

static void
test_fuse_jfs_chmod(void)
{
	
}
void
test_fuse_chmod(void)
{
	g_test_add_func("/fuse/chmod", test_fuse_jfs_chmod);
}