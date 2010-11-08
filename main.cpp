// This file is part of OpenPanel - The Open Source Control Panel
// The Grace library is free software: you can redistribute it and/or modify it 
// under the terms of the GNU General Public License as published by the Free 
// Software Foundation, either version 3 of the License.
//
// Please note that use of the OpenPanel trademark may be subject to additional 
// restrictions. For more information, please visit the Legal Information 
// section of the OpenPanel website on http://www.openpanel.com/

#include <opencore/moduleapp.h>
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
		value prefs = $("mailcontact", "root@localhost") ->
					  $("smtphost", "127.0.0.1") ->
					  $("sendalerts", false);
		
		if (fs.exists ("/etc/openpanel/preferences.xml"))
		{
			prefs.loadxml ("/etc/openpanel/preferences.xml");
		}
		value ret = $type("opencore.module") ->
					$("User",
						$attr("type","class") ->
						$("openadmin",
							$attr("type","object") ->
							$("id", "openadmin") ->
							$("password", "*") ->
							$("name_customer", "Administrator") ->
							$("emailaddress", "openadmin@example.net")
						 )
					 ) ->
					 $("OpenCORE:Prefs",
					 	$attr("type","class") ->
					 	$("prefs",
					 		$attr("type","object") ->
					 		$("id", "prefs") ->
					 		$merge(prefs)
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
	                       quota.ival() * 1024,
                           quota.ival() * 1024);
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
	else if (data["OpenCore:Session"]["classid"] == "OpenCORE:Prefs")
	{
		value prefs = data["OpenCORE:Prefs"];
		prefs.rmval ("uuid");
		prefs.rmval ("version");
		prefs.rmval ("id");
		prefs.rmval ("metaid");
		prefs.savexml ("/var/opencore/conf/staging/User/preferences.xml");
		if (authd.installfile ("preferences.xml","/etc/openpanel"))
		{
			sendresult( err_authdaemon, makeauthderror("Error updating preferences"));
			authd.rollback ();
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

