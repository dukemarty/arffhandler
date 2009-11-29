# -*- python -*- coding: latin1
#
# Last Author: Martin Loesch (<loesch@@ira.uka.de>)
# Date of last change: 29.11.09
#
# Author: Martin Loesch (loesch@@ira.uka.de)
# Date: 2009-11-26
# Copyright:
#              Martin Loesch, Chair Prof. Dillmann (IAIM)\n
#              Institute for Computer Science and Engineering (CSE)\n
#	       University of Karlsruhe. All rights reserved\n
#	       http://wwwiaim.ira.uka.de
#

import os, glob

env = Environment(ENV = os.environ)
docbuilder = Builder(action = 'doxygen')
env.Append(BUILDERS = {'DoxygenDoc' : docbuilder})

bgtoolspath = '/org/opt/bgtools'

project_cc_flags = '-Wall -Wno-non-virtual-dtor -Wno-reorder -fno-common'
project_ld_flags = ''

system_libraries = ['gcc', 'System', 'stdc++', 'SystemStubs']



external_libraries = ['bgcpp', 'boost_program_options-mt']
project_libraries = []


subproject_scripts = []
subproject_paths = []

lib_input_files = ['Containers.cc', 'ARFFData.cc', 'ARFFFileHandler.cc']
program_input_files = ['Main.cc']


external_include_paths = [bgtoolspath + '/bglibcpp',
                          '/opt/local/include']
project_include_paths = ['.'] + subproject_paths


external_library_paths = [bgtoolspath + '/bglibcpp',
                          '/opt/local/lib']
project_library_paths = ['.'] + subproject_paths



include_paths = external_include_paths + project_include_paths
library_paths = external_library_paths + project_library_paths
libraries_program =  system_libraries + external_libraries + project_libraries + ['arffhandlerlib']


SConscript(subproject_scripts, exports={'BGTOOLS' : bgtoolspath,
                                        'MYCCFLAGS' : project_cc_flags,
                                        'MYLDFLAGS' : project_ld_flags,
                                        'MYLIBS' : system_libraries})


arfflibrary = StaticLibrary('arffhandlerlib', lib_input_files,
                            CPPPATH = include_paths,
                            LIBPATH = library_paths,
                            LIBS = libraries_program,
                            CCFLAGS = project_cc_flags,
                            LINKFLAGS = project_ld_flags)

arffmanager = Program('ARFFManager', program_input_files,
                                     CPPPATH = include_paths,
                                     LIBPATH = library_paths,
                                     LIBS = libraries_program,
                                     CCFLAGS = project_cc_flags,
                                     LINKFLAGS = project_ld_flags)


env.DoxygenDoc('docu', 'Doxyfile')

Clean(arffmanager, glob.glob('*~'))

Default(arffmanager, arfflibrary)
