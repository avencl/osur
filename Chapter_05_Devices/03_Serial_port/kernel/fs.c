#define _K_FS_C_

#include <kernel/errno.h>
#include <kernel/memory.h>
#include <types/io.h>
#include <lib/list.h>
#include <lib/string.h>
#include "fs.h"
#include "device.h"
#include "time.h"
#include "memory.h"

static kdevice_t *disk;
#define DISK_WRITE(buffer, blocks, first_block) \
k_device_send(buffer, (first_block << 16) | blocks, 0, disk);

#define DISK_READ(buffer, blocks, first_block) \
k_device_recv(buffer, (first_block << 16) | blocks, 0, disk);

static struct fs_table *ft;
static size_t ft_size;
static struct kfile_desc *last_check;
static list_t open_files;

int k_fs_init(char *disk_device, size_t bsize, size_t blocks)
{
    disk = k_device_open ( disk_device, 0 );
    assert(disk);

    //initialize disk
    ft_size = sizeof(struct fs_table) + blocks;
    ft_size = (ft_size + bsize - 1) / bsize;
    ft = kmalloc(ft_size * bsize);
    memset(ft, 0, ft_size * bsize);
    ft->file_system_type = FS_TYPE;
    strcpy(ft->partition_name, disk_device);
    ft->block_size = bsize;
    ft->blocks = blocks;
    ft->max_files = MAXFILESONDISK;

    int i;
    for (i = ft_size; i < ft->blocks; i++)
        ft->free[i] = 1;

    DISK_WRITE(ft, ft_size, 0);

    list_init(&open_files);

    last_check = NULL;

    return 0;
}

int k_fs_is_file_open(descriptor_t *desc)
{
    kobject_t *kobj;

    kobj = desc->ptr;
    if (kobj && list_find(&kobjects, &kobj->list) &&
        (kobj->flags & KTYPE_FILE) != 0)
    {
        struct kfile_desc *fd = kobj->kobject;
        if (fd && fd->id == desc->id &&
            list_find(&open_files, &fd->list))
        {
            last_check = fd;
            return 0;
        }
    }

    return -1;
}

int k_fs_open_file(char *pathname, int flags, mode_t mode, descriptor_t *desc)
{
    struct fs_node *tfd = NULL;
    char *fname = &pathname[5];

    //check for conflicting flags
    if (	((flags & O_RDONLY) && (flags & O_WRONLY)) ||
            ((flags & O_RDONLY) && (flags & O_RDWR))   ||
            ((flags & O_WRONLY) && (flags & O_RDWR))
            )
        return -EINVAL;

    //check if file already open
    struct kfile_desc *fd = list_get(&open_files, FIRST);
    while (fd != NULL) {
        if (strcmp(fd->tfd->node_name, fname) == 0) {
            //already open!
            //if its open for reading and O_RDONLY is set in flags
            if ((fd->flags & O_RDONLY) == (flags & O_RDONLY))
                tfd = fd->tfd;//fine, save pointer to descriptor
            else
                return -EADDRINUSE; //not fine

        }
        fd = list_get_next(&fd->list);
    }

    if (!tfd) {
        //not already open; check if such file exists in file table
        int i;
        for (i = 0; i < ft->max_files; i++) {
            if (strcmp(ft->fd[i].node_name, fname) == 0) {
                tfd = &ft->fd[i];
                break;
            }
        }
    }

    if (!tfd) {
        //file doesn't exitst
        if ((flags & (O_CREAT | O_WRONLY)) == 0)
            return -EINVAL;

        //create fs_node in fs_table
        //1. find unused descriptor
        int i;
        for (i = 0; i < ft->max_files; i++) {
            if (ft->fd[i].node_name[0] == 0) {
                tfd = &ft->fd[i];
                break;
            }
        }
        if (!tfd)
            return -ENFILE;

        //2. initialize descriptor
        strcpy(tfd->node_name, fname);
        tfd->id = i;
        timespec_t t;
        kclock_gettime (CLOCK_REALTIME, &t);
        tfd->tc = tfd->ta = tfd->tm = t;
        tfd->size = 0;
        tfd->blocks = 0;
    }

    //create kobject and a new struct kfile_desc
    kobject_t *kobj = kmalloc_kobject(sizeof(struct kfile_desc));
    kobj->flags = KTYPE_FILE;
    fd = kobj->kobject;
    fd->tfd = tfd;
    fd->flags = flags;
    fd->fp = 0;
    fd->id = k_new_id ();
    list_append(&open_files, fd, &fd->list);

    //fill desc
    desc->id = fd->id;
    desc->ptr = kobj;

    return 0;
}

