/*
 * Test double list function
 * 
 */
#include "music_list.h"

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

int test_music_next_get()
{
	music_obj *g_m;
	printf("----------------test music next get----------------\n");
	int i = 1;
	while (i--) {
		music_list_alloc(&g_m, 20);
		music_info *tmp;
		music_info_alloc(&tmp, "a", "b", "c");
		music_list_insert(g_m, tmp);

		music_info_alloc(&tmp, "d", "e", "f");
		music_list_insert(g_m, tmp);
			
		music_info_alloc(&tmp, "1", "2", "3");
		music_list_insert(g_m, tmp);

		music_info_alloc(&tmp, "4", "5", "6");
		music_list_insert(g_m, tmp);

		tmp = music_cur_get(g_m);
		printf("cur: %s\n", tmp->url);
		tmp = music_next_get(g_m);
		if (tmp == NULL) {
			printf("no next music\n");
		} else {
			printf("next url: %s\n", tmp->url);
		}
		music_list_destroy(g_m);
	}
	printf("PASS\n");
	return 0;
}

int test_music_prev_get()
{
	music_obj *g_m;
	printf("----------------test music prev get----------------\n");
	int i = 1;
	while (i--) {
		music_list_alloc(&g_m, 20);
		music_info *tmp;
		music_info_alloc(&tmp, "a", "b", "c");
		music_list_insert(g_m, tmp);

		music_info_alloc(&tmp, "d", "e", "f");
		music_list_insert(g_m, tmp);
			
		music_info_alloc(&tmp, "1", "2", "3");
		music_list_insert(g_m, tmp);

		music_info_alloc(&tmp, "4", "5", "6");
		music_list_insert(g_m, tmp);

		tmp = music_cur_get(g_m);
		printf("cur: %s\n", tmp->url);
		while (1) {
			tmp = music_prev_get(g_m);
			if (tmp == NULL) {
				printf("no prev music\n");
				break;
			} else {
				printf("prev url: %s\n", tmp->url);
			}
		}
		printf("%s-------------------------------\n", g_m->cur_music->url);
		while (1) {
			tmp = music_next_get(g_m);
			if (tmp == NULL) {
				printf("no next music\n");
				break;
			} else {
				printf("next url: %s\n", tmp->url);
			}

		}
		music_list_destroy(g_m);
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
