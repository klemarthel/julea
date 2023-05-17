#include "file.h"
#include "julea-fuse.h"
#include <stdio.h>

char*
nr_to_object_name(guint64 nr){
	char* res=(char*)g_malloc(25);
	sprintf(res,"%lu",nr);
	return res;
}
char*
get_object_name(JFileMetadataIn* in){
	guint64 nr=get_object(in);
	return nr_to_object_name(nr);
}
int j_file_dir_rename(const char* old,const char* new){
	g_autoptr(JDirectoryIterator) iter=NULL;
	size_t old_size=strlen(old);
	size_t new_size=strlen(new);
	size_t diff_size=new_size-old_size;
	g_autofree gchar* prefix=NULL;
	int ret=0;
	if (g_strcmp0(old, "/") == 0)
	{
		prefix = g_strdup(old);
	}
	else
	{
		prefix = g_strdup_printf("%s/", old);
	}
	iter=j_directory_iterator_new(prefix);
	while (j_directory_iterator_next(iter))
	{
		g_autofree gchar* current;
		g_autofree gchar* new_name;
		g_autoptr(JFileSelector) old_fs=NULL;
		g_autoptr(JFileSelector) new_fs=NULL;

		current=j_directory_iterator_get(iter);

		old_fs=j_file_selector_new(current);
		new_name=(gchar*)g_malloc(strlen(current)+diff_size+1);
		memcpy(new_name,new,new_size);
		memcpy(new_name+new_size,current+old_size,strlen(current)-old_size+1);
		new_fs=j_file_selector_new(new_name);
		ret=j_file_metadata_rename(old_fs,new_fs,new_name);
		if (ret!=0)
		{
			break;
		}
		
	}
	
	return ret;
}
#ifdef DATABASE_METADATA
JDBSchema* metadata_schema(){
	JDBSchema* schema=NULL;
	gboolean success=false;
	const gchar* indizes[]={"path",NULL};
	schema=j_db_schema_new("posix","metadata", NULL);
	success=j_db_schema_add_field(schema,"path",J_DB_TYPE_STRING,NULL);
	success=j_db_schema_add_field(schema,"object",J_DB_TYPE_UINT64,NULL);
	success=j_db_schema_add_field(schema,"file",J_DB_TYPE_SINT32,NULL);
	success=j_db_schema_add_field(schema,"size",J_DB_TYPE_UINT64,NULL);
	success=j_db_schema_add_field(schema,"owner",J_DB_TYPE_UINT64,NULL);
	success=j_db_schema_add_field(schema,"group",J_DB_TYPE_UINT64,NULL);
	success=j_db_schema_add_field(schema,"mode",J_DB_TYPE_SINT32,NULL);
	success=j_db_schema_add_field(schema,"dir",J_DB_TYPE_STRING,NULL);
	success=j_db_schema_add_field(schema,"atime_n",J_DB_TYPE_UINT64,NULL);
	success=j_db_schema_add_field(schema,"mtime_n",J_DB_TYPE_UINT64,NULL);
	success=j_db_schema_add_field(schema,"ctime_n",J_DB_TYPE_UINT64,NULL);
	success=j_db_schema_add_field(schema,"atime_s",J_DB_TYPE_UINT64,NULL);
	success=j_db_schema_add_field(schema,"mtime_s",J_DB_TYPE_UINT64,NULL);
	success=j_db_schema_add_field(schema,"ctime_s",J_DB_TYPE_UINT64,NULL);
	(void)success;
	j_db_schema_add_index(schema,indizes,NULL);
	return schema;
}

JFileSelector*
j_file_selector_new(const char* path)
{
	JDBSelector* selector = NULL;

	selector = j_db_selector_new(db_schema, J_DB_SELECTOR_MODE_AND, NULL); //TODO Error Handling
	j_db_selector_add_field(selector, "path", J_DB_SELECTOR_OPERATOR_EQ, path, 5, NULL);
	return selector;
}

JFileMetadataIn*
j_file_metadata_new_load(JFileSelector* fs)
{
	JDBIterator* iterator = NULL;
	iterator = j_db_iterator_new(db_schema, fs, NULL);
	if (!j_db_iterator_next(iterator, NULL))
	{
		j_db_iterator_unref(iterator);
		iterator = NULL;
	}
	return iterator;
}
JFileMetadataIn*
j_file_metadata_new(JFileSelector* fs, JBatch* batch)
{
	(void)batch;
	return j_file_metadata_new_load(fs);
}

