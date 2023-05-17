#include <glib.h>
#include "julea-object.h"
#include "julea-kv.h"
#include "julea-db.h"

#ifndef FILE_H
#define FILE_H

#ifdef DATABASE_METADATA
/**
 * The schema that loaded at start up by julea-fuse.
**/
extern JDBSchema* db_schema;
/**
 * The metadata loaded from the database.
**/
typedef JDBIterator JFileMetadataIn;
/**
 * The updated/new metadata to be written to the database.
**/
typedef JDBEntry JFileMetadataOut;
/**
 * Iterator for all files in a directory (and all of its subdirectories).
 **/
typedef JDBIterator JDirectoryIterator;
/**
 * Returns the schema of the table as used by julea-fuse. 
 * \return A JDBSchema as expected by julea-fuse
**/
JDBSchema* metadata_schema(void);
/**
 * Selects the desired metadata value.
**/
typedef JDBSelector JFileSelector;
#else
/**
 * A struct with all the necessary metadata.
 **/
struct file_struct;
/**
 * Selects the desired metadata value.
**/
typedef JKV JFileSelector;

typedef struct file_struct JFileMetadataOut;
typedef struct file_struct JFileMetadataIn;
/**
 * Iterator for all files in a directory (and all of its subdirectories).
 **/
typedef JKVIterator JDirectoryIterator;
#endif
/**
 * New file selector. 
 * \param path the file's path
 * \return A JFileSelector for the file with the given name.
**/
JFileSelector* j_file_selector_new(const char* path);

/**
 * New metadata_in.
 * \param selector a selector for the file
 * \param batch the batch with whom the metadata will be loaded
 * \return A JFileMetadataIn to read metadata attributes from.
**/
JFileMetadataIn* j_file_metadata_new(JFileSelector* selector, JBatch* batch);
/**
 * New metadata_in, directly loaded with an internal batch for file operations which do not need the batch anywhere else.
 * \param selector a selector for the file
 * \return A JFileMetadataIn to read metadata attributes from.
**/
JFileMetadataIn* j_file_metadata_new_load(JFileSelector* selector);
/**
 * Convert metadata from in to out.
 * Uses the same data when the key value store is used.
 * Creates an new empty database entry if the relational database is used.
 * \param metadata the metadata in to convert
 * \return The JFileMetadataOut which can be manipulated.
**/
JFileMetadataOut* j_file_metadata_in_to_out(JFileMetadataIn* metadata);
/**
 * New metadata which can be used for updates. 
 * Loaded from the database in case of key value stores.
 * \param path the file's path
 * \param fs A JFileSelector for the file
 * \return The JFileMetadataOut which can be manipulated.
**/
JFileMetadataOut* j_file_metadata_out_new_load(const char* path, JFileSelector* fs);

/**
 * Get the id integer of the JObject holding the contents of the file.
 * \param in the file metadata with the apropriate id
 * \return the id of the JObject assosiated with the file
**/
guint64 get_object(JFileMetadataIn* in);

/**
 * Get the id of the JObject holding the contents as char* of the file.
 * This can then directly be used to create the JObject.
 * \param in the file metadata the value is exctracted from
 * \return the char* of the id of the JObject assosiated with the file
**/
char* get_object_name(JFileMetadataIn* in);

/**
 * Translate from the id integer to a char* which can be used directly to create
 * the JObject.
 * \param id the integer that is to be mapped to a char*
 * \return the char* which is the name of the JObject
**/
char*
	nr_to_object_name(guint64 id);

/**
 * Get the size of the file content in bytes.
 * \param in the file metadata the value is exctracted from
 * \return the file size in bytes
**/
guint64 get_size(JFileMetadataIn* in);
/**
 * Get the id of the owner of the file.
 * \param in the file metadata the value is exctracted from
 * \return the id of the owner
**/
guint64 get_owner(JFileMetadataIn* in);
/**
 * Get the id of the group of the file.
 * \param in the file metadata the value is exctracted from
 * \return the id of the group
**/
guint64 get_group(JFileMetadataIn* in);
/**
 * Get the access mode of the file.
 * \param in the file metadata the value is exctracted from
 * \return the access mode
**/
gint32 get_mode(JFileMetadataIn* in);

/**
 * Get the access time of the file.
 * \param in the file metadata the value is exctracted from
 * \return the access time as timespec
**/
struct timespec* get_atime(JFileMetadataIn* in);

/**
 * Get the modification time of the file.
 * \param in the file metadata the value is exctracted from
 * \return the modification time as timespec
**/
struct timespec* get_mtime(JFileMetadataIn* in);

/**
 * Get the status change time of the file.
 * \param in the file metadata the value is exctracted from
 * \return the status change time as timespec
**/
struct timespec* get_ctime(JFileMetadataIn* in);

/**
 * Shorthand for checking whether this is a dir or not (based on the file mode).
 * \param in the file metadata the value is exctracted from
 * \return true if it is a dir
**/
bool is_dir(JFileMetadataIn* in);

/**
 * Set the id integer of the JObject holding the contents of the file.
 * \param out the file metadata to alter
 * \param object the new object id
**/
void set_object(JFileMetadataOut* out, guint64 object);

