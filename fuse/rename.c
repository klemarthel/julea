#include <julea-config.h>

#include "julea-fuse.h"
#include <glib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "file.h"
#include <fcntl.h>
#include <sys/stat.h>

int
jfs_rename(const char* old, const char* new, unsigned int flags)
{
	int ret = -EOF;
	

	g_autoptr(JBatch) batch = NULL;
	g_autoptr(JObject) object = NULL;
	g_autoptr(JFileSelector) old_fs = NULL;
	g_autoptr(JFileSelector) new_fs = NULL;
	g_autoptr(JFileMetadataIn) old_in = NULL;
	g_autoptr(JFileMetadataIn) new_in = NULL;
	g_autoptr(JFileMetadataOut) out = NULL;
	g_autoptr(JFileMetadataOut) new_out = NULL;
	if ((flags & 1) | (flags & 2) | (flags & 4))
	{
		return -EOPNOTSUPP;
	}
	old_fs = j_file_selector_new(old);
	new_fs = j_file_selector_new(new);

	batch = j_batch_new_for_template(J_SEMANTICS_TEMPLATE_POSIX);
	old_in = j_file_metadata_new_load(old_fs);
	new_in = j_file_metadata_new_load(new_fs);
	if (old_in)
	{
		if (S_ISDIR(get_mode(old_in)))
		{
			if (new_in)
			{
				new_out = j_file_metadata_in_to_out(new_in);
				j_file_metadata_delete(new_fs, new_out, batch);

				if (j_batch_execute(batch))
				{
					ret = 0;
				}
			}
			ret = j_file_dir_rename(old, new);
			if(ret==0){
				j_file_metadata_rename(old_fs, new_fs,new);
			}
		}
		else if (S_ISREG(get_mode(old_in)))
		{
			if (new_in)
			{

				g_autofree gchar* object_name = get_object_name(new_in);
				new_out = j_file_metadata_in_to_out(new_in);
				j_file_metadata_delete(new_fs, new_out, batch);
				object = j_object_new("posix", object_name);
				j_object_delete(object, batch);
				if (j_batch_execute(batch))
				{
					ret = 0;
				}
			}
			ret = j_file_metadata_rename(old_fs, new_fs,new);
		}
	}

	return ret;
}