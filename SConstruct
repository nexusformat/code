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

shlibList = []
libList = []
if not env.GetOption('clean'): # do not re-rerun checking on a "scons -c"
	conf = Configure(env)
#	if conf.CheckLib()
	env = conf.Finish()

shlibDirList = []
libDirList = []
cflags = []
shcflags = []
cxxflags = []
shcxxflags = []
ccflags = []
shccflags = []
dirList = []
cppPaths = []
cppDefines = [ ('HDF5',1), ('NXXML',1), ('HDF4',1) ]
#Get paths
if os.name == 'nt': 
	print "* COMMENCING WINDOWS BUILD *"

	libList.append('hdf5')
	libList.append('szlib')
	libList.append('zlib')
	libList.append('hd423.lib')
	libList.append('hm423.lib')
	libList.append('xdr.lib')
	libList.append('libjpeg.lib')
	libList.append('ws2_32.lib')
	libList.append('mxml_libcmt.lib')
	shlibList.append('hdf5dll')
	shlibList.append('hd423m.lib')
	shlibList.append('hm423m.lib')
	shlibList.append('mxml_msvcrt.lib')
	libDirList.append('C:/Program Files/hdf/jpeg6-vs2005/lib')
	libDirList.append('c:/program files/hdf/5-167-win-vs2005/lib') 
	libDirList.append('c:/program files/hdf/42r3-win-vs2005/lib')
	shlibDirList.append('c:/program files/hdf/5-167-win-vs2005/dll')
	shlibDirList.append('c:/program files/hdf/42r3-win-vs2005/dll')
	shlibDirList.append('c:/msys/1.0/local/lib')
#	libDirList.append('C:/Program Files/hdf/zlib123-vs2005/lib')
	libDirList.append('C:/Install Kits/zlib-1.2.3')
	libDirList.append('C:/Program Files/hdf/szip21-vs2005-noenc/lib')
	libDirList.append('c:/msys/1.0/local/lib')
	cppPaths.append(['c:/msys/1.0/local/include','c:/program files/hdf/5-167-win-vs2005/include','c:/program files/hdf/42r3-win-vs2005/include'])

	if int(mingw) :
		cflags = ['-g', '-O2']
		env.Append(LINKFLAGS=['-g','-Wl,--export-all-symbols'])
		shcflags.append(['-D_HDF5USEDLL_=1'])
	else :
		if int(debug) :
			ccflags = [ '/W3', '/Od', '/FD', '/EHsc', '/MTd', '/TP', '/D_DEBUG', '/DMS_VISUAL_STUDIO=1', '/DWIN32', '/D_WINDOWS', '/DWINVER=0x0500', '/D_USE_MATH_DEFINES', '/D_SCL_SECURE_NO_WARNINGS', '/D_CRT_SECURE_NO_WARNINGS', '/wd4275', '/wd4996' ]
			shccflags = [ '/W3', '/Od', '/FD', '/EHsc', '/MDd', '/TP', '/D_DEBUG', '/DMS_VISUAL_STUDIO=1', '/DWIN32', '/D_WINDOWS', '/DWINVER=0x0500', '/D_USE_MATH_DEFINES', '/D_SCL_SECURE_NO_WARNINGS', '/D_CRT_SECURE_NO_WARNINGS', '/wd4275', '/wd4996' ]
		else :
			cflags = [ '/TC' ]
			shcflags = cflags
			cxxflags = [ '/TP' ]
			shcxxflags = cxxflags
			ccflags = ['/Ox', '/Ot', '/W3','/EHsc', '/MT', '/DMS_VISUAL_STUDIO=1', '/DWIN32', '/D_WINDOWS', '/DWINVER=0x0500', '/D_USE_MATH_DEFINES', '/D_SCL_SECURE_NO_WARNINGS', '/D_CRT_SECURE_NO_WARNINGS', '/wd4275']
			shccflags = ['/Ox', '/Ot', '/W3','/EHsc', '/MD', '/DMS_VISUAL_STUDIO=1', '/DWIN32', '/D_WINDOWS', '/DWINVER=0x0500', '/D_USE_MATH_DEFINES', '/D_SCL_SECURE_NO_WARNINGS', '/D_CRT_SECURE_NO_WARNINGS', '/wd4275']
		cppPaths.append(['#Windows_extra/include'])
		shccflags.append(['/D_HDF5USEDLL_=1'])
else :
	print "* COMMENCING LINUX BUILD *"
	ccflags.append(['-Wall','-Wno-unused-variable','-Wno-sign-compare','-Wno-comment'])
	if int(debug) :
	    ccflags.append(['-g','-O0'])
	else :
	    ccflags.append(['-g','-O3'])
	ccflags.append(['-fPIC'])
	shccflags = ccflags

	env.Append(LINKFLAGS=['-g'])
	libList.append(['hdf5','mfhdf','df','mxml','jpeg','z'])
	libDirList.append(['/usr/lib64/hdf','/usr/lib/hdf'])
	shlibList.append(['hdf5','mfhdf','df','mxml','jpeg','z'])
	shlibDirList.append(['/usr/lib64/hdf','/usr/lib/hdf'])
	cppPaths.append(['/usr/include/hdf'])

libDirList.append('#Bin/Static')
shlibDirList.append('#Bin/Shared')

#Export for SConscripts
env.Replace(SHOBJSUFFIX='.sob') # force separate shared and static objects
env.Replace(CFLAGS=cflags)
env.Replace(SHCFLAGS=shcflags)
env.Append(CXXFLAGS=cxxflags)
env.Append(SHCXXFLAGS=shcxxflags)
env.Replace(CCFLAGS=ccflags)
env.Replace(SHCCFLAGS=shccflags)
env.Append(CPPPATH=cppPaths)
env.Append(CPPDEFINES=cppDefines)
env.Replace(MYLIBLIST=libList)
env.Replace(MYLIBDIRLIST=libDirList)
env.Replace(MYSHLIBLIST=shlibList)
env.Replace(MYSHLIBDIRLIST=shlibDirList)

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
shorig_libs = env['MYSHLIBLIST']

for subproj in subProjs:
	ret = SConscript([subproj+'/SConscript'])
	env.Install('Bin/Shared', ret['shared'])
	env.Install('Bin/Static', ret['static'])
	shared_libraries.insert(0,ret['shared'])
	static_libraries.insert(0,ret['static'])
	shared_objects = shared_objects + ret['sharedobjs']
	lib_list.insert(0,ret['libs'])
	env.Replace(MYLIBLIST=lib_list + orig_libs)
	env.Replace(MYSHLIBLIST=lib_list + shorig_libs)

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
