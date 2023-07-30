#include <julea-config.h>
#include <errno.h>
#include "../fuse/file.h"
#include "../fuse/julea-fuse.h"

static void
test_fuse_jfs_access(void)
{
	
}
void
test_fuse_access(void)
{
	g_test_add_func("/fuse/access", test_fuse_jfs_access);
}
