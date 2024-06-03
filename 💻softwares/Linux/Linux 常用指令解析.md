全部的命令可以从 https://www.gnu.org/software/coreutils/manual/coreutils.pdf 
找到, 并且在 OneDrive 中有对应的 pdf  

![[attachments/Pasted image 20240603124244.png]]

常用的获取帮助的方法
```
man rm
```
### rm 指令的使用
```shell
Usage: rm [OPTION]... [FILE]...
Remove (unlink) the FILE(s).

  -f, --force           ignore nonexistent files and arguments, never prompt
  -i                    prompt before every removal
  -I                    prompt once before removing more than three files, or
                          when removing recursively; less intrusive than -i,
                          while still giving protection against most mistakes
      --interactive[=WHEN]  prompt according to WHEN: never, once (-I), or
                          always (-i); without WHEN, prompt always
      --one-file-system  when removing a hierarchy recursively, skip any
                          directory that is on a file system different from
                          that of the corresponding command line argument
      --no-preserve-root  do not treat '/' specially
      --preserve-root   do not remove '/' (default)
  -r, -R, --recursive   remove directories and their contents recursively
  -d, --dir             remove empty directories
  -v, --verbose         explain what is being done
      --help     display this help and exit
      --version  output version information and exit

By default, rm does not remove directories.  Use the --recursive (-r or -R)
option to remove each listed directory, too, along with all of its contents.

To remove a file whose name starts with a '-', for example '-foo',
use one of these commands:
  rm -- -foo

  rm ./-foo

Note that if you use rm to remove a file, it might be possible to recover
some of its contents, given sufficient expertise and/or time.  For greater
assurance that the contents are truly unrecoverable, consider using shred.

GNU coreutils online help: <http://www.gnu.org/software/coreutils/>
Full documentation at: <http://www.gnu.org/software/coreutils/rm>
or available locally via: info '(coreutils) rm invocation'

```

shred 
```shell 
Usage: shred [OPTION]... FILE...
Overwrite the specified FILE(s) repeatedly, in order to make it harder
for even very expensive hardware probing to recover the data.

If FILE is -, shred standard output.

Mandatory arguments to long options are mandatory for short options too.
  -f, --force    change permissions to allow writing if necessary
  -n, --iterations=N  overwrite N times instead of the default (3)
      --random-source=FILE  get random bytes from FILE
  -s, --size=N   shred this many bytes (suffixes like K, M, G accepted)
  -u             truncate and remove file after overwriting
      --remove[=HOW]  like -u but give control on HOW to delete;  See below
  -v, --verbose  show progress
  -x, --exact    do not round file sizes up to the next full block;
                   this is the default for non-regular files
  -z, --zero     add a final overwrite with zeros to hide shredding
      --help     display this help and exit
      --version  output version information and exit

Delete FILE(s) if --remove (-u) is specified.  The default is not to remove
the files because it is common to operate on device files like /dev/hda,
and those files usually should not be removed.
The optional HOW parameter indicates how to remove a directory entry:
'unlink' => use a standard unlink call.
'wipe' => also first obfuscate bytes in the name.
'wipesync' => also sync each obfuscated byte to disk.
The default mode is 'wipesync', but note it can be expensive.

CAUTION: Note that shred relies on a very important assumption:
that the file system overwrites data in place.  This is the traditional
way to do things, but many modern file system designs do not satisfy this
assumption.  The following are examples of file systems on which shred is
not effective, or is not guaranteed to be effective in all file system modes:

* log-structured or journaled file systems, such as those supplied with
AIX and Solaris (and JFS, ReiserFS, XFS, Ext3, etc.)

* file systems that write redundant data and carry on even if some writes
fail, such as RAID-based file systems

* file systems that make snapshots, such as Network Appliance's NFS server

* file systems that cache in temporary locations, such as NFS
version 3 clients

* compressed file systems

In the case of ext3 file systems, the above disclaimer applies
(and shred is thus of limited effectiveness) only in data=journal mode,
which journals file data in addition to just metadata.  In both the
data=ordered (default) and data=writeback modes, shred works as usual.
Ext3 journaling modes can be changed by adding the data=something option
to the mount options for a particular file system in the /etc/fstab file,
as documented in the mount man page (man mount).

In addition, file system backups and remote mirrors may contain copies
of the file that cannot be removed, and that will allow a shredded file
to be recovered later.

GNU coreutils online help: <http://www.gnu.org/software/coreutils/>
Full documentation at: <http://www.gnu.org/software/coreutils/shred>
or available locally via: info '(coreutils) shred invocation'

```