/*char*
get_name(JFileMetadataIn* fe)
{
	gpointer tmp;
	char* result;
	guint64 length;
	JDBType type;
	j_db_iterator_get_field(fe, "name", &type, &tmp, &length, NULL);
	result = (char*)tmp;
	return result;
}*/
guint64
get_object(JFileMetadataIn* fe)
{
	gpointer tmp;
	guint64 result;
	guint64 length;
	JDBType type;
	j_db_iterator_get_field(fe, "object", &type, &tmp, &length, NULL);
	result = *((guint64*)tmp);
	return result;
}
guint64
get_size(JFileMetadataIn* fe)
{
	gpointer tmp;
	guint64 result;
	guint64 length;
	JDBType type;
	j_db_iterator_get_field(fe, "size", &type, &tmp, &length, NULL);
	result = *((gint64*)tmp);
	g_free(tmp);
	return result;
}
guint64
get_owner(JFileMetadataIn* fe)
{
	gpointer tmp;
	guint64 result;
	guint64 length;
	JDBType type;
	j_db_iterator_get_field(fe, "owner", &type, &tmp, &length, NULL);
	result = *((gint64*)tmp);
	g_free(tmp);
	return result;
}
guint64
get_group(JFileMetadataIn* fe)
{
	gpointer tmp;
	guint64 result;
	guint64 length;
	JDBType type;
	j_db_iterator_get_field(fe, "group", &type, &tmp, &length, NULL);
	result = *((gint64*)tmp);
	g_free(tmp);
	return result;
}
gint32
get_mode(JFileMetadataIn* fe)
{
	gpointer tmp;
	guint32 result;
	guint64 length;
	JDBType type;
	j_db_iterator_get_field(fe, "mode", &type, &tmp, &length, NULL);
	result = *((gint32*)tmp);
	g_free(tmp);
	return result;
}
struct timespec*
get_atime(JFileMetadataIn* fe)
{
	struct timespec* result=(struct timespec*)g_malloc(sizeof(struct timespec));
	gpointer tmp_n;
	guint64 length_n;
	JDBType type_n;
	
	gpointer tmp_s;
	guint64 length_s;
	JDBType type_s;

	j_db_iterator_get_field(fe, "atime_n", &type_n, &tmp_n, &length_n, NULL);
	j_db_iterator_get_field(fe, "atime_s", &type_s, &tmp_s, &length_s, NULL);
	result->tv_nsec = *((gint64*)tmp_n);
	result->tv_sec = *((gint64*)tmp_s);
	g_free(tmp_n);
	g_free(tmp_s);
	return result;
}
struct timespec*
get_mtime(JFileMetadataIn* fe)
{
	struct timespec* result=(struct timespec*)g_malloc(sizeof(struct timespec));
	gpointer tmp_n;
	guint64 length_n;
	JDBType type_n;
	
	gpointer tmp_s;
	guint64 length_s;
	JDBType type_s;

	j_db_iterator_get_field(fe, "mtime_n", &type_n, &tmp_n, &length_n, NULL);
	j_db_iterator_get_field(fe, "mtime_s", &type_s, &tmp_s, &length_s, NULL);
	result->tv_nsec = *((guint64*)tmp_n);
	result->tv_sec = *((guint64*)tmp_s);
	g_free(tmp_n);
	g_free(tmp_s);
	return result;
}
struct timespec*
get_ctime(JFileMetadataIn* fe)
{
	struct timespec* result=(struct timespec*)g_malloc(sizeof(struct timespec));
	gpointer tmp_n;
	guint64 length_n;
	JDBType type_n;
	
	gpointer tmp_s;
	guint64 length_s;
	JDBType type_s;

	j_db_iterator_get_field(fe, "ctime_n", &type_n, &tmp_n, &length_n, NULL);
	j_db_iterator_get_field(fe, "ctime_s", &type_s, &tmp_s, &length_s, NULL);
	result->tv_nsec = *((guint64*)tmp_n);
	result->tv_sec = *((guint64*)tmp_s);
	g_free(tmp_n);
	g_free(tmp_s);
	return result;
}
bool is_dir(JFileMetadataIn* fe){
	gpointer tmp;
	guint64 result;
	guint64 length;
	JDBType type;
	j_db_iterator_get_field(fe, "file", &type, &tmp, &length, NULL);
	result = *((guint32*)tmp);
	g_free(tmp);
	return !(bool)result;
} 

