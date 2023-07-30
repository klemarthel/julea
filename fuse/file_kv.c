#include "file.h"
#include "julea-fuse.h"
#include <sys/stat.h>

#ifndef DATABASE_METADATA
#ifndef BSON_METADATA
typedef struct
{
	guint64 object;
	guint64 size;
	guint64 owner;
	guint64 group;
	gint32 mode;
	struct timespec atime;
	struct timespec mtime;
	struct timespec ctime;
} file_metadata;
struct file_struct
{
	file_metadata* metadata;
	guint32 length;
	guint64 ref;
};
#else
struct file_struct
{
	uint8_t* metadata;
	guint32 length;
	guint64 ref;
};
#endif

JFileSelector*
j_file_selector_new(const char* path)
{
	JKV* kv = NULL;
	kv = j_kv_new("posix", path);
	return kv;
}

JFileMetadataIn*
j_file_metadata_new(JFileSelector* fs, JBatch* batch)
{
	guint32* len;
#ifndef BSON_METADATA
	file_metadata** entr;
#else
	uint8_t ** entr;
#endif
	JFileMetadataIn* fe = (struct file_struct*)g_malloc(sizeof(struct file_struct));
	fe->ref = 1;
	fe->metadata = NULL;
	entr = &(fe->metadata);
	len = &(fe->length);
	j_kv_get(fs, (gpointer*)entr, len, batch);
	return fe;
}
JFileMetadataIn*
j_file_metadata_new_load(JFileSelector* fs)
{
	g_autoptr(JBatch) batch;
	JFileMetadataIn* result;
	batch = j_batch_new_for_template(J_SEMANTICS_TEMPLATE_POSIX);
	result = j_file_metadata_new(fs, batch);
	if (!j_batch_execute(batch))
	{
		g_free((gpointer)(result->metadata));
		g_free(result);
		result = NULL;
	}
	return result;
}

#ifndef BSON_METADATA
guint64
get_object(JFileMetadataIn* fe)
{
	return fe->metadata->object;
}
guint64
get_size(JFileMetadataIn* fe)
{
	return fe->metadata->size;
}
guint64
get_owner(JFileMetadataIn* fe)
{
	return fe->metadata->owner;
}
guint64
get_group(JFileMetadataIn* fe)
{
	return fe->metadata->group;
}
gint32
get_mode(JFileMetadataIn* fe)
{
	return fe->metadata->mode;
}
struct timespec*
get_atime(JFileMetadataIn* fe)
{
	return g_memdup2(&fe->metadata->atime, sizeof(struct timespec));
}
struct timespec*
get_mtime(JFileMetadataIn* fe)
{
	return g_memdup2(&fe->metadata->mtime, sizeof(struct timespec));
}
struct timespec*
get_ctime(JFileMetadataIn* fe)
{
	return g_memdup2(&fe->metadata->ctime, sizeof(struct timespec));
}
bool
is_dir(JFileMetadataIn* fe)
{
	return S_ISDIR(get_mode(fe));
}
void
set_object(JFileMetadataOut* fe, guint64 object)
{
	fe->metadata->object = object;
}
void
set_size(JFileMetadataOut* fe, guint64 size)
{
	fe->metadata->size = size;
}
void
set_owner(JFileMetadataOut* fe, guint64 owner)
{
	fe->metadata->owner = owner;
}
void
set_group(JFileMetadataOut* fe, guint64 group)
{
	fe->metadata->group = group;
}
void
set_mode(JFileMetadataOut* fe, gint32 mode)
{
	fe->metadata->mode = mode;
}
void
set_atime(JFileMetadataOut* fe, const struct timespec* atime)
{
	fe->metadata->atime.tv_sec = atime->tv_sec;
	fe->metadata->atime.tv_nsec = atime->tv_nsec;
}
void
set_mtime(JFileMetadataOut* fe, const struct timespec* mtime)
{
	fe->metadata->mtime.tv_sec = mtime->tv_sec;
	fe->metadata->mtime.tv_nsec = mtime->tv_nsec;
}
void
set_ctime(JFileMetadataOut* fe, const struct timespec* ctime)
{
	fe->metadata->ctime.tv_sec = ctime->tv_sec;
	fe->metadata->ctime.tv_nsec = ctime->tv_nsec;
}

#else
guint64
get_object(JFileMetadataIn* fe)
{
	bson_iter_t iterator;
	bson_iter_init_from_data(&iterator, fe->metadata, fe->length);
	bson_iter_find(&iterator, "object");
	return bson_iter_int64(&iterator);
}