/**
 * Set the size of the file.
 * \param out the file metadata to alter
 * \param size the new size
**/
void set_size(JFileMetadataOut* out, guint64 size);

/**
 * Set the owner of the file.
 * \param out the file metadata to alter
 * \param owner the new owner
**/
void set_owner(JFileMetadataOut* out, guint64 owner);

/**
 * Set the group of the file.
 * \param out the file metadata to alter
 * \param group the new group
**/
void set_group(JFileMetadataOut* out, guint64 group);

/**
 * Set the mode of the file.
 * \param out the file metadata to alter
 * \param mode the new mode
**/
void set_mode(JFileMetadataOut* out, gint32 mode);

/**
 * Set the access time of the file.
 * \param out the file metadata to alter
 * \param atime the new access time
**/
void set_atime(JFileMetadataOut* out, const struct timespec* atime);

/**
 * Set the modification time of the file.
 * \param out the file metadata to alter
 * \param mtime the new modification time
**/
void set_mtime(JFileMetadataOut* out, const struct timespec* mtime);

/**
 * Set the status change time of the file.
 * \param out the file metadata to alter
 * \param ctime the new status change time
**/
void set_ctime(JFileMetadataOut* out, const struct timespec* ctime);

/**
 * Update the metadata in the database.
 * \param fs the file selector for the metadata
 * \param out the file metadata to update
 * \param batch the batch used for this operation
**/
void j_file_metadata_write(JFileSelector* fs, JFileMetadataOut* out, JBatch* batch);

/**
 * Rename the metadata of a regular file in the database.
 * \param fs_old the current file selector for the metadata
 * \param fs_new the new file selector for the metadata
 * \param path the new path of the file
 * \return 0 if the operation is successful, or another number if the batch execution fails.
**/
int j_file_metadata_rename(JFileSelector* fs_old, JFileSelector* fs_new,  const char* new_name);

/**
 * Rename the metadata of a directory in the database.
 * Also changes the paths of all of its entries
 * \param old the current path of the file
 * \param new the new path of the file
 * \return 0 if the operation is successful, or another number if the batch execution fails.
**/
int j_file_dir_rename(const char* old, const char* new);

/**
 * Inserts the metadata into the database.
 * \param fs the file selector for the metadata
 * \param out the file metadata to newly create 
 * \param batch the batch used for this operation
**/
void j_file_metadata_create(JFileSelector* fs, JFileMetadataOut* out, JBatch* batch);

/**
 * Obtain a new metadata structure (in memory, not in the database).
 * Designed to be used while creating files.
 * \param path the file's path
 * \return an instance of JFileMetadataOut*
**/
JFileMetadataOut* j_file_metadata_out_new(const char* path);

/**
 * Obtain a metadata structure for an existing file.
 * \param fs a file selector for the metadata
 * \param path the file's path
 * \return an instance of JFileMetadataOut*
**/
JFileMetadataOut* j_file_metadata_out_new_load_for_update(const char* path, JFileSelector* fs);

/**
 * Delete the metadata of a file.
 * \param fs a file selector for the metadata
 * \param path the file's path
 * \param batch the batch holding this operation
**/
void j_file_metadata_delete(JFileSelector* fs, JFileMetadataOut* out, JBatch* batch);

/**
 * Destructor to free memory of a JFileMetadataOut.
 * \param out JFileMetadataOut not used any more
**/
void j_file_metadata_out_destroy(JFileMetadataOut* out);

/**
 * Destructor to free memory of a JFileMetadataIn.
 * \param in JFileMetadataIn not used any more
**/
void j_file_metadata_in_destroy(JFileMetadataIn* in);

/**
 * Destructor to free memory of a JFileSelector.
 * \param fs JFileSelector not used any more
**/
void j_file_selector_destroy(JFileSelector* fs);

/**
 * Init an iterator for directory entries.
 * \param dirname the directory path
**/
JDirectoryIterator*
j_directory_iterator_new(const char* dirname);

/**
 * Check if the iterator reached the end.
 * \param dir_iter the directory iterator
 * \return true if there are further elements (also elements in subdirectories). 
**/
gboolean
j_directory_iterator_next(JDirectoryIterator* dir_iter);

/**
 * Get the path of the current file entry of the iterator.
 * \param dir_iter the directory iterator
 * \return the current entries path (also entries of subdirectories are returned). 
**/
char*
j_directory_iterator_get(JDirectoryIterator* dir_iter);

/**
 * Destructor to free memory of a JDirectoryIterator.
 * \param dir_iter JDirectoryIterator not used any more
**/
void
j_directory_iterator_destroy(JDirectoryIterator* dir_iter);

G_DEFINE_AUTOPTR_CLEANUP_FUNC(JFileSelector, j_file_selector_destroy)
G_DEFINE_AUTOPTR_CLEANUP_FUNC(JFileMetadataIn, j_file_metadata_in_destroy)
G_DEFINE_AUTOPTR_CLEANUP_FUNC(JFileMetadataOut, j_file_metadata_out_destroy)
G_DEFINE_AUTOPTR_CLEANUP_FUNC(JDirectoryIterator, j_directory_iterator_destroy)

#endif