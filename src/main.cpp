#include "mbed.h"

DigitalOut led1(LED1);
CAN can(p9, p10);
CANMessage msg;
volatile bool msg_rx_flag = 0;

void can_interrupt() {
  can.attach(NULL);
  msg_rx_flag = 1;
}

int main() {
  printf("[DEBUG LOG] Main loop started. \n\r");

  can.mode(CAN::Normal);
  can.frequency(100000);  // Initialise mbed CAN with a baudrate of 100 kbps
  can.attach(callback(&can_interrupt), CAN::RxIrq);

  while (1) {
    if (msg_rx_flag == 1) {
      if (can.read(msg) == 1) {
        printf(
            "Received CAN message | id: %x, data: [0] %x, [1] %x, "
            "[2] %x, [3] %x, [4] %x, [5] %x, [6] %x, [7] %x \r\n",
            msg.id, msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5], msg.data[6],
            msg.data[7]);
      }
      led1 = !led1;
      msg_rx_flag = 0;
      can.attach(callback(&can_interrupt), CAN::RxIrq);
    }
  }
}
