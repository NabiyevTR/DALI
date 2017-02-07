#include <Dali.h>


const int DALI_TX = 3;
const int DALI_RX_A = 0;



#define BROADCAST_DP 0b11111110
#define BROADCAST_C 0b11111111
#define ON_DP 0b11111110
#define OFF_DP 0b00000000
#define ON_C 0b00000101
#define OFF_C 0b00000000
# define QUERY_STATUS 0b10010000
# define RESET 0b00100000

void setup() {

  Serial.begin(74880);
  dali.setupTransmit(DALI_TX);
  dali.setupAnalogReceive(DALI_RX_A);
  dali.busTest();
  dali.msgMode = true;
  Serial.println(dali.analogLevel);
  help(); //Show help

}


void help() {
  Serial.println("Enter 16 bit command or another command from list:");
  Serial.println("help -  command list");
  Serial.println("on -  broadcast on 100%");
  Serial.println("off -  broadcast off 0%");
  Serial.println("scan -  device short address scan");
  Serial.println("initialise -  start process of initialisation");
  Serial.println();
}


void sinus () {
  uint8_t lf_1_add = 0;
  uint8_t lf_2_add = 1;
  uint8_t lf_3_add = 2;
  uint8_t lf_1;
  uint8_t lf_2;
  uint8_t lf_3;
  int i;
  int j = 0;

  while (Serial.available() == 0) {
    for (i = 0; i < 360; i = i + 1) {

      if (Serial.available() != 0) {
        dali.transmit(BROADCAST_C, ON_C);
        break;
      }

      lf_1 = (int) abs(254 * sin(i * 3.14 / 180));
      lf_2 = (int) abs(254 * sin(i * 3.14 / 180 + 2 * 3.14 / 3));
      lf_3 = (int) abs(254 * sin(i * 3.14 / 180 + 1 * 3.14 / 3));
      dali.transmit(lf_1_add << 1, lf_1);
      delay(5);
      dali.transmit(lf_2_add << 1, lf_2);
      delay(5);
      dali.transmit(lf_3_add << 1, lf_3);
      delay(5);
      delay(20);
    }
  }
}



void testReceive () {



  dali.transmit(0, 254);
  delay(200);
  dali.transmit(1, 0x90);
  Serial.println(dali.receive());




}














void loop() {

  const int delaytime = 500;
  int i;
  int cmd1;
  int cmd2;
  String comMsg;


  // Read command from port

  delay(delaytime);

  while (Serial.available()) {
    comMsg = comMsg + (char)(Serial.read());
  }; // read data from serial

  if (comMsg == "sinus") {
    sinus();
  };

  if (comMsg == "scan") {
    dali.scanShortAdd();
  }; // scan short addresses

  if (comMsg == "on") {
    dali.transmit(BROADCAST_C, ON_C);
  }; // broadcast, 100%

  if (comMsg == "off") {
    dali.transmit(BROADCAST_C, OFF_C);
  }; // broadcast, 0%

  if (comMsg == "initialise" or comMsg == "ini") {
    dali.initialisation();
  }; // initialisation

  if (comMsg == "help") {
    help();
  }; //help

  if (comMsg == "test") {
    testReceive();
  }; //graph



  if (dali.cmdCheck(comMsg, cmd1, cmd2)) {
    dali.transmit(cmd1, cmd2);  // command in binary format: (address byte, command byte)
  }
  delay(delaytime);

};











