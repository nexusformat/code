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
# $Id$
#
# @configure_input@
#
#====================================================================

# so   "make test"   is the same as   "make check"
test: check

SUBDIRS=include src bindings test applications scripts

EXTRA_DIST=build_rules.am nexus.spec.in nexus.spec autoversion.sh

# Add any flags here that you want passed to configure when you do 
# a "make distcheck"
# DISTCHECK_CONFIGURE_FLAGS=--without-java
