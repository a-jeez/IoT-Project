#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <TimeLib.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", 25200);

LiquidCrystal_I2C lcd(0x27,16,2); 

const char* NamaHari[] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
char Format_Jam[] = "00:00";
char Format_Detik[] = "00";
char Format_Tanggal[] = "00-00-2007";
char Format_Tampilan_Jam[24];
char Format_Tampilan_Tanggal[28];
byte state, detik, menit, jam, hari, bulan;
int tahun;

// karakter
byte gembokKiri[] = {
  B00000,
  B00000,
  B00011,
  B00100,
  B01111,
  B01110,
  B01110,
  B01111,
};

byte gembokKanan[] = {
  B00000,
  B00000,
  B11000,
  B00100,
  B11110,
  B01110,
  B01110,
  B11110,
};

byte kalender_1[] = {
  0b11111, // header tebal
  0b10101, // cincin kiri-kanan
  0b11111, // garis pemisah
  0b10000,
  0b10000,
  0b11111,
  0b00000,
  0b00000
};

byte kalender_2[] = {
  0b11111,
  0b10101,
  0b11111,
  0b00001,
  0b00001,
  0b11111,
  0b00000,
  0b00000
};

byte kalender_3[] = {
  0b11111,
  0b10000,
  0b10100,
  0b10000,
  0b10100,
  0b11111,
  0b00000,
  0b00000
};

byte kalender_4[] = {
  0b11111,
  0b00001,
  0b00101,
  0b00001,
  0b00101,
  0b11111,
  0b00000,
  0b00000
};

byte wifiKiri[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00001,
  B00111,
  B11111,
};

byte wifiKanan[] = {
  B00000,
  B00000,
  B00001,
  B00111,
  B11111,
  B11111,
  B11111,
  B11111,
};
void setup(){
  
  // inisialisasi lcd.
  lcd.init(); 
  lcd.backlight();
  lcd.clear();
  lcd.setContrast(75);

  // inisialisasi karakter
  lcd.createChar(0, gembokKiri);
  lcd.createChar(1, gembokKanan);
  lcd.createChar(2, kalender_1);
  lcd.createChar(3, kalender_2);
  lcd.createChar(4, kalender_3);
  lcd.createChar(5, kalender_4);
  lcd.createChar(6, wifiKiri);
  lcd.createChar(7, wifiKanan);
  // inisialisasi wifi.
  WiFi.begin("Ajiz", "AzizAziz2007");
  lcd.setCursor(2, 0);
  lcd.print("Connecting to");

  int state_counter = 0;
  while(WiFi.status() != WL_CONNECTED) {
    delay(1500);
    state_counter++;
    lcd.setCursor(2, 1);
    switch(state_counter) {
      case 0: lcd.clear(), lcd.print("ajiz's home");
      case 1: lcd.clear(),lcd.print("ajiz's home.");
      case 2: lcd.clear(),lcd.print("ajiz's home..");
      case 3: lcd.clear(),lcd.print("ajiz's home...");
    }
  }
   
  // inisialisasi timeclient.
  timeClient.begin();

}

void loop(){
  timeClient.update();
  unsigned long data_waktu = timeClient.getEpochTime();
  int Hari = timeClient.getDay();
  detik = second(data_waktu);

  if(state != detik) 
  {
    hari = day(data_waktu);
    bulan = month(data_waktu);
    tahun = year(data_waktu);
    menit = minute(data_waktu);
    jam = hour(data_waktu);

    Format_Detik[1] = detik % 10 + 48;
    Format_Detik[0] = detik / 10 + 48;
    Format_Jam[4] = menit % 10 + 48;
    Format_Jam[3] = menit / 10 + 48;
    Format_Jam[1] = jam % 10 + 48;
    Format_Jam[0] = jam / 10 + 48;

    Format_Tanggal[0] = hari / 10 + 48;
    Format_Tanggal[1] = hari % 10 + 48;
    Format_Tanggal[3] = bulan / 10 + 48;
    Format_Tanggal[4] = bulan % 10 + 48;
    Format_Tanggal[8] = (tahun / 10) % 10 + 48;
    Format_Tanggal[9] = tahun % 10 % 10 + 48; 

    if(detik < 6) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(2));
      lcd.write(byte(3));
      lcd.setCursor(0, 1);
      lcd.write(byte(4));
      lcd.write(byte(5));
      lcd.setCursor(3, 0);
      lcd.print(NamaHari[Hari]);
      lcd.setCursor(3, 1);
      lcd.print(Format_Tanggal);
    } else {
      lcd.clear();
      lcd.setCursor(4, 0);
      String stringJam = String(Format_Jam) + ":" + Format_Detik;
      stringJam.toCharArray(Format_Tampilan_Jam, sizeof(Format_Tampilan_Jam));
      lcd.print(Format_Tampilan_Jam);

      lcd.setCursor(6, 1);
      lcd.write(byte(0));
      lcd.write(byte(1));
      lcd.write(byte(6));
      lcd.write(byte(7));
    }
  }
  delay(500);
}
