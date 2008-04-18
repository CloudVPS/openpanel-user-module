// --------------------------------------------------------------------------
// OpenPanel - The Open Source Control Panel
// Copyright (c) 2006-2007 PanelSix
//
// This software and its source code are subject to version 2 of the
// GNU General Public License. Please be aware that use of the OpenPanel
// and PanelSix trademarks and the IconBase.com iconset may be subject
// to additional restrictions. For more information on these restrictions
// and for a copy of version 2 of the GNU General Public License, please
// visit the Legal and Privacy Information section of the OpenPanel
// website on http://www.openpanel.com/
// --------------------------------------------------------------------------

#ifndef _userModule_H
#define _userModule_H 1

#include <moduleapp.h>
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
