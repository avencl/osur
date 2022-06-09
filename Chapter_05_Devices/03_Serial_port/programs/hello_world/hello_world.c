/*! Hello world program */

#include <stdio.h>
#include <api/prog_info.h>
#include <api/malloc.h>
#include <arch/interrupt.h>
#include <arch/processor.h>

int hello_world()
{
    printf("Example program: [%s:%s]\n%s\n\n", __FILE__, __FUNCTION__,
           hello_world_PROG_HELP);

    printf("Hello World!\n");

    void *ptr1, *ptr2, *ptr3, *ptr4, *ptr5, *ptr6, *ptr7, *ptr8, *ptr9, *ptr10, *ptr11, *ptr12, *ptr13, *ptr14, *ptr15, *ptr16, *ptr17, *ptr18, *ptr19, *ptr20;

    printf("\n\n\n\n\n");


    ptr1 = malloc(10);
    printf("adresa od ptr1 je %d\n", ptr1);
    ptr2 = malloc(10);
    printf("adresa od ptr2 je %d\n", ptr2);
    ptr3 = malloc(10);
    printf("adresa od ptr3 je %d\n", ptr3);
    ptr4 = malloc(10);
    printf("adresa od ptr4 je %d\n", ptr4);
    ptr5 = malloc(10);
    printf("adresa od ptr1 je %d\n", ptr5);
    ptr6 = malloc(10);
    printf("adresa od ptr2 je %d\n", ptr6);
    ptr7 = malloc(10);
    printf("adresa od ptr3 je %d\n", ptr7);
    ptr8 = malloc(10);
    printf("adresa od ptr4 je %d\n", ptr8);
    ptr9 = malloc(10);
    printf("adresa od ptr1 je %d\n", ptr9);
    ptr10 = malloc(10);
    printf("adresa od ptr2 je %d\n", ptr10);
    ptr11 = malloc(10);
    printf("adresa od ptr3 je %d\n", ptr11);
    ptr12 = malloc(10);
    printf("adresa od ptr4 je %d\n", ptr12);
    ptr13 = malloc(10);
    printf("adresa od ptr1 je %d\n", ptr13);
    ptr14 = malloc(10);
    printf("adresa od ptr2 je %d\n", ptr14);
    ptr15 = malloc(10);
    printf("adresa od ptr3 je %d\n", ptr15);
    ptr16 = malloc(10);
    printf("adresa od ptr4 je %d\n", ptr16);
    ptr17 = malloc(10);
    printf("adresa od ptr1 je %d\n", ptr17);
    ptr18 = malloc(10);
    printf("adresa od ptr2 je %d\n", ptr18);
    ptr19 = malloc(10);
    printf("adresa od ptr3 je %d\n", ptr19);
    ptr20 = malloc(10);
    printf("adresa od ptr4 je %d\n", ptr20);

    free(ptr1);
    free(ptr2);
    free(ptr3);
    free(ptr4);
    free(ptr5);
    free(ptr6);
    free(ptr7);
    free(ptr8);
    free(ptr9);
    free(ptr10);
    free(ptr11);


#if 0	/* test escape sequence */
    printf("\x1b[20;40H" "Hello World at 40, 20!\n");
#endif

    return 0;
}