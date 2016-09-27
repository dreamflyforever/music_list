/*
 * Test double list function
 */
#include "music_list.h"

typedef struct TEST {
	LIST task_list;
	int num;
} TEST;

int test_music_list_destroy()
{
	music_obj *g_m;
	music_list_alloc(&g_m, 20);
	music_list_destroy(g_m);
	return 1;
}

int test_music_cur_get()
{
	int ret = 0;
	music_obj *g_m;
	music_info *tmp;
	music_list_alloc(&g_m, 20);
	tmp = music_cur_get(g_m);
	if (tmp == NULL) {
		ret = 1;
		printf("current music is HEAD\n");
	}
	return ret;
}
int test_insert_delete_list(int input_a,
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

	int result = 0;

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
		result++;
	} else {
		printf("[out == expect] FAIL\n");
		result--;
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
		result++;
	} else {
		printf("[out == expect] FAIL\n");
		result--;
	}
	if (result == 2)
		result = 1;
	else
		result = 0;

	return result;
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
		printf("cur url: %s\n", tmp->url);

		tmp = music_next_get(g_m);
		if (tmp == NULL) {
			printf("no next music\n");
		} else {
			printf("next url: %s\n", tmp->url);
		}
		music_list_destroy(g_m);
	}
	printf("PASS\n");
	return 1;
}

int test_music_prev_get()
{
	music_obj *g_m;
	printf("----------------test music prev get----------------\n");
	int i = 1;
	char k[0];
	while (i--) {
		music_list_alloc(&g_m, 20);
		music_info *tmp;
		int j;
		for (j = 0; j < 22; j++) {
			k[0] = 'a' + j;
			music_info_alloc(&tmp, "a", "b", k);
			music_list_insert(g_m, tmp);
		}

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
		printf("current music: %s-------------------------------\n",
			g_m->cur_music->url);
		while (1) {
			tmp = music_next_get(g_m);
			if (tmp == NULL) {
				printf("no prev music\n");
				break;
			} else {
				printf("next url: %s\n", tmp->url);
			}

		}
		music_list_destroy(g_m);
	}
	printf("PASS\n");
	return 1;
}
#define CHECK_BASE_LIST(a, b, c, insert_expect, delete_expect, caculate, pass) \
	do { \
		caculate++; \
		pass += test_insert_delete_list(a, b, c, insert_expect, delete_expect); \
	} while (0);

#define CHECK_MUSIC_NEXT_GET(caculate, pass) \
	do { \
		caculate++; \
		pass += test_music_next_get(); \
	} while (0);

#define CHECK_MUSIC_PREV_GET(caculate, pass) \
	do {\
		caculate++; \
		pass += test_music_prev_get(); \
	} while (0);

#define CHECK_MUSIC_CUR_GET(caculate, pass) \
	do {\
		caculate++; \
		pass += test_music_cur_get(); \
	} while (0);

#define CHECK_MUSIC_LIST_DESTROY(caculate, pass) \
	do {\
		caculate++; \
		pass += test_music_list_destroy(); \
	} while (0);

int main()
{
	int pass = 0, all = 0;
	int retvalue;
	CHECK_BASE_LIST(10, 20, 30, 30, 30, all, pass);
	CHECK_MUSIC_NEXT_GET(all, pass);
	CHECK_MUSIC_PREV_GET(all, pass);
	CHECK_MUSIC_CUR_GET(all, pass);
	CHECK_MUSIC_LIST_DESTROY(all, pass);

	printf("\nAll Test Done!\n\n");
	printf("Result: %2d/%2d [PASS/TOTAL]\n\n", pass, all);
	if (pass != all) {
		printf("!!! TEST FAIL !!!\n\n");
		retvalue = -1;
	} else {
		printf("$$$ TEST PASS $$$\n\n");
		retvalue = 0;
	}

	return retvalue;
}
