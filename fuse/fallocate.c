

#include "julea-fuse.h"
#include <julea-config.h>
#include <errno.h>
#include "file.h"
int
jfs_fallocate(const char* path, int mode, off_t offset, off_t len, struct fuse_file_info* fi)
{
	int ret = -EOF;
	(void)fi;
	if (mode)
	{
		ret = -EOPNOTSUPP;
	}
	else
	{
		g_autoptr(JObject) object = NULL;
		g_autoptr(JFileMetadataIn) in = NULL;
		g_autoptr(JFileMetadataOut) out = NULL;
		g_autoptr(JFileSelector) fs = NULL;
		g_autoptr(JBatch) batch = NULL;
		g_autofree gchar* object_name = g_base64_encode(
			(const guchar*)&(fi->fh),
			sizeof(fi->fh));

		batch = j_batch_new_for_template(J_SEMANTICS_TEMPLATE_POSIX);

		object = j_object_new("posix", object_name);
		fs = j_file_selector_new(path);
		in = j_file_metadata_new_load(fs);

		if (in)
		{
			gint64 size;

			size = get_size(in); //TODO fix race condition
			out = j_file_metadata_in_to_out(in);
			if (size <= offset + len)
			{
				guint64 length = size - (offset + len);
				gpointer zeroes = g_malloc0(length);
				guint64 bytes_written;

				j_object_write(object, zeroes, length, size, &bytes_written, batch);
				if (j_batch_execute(batch))
				{
					//TODO change size
					if (bytes_written != length)
					{
						ret = -EIO; //TODO correct return value?
					}
					ret = 0;
				}
				g_free(zeroes);
			}
		}
		
	}

	return ret;
}