int k_fs_close_file(descriptor_t *desc)
{
    struct kfile_desc *fd = last_check;
    kobject_t *kobj;

    kobj = desc->ptr;
    /* - already tested!
    if (!kobj || !list_find(&kobjects, &kobj->list))
        return -EINVAL;

    fd = kobj->kobject;
    if (!fd || fd->id != desc->id)
        return -EINVAL;
    */
    if (!list_find_and_remove(&open_files, &fd->list))
        return -EINVAL;

    kfree_kobject ( kobj );

    return 0;
}

int k_fs_read_write(descriptor_t *desc, void *buffer, size_t size, int op)
{
    //desc already checked, use "last_check";
    struct kfile_desc *fd = last_check;

    //sanity check
    if ((op && (fd->flags & O_WRONLY)) || (!op && (fd->flags & O_RDONLY)))
        return -EPERM;

    if (op) {
        //read from offset "fd->fp" to "buffer" "size" bytes

        // possible scenarios: (#=block boundary)
        // fp % block_size == 0
        // #|fp|-----------#-----------#
        //     | size |

        // fp % block_size == 0
        // #|fp|-----------#-----------#
        //     |      size     |

        // fp % block_size > 0
        // #----|fp|-------#-----------#
        //         |size|

        // fp % block_size > 0
        // #----|fp|-------#-----------#
        //         |    size    |

        //start with block x where address fd->fp is (x=fp/bsize)
        //fd->tfd->block[x] has address of block on disk
        //read that block and copy data from fd->fp to the end of block
        //DISK_READ(buf, 1, fd->tfd->block[x]);
        //which part of buf to buffer? ...
        //read next block ...
        //stop when all required data is read or end of the file is reached

        //update "ta", fp
        //return number of bytes read


        size_t todo = size;
        size_t newBlock = (fd->fp / ft->block_size);

        size_t target_block = (size + fd->fp - 1) / ft->block_size;

        char *buffer_c = (char *)buffer;
        char newBuff[ft->block_size];
        if (newBlock == target_block) {

            DISK_READ(newBuff, 1, fd->tfd->block[newBlock]);

            int start = fd->fp % ft->block_size;
            int end = (size + fd->fp - 1) % ft->block_size;
            while(start <= end) {
                int i = start;
                *(buffer_c + size - todo) = newBuff[i];
                todo = todo -1;
                start = start + 1;
            }
        } else {

            DISK_READ(newBuff, 1, fd->tfd->block[newBlock]);

            int start = fd->fp % ft->block_size;
            while(start <= ft->block_size) {
                int i = start;
                *(buffer_c + size-todo) = newBuff[i];
                todo = todo - 1;
                start = start + 1;
            }

            newBlock++;

            while (newBlock < target_block) {
                DISK_READ(newBuff, 1, fd->tfd->block[newBlock]);
                int i = 0;
                while(i <= ft ->block_size){
                    *(buffer_c + size-todo) = newBuff[i];
                    todo = todo -1;
                    i = i + 1;
                }

                newBlock++;
            }

            DISK_READ(newBuff, 1, fd->tfd->block[newBlock]);

            int end = (size + fd->fp - 1) % ft->block_size;
            int i = 0;
            while(i <= end) {
                *(buffer_c + size-todo) = newBuff[i];
                todo = todo -1;
                i = i +1;

            }
        }

        return size - todo;
    }
    else {
        //assume there is enough space on disk

        //write ...
        //if ...->block[x] == 0 => find free block on disk
        //when fp isn't block start, read block from disk first
        //and then replace fp+ bytes ... and then write block back


        size_t todo = size;
        size_t newBlock = fd->fp / ft->block_size;

        char *buffer_c = (char *)buffer;
        char newBuffer[ft->block_size];

        size_t target_block = (size + fd->fp - 1) / ft->block_size;

        if (newBlock == target_block) {

            int start = fd->fp % ft->block_size;
            int end = (size + fd->fp - 1) % ft->block_size;
            DISK_READ(newBuffer, 1, fd->tfd->block[newBlock]);
            while(start <= end) {
                int i = start;
                newBuffer[i] = *(buffer_c + size - todo);
                todo = todo -1;
                start = start + 1;
            }

            DISK_WRITE(newBuffer, 1, newBlock);

        } else {
            DISK_READ(newBuffer, 1, fd->tfd->block[newBlock]);

            int start = fd->fp % ft->block_size;
            while(start <= ft->block_size) {
                int i = start;
                newBuffer[i] = *(buffer_c + size - todo);
                todo = todo - 1;
                start = start + 1;
            }
            DISK_WRITE(newBuffer, 1, newBlock);

            newBlock++;

            while (newBlock < target_block) {
                DISK_READ(newBuffer, 1, fd->tfd->block[newBlock]);
                int i = 0;
                while(i <= ft ->block_size){
                    newBuffer[i] = *(buffer_c + size - todo);
                    todo = todo -1;
                    i = i + 1;
                }
                DISK_WRITE(newBuffer, 1, newBlock);

                newBlock++;
            }

            DISK_READ(newBuffer, 1, fd->tfd->block[newBlock]);

            int end = (size + fd->fp - 1) % ft->block_size;
            int i = 0;
            while(i <= end) {
                newBuffer[i] = *(buffer_c + size - todo);
                todo = todo -1;
                i = i +1;

            }
            DISK_WRITE(newBuffer, 1, newBlock);
        }

        return size - todo;
    }

}

