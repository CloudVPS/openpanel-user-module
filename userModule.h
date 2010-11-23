// This file is part of OpenPanel - The Open Source Control Panel
// OpenPanel is free software: you can redistribute it and/or modify it 
// under the terms of the GNU General Public License as published by the Free 
// Software Foundation, using version 3 of the License.
//
// Please note that use of the OpenPanel trademark may be subject to additional 
// restrictions. For more information, please visit the Legal Information 
// section of the OpenPanel website on http://www.openpanel.com/

#ifndef _userModule_H
#define _userModule_H 1

#include <openpanel-core/moduleapp.h>
#include <grace/system.h>
#include <grace/configdb.h>


typedef configdb<class userModule> appconfig;

//  -------------------------------------------------------------------------
/// Main application class.
//  -------------------------------------------------------------------------
class userModule : public moduleapp
{
public:
		 	 userModule (void) :
				moduleapp ("openpanel.module.user"),
				conf (this)
			 {
			 }
			~userModule (void)
			 {
			 }
	
	string	*makeauthderror(const statstring &s);

	int		 main (void);
	
protected:
	appconfig		conf;		///< Module configuration data	
};

#endif
