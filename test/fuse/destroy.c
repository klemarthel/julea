#include <julea-config.h>
#include "../fuse/file.h"
#include "../fuse/julea-fuse.h"

static void
test_fuse_jfs_destroy(void)
{
	
}
void
test_fuse_destroy(void)
{
	g_test_add_func("/fuse/destroy", test_fuse_jfs_destroy);
}