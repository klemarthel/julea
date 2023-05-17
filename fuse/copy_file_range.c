#include <julea-config.h>

#include "julea-fuse.h"

#include <errno.h>

#include "file.h"

ssize_t
jfs_copy_file_range(
	const char* path_in, struct fuse_file_info* fi_in, off_t offset_in, const char* path_out, struct fuse_file_info* fi_out, off_t offset_out, size_t size, int flags)
{
	g_autoptr(JBatch) batch = NULL;
	JFileSelector* fs_in = NULL;
	JFileSelector* fs_out = NULL;
	JFileMetadataIn* fe_in = NULL;
	JFileMetadataOut* fe_out = NULL;
	JFileMetadataIn* fo_in = NULL;
	JFileMetadataOut* fo_out = NULL;
	g_autoptr(JObject) object_in = NULL;
	g_autoptr(JObject) object_out = NULL;

	gpointer data = NULL;

	guint32 len_in;
	guint32 len_out;

	ssize_t ret = -EOF;
	guint64 len_read;

	(void)flags;

	data = g_malloc(size); //TODO: catch Error
	fs_in = j_file_selector_new(path_in);
	fs_out = j_file_selector_new(path_out);
	batch = j_batch_new_for_template(J_SEMANTICS_TEMPLATE_POSIX);

	fe_in = j_file_metadata_new(fs_in, batch);
	fo_in = j_file_metadata_new(fs_out, batch);
	if (j_batch_execute(batch))
	{
		g_autofree gchar* object_name_out;
		g_autofree gchar* object_name_in;
		object_name_in = get_object_name(fe_in);
		object_name_out = get_object_name(fo_in);
		object_in = j_object_new("posix", path_in);
		object_out = j_object_new("posix", path_out);

		j_object_read(object_in, data, size, offset_in, &len_read, batch);

		if (j_batch_execute(batch))
		{
			guint64 len_written;
			fe_out = j_file_metadata_in_to_out(fe_in);
			fo_out = j_file_metadata_in_to_out(fo_in);
			//TODO: in
			j_object_write(object_out, data, len_read, offset_out, &len_written, batch);
			if (j_batch_execute(batch))
			{
				size_t len_out1 = len_out;
				size_t len_in1 = len_in;

				gint64 old_size = get_size(fo_in);
				gint64 new_size = old_size + len_written;
				set_size(fo_out, new_size);
				j_file_metadata_write(fs_out, fo_out, batch);
				if (j_batch_execute(batch))
				{
					ret = len_written;
				}
			}
		}
	}
	j_file_metadata_in_destroy(fe_in);
	j_file_metadata_in_destroy(fo_in);
	j_file_metadata_out_destroy(fe_out);
	j_file_metadata_out_destroy(fo_out);
	j_file_selector_destroy(fs_in);
	j_file_selector_destroy(fs_out);
	g_free(data);
	return ret;
}