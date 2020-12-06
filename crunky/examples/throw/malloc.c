#define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <assert.h>

#undef NULL
#define NULL 0

struct chunk {
	struct chunk *next, *prev;
	size_t        size;
	long          free;
	void         *data;
};



	static inline
size_t word_align(size_t n)
{
	size_t size = sizeof(size_t);
	return (n + (size -1)&~(size-1));
}

void zerofill(void *ptr, size_t len)
{
	char  *pointeur;
	pointeur = (char*)ptr;
	size_t i;
	for (i = 0 ; i < len ; i++)
	{
		pointeur[i] = 0;
	}
}

void wordcpy(void *dst, void *src, size_t len)
{
	char *ptr, *dos;
	dos = dst;
	ptr = src;

	for (size_t i = 0 ; i < len ; i++)
	{
		dos[i] = ptr[i]; 
	}
}

	static
struct chunk* get_base(void) 
{
	static struct chunk *base = 0;

	if(base) return base;

	puts("initialising base");
	base = sbrk(word_align(sizeof(struct chunk)));
	assert(base != (void*)-1);
	base->next = NULL;
	base->prev = NULL;
	base->size = 0;
	base->free = 0;
	//base->data = NULL;
	return base;
}

	static
int extend_heap(struct chunk *last, size_t size)
{
	struct chunk *basec;
	basec = sbrk(0);
	if (sbrk(sizeof(struct chunk) + size) == (void*)-1)
	{
		return 0;
	}
	else
	{
		basec-> size = size;
		basec->next = NULL;

		if (last != NULL)
		{
			last->next = basec;
		} 
		basec->free = 0;
		return 1;
	}
}

	static
struct chunk* find_chunk(size_t size)
{
	struct chunk *prev = NULL;
	struct chunk *premier = get_base();


	while(premier != NULL && (premier->size < size || premier->free == 0))
	{
		prev = premier;
		premier = premier->next;
	}
	return prev;
}

	static
struct chunk* get_chunk(void *p)
{

	if (p != NULL)
	{
		return NULL;
	} 

	if(p>(void*)(get_base()) && p<(void*)(sbrk(0)))
	{
		struct chunk *base = p - sizeof(struct chunk);
		if (p == base->data)
		{
			return base;
		}
	}
	return NULL;
}


void *malloc(size_t size)
{

	if (size == 0 )
		return NULL;
	size_t lenght = word_align(size);
	struct chunk *c = find_chunk(size);

	if(c->next==NULL)
	{
		if (extend_heap(c,lenght) == 0)
			return NULL;
	}
	else
	{
		c->next->free = 0;
	}
	return (&c->next->data);

}
void free ( void *ptr)
{
	struct chunk *test = get_chunk(ptr);
	if (test != NULL)
	{
		test->free = 1;
	}
}

void *calloc (size_t nmenb, size_t size)
{
	size_t *new;
	new = malloc (nmenb * size);
	zerofill(new, nmenb*size);
	return new;
}

void* realloc(void *old, size_t newsize)
{
	if (newsize == 0)
	{
		free(old);
		return NULL;
	}

	void *tmp = malloc(newsize);
	if (old == NULL)
	{
		return tmp;
	}
	else
	{
		struct chunk  *new = get_chunk(old);

		if (!tmp || !new)
			return NULL;
		else
		{
			wordcpy(tmp,new->data,new->size);
			free(old);
			return tmp;
		}
	}
}






