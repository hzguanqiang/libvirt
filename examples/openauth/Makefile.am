## Copyright (C) 2005-2013 Red Hat, Inc.
##
## This library is free software; you can redistribute it and/or
## modify it under the terms of the GNU Lesser General Public
## License as published by the Free Software Foundation; either
## version 2.1 of the License, or (at your option) any later version.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public
## License along with this library.  If not, see
## <http://www.gnu.org/licenses/>.

INCLUDES = -I$(top_builddir)/include -I$(top_srcdir)/include
noinst_PROGRAMS = openauth
openauth_CFLAGS = $(WARN_CFLAGS)
openauth_SOURCES = openauth.c
openauth_LDADD = $(top_builddir)/src/libvirt.la
