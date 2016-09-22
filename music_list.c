#include "music_list.h"

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
		return NULL;
	} else {
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