void
set_object(JFileMetadataOut* fe, guint64 object)
{
	j_db_entry_set_field(fe, "object", &object, -1, NULL);
}
void
set_size(JFileMetadataOut* fe, guint64 size)
{
	j_db_entry_set_field(fe, "size", &size, -1, NULL);
}
void
set_owner(JFileMetadataOut* fe, guint64 owner)
{
	j_db_entry_set_field(fe, "owner", &owner, -1, NULL);
}
void
set_group(JFileMetadataOut* fe, guint64 group)
{
	j_db_entry_set_field(fe, "group", &group, -1, NULL);
}
void
set_mode(JFileMetadataOut* fe, gint32 mode)
{
	gboolean file=!S_ISDIR(mode);
	j_db_entry_set_field(fe, "file", &file, -1, NULL);
	j_db_entry_set_field(fe, "mode", &mode, -1, NULL);
}
void
set_atime(JFileMetadataOut* fe,const struct timespec*  atime)
{
	j_db_entry_set_field(fe, "atime_n", &(atime->tv_nsec), -1, NULL);
	j_db_entry_set_field(fe, "atime_s", &(atime->tv_sec), -1, NULL);
}
void
set_mtime(JFileMetadataOut* fe,const struct timespec* mtime)
{
	j_db_entry_set_field(fe, "mtime_n", &(mtime->tv_nsec), -1, NULL);
	j_db_entry_set_field(fe, "mtime_s", &(mtime->tv_sec), -1, NULL);
}
void
set_ctime(JFileMetadataOut* fe,const struct timespec* ctime)
{
	j_db_entry_set_field(fe, "ctime_n", &(ctime->tv_nsec), -1, NULL);
	j_db_entry_set_field(fe, "ctime_s", &(ctime->tv_sec), -1, NULL);
}
void
set_dir(JFileMetadataOut* fe,const char* dir)
{
	j_db_entry_set_field(fe,"dir",dir,-1,NULL);
}
void
j_file_metadata_write(JFileSelector* fs, JFileMetadataOut* fe, JBatch* batch)
{
	j_db_entry_update(fe, fs, batch, NULL);
}
void
j_file_metadata_create(JFileSelector* fs, JFileMetadataOut* fe, JBatch* batch)
{
	(void)fs;

	j_db_entry_insert(fe, batch, NULL);
}

JFileMetadataOut*
j_file_metadata_out_new(const char* path)
{
	JDBEntry* kv = j_db_entry_new(db_schema, NULL);
	char* dir_name = g_path_get_dirname(path);
	j_db_entry_set_field(kv, "dir", dir_name, -1, NULL);
	j_db_entry_set_field(kv, "path", path, -1, NULL);
	return kv;
}
JFileMetadataOut* j_file_metadata_out_new_load(const char* path,JFileSelector* fs){
	(void) fs;
	return j_file_metadata_out_new(path);
}
JFileMetadataOut* j_file_metadata_out_new_load_for_update(const char* path, JFileSelector* fs){
	(void) fs;
	return j_file_metadata_out_new(path);
}

int j_file_metadata_rename(JFileSelector* old,JFileSelector* new, const char* destination){

	int ret=-1;
	g_autoptr(JDBEntry) entry=NULL;
	g_autoptr(JBatch) batch=NULL;
	
	(void)new;
	entry=j_db_entry_new(db_schema,NULL);
	j_db_entry_set_field(entry,"path",(gconstpointer)destination,-1,NULL);
	batch=j_batch_new_for_template(J_SEMANTICS_TEMPLATE_POSIX);
	j_db_entry_update(entry,old,batch,NULL);
	if (j_batch_execute(batch))
	{
		ret=0;
	}
	return ret;
}
JFileMetadataOut*
j_file_metadata_in_to_out(JFileMetadataIn* in)
{
	(void)in;
	return j_db_entry_new(db_schema, NULL);
}
void
j_file_metadata_delete(JFileSelector* fs, JFileMetadataOut* out, JBatch* batch)
{
	j_db_entry_delete(out, fs, batch, NULL);
}

void
j_file_metadata_out_destroy(JFileMetadataOut* out)
{
	if (!out)
		return;
	j_db_entry_unref(out);
}
void
j_file_metadata_in_destroy(JFileMetadataIn* in)
{
	if (!in)
		return;
	j_db_iterator_unref(in);
}
void
j_file_selector_destroy(JFileSelector* fs)
{
	if (!fs)
		return;
	j_db_selector_unref(fs);
}

JDirectoryIterator*
j_directory_iterator_new(const char* dirname)
{
	g_autoptr(JDBSelector) selector = NULL;
	g_autofree gchar* min=NULL;
	g_autofree gchar* max=NULL;
	g_autofree gchar* prefix=NULL;

	JDBIterator* iter;

	prefix=(gchar*)g_malloc(strlen(dirname));
	memcpy(prefix,dirname,strlen(dirname)-1);
	prefix[strlen(dirname)-1]='\0';
	min=g_strconcat(prefix,".",NULL);
	max=g_strconcat(prefix,"0",NULL);
	selector = j_db_selector_new(db_schema, J_DB_SELECTOR_MODE_AND, NULL);
	j_db_selector_add_field(selector, "path", J_DB_SELECTOR_OPERATOR_GT, min, -1, NULL);
	j_db_selector_add_field(selector, "path", J_DB_SELECTOR_OPERATOR_LT, max, -1, NULL);
	iter = j_db_iterator_new(db_schema, selector, NULL);
	return iter;
}
gboolean
j_directory_iterator_next(JDirectoryIterator* dir_iter)
{
	return j_db_iterator_next(dir_iter, NULL);
}
char*
j_directory_iterator_get(JDirectoryIterator* dir_iter)
{
	char* result;
	guint64 length;
	JDBType type;
	j_db_iterator_get_field(dir_iter, "path", &type, (gpointer*)&result, &length, NULL);
	return result;
}
void
j_directory_iterator_destroy(JDirectoryIterator* dir_iter)
{
	j_db_iterator_unref(dir_iter);
}

#endif