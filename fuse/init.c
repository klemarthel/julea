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
#include "unistd.h"
#include "julea-fuse.h"
#ifndef DATABASE_METADATA
gint* last_entry;
#endif
void*
jfs_init(struct fuse_conn_info* conn, struct fuse_config* cfg)
{
	
	
	#ifndef DATABASE_METADATA
	gint newval=0;
	char* mount_id;
	gpointer val;
	guint32 len;
	g_autoptr(JBatch) batch=NULL;
	g_autoptr(JKV) kv=NULL;

	mount_id=g_get_host_name();
	last_entry=g_malloc(sizeof(gint));
	batch=j_batch_new_for_template(J_SEMANTICS_TEMPLATE_POSIX);
	kv=j_kv_new("posix_ids",mount_id);
	j_kv_get(kv,&val,&len,batch);
	if(j_batch_execute(batch)){
		if(len==sizeof(gint)){
			newval=*((gint*)val);
			g_atomic_int_set(last_entry,newval);
		}
	}
	
	#endif
	(void)conn;
(void)cfg;
	return NULL;
}
