env = Environment(
	CC = 'clang',
	CFLAGS = ['-g', '-Wall', '-I.']
)

obj_list = env.Object('list.c')

test_case = env.Program("test_case", ['test_case.c', obj_list])
