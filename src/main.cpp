#include "mbed.h"

Ticker ticker;
DigitalOut led1(LED1);
DigitalOut led2(LED2);

// CAN can(p30, p29);
CAN can(p9, p10);

char counter = 0;
bool msg_rx_flag = 0;
char sendPayload[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

void send() {

  int rc =
      can.write(CANMessage(0x00000000, sendPayload, 8, CANData, CANExtended));
  printf("[-INFO-] CAN Message sent. [rc] %d \n\r", rc);
}

void can_interrupt() {
  can.attach(NULL); // Disable the interrupt - the application must re-anable
                    // the interrupt after it has acted upon it
  msg_rx_flag = 1;  // Set a 'Flag' to say that there is an interrupt that needs
                    // to be processed
}

void blink() {
  led1 = 1;
  wait(1);
  led1 = 0;
  wait(1);
}

int main() {
  printf("[DEBUG LOG] Main loop started. \n\r");
  // ticker.attach(&send, 3);
  CANMessage msg;

  can.mode(CAN::Normal);
  can.frequency(500000); // Initialise mbed CAN with a baudrate of 500 kbps

  while (1) {

    msg_rx_flag = 1; // This is due to the mbed bug
                     // https://github.com/ARMmbed/mbed-os/issues/56 once a
                     // proper fix this can be removed.

    // if(msg_rx_flag == 1 || true){

    if (can.read(msg)) {
      printf("Message received: %d\n\r", msg.data[0]);
      led2 = !led2;
    } else {
      // printf("Message Not loaded \n\r");
    }

    // can.attach(&can_interrupt,CAN::RxIrq);
    msg_rx_flag = 0;
    // wait(0.2);
    send();
    wait(0.2);
    blink();
    // }
  }
}
