#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
/*
 * Test double list function
 * 
 */

typedef struct TEST {
	LIST task_list;
	int num;

} TEST;

void test_insert_delete_list(int input_a,
				int input_b,
				int input_c,
				int insert_expect,
				int delete_expect)
{
	TEST a;
	TEST b;
	TEST c;
	a.num = input_a;
	b.num = input_b;
	c.num = input_c;

	printf("----------------test list insert----------------\n");
	list_init(&a.task_list);
	list_insert(&a.task_list, &b.task_list);
	//list_insert_spec(&a.task_list, &c.task_list);
	list_insert(&b.task_list, &c.task_list);
	LIST *tmp = &a.task_list;
	TEST *d;
	while (!is_list_last(tmp)) {
		d = list_entry(tmp, TEST, task_list);
		tmp = tmp->next;
		printf("%d\n", d->num);
	}
	d = list_entry(tmp, TEST, task_list);
	printf("%d\n", d->num);
	if (d->num == insert_expect) {
		printf("[out == expect] PASS\n");
	} else {
		printf("[out == expect] FAIL\n");
	}

	printf("----------------test list delete-----------------\n");
	list_delete(&b.task_list);
	tmp = &a.task_list;
	while (!is_list_last(tmp)) {
		d = list_entry(tmp, TEST, task_list);
		tmp = tmp->next;
		printf("%d\n", d->num);
	}
	d = list_entry(tmp, TEST, task_list);
	printf("%d\n", d->num);
	if (d->num == delete_expect) {
		printf("[out == expect] PASS\n");
	} else {
		printf("[out == expect] FAIL\n");
	}
}

typedef struct music_info_t {
	LIST list;
	char *title;
	char *artist;
	char *url;
} music_info;

typedef struct music_obj_t {
	music_info head;
	int max;
	int cur_num;
	music_info *cur_music;
} music_obj;

music_obj g_m;

int music_list_init(music_obj *obj, int max)
{
	int retvalue = 0;

	if ((obj == NULL) || (max == 0)) {
		printf("error: %d\n", __LINE__);
		retvalue = -1;
		goto error;
	}

	list_init(&obj->head.list);
	obj->head.title = NULL;
	obj->head.artist = NULL;
	obj->head.url = NULL;
	obj->max = max;
	obj->cur_num = 0;
	obj->cur_music = NULL;
error:	
	return retvalue;
}

music_info *music_cur_get(music_obj *obj)
{
	if (obj->cur_music == NULL)
		return NULL;

	music_info *tmp = list_entry(&obj->cur_music->list, music_info, list);
	return (tmp)? tmp: NULL;
}

music_info *music_next_get(music_obj *obj)
{
	music_info *next = list_entry(obj->cur_music->list.next,
					music_info,
					list);

	music_info *cur = music_cur_get(obj);
	if (next == cur) {
		printf("LINE: %d\n", __LINE__);
		return NULL;
	} else {
		printf("LINE: %d\n", __LINE__);
		obj->cur_music = next;
		return next;
	}
}

music_info *music_prev_get(music_obj *obj)
{
	if (obj->cur_music == NULL) {
		return NULL;
	}
	music_info *prev = list_entry(obj->cur_music->list.prev,
					music_info,
					list);
	music_info *cur = music_cur_get(obj);
	if (prev->artist == NULL) {
		return NULL;
	}
	
	if (prev == cur) {
		return NULL;
	} else {
		obj->cur_music = prev;
		return prev;
	}
}

int music_list_delete(music_info *info)
{
	int retvalue = 0;
	if (info == NULL) {
		printf("error: %d\n", __LINE__);
		retvalue = -1;
		goto error;
	}

	list_delete(&info->list);
	free(info->title);
	info->title = NULL;
	free(info->artist);
	info->artist = NULL;
	free(info->url);
	info->url = NULL;
error:
	return retvalue;
}

