#include <stdio.h>
#include <time.h>
#include <api/prog_info.h>
#include <arch/processor.h>
#include <arch/interrupt.h>

//void higher_prio_handler() {
//  printf("HIGH PRIO START\n");
//for (int i = 0; i < 1000000000; i++);
//printf("HIGH PRIO END\n");
//}

//void lower_prio_handler() {
//  printf("LOW PRIO START\n");
// for (int i = 0; i < 1000000000; i++);
// printf("LOW PRIO END\n");
//}

//void lower_prio_raise() {
//  raise_interrupt(38);
//}

//void higher_prio_raise() {
//  raise_interrupt(30);
//}

void raise_X_prio() {
    raise_interrupt(30);
}

void x_prio_handler() {
    printf("X PRIO START\n");
    for (int i = 0; i < 1000000000; i++);
    printf("X PRIO END\n");
}

void raise_Y_prio() {
    raise_interrupt(38);
}

void y_prio_handler() {
    printf("Y PRIO START\n");
    for (int i = 0; i < 1000000000; i++);
    printf("Y PRIO END\n");
}

void raise_Z_prio() {
    raise_interrupt(40);
}

void z_prio_handler() {
    printf("Z PRIO START\n");
    for (int i = 0; i < 1000000000; i++);
    printf("Z PRIO END\n");
}

void raise_W_prio() {
    raise_interrupt(42);
}

void w_prio_handler() {
    printf("W PRIO START\n");
    for (int i = 0; i < 1000000000; i++);
    printf("W PRIO END\n");
}

int interrupt() {
    // arch_register_interrupt_handler(30, higher_prio_handler, NULL);
    // arch_register_interrupt_handler(38, lower_prio_handler, NULL);
    arch_register_interrupt_handler(30, x_prio_handler, NULL);
    arch_register_interrupt_handler(38, y_prio_handler, NULL);
    arch_register_interrupt_handler(40, z_prio_handler, NULL);
    arch_register_interrupt_handler(42, w_prio_handler, NULL);

    sigevent_t evpX, evpY, evpZ, evpW;
    timer_t timerX, timerY, timerZ, timerW;
    itimerspec_t tX, tY, tZ, tW;
    timespec_t ts;

    /*evp.sigev_notify = SIGEV_THREAD;
    evp.sigev_notify_function = lower_prio_raise;
    evp.sigev_notify_attributes = NULL;

    t.it_interval.tv_sec = 0;
    t.it_interval.tv_nsec = 0;
    t.it_value.tv_sec = 3;
    t.it_value.tv_nsec = 0;

    timer_create ( CLOCK_REALTIME, &evp, &timer );
    timer_settime ( &timer, 0, &t, NULL );

    evp2.sigev_notify = SIGEV_THREAD;
    evp2.sigev_notify_function = higher_prio_raise;
    evp2.sigev_notify_attributes = NULL;

    t2.it_interval.tv_sec = 0;
    t2.it_interval.tv_nsec = 0;
    t2.it_value.tv_sec = 4;
    t2.it_value.tv_nsec = 0;

    timer_create ( CLOCK_REALTIME, &evp2, &timer2 );
    timer_settime ( &timer2, 0, &t2, NULL ); */

    evpY.sigev_notify = SIGEV_THREAD;
    evpY.sigev_notify_function = raise_Y_prio;
    evpY.sigev_notify_attributes = NULL;

    tY.it_interval.tv_sec = 0;
    tY.it_interval.tv_nsec = 0;
    tY.it_value.tv_sec = 5;
    tY.it_value.tv_nsec = 0;

    timer_create ( CLOCK_REALTIME, &evpY, &timerY );
    timer_settime ( &timerY, 0, &tY, NULL );

    evpX.sigev_notify = SIGEV_THREAD;
    evpX.sigev_notify_function = raise_X_prio;
    evpX.sigev_notify_attributes = NULL;

    tX.it_interval.tv_sec = 0;
    tX.it_interval.tv_nsec = 0;
    tX.it_value.tv_sec = 6;
    tX.it_value.tv_nsec = 0;

    timer_create ( CLOCK_REALTIME, &evpX, &timerX );
    timer_settime ( &timerX, 0, &tX, NULL );



    evpZ.sigev_notify = SIGEV_THREAD;
    evpZ.sigev_notify_function = raise_Z_prio;
    evpZ.sigev_notify_attributes = NULL;

    tZ.it_interval.tv_sec = 0;
    tZ.it_interval.tv_nsec = 0;
    tZ.it_value.tv_sec = 4;
    tZ.it_value.tv_nsec = 0;

    timer_create ( CLOCK_REALTIME, &evpZ, &timerZ );
    timer_settime ( &timerZ, 0, &tZ, NULL );

    evpW.sigev_notify = SIGEV_THREAD;
    evpW.sigev_notify_function = raise_W_prio;
    evpW.sigev_notify_attributes = NULL;

    tW.it_interval.tv_sec = 0;
    tW.it_interval.tv_nsec = 0;
    tW.it_value.tv_sec = 3;
    tW.it_value.tv_nsec = 0;

    timer_create ( CLOCK_REALTIME, &evpW, &timerW );
    timer_settime ( &timerW, 0, &tW, NULL );

    ts.tv_sec = 30;

    while ( TIME_IS_SET (&ts) )
        clock_nanosleep ( CLOCK_REALTIME, 0, &ts, &ts );

    return 0;
} 