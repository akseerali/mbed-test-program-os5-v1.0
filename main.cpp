#include "mbed.h"
#include "MMA7660.h"
#include "C12832.h"
#include "LM75B.h"

Serial pc(USBTX, USBRX);
C12832 lcd(p5, p7, p6, p8, p11);
MMA7660 MMA(p28, p27);
PwmOut Buzzer(p26);
AnalogIn Potentiometer1(p19);
AnalogIn Potentiometer2(p20);
InterruptIn Joystick_Down(p12);
InterruptIn Joystick_Left(p13);
InterruptIn Joystick_Center(p14);
InterruptIn Joystick_Up(p15);
InterruptIn Joystick_Right(p16);
DigitalOut Red_Led(p23);
DigitalOut Green_Led(p24);
DigitalOut Blue_Led(p25);
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);
DigitalOut connectionLed(LED1);

Ticker flipper;
EventQueue *queue = mbed_event_queue(); // event queue

int x=0,y=0;
float temp, pot1, pot2;
int j_up = 0, j_down = 0, j_left = 0, j_right = 0, j_center = 0;
bool flag = true;

void tickerCallback();
void bubble();
void update_non_interrupt_serial();
void interrupt_up();
void interrupt_down();
void interrupt_left();
void interrupt_right();
void interrupt_center();
void joystick_Red_led();
void joystick_Green_led();
void joystick_Blue_led();
void joystick_White_led();
void sound();

int main()
{
    pc.baud(9600);
    Joystick_Up.rise(&interrupt_up);
    Joystick_Down.rise(&interrupt_down);
    Joystick_Left.rise(&interrupt_left);
    Joystick_Right.rise(&interrupt_right);
    Joystick_Center.rise(&interrupt_center);

    flipper.attach(&tickerCallback, 5);
    lcd.cls();
    
    while(1) 
    {
        bubble();
    }
}

void tickerCallback() 
{
    queue->call(&update_non_interrupt_serial);
} 

void bubble()
{    
    //read X,Y +/-Gs and scale for #display pixels
    x = (x + MMA.x() * 32.0)/2.0;
    y = (y -(MMA.y() * 16.0))/2.0;
    lcd.fillcircle(x+103, y+15, 3, 1); //draw bubble
    lcd.circle(103, 15, 8, 1);
    wait_us(100000); //time delay
    lcd.fillcircle(x+103, y+15, 3, 0); //erase bubble
}

void update_non_interrupt_serial()
{
    LM75B sensor(p28,p27);
    temp = sensor.read();
    MMA7660 MMA(p28, p27);

    led1 = !led1;
    led2 = !led2;
    led3 = !led3;
    led4 = !led4;

    pot1 = Potentiometer1.read();
    pot2 = Potentiometer2.read();

    pc.printf("Accelerometer value = {(x,%d), (y,%d)}   ", x, y);
    pc.printf("On-board temperature = %.3f   ", (float)temp);
    pc.printf("Potentiometers value = {(Pot-1 = %.3f), (Pot-2 = %.3f)}   ", (float)pot1, (float)pot2);
    pc.printf("Buzzer status = %d   ", j_center);
    pc.printf("Leds status = {(LED1 = %d), (LED2 = %d), (LED3 = %d), (LED4 = %d), (Red = %d), (Green = %d), (Blue = %d)} \n\n", led1.read(), led2.read(), led3.read(), led4.read(), Red_Led.read(), Green_Led.read(), Blue_Led.read());
}

void interrupt_up()
{
    j_up = !j_up;
    queue->call(&joystick_Red_led);
}
void interrupt_down()
{
    j_down = !j_down;
    queue->call(&joystick_White_led);
}
void interrupt_left()
{
    j_left = !j_left;
    queue->call(&joystick_Blue_led);
    
}
void interrupt_right()
{
    j_right = !j_right;
    queue->call(&joystick_Green_led);
}
void interrupt_center()
{
    j_center = !j_center;
    queue->call(&sound);
}

void joystick_Red_led()
{
    Red_Led = !Red_Led;
    pc.printf("Joystick = {(up,%d), (down,%d), (left,%d), (right,%d), (center,%d)} \n\n",
        j_up, 
        j_down, 
        j_left, 
        j_right,
        j_center);
}

void joystick_Green_led()
{
    Green_Led = !Green_Led;
    pc.printf("Joystick = {(up,%d), (down,%d), (left,%d), (right,%d), (center,%d)} \n\n",
        j_up, 
        j_down, 
        j_left, 
        j_right,
        j_center);
}

void joystick_Blue_led()
{
    Blue_Led = !Blue_Led;
    pc.printf("Joystick = {(up,%d), (down,%d), (left,%d), (right,%d), (center,%d)} \n\n",
        j_up, 
        j_down, 
        j_left, 
        j_right,
        j_center);
}

void joystick_White_led()
{
    if(flag == true)
    {
        Red_Led = 0;
        Green_Led = 0;
        Blue_Led = 0;
        flag = !flag;
    }
    else
    {
        Red_Led = 1;
        Green_Led = 1;
        Blue_Led = 1;
        flag = !flag;
    }
    pc.printf("Joystick = {(up,%d), (down,%d), (left,%d), (right,%d), (center,%d)} \n\n",
        j_up, 
        j_down, 
        j_left, 
        j_right,
        j_center);
}

void sound()
{
    if(j_center == 1)
    {
        Buzzer.period(1.0/700.0);
        Buzzer = 0.5;
    }
    else 
    {
        Buzzer=0;  
    }
}