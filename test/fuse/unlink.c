#include <julea-config.h>
#include "../fuse/julea-fuse.h"
#include <errno.h>
#include "../fuse/file.h"

static void
test_fuse_jfs_unlink(void)
{
	
}
void
test_fuse_unlink(void)
{
	g_test_add_func("/fuse/unlink", test_fuse_jfs_unlink);
}