#include <malloc.h>
#include <stdio.h>
#include "memory_pool.h"

#define MEMPOOL_ALIGNMENT 4



s_memory_pool *memory_pool_create(unsigned int size)
{
	s_memory_pool *mp;
	mp = (s_memory_pool*)malloc(sizeof(s_memory_pool));
	mp->first_block = NULL;
	mp->init_size = 5;
	mp->grow_size = 5;

	if(size < sizeof(unsigned int))
		mp->obj_size = sizeof(unsigned int);
	mp->obj_size = (size + (MEMPOOL_ALIGNMENT-1)) & ~(MEMPOOL_ALIGNMENT-1);

	return mp;
}

/***********************************************************************************************************/
/* memory_alloc
 * return a free memory
 * allocate memory in memory_pool
 */
inline void *memory_alloc(s_memory_pool *mp)
{

	register unsigned int i;
	register char *data;
	//unsigned int length;

	if(mp->first_block == NULL)//memory_pool is NULL
	{

		s_memory_block *mb;
		mb = (s_memory_block *)malloc((mp->init_size)*(mp->obj_size) + sizeof(s_memory_block));//create first memory_block
		if(mb == NULL)
		{
			perror("memory allocate failed!\n");
			return NULL;
		}

		/* init the first block */
		mb->next = NULL;
		mb->free_size = mp->init_size - 1;
		mb->first_free = 1;
		mb->size = mp->init_size*mp->obj_size;

		mp->first_block = mb;
		
		data = mb->a_data;

		/* set the mark */
		for(i=1; i<mp->init_size; ++i)
		{
			*(unsigned int *)data = i;

			data += mp->obj_size;
		}

                printf("first_free: %d\n", mb->first_free);
                char *dt = mb->a_data;
                for (i = 1; i < mp->init_size; ++i) {
                  printf("data value: %d\n", *(unsigned int *)dt);
                  dt += mp->obj_size;
                }

		return (void *)mb->a_data;
	}

	s_memory_block *pm_block = mp->first_block;

	while((pm_block!=NULL) && (pm_block->free_size==0))
	{
		pm_block = pm_block->next;
	}

	if(pm_block != NULL)
	{
		char *pfree = pm_block->a_data + pm_block->first_free * mp->obj_size;

		pm_block->first_free = *((unsigned int *)pfree);
		pm_block->free_size--;

                printf("first_free: %d\n", pm_block->first_free);
                char *dt = pm_block->a_data;
                for (i = 1; i < mp->init_size; ++i) {
                  printf("data value: %d\n", *(unsigned int *)dt);
                  dt += mp->obj_size;
                }

		return (void *)pfree;
	}
	else
	{
		if(mp->grow_size == 0)
			return NULL;
		s_memory_block *new_block = (s_memory_block *)malloc((mp->grow_size)*(mp->obj_size) + sizeof(s_memory_block));

		if(new_block == NULL)
			return NULL;

		data = new_block->a_data;

		for(i=1; i<mp->grow_size; ++i)
		{
			*(unsigned int *)data = i;
			data += mp->obj_size;
		}

		new_block->size = mp->grow_size*mp->obj_size;
		new_block->free_size = mp->grow_size-1;
		new_block->first_free = 1;
		new_block->next = mp->first_block;
		mp->first_block = new_block;
		
		return (void *)(new_block->a_data);
	}
}


/***********************************************************************************************************/
/* memory_free
 * free memory to memory_pool
 */
inline void* memory_free(s_memory_pool *mp, void *pfree)
{
	if(mp->first_block == NULL)
		return;

	s_memory_block *pm_block = mp->first_block;
	s_memory_block *pm_pre_block = mp->first_block;
	
	/* research the memory_block which the pfree in */
	while(pm_block && ((unsigned int)pfree<(unsigned int)pm_block->a_data || 
		(unsigned int)pfree>((unsigned int)pm_block->a_data+pm_block->size)))
	{
		//pm_pre_block = pm_block;
		pm_block = pm_block->next;

		if(pm_block == NULL)
			return NULL;
	}

	unsigned int offset = pfree -(void*) pm_block->a_data;

	if((offset&(mp->obj_size -1)) > 0)
		return pfree;

	pm_block->free_size++;
	*((unsigned int *)pfree) = pm_block->first_free;

	pm_block->first_free=(unsigned int)(offset/mp->obj_size);

	return NULL;


	/*pm_block->free_size++;

	*((unsigned int *)pfree) = pm_block->first_free;

	pm_block->first_free = (unsigned short)((unsigned int)pfree - (unsigned int)pm_block->a_data)/mp->obj_size;

	if(pm_block->free_size*mp->obj_size == pm_block->size)
	{
		pm_pre_block->next = pm_block->next;

		if((unsigned int)pm_pre_block == (unsigned int)mp->first_block)
			mp->first_block = NULL;

		free(pm_block);
	}*/
}


/***********************************************************************************************************/
/* free_memory_pool
 * free memory_pool
 */
void free_memory_pool(s_memory_pool *mp)
{
	s_memory_block *mb = mp->first_block;

	if(mb != NULL)
	{
		
		while(mb->next != NULL)
		{
			s_memory_block *delete_block = mb;
			mb = mb->next;

			free(delete_block);
		}

		free(mb);
	}

	
	free(mp);
}

