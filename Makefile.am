#====================================================================
#  NeXus - Neutron & X-ray Common Data Format
#  
#  $Id$
#
#  Top level Makefile for coordinating NeXus build
#  
#  Copyright (C) 2004 Freddie Akeroyd
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
#  For further information, see <http://www.neutron.anl.gov/NeXus/>
#
#  @configure_input@
#
#====================================================================

# so   "make test"   is the same as   "make check"
test: check

if BUILD_CONTRIB
CONTRIB=contrib
endif
SUBDIRS=include src bindings applications test scripts examples doc $(CONTRIB)

EXTRA_DIST=build_rules.am nexus_spec.in nexus.spec autoversion.sh \
		build_rpm.in README.cygwin README.WIN32 README.macosx \
		README.MinGW make_mingw_links SConstruct nexus_scons_utils.py

# Add any flags here that you want passed to configure when you do 
# a "make distcheck"
# DISTCHECK_CONFIGURE_FLAGS=--without-java
