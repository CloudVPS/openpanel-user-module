#!/usr/bin/env python
from OpenPanel import coreclient
import os

# FIXME: find right partition for user partitions
# FIXME: or just add up all usage over all partitions?
f = os.popen("repquota -vu /")

# skip header
for line in f:
    if line.startswith("-"):
        break

usage = dict()
for line in f:
    if line.startswith("#"):
        continue
    
    parts = line.split()
    if len(parts)<3:
        continue

    try:
        usage[parts[0]]=int(parts[2])
    except:
        pass
        
# print usage

c = coreclient.CoreSession()
c.login("!User","")

for user in usage:
    # TODO: optimisation: skip user if he's not in the paneluser group
    try:
        uuid = c.rpc.getrecord(classid="User", objectid=user)["body"]["data"]["object"]["User"]["uuid"]
    except:
        continue # ignore unknown users
    print "user %s usage %d" % (user,usage[user])
    print uuid
    
    c.updateobject(parentid=uuid, classid="OpenCORE:Quota", objectid="User/diskspace:usage", data=dict(usage=usage[user]))