int k_fs_read_wipe(descriptor_t *desc)
{
    //desc already checked, use "last_check";
    struct kfile_desc *fd = last_check;

    //sanity check
    //if ((op && (fd->flags & O_WRONLY)) || (!op && (fd->flags & O_RDONLY)))
    //	return -EPERM;

    if (1) {
        //read from offset "fd->fp" to "buffer" "size" bytes

        // possible scenarios: (#=block boundary)
        // fp % block_size == 0
        // #|fp|-----------#-----------#
        //     | size |

        // fp % block_size == 0
        // #|fp|-----------#-----------#
        //     |      size     |

        // fp % block_size > 0
        // #----|fp|-------#-----------#
        //         |size|

        // fp % block_size > 0
        // #----|fp|-------#-----------#
        //         |    size    |

        //start with block x where address fd->fp is (x=fp/bsize)
        //fd->tfd->block[x] has address of block on disk
        //read that block and copy data from fd->fp to the end of block
        //DISK_READ(buf, 1, fd->tfd->block[x]);
        //which part of buf to buffer? ...
        //read next block ...
        //stop when all required data is read or end of the file is reached

        //update "ta", fp
        //return number of bytes read


        // size_t todo = size;
        size_t newBlock = (fd->fp / ft->block_size);


        // size_t target_block = (size + fd->fp - 1) / ft->block_size;
        char newBuffer[ft->block_size];
        int start = fd->fp % ft->block_size;
        int end = (6 + fd->fp - 1) % ft->block_size;
        DISK_READ(newBuffer, 1, fd->tfd->block[newBlock]);
        while(start <= end) {
            int i = start;
            newBuffer[i] = 'x';
            start = start + 1;
        }


        DISK_WRITE(newBuffer, 1, newBlock);

        // return size - todo;
        return 1;


    }

}