/*-------------------------------------------------------------
Bee Monitor V2.0 
ArduinoRx - Receive data via RF, decod and print to serial - Arduino connected to web-connected Raspberry Pi via USB serial

Glyn Hudson 10/4/12
OpenEnergyMonitor.org
BeeMonitor.org

GNU GPL V3

//RF module: http://openenergymonitor.org/emon/buildingblocks/simple-rf-link
--------------------------------------------------------------*/
#include <VirtualWire.h> // library for RF RX/TX

int variableA, variableB, variableC, variableD, variableE;
float A,B,C,D,E;

char cArray[30];
int ic=0;

const int nodeID=11;
const int LED_PIN=13;




int readDataRF()
{
   int done = 0;
   int inByte;
  
   uint8_t buf[VW_MAX_MESSAGE_LEN];
   uint8_t buflen = VW_MAX_MESSAGE_LEN;
   
   if (vw_get_message(buf, &buflen))
   {
      for (int i = 0; i < buflen; i++)
      { 
         inByte = (char)buf[i];
         
         //--------------------------------------------------
         // On recieve of variables
         //--------------------------------------------------
         if (inByte=='A') 
         {
            variableA = atoi(cArray);      //String to float
         }
         
         if (inByte=='B') 
         {
            variableB = atoi(cArray);      //String to float
         }
         
          if (inByte=='C') 
         {
            variableC = atoi(cArray);      //String to float
         }
         
          if (inByte=='D') 
         {
            variableD = atoi(cArray);      //String to float
         }
         
            if (inByte=='E') 
         {
            variableE = atoi(cArray);      //String to float
         }

         //--------------------------------------------------
         
         //--------------------------------------------------
         // if character A to Z : reset arrays
         //--------------------------------------------------
         if (inByte>64 && inByte<91) 
         {
            ic=0;
            for(int i=0; i<10; i++) cArray[i] = 0;
         }
         //--------------------------------------------------

         //--------------------------------------------------
         //If digit 0 to 9 .. or decimal point (46) or negative (45)
         //--------------------------------------------------
         if ((inByte>47 && inByte<58) || inByte==46 || inByte==45 )   
         {
            //add to character array
            cArray[ic] = inByte; ic++;
         }
         //--------------------------------------------------
      }
      done = 1;
   }
   //Return status
   return done;
}




void setup()
{
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(9600);

      //---------------------------------------------------------
  // Reciever setup
  //---------------------------------------------------------
  vw_set_ptt_inverted(true);          // Required for DR3100
  vw_setup(2000);	              	// Bits per sec
  vw_set_rx_pin(7);                   //Dig 6
  vw_rx_start();                      // Start 
  //---------------------------------------------------------

    digitalWrite(LED_PIN, HIGH);
    Serial.println("Bee Monitor Rx");
    delay(1000);
    digitalWrite(LED_PIN, LOW);
   
}

void loop()
{
 
    
if (readDataRF())
  {
    digitalWrite(LED_PIN,HIGH);
    /*A=variableA/100.0;
    B=variableB/100.0;
    C=variableC/100.0;
    D=variableD/100.0;
    E=variableE/100.0;*/

    //write data to serial in formatt OEM_gateway python script can understand nodeID Val1 Val2 etc. 
    Serial.print(nodeID);    Serial.print(" "); 
    Serial.print(variableA); Serial.print(" ");
    Serial.print(variableB); Serial.print(" ");
    Serial.print(variableC); Serial.print(" ");
    Serial.print(variableD); Serial.print(" ");
    Serial.println(variableE); 

  delay(1000);
  digitalWrite(LED_PIN,LOW);  
  } // end if RF

}
