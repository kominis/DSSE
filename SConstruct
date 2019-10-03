import os

env=Environment(ENV=os.environ)

print(env['ENV']['HOME'])
print(env['ENV']['PATH'])

if 'CC' in os.environ:
    env['CC']=os.environ['CC']

if 'CXX' in os.environ:
    env['CXX']=os.environ['CXX']

if 'config_file' in ARGUMENTS:
    SConscript(ARGUMENTS['config_file'], exports=['env','config'])

env.Append(CCFLAGS = ['-fPIC','-Wall', '-march=native'])
env.Append(CXXFLAGS = ['-std=c++14'])

env.Append(LIBS = ['crypto', 'libsodium'])

#Workaround for OS X
if env['PLATFORM'] == 'darwin':
    rpathprefix = '-rpath'
    env.Append(LINKFLAGS = [[rpathprefix, lib] for lib in env['RPATH']])
    env.Append(CPPPATH=['/usr/local/opt/openssl/include'])
    env.Append(LIBPATH=['/usr/local/opt/openssl/lib'])



env['STATIC_AND_SHARED_OBJECTS_ARE_THE_SAME'] = 1

env.Append(CCFLAGS = ['-g','-O0'])

env.Append(CPPDEFINES = ['BENCHMARK'])

def run_test(target, source, env):
    app = str(source[0].abspath)
    if os.spawnl(os.P_WAIT, app, app)==0:
        return 0
    else:
        return 1

bld = Builder(action = run_test)
env.Append(BUILDERS = {'Test' :  bld})

objects = SConscript('src/build.scons', exports='env', variant_dir='build')

outter_env = env.Clone()
outter_env.Append(CPPPATH = ['build'])

mara_prog = outter_env.Program('mara', ['test_mara.cpp'] + objects["mara"])

env.Alias('mara', [mara_prog])
env.Default(['mara'])
