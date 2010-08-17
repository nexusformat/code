pkgconfigdir		= ${libdir}/pkgconfig
nodist_bin_SCRIPTS 	= nxbuild nexus-config

EXTRA_DIST		= nxbuild.in nexus.pc.in nexus-cpp.pc.in nexus-config.in

pkgconfig_DATA		= nexus.pc

if HAVE_CPP
pkgconfig_DATA		+= nexus-cpp.pc
endif
