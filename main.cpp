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

#include <moduleapp.h>
#include "userModule.h"

#include <grace/file.h>
#include <grace/filesystem.h>

using namespace moderr;

APPOBJECT(userModule);

bool verifymd5pw(const string &passwd)
{
    static string validmd5pass ("abcdefghijklmnopqrstuvwxyz0123456789"
                                "ABCDEFGHIJKLMNOPQRSTUVWXYZ/.$");
                                
    return passwd.validate(validmd5pass);                                
}

//  =========================================================================
/// Main method.
//  =========================================================================
int userModule::main (void)
{
	string conferr;

	if (data["OpenCORE:Command"] == "getconfig")
	{
		value ret = $type("opencore.module") ->
					$("User",
						$attr("type","class") ->
						$("openadmin",
							$attr("type","object") ->
							$("id", "openadmin") ->
							$("password", "*") ->
							$("name_customer", "Administrator") ->
							$("language", "en_EN") ->
							$("emailaddress", "openadmin@example.net")
						 )
					 ) ->
					 $("OpenCORE:Result",
					 	$("code", moderr::ok) ->
					 	$("message", "OK")
					  );

		string res = ret.toxml ();
		fout.writeln ("%u" %format (res.strlen()));
		fout.puts (res);

		return true;
	}
	else if (data["OpenCORE:Command"] == "setspecialphysicalquota")
	{
	    foreach(quota, data["quota"])
	    {
	        authd.setquota(quota.label().sval(),
	                       quota.ival(),
                           quota.ival());
	    }
	}
	else if (data["OpenCORE:Session"]["classid"] == "User")
	{
		if (data["OpenCORE:Command"] == "create")
		{
		    if (verifymd5pw(data["User"]["password"]))
			// Create a new unix user if not yet exists, error
			// if already exists
            {        	
            	if (authd.adduser (data["User"]["id"], "", data["User"]["password"]))
        		{
        			sendresult( err_authdaemon, makeauthderror("Error creating system user"));
        			authd.rollback ();
        		}
        		if (authd.setusershell (data["User"]["id"], "/bin/false"))
        		{
        			sendresult( err_authdaemon, makeauthderror("Error setting shell to /bin/false for new user"));
        			authd.rollback ();
        		}
            }
            else
            {
		        sendresult( err_command, "Invalid crypted password");
				authd.rollback ();
		    }
		}
		else if (data["OpenCORE:Command"] == "delete")
		{
			// Delete unix user account
			if (verifymd5pw(data["User"]["password"]))
            {
        		if (authd.deluser (data["User"]["id"]))
        		{
        			sendresult( err_authdaemon, makeauthderror("Error removing system user"));
        			authd.rollback ();
        		}
            }
            else
            {
		        sendresult( err_command, "Invalid crypted password");
				authd.rollback ();
		    }
		}
 		else if (data["OpenCORE:Command"] == "update")
		{
			// Update password
			if (authd.setuserpass (data["User"]["id"], data["User"]["password"]))
			{
				sendresult( err_authdaemon, makeauthderror("Error updating system user's password"));
				authd.rollback ();
			}

		}
		else
		{
			// Error, unknown command found in request
			sendresult (err_command, 
						"Unsupported command for User");
			return 0;
		}
	}
	else
	{
		// Error, unknown command found in request
		sendresult (err_command, 
					"Unsupported class");
		return 0;
	}


	// Finalize authd request and close socket
	authd.quit ();

	// Send final result
	// TODO: it appears we'll send an OK even after we send an error
	// but this appears to work
	sendresult (moderr::ok, "");

	return 0;
}

string *userModule::makeauthderror(const statstring &s)
{
	returnclass (string) res retain;

	value authderr = authd.getlasterror();

	res=s;
	res.strcat(": ");
	res.strcat(authderr["resultmsg"]);
	return &res;
}

