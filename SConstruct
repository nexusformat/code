#====================================================================
#  NeXus - Neutron & X-ray Common Data Format
#  
#  $Id: Makefile.am 961 2007-09-04 12:31:49Z Freddie Akeroyd $
#
#  Top level scons file for coordinating NeXus build
#  
#  Copyright (C) 2008 Freddie Akeroyd
#  
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2 of the License, or (at your option) any later version.
# 
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
# 
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free 
#  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, 
#  MA  02111-1307  USA
#             
#  For further information, see <http://www.nexusformat.org/>
#
######################## Subversion Reposity details ########################
# Repository Location     $HeadURL$
# Revision of last commit $LastChangedRevision$ 
# Date of last commit     $LastChangedDate$
# Author of last commit   $LastChangedBy$
############################################################################

import os
import platform
import sys
import shutil
import re
import string
from socket import gethostname

import nexus_scons_utils
  
opts = Options('scons_options.py')
opts.Add('mingw','Set to 1 to use MinGW on Windows', 0)
opts.Add('msvc','Set to 1 to use Microsoft Visual Studio Compiler on Windows', 1)
opts.Add('debug','Set to 1 to build debug', 0)

base_env = Environment(ENV=os.environ, options = opts)
mingw_env = Environment(ENV=os.environ, options = opts, tools = [ 'mingw' ])
Help(opts.GenerateHelpText(base_env))
mingw = ARGUMENTS.get('mingw', 0)
msvc = ARGUMENTS.get('msvc', 1)
debug = ARGUMENTS.get('debug', 0)
if int(msvc) :
	env = base_env
if int(mingw) :
	env = mingw_env
#get libraries and the library directories
print 'Current path == ',os.getcwd()
cwdpath=os.getcwd()
uppath=os.path.abspath('../')

libList = []
if not env.GetOption('clean'): # do not re-rerun checking on a "scons -c"
	conf = Configure(env)
#	if conf.CheckLib()
	env = conf.Finish()

libDirList = []
cflags = []
dirList = []
cppPaths = []
cppDefines = [ ('HDF4',1), ('HDF5',1), ('NXXML',1) ]

#Get paths
if os.name == 'nt': 
	print "* COMMENCING WINDOWS BUILD *"

	libList.append('hdf5')
	libDirList.append('c:\\')

	if int(mingw) :
		cflags = ['-g', '-O2']
		env.Append(LINKFLAGS=['-g','-Wl,--export-all-symbols'])
	else :
		if int(debug) :
			cflags = [ '/W3', '/Od', '/FD', '/EHsc', '/MDd', '/TP', '/D_DEBUG', '/DMS_VISUAL_STUDIO=1', '/DWIN32', '/D_WINDOWS', '/DWINVER=0x0500', '/D_USE_MATH_DEFINES', '/D_SCL_SECURE_NO_WARNINGS', '/D_CRT_SECURE_NO_WARNINGS', '/wd4275', '/wd4996' ]
		else :
			cflags = ['/Ox', '/Ot', '/W3','/EHsc', '/MD', '/TP', '/DMS_VISUAL_STUDIO=1', '/DWIN32', '/D_WINDOWS', '/DWINVER=0x0500', '/D_USE_MATH_DEFINES', '/D_SCL_SECURE_NO_WARNINGS', '/D_CRT_SECURE_NO_WARNINGS', '/wd4275']

else :
	print "* COMMENCING LINUX BUILD *"
	cflags.append(['-Wall','-Wno-unused-variable','-Wno-sign-compare','-Wno-comment'])
	cppPaths.append(['/usr/include/hdf'])
	if int(debug) :
	    cflags.append(['-g','-O0'])
	else :
	    cflags.append(['-g','-O3'])

	env.Append(LINKFLAGS=['-g'])
	libList.append(['hdf5','mfhdf','df','mxml','jpeg','z'])
	libDirList.append(['/usr/lib64/hdf','/usr/lib/hdf'])

#Export for SConscripts
env.Append(CFLAGS=cflags)
env.Append(CPPPATH=cppPaths)
env.Append(CPPDEFINES=cppDefines)
env.Replace(MYLIBLIST=libList)
env.Replace(MYLIBDIRLIST=libDirList)

Export('env')

#Real work begins here
subProjs = []

subProjs.append('src')
subProjs.append('bindings/cpp')

# we want to link libraries in the reverse order we build them
shared_libraries = []
static_libraries = []
shared_objects = []
lib_list = []
orig_libs = env['MYLIBLIST']

for subproj in subProjs:
	ret = SConscript([subproj+'/SConscript'])
	env.Install('Bin/Shared', ret['shared'])
	env.Install('Bin/Static', ret['static'])
	shared_libraries.insert(0,ret['shared'])
	static_libraries.insert(0,ret['static'])
	shared_objects = shared_objects + ret['sharedobjs']
	lib_list.insert(0,ret['libs'])
	env.Replace(MYLIBLIST=lib_list + orig_libs)

env.Replace(SHARED_LIST=shared_libraries)
env.Replace(STATIC_LIST=static_libraries)

#
# Correct bug in windows scons - OBJSUFFIX is not substituted for some reason
#
for i in range(len(shared_objects)) :
    shared_objects[i] = string.replace(shared_objects[i], '$OBJSUFFIX', env['OBJSUFFIX'])

# main program
ret = SConscript(['applications/SConscript'])
env.Install('Bin/Shared', ret['shared'])
env.Install('Bin/Static', ret['static'])
