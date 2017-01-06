#include <LiquidCrystal.h>

#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  lcd.begin(16, 2);

}
void loop() 
{
  // Look for new cards
  /*if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }*/
  // Select one of the cards
  /*if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    lcd.println("Nem olvasott");
    return;
  }*/
  //Show UID on serial monitor
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     lcd.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     lcd.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  /*lcd.println();
  lcd.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "BD 31 15 2B") //change here the UID of the card/cards that you want to give access
  {
    lcd.println("Authorized access");
    lcd.println();
    delay(3000);
  }
 
 else   {
    lcd.println(" Access denied");
    delay(3000);
  }*/
} 
