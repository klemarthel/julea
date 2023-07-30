#include "file.h"
#include "julea-fuse.h"
#include <stdio.h>

static void
nr_to_object_name(){}
static void
get_object_name(){}
static void
j_file_dir_rename(void){}

#ifdef DATABASE_METADATA
static void
metadata_schema(void){}
#endif

static void
test_fuse_j_file_selector_new(void){}

static void
test_fuse_j_file_metadata_new_load(void){}

static void
test_fuse_j_file_metadata_new(void){}

static void
test_fuse_get_object(void){}

static void
test_fuse_get_size(void){}

static void
test_fuse_get_owner(void){}

static void
test_fuse_get_group(void){}

static void
test_fuse_get_mode(void){}

static void
test_fuse_get_atime(void){}

static void
test_fuse_get_mtime(void){}

static void
test_fuse_get_ctime(void){}

static void
test_fuse_is_dir(void){} 

static void
test_fuse_set_object(void){}

static void
test_fuse_set_size(void){}

static void
test_fuse_set_owner(void){}

static void
test_fuse_set_group(void){}

static void
test_fuse_set_mode(void){}

static void
test_fuse_set_atime(void){}

static void
test_fuse_set_mtime(void){}

static void
test_fuse_set_ctime(void){}

static void
test_fuse_j_file_metadata_write(void){}

static void
test_fuse_j_file_metadata_create(void){}

static void
test_fuse_j_file_metadata_out_new(void){}

static void 
test_fuse_j_file_metadata_out_new_load(void){}

static void
test_fuse_j_file_metadata_out_new_load_for_update(void){}

static void
test_fuse_j_file_metadata_rename(void){}

static void
test_fuse_j_file_metadata_in_to_out(void){}

static void
test_fuse_j_file_metadata_delete(void){}

static void
test_fuse_j_file_metadata_out_destroy(void){}

static void
test_fuse_j_file_metadata_in_destroy(void){}

static void
test_fuse_j_file_selector_destroy(void){}

static void
test_fuse_j_directory_iterator_new(void){}

static void
test_fuse_j_directory_iterator_next(void){}

static void
test_fuse_j_directory_iterator_get(void){}

static void
test_fuse_j_directory_iterator_destroy(void){}



void
test_fuse_file(void)
{
	g_test_add_func("/fuse/file/set_ctime", test_fuse_set_ctime);
}
