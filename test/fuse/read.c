#include <julea-config.h>
#include "../fuse/julea-fuse.h"
#include "../fuse/file.h"
#include <errno.h>

static void
test_fuse_jfs_read(void)
{
	
}
void
test_fuse_read(void)
{
	g_test_add_func("/fuse/read", test_fuse_jfs_read);
}