#include <julea-config.h>
#include <errno.h>
#include "../fuse/file.h"
#include "../fuse/julea-fuse.h"

static void
test_fuse_jfs_chown(void)
{
	
}
void
test_fuse_chown(void)
{
	g_test_add_func("/fuse/chown", test_fuse_jfs_chown);
}
