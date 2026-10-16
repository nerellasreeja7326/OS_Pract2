
# OSSP Practical 10

## Part A: Investigating Inodes, Hard Links and Symbolic Links

### Aim
To investigate inode structures using Linux commands and understand hard links and symbolic links.

### Commands Used
- ls -i
- ls -li
- stat
- stat -L
- ln
- ln -s
- readlink
- find
- rm

### Result
Hard links share the same inode. Symbolic links have their own inode and refer to a target pathname. Deleting a symbolic link's target can make the link broken.

## Part B: File I/O Using mmap() and read()/write()

### Aim
To implement file modification using mmap() and traditional read()/write() system calls.

### Files
- mmap_file.c
- read_write_file.c
- data.txt

### Compile and Run mmap()
gcc -Wall -Wextra -g mmap_file.c -o mmap_file
./mmap_file

### Compile and Run read()/write()
gcc -Wall -Wextra -g read_write_file.c -o read_write_file
./read_write_file

### Result
Both programs replace the first five characters of data.txt with HELLO.

## Comparison

| Feature | read()/write() | mmap() |
|---|---|---|
| Access | Buffer-based | Memory-based |
| Modification | Uses write() | Uses mapped memory |
| File positioning | Uses file offsets | Uses memory addresses |
| Synchronization | write() transfers data | msync() requests synchronization |
