import os

env = Environment(
    CPPPATH = [
        '#include',
    ],
    CCFLAGS = [
        '-g',
        '-Wall',
        '-O3',
        '-fno-strict-aliasing',
        '-DBOOST_DATE_TIME_POSIX_TIME_STD_CONFIG',
    ],
    CXXFLAGS = [
        '-Woverloaded-virtual',
    #   '-std=c++11',
        '-std=c++17',
    ],
    LIBPATH = ['#lib'],
    LIBS = [
        'feathersclient',
        'boost_thread',
        'boost_filesystem',
        'boost_system',
        'boost_date_time',
        'protobuf',
        'tbb',
        'pthread',
    ],
)

feathersClientTest = env.Program(
    '#bin/feathersClientTest',
    [ 'feathersClientTest.cpp' ]
)

