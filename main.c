/**
 * main.c
 *
 * Main file for CprE 288 Lab 5
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 *
 * @author Phillip Jones, updated 6/4/2019
 * @author Diane Rover, updated 2/25/2021, 2/17/2022
 * 
 * @author Lucas England
 */

#include "movement.h"
#include "button.h"
#include "timer.h"
#include "lcd.h"

#include "uart-interrupt.h"

#include "cyBot_Scan.h"  // Scan using CyBot servo and sensors
#include "scan_objects.h"
#include "adc.h"

#define PI 3.1415926535897
#define LINEAR_WIDTH(o) ((o.width * PI * o.dist) / 180.0)
#define SPEED (MAX_SPEED / 4)

int main(void)
{
    button_init();
    timer_init(); // Must be called before lcd_init(), which uses timer functions
    lcd_init();

    // initialize the cyBot UART1 before trying to use it
    uart_interrupt_init();

adc_init();

//  // (Uncomment ME for UART init part of lab)
//    cyBot_uart_init_clean();  // Clean UART1 initialization, before running your UART1 GPIO init code
//
//    // Complete this code for configuring the GPIO PORTB part of UART1 initialization (your UART1 GPIO init code)
//    SYSCTL_RCGCGPIO_R |= 0b000010;
//    while ((SYSCTL_PRGPIO_R & 0b000010) == 0) {};
//    GPIO_PORTB_DEN_R |= 0b000011;
//    GPIO_PORTB_AFSEL_R |= 0b000011;
//    GPIO_PORTB_PCTL_R &= ~0x0000ee; // Force 0's in the desired locations
//    GPIO_PORTB_PCTL_R |= 0x000011; // Force 1's in the desired locations
//    // Or see the notes for a coding alternative to assign a value to the PCTL field
//
//    // (Uncomment ME for UART init part of lab)
//    cyBot_uart_init_last_half();  // Complete the UART device configuration

    // Initialize the scan
    cyBOT_init_Scan(0b0111);
    // Remember servo calibration function and variables from Lab 3
    right_calibration_value = 290500;
    left_calibration_value = 1288000;

    object_t objs[MAX_OBJS];
//    int i, smallest_i = 0;
//    float smallest_width;
    size_t objc;
    int timeout, smallest_i;
    float distleft;
    oi_t *bump = oi_alloc();
    oi_init(bump);

    while (1)
    {
        uint16_t adc_value = adc_read(); // change adc_read() to adc_read_average()
        // add float dist = adc_getdistance();
        lcd_printf("adc: %u"), adc_value);//,adc_value,dist); for part 2 plus change formatting to add dist:
        timer_waitMillis(200);
    }
}
//        char cmd = uart_receive();
//        // echo the received key back to PC
//        uart_sendChar(cmd);
//        // uart_sendChar('\n');
//        // uart_sendf("Received a '%c'\r\n", cmd);

        if (command_flag)
        {
            char cmd = command_byte;
            command_flag = 0;

            switch (cmd)
            {
            case CMD_SERVO_CAL:
                cyBOT_SERVO_cal();
                break;
            case CMD_SWEEP_IR:
                objc = sweep_ir(objs);
                if (objc <= 0)
                    break;
                smallest_i = get_smallest_i(objc, objs);
                distleft = objs[smallest_i].dist * 6;
                uart_printf(
                        "navigating towards object %d (angle: %d, distance: %f)\n",
                        smallest_i, MIDPOINT(objs[smallest_i]), distleft);
                if (MIDPOINT(objs[smallest_i]) > 90)
                {
                    uart_printf("turning %d degrees counterclockwise\n",
                    MIDPOINT(objs[smallest_i]) - 90);
                    turn_ccw(bump, MIDPOINT(objs[smallest_i]) - 90, SPEED / 4);
                }
                else if (MIDPOINT(objs[smallest_i]) < 90)
                {
                    uart_printf("turning %d degrees clockwise\n",
                                90 - MIDPOINT(objs[smallest_i]));
                    turn_cw(bump, 90 - MIDPOINT(objs[smallest_i]), SPEED / 4);
                }
                for (timeout = 0; distleft > 10 && timeout < 10; ++timeout)
                {
                    distleft = move_forward(bump, distleft, SPEED);
                    uart_printf("%f\n", distleft);
                    if (bump->bumpLeft)
                    {
                        distleft += 25 - move_backward(bump, 25, SPEED, 1);
                        turn_cw(bump, 90, SPEED / 4);
                        move_forward(bump, 25, SPEED / 2);
                        turn_ccw(bump, 90, SPEED / 4);
                    }
                    else if (bump->bumpRight)
                    {
                        distleft += 50 - move_backward(bump, 50, SPEED, 1);
                        turn_ccw(bump, 90, SPEED / 4);
                        move_fwd(bump, 100, SPEED / 2);
                        turn_cw(bump, 90, SPEED / 4);
                    }
                }
                break;

            default:
                break;
            }
        }
    }
}
