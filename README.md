# hyperv2hosts

Hyper-V is a top virtualization solution for Microsoft Windows.
One problem that frustrates some users is that the default switch changes its IP address every time
it restarts, and there is no way one can access the guest by hostname.
The purpose of this program is to enumerate Hyper-V guests and their IP addresses and add them
to the host file at `c:\windows\system32\drivers\etc\hosts`, so one can use hostnames to refer to
the guests, instead of having to figure out their IP dynamically each time the machine restarts.

The intended way to run this program is to register a task with Task Manager and have this program
run everytime after a guest restarts.