guint64
get_size(JFileMetadataIn* fe)
{
	bson_iter_t iterator;
	bson_iter_init_from_data(&iterator, fe->metadata, fe->length);
	bson_iter_find(&iterator, "size");
	return bson_iter_int64(&iterator);
}
guint64
get_owner(JFileMetadataIn* fe)
{
	bson_iter_t iterator;
	bson_iter_init_from_data(&iterator, fe->metadata, fe->length);
	bson_iter_find(&iterator, "owner");
	return bson_iter_int64(&iterator);
}
guint64
get_group(JFileMetadataIn* fe)
{
	bson_iter_t iterator;
	bson_iter_init_from_data(&iterator, fe->metadata, fe->length);
	bson_iter_find(&iterator, "group");
	return bson_iter_int64(&iterator);
}
gint32
get_mode(JFileMetadataIn* fe)
{
	bson_iter_t iterator;
	bson_iter_init_from_data(&iterator, fe->metadata, fe->length);
	bson_iter_find(&iterator, "mode");
	return bson_iter_int32(&iterator);
}
struct timespec*
get_atime(JFileMetadataIn* fe)
{
	struct timespec* ret = (struct timespec*)g_malloc(sizeof(struct timespec));
	bson_iter_t iterator;
	bson_iter_t iterator2;
	bson_iter_init_from_data(&iterator, fe->metadata, fe->length);
	bson_iter_find(&iterator, "atime_n");
	ret->tv_nsec = bson_iter_int32(&iterator);
	bson_iter_init_from_data(&iterator2, fe->metadata, fe->length);
	bson_iter_find(&iterator2, "atime_s");
	ret->tv_sec = bson_iter_int64(&iterator2);
	return ret;
}
struct timespec*
get_mtime(JFileMetadataIn* fe)
{
	struct timespec* ret = (struct timespec*)g_malloc(sizeof(struct timespec));
	bson_iter_t iterator;
	bson_iter_t iterator2;
	bson_iter_init_from_data(&iterator, fe->metadata, fe->length);
	bson_iter_find(&iterator, "mtime_n");
	ret->tv_nsec = bson_iter_int32(&iterator);
	bson_iter_init_from_data(&iterator, fe->metadata, fe->length);
	bson_iter_find(&iterator2, "mtime_s");
	ret->tv_sec = bson_iter_int64(&iterator2);
	return ret;
}
struct timespec*
get_ctime(JFileMetadataIn* fe)
{
	struct timespec* ret = (struct timespec*)g_malloc(sizeof(struct timespec));
	bson_iter_t iterator;
	bson_iter_find(&iterator, "ctime_n");
	bson_iter_init_from_data(&iterator, fe->metadata, fe->length);
	ret->tv_nsec = bson_iter_int32(&iterator);
	bson_iter_t iterator2;
	bson_iter_init_from_data(&iterator, fe->metadata, fe->length);
	bson_iter_find(&iterator2, "ctime_s");
	ret->tv_sec = bson_iter_int64(&iterator2);
	return ret;
}

void
set_object(JFileMetadataOut* fe, guint64 object)
{
	bson_iter_t iterator;
	bson_iter_init_from_data(&iterator, fe->metadata, fe->length);
	bson_iter_find(&iterator, "object");
	bson_iter_overwrite_int64(&iterator, object);
}

void
set_size(JFileMetadataOut* fe, guint64 size)
{
	bson_iter_t iterator;
	bson_iter_init_from_data(&iterator, fe->metadata, fe->length);
	bson_iter_find(&iterator, "size");
	bson_iter_overwrite_int64(&iterator, size);
}
void
set_owner(JFileMetadataOut* fe, guint64 owner)
{
	bson_iter_t iterator;
	bson_iter_init_from_data(&iterator, fe->metadata, fe->length);
	bson_iter_find(&iterator, "owner");
	bson_iter_overwrite_int64(&iterator, owner);
}
void
set_group(JFileMetadataOut* fe, guint64 group)
{
	bson_iter_t iterator;
	bson_iter_init_from_data(&iterator, fe->metadata, fe->length);
	bson_iter_find(&iterator, "group");
	bson_iter_overwrite_int64(&iterator, group);
}
void
set_mode(JFileMetadataOut* fe, gint32 mode)
{
	bson_iter_t iterator;
	bson_iter_init_from_data(&iterator, fe->metadata, fe->length);
	bson_iter_find(&iterator, "mode");
	bson_iter_overwrite_int32(&iterator, mode);
}
void
set_atime(JFileMetadataOut* fe, const struct timespec* atime)
{
	bson_iter_t iterator;
	bson_iter_t iterator2;
	bson_iter_init_from_data(&iterator, fe->metadata, fe->length);
	bson_iter_find(&iterator, "atime_n");
	bson_iter_overwrite_int32(&iterator, atime->tv_nsec);
	bson_iter_init_from_data(&iterator2, fe->metadata, fe->length);
	bson_iter_find(&iterator2, "atime_s");
	bson_iter_overwrite_int64(&iterator2, atime->tv_sec);
}
void
set_mtime(JFileMetadataOut* fe, const struct timespec* mtime)
{
	bson_iter_t iterator;
	bson_iter_t iterator2;
	bson_iter_init_from_data(&iterator, fe->metadata, fe->length);
	bson_iter_find(&iterator, "mtime_n");
	bson_iter_overwrite_int32(&iterator, mtime->tv_nsec);
	bson_iter_init_from_data(&iterator2, fe->metadata, fe->length);
	bson_iter_find(&iterator2, "mtime_s");
	bson_iter_overwrite_int64(&iterator2, mtime->tv_sec);
}
void
set_ctime(JFileMetadataOut* fe, const struct timespec* ctime)
{
	bson_iter_t iterator;
	bson_iter_t iterator2;
	bson_iter_init_from_data(&iterator, fe->metadata, fe->length);
	bson_iter_find(&iterator, "ctime_n");
	bson_iter_overwrite_int32(&iterator, ctime->tv_nsec);
	bson_iter_init_from_data(&iterator2, fe->metadata, fe->length);
	bson_iter_find(&iterator2, "ctime_s");
	bson_iter_overwrite_int64(&iterator2, ctime->tv_sec);
}