int music_list_insert(music_obj *obj, music_info *info)
{
	int retvalue = 0;
	if ((info == NULL) || (obj == NULL) || (obj->cur_num > obj->max)) {
		printf("error: %d\n", __LINE__);
		retvalue = -1;
		goto end;
	}

	if (NULL == info->url) {
		printf("error: %d\n", __LINE__);
		retvalue = -1;
		goto end;
	}

	if (obj->cur_num > obj->max) {
		printf("LINE: %d\n", __LINE__);
		music_info *tmp = list_entry(&obj->head.list, music_info, list);
		music_list_delete(tmp);
	}

	obj->cur_music = info;

	LIST *tmp = &obj->head.list;
	music_info *m;
	while (!is_list_last(tmp)) {
		m = list_entry(tmp, music_info, list);
		if (m->url != NULL) {
			if (0 == strncmp(m->url, info->url, strlen(info->url))) {
				music_list_delete(info);
				list_insert_behind(&obj->head.list, &info->list);
				goto end;
			}
		}
		tmp = tmp->next;
	}
	m = list_entry(tmp, music_info, list);
	if (m->url != NULL) {
		if (0 == strncmp(m->url, info->url, strlen(info->url))) {
			printf("LINE: %d\n", __LINE__);
			music_list_delete(info);
		}
	}
	list_insert_behind(&obj->head.list, &info->list);
	obj->cur_num++;
end:
	return retvalue;
}

int music_info_init(music_info **info, char *title, char *artist, char *url)
{
	/*XXX*/
	*info = malloc(sizeof(music_info));
	(*info)->title = strdup(title);
	(*info)->artist = strdup(artist);
	(*info)->url = strdup(url);
	return 0;
}

int music_list_destroy(music_obj *obj)
{
	LIST *tmp = &obj->head.list;
	music_info *m;
	while (!is_list_last(tmp)) {
		m = list_entry(tmp, music_info, list);
		tmp = tmp->next;
		music_list_delete(m);
	}
	m = list_entry(tmp, music_info, list);
	music_list_delete(m);
	
	obj->max = 0;
	obj->cur_num = 0;
	obj->cur_music = NULL;
	return 0;
}

int test_music_next_get()
{
	printf("----------------test music next get----------------\n");
	int i = 1;
	while (i--) {
		music_list_init(&g_m, 20);
		music_info *tmp;
		music_info_init(&tmp, "a", "b", "c");
		music_list_insert(&g_m, tmp);

		music_info_init(&tmp, "d", "e", "f");
		music_list_insert(&g_m, tmp);
			
		music_info_init(&tmp, "1", "2", "3");
		music_list_insert(&g_m, tmp);

		music_info_init(&tmp, "4", "5", "6");
		music_list_insert(&g_m, tmp);

		tmp = music_cur_get(&g_m);
		printf("cur: %s\n", tmp->url);
		tmp = music_next_get(&g_m);
		if (tmp == NULL) {
			printf("no next music\n");
		} else {
			printf("next url: %s\n", tmp->url);
		}
		music_list_destroy(&g_m);
	}
	printf("PASS\n");
	return 0;
}

int test_music_prev_get()
{
	printf("----------------test music prev get----------------\n");
	int i = 1;
	while (i--) {
		music_list_init(&g_m, 20);
		music_info *tmp;
		music_info_init(&tmp, "a", "b", "c");
		music_list_insert(&g_m, tmp);

		music_info_init(&tmp, "d", "e", "f");
		music_list_insert(&g_m, tmp);
			
		music_info_init(&tmp, "1", "2", "3");
		music_list_insert(&g_m, tmp);

		music_info_init(&tmp, "4", "5", "6");
		music_list_insert(&g_m, tmp);

		tmp = music_cur_get(&g_m);
		printf("cur: %s\n", tmp->url);
		while (1) {
			tmp = music_prev_get(&g_m);
			if (tmp == NULL) {
				printf("no prev music\n");
				break;
			} else {
				printf("prev url: %s\n", tmp->url);
			}
		}
		music_list_destroy(&g_m);
	}
	printf("PASS\n");
	return 0;
}

int main()
{
	test_insert_delete_list(10, 20, 30, 30, 30);
	test_music_next_get();
	test_music_prev_get();
	return 0;
}
