// xml_pkg.cpp:  ActionScript "flash.xml" package, for Gnash.
// 
//   Copyright (C) 2005, 2006, 2007, 2008, 2009 Free Software Foundation, Inc.
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#include "Object.h" // for getObjectInterface
#include "as_object.h"

#include "string_table.h"
#include "VM.h"
#include "fn_call.h"
#include "MovieClip.h"

#include "XMLDocument_as.h"
#include "XMLNodeType_as.h"
#include "XMLNode_as.h"

#include "xml_pkg.h"
#include "xmlclasses.h"

namespace gnash {

static as_value
get_flash_xml_package(const fn_call& fn)
{

    /// This package is AS3 only!
    assert(isAS3(fn));

	log_debug("Loading AVM2 flash.xml package");
	as_object *pkg = new as_object(getObjectInterface());

	// Call the [objectname]_init() function for each AS2 class.
	int i = 0;
	while (as2xmlclasses[i]) {
	    as2xmlclasses[i](*pkg);
        ++i;
	} 

	// Call the [objectname]_init() function for each AS3 class.
	i = 0;
	while (as3xmlclasses[i]) {
	    as3xmlclasses[i](*pkg);
        ++i;
	} 

	return pkg;
}

void
flash_xml_package_init(as_object& where)
{
	string_table& st = where.getVM().getStringTable();
	where.init_destructive_property(st.find("xml"), get_flash_xml_package);
}


} // end of gnash namespace
