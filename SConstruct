env = Environment(CCFLAGS='-Wall -Wextra -Werror -O3 -std=c++11', LIBS='pthread', CPPPATH=['boost_1_52_0/include'])
test = env.Program('FeverMJTest', ['test/test.cc', 'test/gtest/gtest-all.cc', 'test/gtest/gtest_main.cc'])
test_alias = env.Alias('check', test, test[0].abspath)
env.AlwaysBuild(test_alias)
