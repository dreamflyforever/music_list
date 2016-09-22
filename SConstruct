env = Environment(
	CC = 'clang',
	CFLAGS = ['-g', '-Wall', '-I.']
)

obj_base_list = env.Object('list.c')
obj_music_list = env.Object('music_list.c')

test_case = env.Program("test_case", ['test_case.c', obj_base_list, obj_music_list])
