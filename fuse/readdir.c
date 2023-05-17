/*
 * JULEA - Flexible storage framework
 * Copyright (C) 2010-2022 Michael Kuhn
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <julea-config.h>

#include "julea-fuse.h"

#include <errno.h>
#include <string.h>
#include "file.h"
int
jfs_readdir(char const* path, void* buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info* fi, enum fuse_readdir_flags flags)
{
	int ret = -ENOENT;
	JDirectoryIterator* it;
	g_autofree gchar* prefix = NULL;

	(void)offset;
	(void)fi;
	(void)flags;
	if (g_strcmp0(path, "/") == 0)
	{
		prefix = g_strdup(path);
	}
	else
	{
		prefix = g_strdup_printf("%s/", path);
	}

	it = j_directory_iterator_new(prefix);

	while (j_directory_iterator_next(it))
	{
		g_autofree char* name = j_directory_iterator_get(it);
		g_autofree char* dir_name=g_path_get_dirname(name);
		if (g_strcmp0(dir_name,path)==0)
		{
			g_autofree char* base=g_path_get_basename(name);
			filler(buf, base, NULL, 0, 0);
		}
		
	}

	j_directory_iterator_destroy(it);

	ret = 0;

	return ret;
}
