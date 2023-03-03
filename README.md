# hyperv2hosts

Hyper-V is a top virtualization solution for Microsoft Windows.
One problem that frustrates some users is that the default switch changes its IP address every time
it restarts, and there is no way one can access the guest by hostname.
The purpose of this program is to query a Hyper-V guest and its IP addresses and add them
to the host file at `c:\windows\system32\drivers\etc\hosts`, so one can use hostnames to refer to
the guests, instead of having to figure out their IP dynamically each time the machine restarts.

The intended way to run this program is to register a task with Task Manager and have this program
run everytime after a guest restarts.

# Warning

The program does not work smoothly, even on my own machine due to the
complicated environment of Windows.  It might be easier for one to write
a script instead of using the code of this project.  The idea is
straightforward:


- 1. Use powershell to query the guest properties.  
```
powershell.exe  -command \"Get-VMNetworkAdapter -VMName GUEST_NAME | Select VMName, IPAddresses\"";
```
- 2. Update the host file using the results.
- 3. Bundle 1 and 2 into a script and add that to the task scheduler as
     described below.


# How to Create Task

Open "Task Scheduler", click "Create Task..." on the right side.  The
important fields are listed below:

- General
	* Name: anything
	* Security options: Run whether user is logged on or not.
	* Check "Run with highest priviledges".
- Triggers, click "New...":
	* Begin the task: On an event
	* Settings: Basic
	* Log: Microsoft-Windows-Hyper-V-Worker/Admin
	* Source: Hyper-V-Worker
	* Event ID: 18500
- Actions, click New...
	* Action: Start a program.
	* Program/script: select this program.
	* Add arguments: `--name <guest name>`
	* Start in: any directory, the program generates a temporary file.

The arguments "--host-fmt vm_{}" can be used to change the hostname
format in the host file.  "{}" will be replaced with guest name. The
example here will prepends "vm_" to the guest name.

# Additional Tips

(From @upadsamay387)

- If IP does not show up, one might have to install `linux-azure` on the
  guest
  (https://learn.microsoft.com/en-us/windows-server/virtualization/hyper-v/supported-ubuntu-virtual-machines-on-hyper-v).
- It might take a few second's delay for the IP address to be updated,
  so add some delay for the task execution.







