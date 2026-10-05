# Operating Systems – Lab 1

## The OS as a Resource Manager

**Name:** Sofia Ermicev  
**Group:** FAF-241  
**Year:** 2026-2027

## System Information

```text
Linux DESKTOP-10DB5P6 5.15.167.4-microsoft-standard-WSL2 #1 SMP Tue Nov 5 00:21:55 UTC 2024 x86_64 x86_64 x86_64 GNU/Linux
17:26:07 up 2 min, 1 user, load average: 0.01, 0.00, 0.00
```

## Part 1 - Files and Directories

### Observations

1. The files I created are owned by user `sofia` and belong to group `sofia`.

2. After running `chmod 600 note.txt`, the permissions became:

`-rw-------`

The first character `-` means that `note.txt` is a regular file.  
`rw-` means that the owner can read and write the file.  
The second `---` means that the group has no permissions.  
The last `---` means that other users have no permissions.

3. Two directories directly under `/` are:

- `/etc` – contains system-wide configuration files.
- `/home` – contains users' home directories.

### Interesting Command Output

```text
total 8
-rw-r--r-- 1 sofia sofia 24 Sep 28 17:28 note.txt
-rw-r--r-- 1 sofia sofia 24 Sep 28 17:28 renamed.txt
```

## Part 2 - Processes

### Observations

1. Process number 1 has PID `1`. On my system, it is `/sbin/init`, which acts as the init process and is responsible for starting and managing system services.

2. There were approximately **29 processes** running on the idle system.

3. For the `sleep` process, `/proc/665/status` showed:

`State: S (sleeping)`

This means that the process was in a sleeping state and was waiting rather than actively using the CPU.

### Interesting Command Output

```text
Name:   sleep
State:  S (sleeping)
Pid:    665
PPid:   287
VmPeak:     3128 kB
VmSize:     3128 kB
```

## Part 3 - Memory

### Observations

1. The system has about **13 GiB of total RAM**, with about **12 GiB free** at the time of observation.

2. Swap is disk space that the operating system can use as additional memory when physical RAM is under pressure. My system has **4.0 GiB of swap configured**, and none of it was being used at the time.

3. The `sleep` process used **1080 kB** of resident memory (`VmRSS`). This is a small amount of memory, which is expected because `sleep` performs almost no computation.

### Interesting Command Output

```text
               total        used        free      shared  buff/cache   available
Mem:            13Gi       711Mi        12Gi       3.3Mi       524Mi        12Gi
Swap:          4.0Gi          0B       4.0Gi
```

## Part 4 - Devices and Storage

### Observations

1. The root filesystem `/` is mounted on `/dev/sdc`.

2. One example from `/dev` is `/dev/console`. It represents the system console device used for terminal input and output.

3. The UNIX idea that "everything is a file" means that many system resources, including devices, are exposed through file-like interfaces that programs can read from or write to.

### Interesting Command Output

```text
NAME
    MAJ:MIN RM   SIZE RO TYPE MOUNTPOINTS
sda   8:0    0 388.4M  1 disk
sdb   8:16   0     4G  0 disk [SWAP]
sdc   8:32   0     1T  0 disk /mnt/wslg/distro
                              /
```

## Conclusion

The operating system manages four main resources: files, processes, memory, and devices/storage.  
I observed file management with `ls`, process management with `ps`, memory management with `free`, and device/storage management with `lsblk`.  
These commands showed how the operating system organizes system resources and provides controlled access to them for running programs.