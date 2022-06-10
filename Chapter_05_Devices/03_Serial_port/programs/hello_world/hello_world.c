/*! Hello world program */

#include <stdio.h>
#include <api/prog_info.h>

int hello_world()
{
    printf("Example program: [%s:%s]\n%s\n\n", __FILE__, __FUNCTION__,
           hello_world_PROG_HELP);

    printf("Hello World!\n");

    int fd = open("file:testaa", O_CREAT | O_WRONLY, 0);
    printf("fd=%d\n", fd);
    int retval = write(fd, "asdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklwasdfghjklw", 700);
    printf("retval=%d\n", retval);
    retval = close(fd);
    printf("retval=%d\n", retval);

    fd = open("file:testaa", O_RDONLY, 0);
    printf("fd=%d\n", fd);
    char buff[700];
    retval = read(fd, buff, 700);
    printf("retval=%d\n", retval);
    printf("buff=%c\n", *(buff + 200));



#if 0	/* test escape sequence */
    printf("\x1b[20;40H" "Hello World at 40, 20!\n");
#endif

    return 0;
}