#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "../fuse/file.h"
#include "../fuse/julea-fuse.h"

tatic void
test_fuse_jfs_getattr(void)
{
	
}
void
test_fuse_getattr(void)
{
	g_test_add_func("/fuse/getattr", test_fuse_jfs_getattr);
}