void
set_dir(JFileMetadataOut* fe, const char* dir)
{
}
#endif

void
j_file_metadata_write(JFileSelector* fs, JFileMetadataOut* fe, JBatch* batch)
{
	j_kv_put(fs, (gpointer)(fe->metadata), fe->length, NULL, batch);
}
void
j_file_metadata_create(JFileSelector* fs, JFileMetadataOut* fe, JBatch* batch)
{
	j_file_metadata_write(fs, fe, batch);
}

int
j_file_metadata_rename(JFileSelector* old, JFileSelector* new, const char* path)
{
	int ret = -EIO;
	gpointer tmp;
	guint32 len;
	g_autoptr(JBatch) batch = NULL;
	(void)path;
	batch = j_batch_new_for_template(J_SEMANTICS_TEMPLATE_POSIX);
	j_kv_get(old, &tmp, &len, batch);
	if (j_batch_execute(batch))
	{
		ret=-EAGAIN;
		j_kv_delete(old,batch);
		j_kv_put(new, tmp, len, g_free, batch);
		if (j_batch_execute(batch))
		{
			ret = 0;
		}
	}
	return ret;
}


JFileMetadataOut*
j_file_metadata_out_new(const char* path)
{
	
	JFileMetadataOut* out = (JFileMetadataOut*)g_malloc(sizeof(JFileMetadataOut));
	guint32 len;
#ifndef BSON_METADATA
	file_metadata* fe = (file_metadata*)g_malloc(sizeof(file_metadata));
	(void)path;
	len = sizeof(file_metadata);
#else
	(void)path;
	uint8_t* fe;
	bson_t doc;

	bson_init(&doc);
	bson_append_int64(&doc, "object", -1, 0);
	bson_append_int64(&doc, "size", -1, 0);
	bson_append_int64(&doc, "group", -1, 0);
	bson_append_int64(&doc, "owner", -1, 0);
	bson_append_int32(&doc, "mode", -1, 0);

	bson_append_int64(&doc, "atime_s", -1, 0);
	bson_append_int32(&doc, "atime_n", -1, 0);

	bson_append_int64(&doc, "ctime_s", -1, 0);
	bson_append_int32(&doc, "ctime_n", -1, 0);

	bson_append_int64(&doc, "mtime_s", -1, 0);
	bson_append_int32(&doc, "mtime_n", -1, 0);

	fe = bson_destroy_with_steal(&doc, true, &len);
#endif
	out->metadata = fe;
	out->length = len;
	out->ref = 1;
	return out;
}
JFileMetadataOut*
j_file_metadata_out_new_load_for_update(const char* path, JFileSelector* fs)
{
	(void)path;
	return j_file_metadata_new_load(fs);
}
JFileMetadataOut*
j_file_metadata_in_to_out(JFileMetadataIn* in)
{
	in->ref++;
	return in;
}
void
j_file_metadata_delete(JFileSelector* fs, JFileMetadataOut* out, JBatch* batch)
{
	(void)out;
	j_kv_delete(fs, batch);
}

void
j_file_metadata_out_destroy(JFileMetadataOut* out)
{
	if (!out)
		return;
	out->ref -= 1;
	if (out->ref == 0)
	{
		g_free((gpointer)(out->metadata));
		g_free(out);
	}
}
void
j_file_metadata_in_destroy(JFileMetadataIn* in)
{
	if (!in)
		return;
	j_file_metadata_out_destroy(in);
}
void
j_file_selector_destroy(JFileSelector* fs)
{
	if (!fs)
		return;
	j_kv_unref(fs);
}

JDirectoryIterator*
j_directory_iterator_new(const char* dirname)
{
	JKVIterator* iter = j_kv_iterator_new("posix", dirname);
	return iter;
}
gboolean
j_directory_iterator_next(JDirectoryIterator* dir_iter)
{
	return j_kv_iterator_next(dir_iter);
}
char*
j_directory_iterator_get(JDirectoryIterator* dir_iter)
{
	gconstpointer val;
	guint32 len;
	char* result;
	const char* path = j_kv_iterator_get(dir_iter, &val, &len);
	result = g_strdup(path);
	return result;
}
void
j_directory_iterator_destroy(JDirectoryIterator* dir_iter)
{
	j_kv_iterator_free(dir_iter);
}
#endif
