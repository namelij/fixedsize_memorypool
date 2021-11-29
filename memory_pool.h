#ifndef _MEMORY_POOL_H
#define _MEMORY_POOL_H

typedef struct memory_block
{
	unsigned int size;
	unsigned int free_size;
	unsigned int first_free;

	struct memory_block *next;
	char a_data[1];	
} s_memory_block;

typedef struct memory_pool
{
	unsigned int obj_size;
	unsigned int init_size;
	unsigned int grow_size;

	s_memory_block *first_block;
} s_memory_pool;

s_memory_pool *memory_pool_create(unsigned int size);

/*****************************************************************************
 * memory_alloc 
 
 * @func                  allocate the memory from memory_pool to object  
 * @name                  memory_alloc
 * @param   mp            the structure of memory_pool
 */
void *memory_alloc(s_memory_pool *mp);

/*****************************************************************************
 * memory_free 
 
 * @func                  free the memory from object to memory_pool  
 * @name                  memory_free
 * @param   mp            the structure of memory_pool
 * @param   pfree         the memory that will be free
 */
void *memory_free(s_memory_pool *mp, void *pfree);

/*****************************************************************************
 * free_memory_pool
 
 * @func                  free the memory_pool  
 * @name                  free_memory_pool
 * @param   mp            the structure of memory_pool
 */
void free_memory_pool(s_memory_pool *mp);

#endif


