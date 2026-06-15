savedcmd_/workspaces/linuxdriver/mydevice.mod := printf '%s\n'   mydevice.o | awk '!x[$$0]++ { print("/workspaces/linuxdriver/"$$0) }' > /workspaces/linuxdriver/mydevice.mod
