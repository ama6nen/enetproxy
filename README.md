# Growtopia enet proxy

# Download release for V3.51 [here](https://github.com/ama6nen/enetproxy/releases/tag/V3.51)
# NOTE: This is not an internal cheat. Proxy is not an internal cheat in any way or form.
## For internal see [this](https://github.com/ama6nen/INZERNAL), my new internal project.

## Don't add me in discord for help with compiling/adding feature/some simple thing with proxy or you will get instantly blocked

skeet.cc for gt at this point 

Anyone using or selling "fixed proxy" is a human failure

## How to use
* Does not need separate http server
* Use HxD or similar to edit "growtopia1.com" and "growtopia2.com" to "localhost" In growtopia binary.
* Alternative: Use hosts file to set growtopia1 and growtopia2 to point to 127.0.0.1 if you want to
* Start proxy -> Run localhost patched file (or normal with hosts) -> Works as supposed

## Features
* Print all variantlists and function calls
* Print all text packets
* Supports modifying, ignoring, and making new packets
* Kill players with wrench
* /legal command to clear malpractice without owning legal briefs
* More commands which you can find in the changelog
* Has a PoC /name name command to call OnNameChanged function for local client.
* Has a PoC OnSpawn modifier which appends netid to each players' name, and gives you unlim zoom
* Can both intercept outgoing and incoming packets
* Integrated http server
* Ignore tracking packets and crash log requests
* Ignore autoban packets (NOTE: you can still get autobanned if the check is serversided)
* Works with subserver switching

## TODO
* Delete this

## Changelog

# 1.4.3
* Add AAP Bypass with no spoof info needed

# 1.4.2
* Add /proxy - shows proxy commands
* Add /tp player - teleports to a player in the world
* Add /ghost - toggles ghost state (you wont move for others when its enabled)
* Major refactor of the whole code
* Add world/player classes, and some helpers for easier adding of new features
* Add solve captcha
* Remove /resolve because its patched
* Update version to 3.011
* Add /kill name, or just "/kill " to kill everyone 
# 1.4
* Add /proxy - shows proxy commands
* Add /tp player - teleports to a player in the world
* Add /ghost - toggles ghost state (you wont move for others when its enabled)
* Major refactor of the whole code
* Add world/player classes, and some helpers for easier adding of new features

# 1.3
* Fix crashes
* Add /uid name - resolves name to uid (only works for online non-mod players)
* Add /flag id - that sets your client flag to be some item id
* Add /legal - clears your malpractice status when you dont own a legal brief.
* Add kill player button when wrenching players - just click give up on the surgery dialog

# 1.2
* Misc fixes
* Add /resolve uid to name and their current world (aka tracking). Uid can be gotten from onspawn or from tracking packet if the person owns the world.
* Automatically resolve moderator uids when they enter
* Edit some values like meta/wk/hash2 so theres a partial automatic unban (you need to add mac address yourself, I didnt because i was using it for something else)

# 1.1
* Subserver switching should work, apparently there was no problem in the first place as the implementation worked already
* Edit ping requests always to be non offensive behavior
* Ignore autoban packets sent by client
* Ignore tracking packets
* Ignore crash logs that would be sent to gt
* Clean code
* Gives unlimited zoom

### Video: https://streamable.com/bhokj  

![x](https://i.imgur.com/RG2o9pM.png "Proxy pic 3")
![x](https://i.imgur.com/3DFiMgS.png "Proxy pic 2")
![x](https://i.imgur.com/Lndhj70.png "Proxy pic 1")

