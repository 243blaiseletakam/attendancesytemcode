
/*par blaise letakamba 
 * 
 *  code serveur du projet attendance
 *                         based system. 
 *                         
 *                         programmeur : Blaise Letakamba
 *                         montage : Blaise Letaka et Mukemo Nzanzu Carmelo
 *                         
 */

 
 #include <Wire.h>
 #include <LiquidCrystal_I2C.h>
 #include <String.h>

  #include <SPI.h>
  #include <SD.h>
 
  #include <ESP8266WiFi.h>
  #include "Adafruit_MQTT.h"
  #include "Adafruit_MQTT_Client.h"
 
//#include <ESP8266WebServer.h>

// #include <SoftwareSerial.h>

 #include <DS1302.h>
 /*
  * 
  * #include <EEPROM.h>
  * 
  */

 //#define rx_pin D7
 //#define tx_pin D8

    #define colonnne 20
    #define ligne 4

  // #define boutonmanuel D5 

  #define vitesseCommunication 9600  

   #define chipSelect D8

  
   String informationAgent;
  // String infomois [60][31];
    String ptr; 
    
//const char* ssid = "JERRYSON ATTENDANCE AGENT";  // Enter SSID here
//const char* password = "1234567891";  //Enter Password here

//IPAddress local_ip(192,168,2,1);
//IPAddress gateway(192,168,2,1);
//IPAddress subnet(255,255,255,0);

// SoftwareSerial datanameRFID (D7 , D8);

   LiquidCrystal_I2C lcd(0x27, colonnne , ligne);
    DS1302 rtc(D1, D0, D2);


    #define WLAN_SSID       "JERRYSON RECEPTION"
    #define WLAN_PASS       "reception@@2021"

    #define AIO_SERVER      "io.adafruit.com"
    #define AIO_SERVERPORT  1883                  
    #define AIO_USERNAME    "blaiseletak"
    #define AIO_KEY         "aio_umJs371ki3HFjbbNRmbn26mdnKtU"

    WiFiClient client;
    
    Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
    
    Adafruit_MQTT_Publish   agent = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/agentEtat");
    // Adafruit_MQTT_Publish agentnoactif = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/agentNonActif");
      
//
//ESP8266WebServer server(80);

int  LEDpin = 2;
bool LEDstatus = LOW;

  int presence[55]; 

   String heure_arrive[55];
    String heure_sortie[55]; 

     String jourdelasemaine_arrivee [55];
     String jourdelasemaine_sortie [55];

     
    
     int temps_moi_presence[55];

      String dateactuelle ;
       String heureactuelle ;

        void enregistrementdatainsdcard (String nomfichier , String nom , String date_ , String temps , String etat ,String joursemaine) {
          File dataFile = SD.open(nomfichier, FILE_WRITE);
           if (dataFile) {
            dataFile.print (nom); 
             dataFile.print (" : "); 
              dataFile.print (etat);
              
               if (etat.equals("SORTIE")) {
                   dataFile.print (" ");
               }
                dataFile.print(" | "); 
                
             dataFile.print(joursemaine);
              dataFile.print(",");
               dataFile.print(date_);
                dataFile.print(" | "); 
                 dataFile.println(temps);
                 dataFile.close();

                 lcd.setCursor (0,2);
                 lcd.print ("SD CARD             ");
                 
                 lcd.setCursor (0,3);
                  lcd.print ("ENREGISTREMENT");
                   lcd.setCursor (14,3);
                   lcd.print (".");
                   delay (400); 
                   lcd.setCursor (15,3);
                   lcd.print (".");
                   delay (400); 
                   lcd.setCursor (16,3);
                   lcd.print (".");
                   delay (400); 
                   lcd.setCursor (17,3);
                   lcd.print (".");
                   delay (400); 
                   lcd.setCursor (18,3);
                   lcd.print (".");
                   delay (400); 
                   lcd.setCursor (19,3);
                   lcd.print (".");
                   delay (500);              
                    lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                   
                                
        } else {

          lcd.setCursor (0,2);
         lcd.print ("SD CARD             ");
            lcd.setCursor (0,3);
           lcd.print ("ERREUR STOCKAGE.....");
           delay (2000);

          lcd.setCursor (0,2);
           lcd.print("                    ");
            lcd.setCursor (0,3); 
          lcd.print ("                    ");
           
        }
    }

      void sautdeligne (String nomfichier_) {
         File dataFile = SD.open(nomfichier_, FILE_WRITE);
          if (dataFile) {
            dataFile.println(" ");
           // dataFile.println(" ");
            dataFile.close();         
          } else {
            /*
             * 
           * lcd.setCursor (0,2);
           * lcd.print ("ERREUR STOCKAGE");
           */    
          }
      }
        

//        void enregistrementheure (String heureeffectuerparagent) {
//          
//        }

         void initialisationheuredebutmois(){ 
               int i=0;
                if ((heureactuelle.equals("00:00:00") || heureactuelle.equals("00:00:01"))  && (dateactuelle.substring(0,2)=="01")){
                       while (i<55) {
                           temps_moi_presence[i]=0; 
                           delay(3);
                           i++;   
                       }
                    }
                 }

//       void  affichagedatainwebsite ( int infoagent , String moiencours) {
//         int heure , minute_; /*String heure_string , minute_string*/ ;  
//         ptr += "<td>";
//           String moiencours_ = rtc.getMonthStr(); 
//           if (moiencours_.equals(moiencours)) {
//               if (presence[infoagent] == 1) {
//              /*   
//               ptr += "<button style=background-color:green>ACTIF</button>";  */
//               ptr += "<p><h4><mark>ACTIF</mark></h4></p>";
//                ptr += "<p>H.A:";
//                ptr += heure_arrive[infoagent];
//                ptr +=  "</p>";
//               }
//                if (presence[infoagent] == 0 && heure_sortie[infoagent] !="") {
//                 /*
//                  * ptr += "<button style=background-color:red>NON ACTIF</button>";
//                  */
//
//                    ptr += "<p>NON ACTIF</p>";
//                 ptr += "<p>H.A:";
//                ptr += heure_arrive[infoagent];
//                ptr +=  "</p>";
//                ptr += "<p>H.S:";
//                ptr += heure_sortie[infoagent];
//                ptr += "</p>";           
//                } 
//                 if (presence[infoagent] == 0 && heure_sortie[infoagent] =="") {
//                     /*
//                      *  ptr += "<button style=background-color:red>NON ACTIF</button>";
//                      *  
//                      */
//                      ptr += "<p>NON ACTIF</p>"; 
//                     
//                 }
//                 ptr += "<p><h3>H.M:";
//                 heure = temps_moi_presence[infoagent]/60;
//                 minute_= temps_moi_presence[infoagent]%60;
//                 /*heure_string = heure;
//                 minute_string = minute_;*/
//                /* ptr += "<h5>\n"; */
//                 ptr += heure; 
//                 ptr += "h";
//                 ptr += minute_;
//                 ptr += "</h3></p>";             
//           }     
//        ptr += "</td>";
//       }

        int infojoursemaineenentier (String datajoursemaine) {
              
               if (datajoursemaine.equals ("Monday")) { 
                return 1 ; 
               }
               if (datajoursemaine.equals ("Tuesday")) { 
                return 2 ; 
               }
               if (datajoursemaine.equals ("Wednesday")) { 
                return 3 ; 
               }
               if (datajoursemaine.equals ("Thursday")) { 
                return 4 ; 
               }
               if (datajoursemaine.equals ("Friday")) { 
                return 5 ; 
               }
               if (datajoursemaine.equals ("Saturday")) { 
                return 6 ; 
               }
               if (datajoursemaine.equals ("Sunday")) { 
                return 7 ; 
               }   
        }

        int Stringtointconversion60(String data) {
  
  if(data.equals("01")||data.equals("1")){
    return 1;
    }
      if(data.equals("02")||data.equals("2")){
    return 2;
    }
      if(data.equals("03")||data.equals("3")){
    return 3;
    }
      if(data.equals("04")||data.equals("4")){
    return 4;
    }
     if(data.equals("05")||data.equals("5")){
    return 5;
    }
      if(data.equals("06")||data.equals("6")){
    return 6;
    }
      if(data.equals("07")||data.equals("7")){
    return 7;
    }
      if(data.equals("08")||data.equals("8")){
    return 8;
    }
      if(data.equals("09")||data.equals("9")){
    return 9;
    }
      if(data.equals("10")){
    return 10;
    }
      if(data.equals("11")){
    return 11;
    }
      if(data.equals("12")){
    return 12;
    }
     if(data.equals("13")){
    return 13;
    }
      if(data.equals("14")){
    return 14;
    }
      if(data.equals("15")){
    return 15;
    }
      if(data.equals("16")){
    return 16;
    }
     if(data.equals("17")){
    return 17;
    }
      if(data.equals("18")){
    return 18;
    }
      if(data.equals("19")){
    return 19;
    }
      if(data.equals("20")){
    return 20;
    }
     if(data.equals("21")){
    return 21;
    }
      if(data.equals("22")){
    return 22;
    }
      if(data.equals("23")){
    return 23;
    }
      if(data.equals("24")){
    return 24;
    }
      if(data.equals("25")){
    return 25;
    }
      if(data.equals("26")){
    return 26;
    }
      if(data.equals("27")){
    return 27;
    }
      if(data.equals("28")){
    return 28;
    }
     if(data.equals("29")){
    return 29;
    }
      if(data.equals("30")){
    return 30;
    }
      if(data.equals("31")){
    return 31;
    }
      if(data.equals("32")){
    return 32;
      }
      if(data.equals("33")){
    return 33;
    }
      if(data.equals("34")){
    return 34;
    }
      if(data.equals("35")){
    return 35;
    }
     if(data.equals("36")){
    return 36;
    }
      if(data.equals("37")){
    return 37;
    }
      if(data.equals("38")){
    return 38;
    }
      if(data.equals("39")){
    return 39;
    }
      if(data.equals("40")){
    return 40;
    }
     if(data.equals("41")){
    return 41;
    }
      if(data.equals("42")){
    return 42;
    }
      if(data.equals("43")){
    return 43;
    }
      if(data.equals("44")){
    return 44;
    }
     if(data.equals("45")){
    return 45;
    }
      if(data.equals("46")){
    return 46;
    }
      if(data.equals("47")){
    return 47;
    }
      if(data.equals("48")){
    return 48;
    }
      if(data.equals("49")){
    return 49;
    }
      if(data.equals("50")){
    return 50;
    }
      if(data.equals("51")){
    return 51;
    }
      if(data.equals("52")){
    return 52;
    }
     if(data.equals("53")){
    return 53;
    }
      if(data.equals("54")){
    return 54;
    }
      if(data.equals("55")){
    return 55;
    }
      if(data.equals("56")){
    return 56;
    }

     if(data.equals("57")){
    return 57;
    }
      if(data.equals("58")){
    return 58;
    }
      if(data.equals("59")){
    return 59;
    }
      if(data.equals("60")){
    return 60;
    }
} 

/*
 * 
 *  void envoiedonnee(int adresse,int data){
        EEPROM.put(adresse,data); 
    }
    
 * 
 */
   
 /*
  * int  lecturedonnee(int infopersonneadresse){
            infopersonneadresse = (infopersonneadresse+1)*10; 
            EEPROM.get(infopersonneadresse , temps_moi_presence[infopersonneadresse]);
            return temps_moi_presence[infopersonneadresse];           
     } 

  * 
  */

     /*
      * 
      *  void configurationEEPROM(int capacitee_eeprom) {
           EEPROM.begin(capacitee_eeprom); 
      }
      
      */
     

        int nombrejouragent (String  premierjour , String deuxiemejour){
               int jourbrute ;
                jourbrute = infojoursemaineenentier(deuxiemejour)-infojoursemaineenentier(premierjour); 
                if (jourbrute == 0) {
                   return 0;        
                }
                 if (jourbrute > 0) {
                   return jourbrute ; 
                 }
                 if (jourbrute < 0) {
                   return (7+jourbrute) ; 
                 }
             }
             
        int calculminutesagent (String heurearrivee , String heuresortie , String premierjour_ , String deuxiemejour_) {
          
     int minutearrivee , minutesortie ;
    minutearrivee = Stringtointconversion60(heurearrivee.substring(0,2))*60 + Stringtointconversion60(heurearrivee.substring(3,5));
    minutesortie = Stringtointconversion60(heuresortie.substring(0,2))*60 + Stringtointconversion60(heuresortie.substring(3,5));

     return (minutesortie-minutearrivee)+nombrejouragent(premierjour_ , deuxiemejour_)*1440;
          }

//           void affichagedureetravailsurlcd (String nomagent , int numeroidagent) {
//
//                 if (informationAgent.equals(nomagent) && digitalRead (boutonmanuel) == LOW) {
//                  
//                       lcd.setCursor (0,2); 
//                        lcd.print (nomagent);
//                         lcd.setCursor (0,3); 
//                          lcd.print ("T.E: ");
//                           lcd.print (temps_moi_presence[numeroidagent]/60);
//                            lcd.print ("h");
//                             lcd.print (temps_moi_presence[numeroidagent] % 60); 
//                          delay (5000);  
//
//                             lcd.setCursor(0,2);
//                              lcd.print("                    ");
//                               lcd.setCursor(0,3);
//                                lcd.print("                    ");
//                 }
//           }

 



// void send_data_mqtt_cloud_agentactif(){
//    agentactif.publish ("ACTUALISATION DONNEES");
//     
//    if (presence [0] == 1) {
//       agentactif.publish ("AIME MUHESI ACTIF"); 
//    }
//
//    if (presence [1] == 1) {
//       agentactif.publish ("MUMBERE VAKE ACTIF"); 
//    }
//
//    if (presence [2] == 1) {
//       agentactif.publish ("DELPHIN KAMABU ACTIF"); 
//    }
//    if (presence [3] == 1) {
//       agentactif.publish ("JANVIER KITOGA ACTIF"); 
//    }
//
//    if (presence [4] == 1) {
//       agentactif.publish ("BAHATI NATHANAEL ACTIF"); 
//    }
//
//    if (presence [5] == 1) {
//       agentactif.publish ("KIKA KIKURU ACTIF"); 
//    }
//
//    if (presence [6] == 1) {
//       agentactif.publish ("MERVEIL KATALIKO ACTIF"); 
//    }
//
//    if (presence [7] == 1) {
//       agentactif.publish ("FISTON SIKULI ACTIF"); 
//    }
//
//    if (presence [8] == 1) {
//       agentactif.publish ("KAMBALE KAMBULA ACTIF"); 
//    }
//
//    if (presence [9] == 1) {
//       agentactif.publish ("GUYLAIN ACTIF"); 
//    }
//
//    if (presence [10] == 1) {
//       agentactif.publish ("OLIVIER MUBMBERE ACTIF"); 
//    }
//
//    if (presence [11] == 1) {
//       agentactif.publish ("CHANCE KASEREKA ACTIF"); 
//    }
//
//    if (presence [12] == 1) {
//       agentactif.publish ("EMMANUEL ACTIF"); 
//    }
//    if (presence [13] == 1) {
//       agentactif.publish ("RODRIGUE MUMBERE ACTIF"); 
//    }
//    if (presence [14] == 1) {
//       agentactif.publish ("EDWINE LUKULIRWA ACTIF"); 
//    }
//    if (presence [15] == 1) {
//       agentactif.publish ("KUBUYA ACTIF"); 
//    }
//    if (presence [16] == 1) {
//       agentactif.publish ("CHRISTIAN BALEKAGUE ACTIF"); 
//    }
//    if (presence [17] == 1) {
//       agentactif.publish ("JUSTIN ACTIF"); 
//    }
//    if (presence [18] == 1) {
//       agentactif.publish ("KAMBALE MUPIKA ACTIF"); 
//    }
//    if (presence [19] == 1) {
//       agentactif.publish ("ALEXIS MALIRA ACTIF"); 
//    }
//    if (presence [20] == 1) {
//       agentactif.publish ("VICARD TEST ACTIF"); 
//    }
//    if (presence [21] == 1) {
//       agentactif.publish ("PRINCE KAMBALE ACTIF"); 
//    }
//    if (presence [22] == 1) {
//       agentactif.publish ("BLANCHARD VALISALIRE ACTIF"); 
//    }
//    if (presence [23] == 1) {
//       agentactif.publish ("RIGO KASEREKA ACTIF"); 
//    }
//    if (presence [24] == 1) {
//       agentactif.publish ("SAMY KAMBALE ACTIF"); 
//    }
//    if (presence [25] == 1) {
//       agentactif.publish ("ERIC SHABANTU ACTIF"); 
//    }
//    if (presence [26] == 1) {
//       agentactif.publish ("CONSOLE KAVIRA ACTIF"); 
//    }
//    if (presence [27] == 1) {
//       agentactif.publish ("EZEKIEL MAMBO ACTIF"); 
//    }
//    if (presence [28] == 1) {
//       agentactif.publish ("MITTERAND KATEMBO ACTIF"); 
//    }
//    if (presence [29] == 1) {
//       agentactif.publish ("YANNICK BAHATI ACTIF"); 
//    }
//    if (presence [30] == 1) {
//       agentactif.publish ("ARSENE KAMBALE ACTIF"); 
//    }
//    if (presence [31] == 1) {
//       agentactif.publish ("DONATIEN BIKUBA ACTIF"); 
//    }
//    if (presence [32] == 1) {
//       agentactif.publish ("JOSEPH MUSHENGE ACTIF"); 
//    }
//    if (presence [33] == 1) {
//       agentactif.publish ("PATIENT MWAMBA ACTIF"); 
//    }
//    if (presence [34] == 1) {
//       agentactif.publish ("KAMBALE KASUKA ACTIF"); 
//    }
//    if (presence [35] == 1) {
//       agentactif.publish ("JULIEN KAMBALE ACTIF"); 
//    }
//    if (presence [36] == 1) {
//       agentactif.publish ("KAMBALE KATAKA ACTIF"); 
//    }
//    if (presence [37] == 1) {
//       agentactif.publish ("DANNY KAKULE ACTIF"); 
//    }
//    if (presence [38] == 1) {
//       agentactif.publish ("ERIC KAMUSWEKERE ACTIF"); 
//    }
//    if (presence [39] == 1) {
//       agentactif.publish ("ALEXIS ACTIF"); 
//    }
//    if (presence [40] == 1) {
//       agentactif.publish ("NZANZU ACTIF"); 
//    }
//    if (presence [41] == 1) {
//       agentactif.publish ("PASCAL ACTIF"); 
//    }
//    if (presence [42] == 1) {
//       agentactif.publish ("MICHEL MISHONA ACTIF"); 
//    }
//    if (presence [43] == 1) {
//       agentactif.publish ("CHRISTIAN BAKWAMAHA ACTIF"); 
//    }
//    if (presence [44] == 1) {
//       agentactif.publish ("ARON ZUMBU ACTIF"); 
//    }
//    if (presence [45] == 1) {
//       agentactif.publish ("JAMES ACTIF"); 
//    }
//    if (presence [46] == 1) {
//       agentactif.publish ("SEMIDA KAVIRA ACTIF"); 
//    }
//    if (presence [47] == 1) {
//       agentactif.publish ("ALEX TSONGO ACTIF"); 
//    }
//    if (presence [48] == 1) {
//       agentactif.publish ("GLORIA MALEMO ACTIF"); 
//    }
//    if (presence [49] == 1) {
//       agentactif.publish ("GUYLAINE NZIAVAKE ACTIF"); 
//    }
//    if (presence [50] == 1) {
//       agentactif.publish ("JEAN MARIE BISIHIWA ACTIF"); 
//    }
//    if (presence [51] == 1) {
//       agentactif.publish ("THOMAS SEBISUSA ACTIF"); 
//    }
//    if (presence [52] == 1) {
//       agentactif.publish ("KAMBALE FATAKI ACTIF"); 
//    }
//    if (presence [53] == 1) {
//       agentactif.publish ("HENRIQUE KAMBALA ACTIF"); 
//    }
//    if (presence [54] == 1) {
//       agentactif.publish ("EMMANUEL MUHIGIRWA ACTIF"); 
//    }
//
//    
// }
//          void send_data_mqtt_cloud_agentnonactif(){
//
//              agentnoactif.publish ("ACTUALISATION DONNEES"); 
//              
//    if (presence [0] == 0) {
//       agentnoactif.publish ("AIME MUHESI NOACTIF"); 
//    }
//
//    if (presence [1] == 0) {
//       agentnoactif.publish ("MUMBERE VAKE NOACTIF"); 
//    }
//
//    if (presence [2] == 0) {
//       agentnoactif.publish ("DELPHIN KAMABU NOACTIF"); 
//    }
//    if (presence [3] == 0) {
//       agentnoactif.publish ("JANVIER KITOGA NOACTIF"); 
//    }
//
//    if (presence [4] == 0) {
//       agentnoactif.publish ("BAHATI NATHANAEL NOACTIF"); 
//    }
//
//    if (presence [5] == 0) {
//       agentnoactif.publish ("KIKA KIKURU NOACTIF"); 
//    }
//
//    if (presence [6] == 0) {
//       agentnoactif.publish ("MERVEIL KATALIKO NOACTIF"); 
//    }
//
//    if (presence [7] == 0) {
//       agentnoactif.publish ("FISTON SIKULI NOACTIF"); 
//    }
//
//    if (presence [8] == 0) {
//       agentnoactif.publish ("KAMBALE KAMBULA NOACTIF"); 
//    }
//
//    if (presence [9] == 0) {
//       agentnoactif.publish ("GUYLAIN NOACTIF"); 
//    }
//
//    if (presence [10] == 0) {
//       agentnoactif.publish ("OLIVIER MUBMBERE NOACTIF"); 
//    }
//
//    if (presence [11] == 0) {
//       agentnoactif.publish ("CHANCE KASEREKA NOACTIF"); 
//    }
//
//    if (presence [12] == 0) {
//       agentnoactif.publish ("EMMANUEL NOACTIF"); 
//    }
//    if (presence [13] == 0) {
//       agentnoactif.publish ("RODRIGUE MUMBERE NOACTIF"); 
//    }
//    if (presence [14] == 0) {
//       agentnoactif.publish ("EDWINE LUKULIRWA NOACTIF"); 
//    }
//    if (presence [15] == 0) {
//       agentnoactif.publish ("KUBUYA NOACTIF"); 
//    }
//    if (presence [16] == 0) {
//       agentnoactif.publish ("CHRISTIAN BALEKAGUE NOACTIF"); 
//    }
//    if (presence [17] == 0) {
//       agentnoactif.publish ("JUSTIN NOACTIF"); 
//    }
//    if (presence [18] == 0) {
//       agentnoactif.publish ("KAMBALE MUPIKA NOACTIF"); 
//    }
//    if (presence [19] == 0) {
//       agentnoactif.publish ("ALEXIS MALIRA NOACTIF"); 
//    }
//    if (presence [20] == 0) {
//       agentnoactif.publish ("VICARD TEST NOACTIF"); 
//    }
//    if (presence [21] == 0) {
//       agentnoactif.publish ("PRINCE KAMBALE NOACTIF"); 
//    }
//    if (presence [22] == 0) {
//       agentnoactif.publish ("BLANCHARD VALISALIRE NOACTIF"); 
//    }
//    if (presence [23] == 0) {
//       agentnoactif.publish ("RIGO KASEREKA NOACTIF"); 
//    }
//    if (presence [24] == 0) {
//       agentnoactif.publish ("SAMY KAMBALE NOACTIF"); 
//    }
//    if (presence [25] == 0) {
//       agentnoactif.publish ("ERIC SHABANTU NOACTIF"); 
//    }
//    if (presence [26] == 0) {
//       agentnoactif.publish ("CONSOLE KAVIRA NOACTIF"); 
//    }
//    if (presence [27] == 0) {
//       agentnoactif.publish ("EZEKIEL MAMBO NOACTIF"); 
//    }
//    if (presence [28] == 0) {
//       agentnoactif.publish ("MITTERAND KATEMBO NOACTIF"); 
//    }
//    if (presence [29] == 0) {
//       agentnoactif.publish ("YANNICK BAHATI NOACTIF"); 
//    }
//    if (presence [30] == 0) {
//       agentnoactif.publish ("ARSENE KAMBALE NOACTIF"); 
//    }
//    if (presence [31] == 0) {
//       agentnoactif.publish ("DONATIEN BIKUBA NOACTIF"); 
//    }
//    if (presence [32] == 0) {
//       agentnoactif.publish ("JOSEPH MUSHENGE NOACTIF"); 
//    }
//    if (presence [33] == 0) {
//       agentnoactif.publish ("PATIENT MWAMBA NOACTIF"); 
//    }
//    if (presence [34] == 0) {
//       agentnoactif.publish ("KAMBALE KASUKA NOACTIF"); 
//    }
//    if (presence [35] == 0) {
//       agentnoactif.publish ("JULIEN KAMBALE NOACTIF"); 
//    }
//    if (presence [36] == 0) {
//       agentnoactif.publish ("KAMBALE KATAKA NOACTIF"); 
//    }
//    if (presence [37] == 0) {
//       agentnoactif.publish ("DANNY KAKULE NOACTIF"); 
//    }
//    if (presence [38] == 0) {
//       agentactif.publish ("ERIC KAMUSWEKERE NOACTIF"); 
//    }
//    if (presence [39] == 0) {
//       agentnoactif.publish ("ALEXIS NOACTIF"); 
//    }
//    if (presence [40] == 0) {
//       agentnoactif.publish ("NZANZU NOACTIF"); 
//    }
//    if (presence [41] == 0) {
//       agentnoactif.publish ("PASCAL NOACTIF"); 
//    }
//    if (presence [42] == 0) {
//       agentnoactif.publish ("MICHEL MISHONA NOACTIF"); 
//    }
//    if (presence [43] == 0) {
//       agentnoactif.publish ("CHRISTIAN BAKWAMAHA NOACTIF"); 
//    }
//    if (presence [44] == 0) {
//       agentnoactif.publish ("ARON ZUMBU NOACTIF"); 
//    }
//    if (presence [45] == 0) {
//       agentnoactif.publish ("JAMES NOACTIF"); 
//    }
//    if (presence [46] == 0) {
//       agentnoactif.publish ("SEMIDA KAVIRA NOACTIF"); 
//    }
//    if (presence [47] == 0) {
//       agentnoactif.publish ("ALEX TSONGO NOACTIF"); 
//    }
//    if (presence [48] == 0) {
//       agentnoactif.publish ("GLORIA MALEMO NOACTIF"); 
//    }
//    if (presence [49] == 0) {
//       agentnoactif.publish ("GUYLAINE NZIAVAKE NOACTIF"); 
//    }
//    if (presence [50] == 0) {
//       agentnoactif.publish ("JEAN MARIE BISIHIWA NOACTIF"); 
//    }
//    if (presence [51] == 0) {
//       agentnoactif.publish ("THOMAS SEBISUSA NOACTIF"); 
//    }
//    if (presence [52] == 0) {
//       agentnoactif.publish ("KAMBALE FATAKI NOACTIF"); 
//    }
//    if (presence [53] == 0) {
//       agentnoactif.publish ("HENRIQUE KAMBALA NOACTIF"); 
//    }
//    if (presence [54] == 0) {
//       agentnoactif.publish ("EMMANUEL MUHIGIRWA NOACTIF"); 
//    }
//            
// }

 void MQTT_connect();
 
void setup() {
/*
 * configurationEEPROM(4000);
 */



  //pinMode (boutonmanuel,INPUT_PULLUP); 
  // pinMode (A0 , OUTPUT);  

  

     rtc.halt(false);
  rtc.writeProtect(false);

  //rtc.setDOW(TUESDAY);       
  //rtc.setTime(18 , 10 , 0);     
  //rtc.setDate( 20 , 07 , 2021); 
  
  Wire.begin (D4 , D3);
  lcd.begin () ;

  lcd.clear (); 
     lcd.setCursor (0,1);
     lcd.print("V");
     delay (400);
     lcd.print("I");
     delay (400);
     lcd.print("C");
     delay (400);
       lcd.print("A");
       delay (400);
       lcd.print("R");
       delay (400);

        lcd.print(" ");
      delay (400);
       lcd.print("i");
      delay (400);
       lcd.print("n");
      delay (400);
       lcd.print("d");
      delay (400);

      lcd.setCursor (17,1);
      lcd.print ("M");
      delay (400); 
      lcd.print ("_");
      delay (400);
     lcd.print ("M");
       lcd.setCursor (0,2);
       lcd.print ("JERRYSON HOTEL");
        delay (2000); 

         lcd.clear ();

     lcd.setCursor (0,1);
      lcd.print ("ATTENDANCE SYSTEM");
       lcd.setCursor (0,2);
       lcd.print ("CHARGEMENT"); 
           for (int i =0 ; i<10 ; i++) {

             lcd.setCursor (10+i,2);
              lcd.print (".") ;
              delay (400);
           }

          lcd.clear();

            lcd.setCursor (0,0);
            lcd.print("ATTENDANCE SYSTEM");

 //Serial.begin(9600);
//  Serial.println ("programmation");

  //WiFi.softAP(ssid, password);
  //WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  //server.on("/", handle_OnConnect);
 // server.on("/ledon", handle_ledon);
 // server.on("/ledoff", handle_ledoff);
  // server.onNotFound(handle_NotFound);

 // server.begin();
 // Serial.println("HTTP server started");

   Serial.begin (vitesseCommunication);

     if (!SD.begin (chipSelect)) {
      lcd.setCursor (0,3); 
    lcd.print ("SD ERROR");
     return ; 
  } else {

    lcd.setCursor (0,1);
    lcd.print ("ANALYSE MATERIEL");
    lcd.setCursor (0,2);
    lcd.print ("SD CARD");
    lcd.setCursor (0,3);

    lcd.print ("CONFIGURATION");

    lcd.setCursor (13, 3);
     lcd.print (".");
     delay (500);
    
     lcd.setCursor (14, 3);
     lcd.print (".");
     delay (500);
     lcd.setCursor (15, 3);
     lcd.print (".");
     delay (500);
     lcd.setCursor (16, 3);
     lcd.print (".");
     delay (500);
     lcd.setCursor (17, 3);
     lcd.print (".");
     delay (500);
     lcd.setCursor (18, 3);
     lcd.print (".");
      delay (500);
     lcd.setCursor (19, 3);
     lcd.print (".");
     delay (500);

     
       lcd.setCursor (0,1);
     lcd.print ("                    "); 
     lcd.setCursor (0,2);
     lcd.print ("                    "); 
      lcd.setCursor (0,3);
     lcd.print ("                    "); 
    
  }

  WiFi.begin(WLAN_SSID, WLAN_PASS);
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
   //  Serial.print(".");
  }

  lcd.setCursor (0,1); 
  lcd.print ("CLOUD DATA");

  lcd.setCursor (0,2);
  lcd.print ("WIFI CONNECTED");

  lcd.setCursor (0,3);
  lcd.print ("IP:");
  lcd.print (WiFi.localIP());
   delay (3000);
   
     lcd.setCursor (0,1);
     lcd.print ("                    "); 
     lcd.setCursor (0,2);
     lcd.print ("                    "); 
     lcd.setCursor (0,3);
     lcd.print ("                    "); 


   
  
}
void loop() {

  

      dateactuelle =   rtc.getDateStr();
       heureactuelle = rtc.getTimeStr();

       initialisationheuredebutmois() ;  // initialisation debut comptage du temps du mois 00:00:00 le 01

    lcd.setCursor(0,1);
    lcd.print(heureactuelle);

    lcd.print (" ") ;
    
    lcd.print (dateactuelle);


    MQTT_connect();
    
 
  if (Serial.available()) {
     informationAgent = Serial.readString(); 


/* 

    if (digitalRead (boutonmanuel) {
      

         affichagedureetravailsurlcd ("AIME MUHESI",0); 
         affichagedureetravailsurlcd ("MUMBERE VAKE",1);
         affichagedureetravailsurlcd ("DELPHIN KAMABU",2); 
         affichagedureetravailsurlcd ("JANVIER KITOGA",3);
         affichagedureetravailsurlcd ("BAHATI NATHANAEL",4);
         affichagedureetravailsurlcd ("KIKA KAKURU",5);
         
         affichagedureetravailsurlcd ("MERVEIL KATALIKO",6);
         affichagedureetravailsurlcd ("FISTON SIKULI",7);
         affichagedureetravailsurlcd ("KAMBALE KAMBULA",8);
         affichagedureetravailsurlcd ("GUYLAIN",9);
         affichagedureetravailsurlcd ("OLIVIER MUMBERE",10);
         affichagedureetravailsurlcd ("CHANCE KASEREKA",11);

          affichagedureetravailsurlcd ("EMMANUEL",12);
           affichagedureetravailsurlcd ("RODRIGUE MUMBERE",13);
            affichagedureetravailsurlcd ("EDWINE LUKULIRWA",14);
             affichagedureetravailsurlcd ("KUBUYA ",15);
              affichagedureetravailsurlcd ("CHRISTIAN BALEKAGUE",16);
               affichagedureetravailsurlcd ("JUSTIN",17);

                 affichagedureetravailsurlcd ("KAMBALE MUPIKA",18);
                 affichagedureetravailsurlcd ("ALEXIS MALIRA",19);
                 affichagedureetravailsurlcd ("PRINCE KAMBALE",21);
                 affichagedureetravailsurlcd ("BLANCHARD VALISALIRE",22);
                 affichagedureetravailsurlcd ("RIGO   KASEREKA",23);
                 affichagedureetravailsurlcd ("SAMY KAMBALE",24);

                  affichagedureetravailsurlcd ("ERIC SHABANTU",25);
                 affichagedureetravailsurlcd ("CONSOLE KAVIRA",26);
                affichagedureetravailsurlcd ("EZEKIEL MAMBO",27);
                 affichagedureetravailsurlcd ("MITTERAND KATEMBO",28);
                  affichagedureetravailsurlcd ("YANNICK BAHATI",29);
                   affichagedureetravailsurlcd ("ARSENE KAMBALE",30);
                    affichagedureetravailsurlcd ("DONATIEN BIKUBA",31);

                      affichagedureetravailsurlcd ("DONATIEN BIKUBA",31);
                        affichagedureetravailsurlcd ("JOSEPH MUSHENGE",32);
                          affichagedureetravailsurlcd ("PATIENT MWAMBA      ",33);
                          affichagedureetravailsurlcd ("KAMBALE KASUKA",34);
                         affichagedureetravailsurlcd ("JULIEN KAMBALE",35);
                        affichagedureetravailsurlcd ("KAMBALE KATAKA",36);

                         affichagedureetravailsurlcd ("DANNY KAKULE",37);
                          affichagedureetravailsurlcd ("ERIC KAMUSWEKERE",38);
                           affichagedureetravailsurlcd ("ALEXIS",39);
                            affichagedureetravailsurlcd ("NZANZU",40);
                             affichagedureetravailsurlcd ("PASCAL",41);
                              affichagedureetravailsurlcd ("MICHEL MISHONA",42);

                                affichagedureetravailsurlcd ("CHRISTIAN BAKWANAMAH",43);
                                  affichagedureetravailsurlcd ("ARON ZUMBU",44);
                                    affichagedureetravailsurlcd ("JAMES",45);
                                      affichagedureetravailsurlcd ("SEMIDA KAVIRA",46);
                                        affichagedureetravailsurlcd ("ALEX TSONGO",47);
                                          affichagedureetravailsurlcd ("GLORIA MALEMO",48);

                         affichagedureetravailsurlcd ("GYLAINE NZIAVAKE",49);
                         affichagedureetravailsurlcd ("JEAN MARIE BISIHIWA",50);
                         affichagedureetravailsurlcd ("THOMAS SEBISUSA",51);
                         affichagedureetravailsurlcd ("KAMBALE FATAKI",52);
                         affichagedureetravailsurlcd ("HENRIQUE KABALA",53);
                         affichagedureetravailsurlcd ("EMMANUEL MUHIGIRWA",54);
                         
                        
    }

     */
    
          if (informationAgent.equals("AIME MUHESI")) {
                 presence [0] = presence [0] + 1 ;
                    if (presence [0] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[0] = rtc.getTimeStr();
                   heure_sortie[0] = ""; 
                   jourdelasemaine_arrivee [0] =rtc.getDOWStr();
                   
                      delay (3000); 

                      
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                         enregistrementdatainsdcard ("1.txt" , "AIME MUHESI" , rtc.getDateStr() , heure_arrive[0] , "ARRIVEE" , jourdelasemaine_arrivee[0]);
                       
                         agent.publish("AIME MUHESI ACTIF");
                    }                    
                if  (presence [0] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[0] = rtc.getTimeStr();
                   jourdelasemaine_sortie[0] = rtc.getDOWStr();

                 temps_moi_presence[0]=temps_moi_presence[0]+calculminutesagent(heure_arrive[0],heure_sortie[0],jourdelasemaine_arrivee[0],jourdelasemaine_sortie[0]);
                 
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [0] = 0 ;
                         enregistrementdatainsdcard ("1.txt" , "AIME MUHESI" , rtc.getDateStr() , heure_sortie[0] , "SORTIE" ,jourdelasemaine_sortie[0]);
                         sautdeligne ("1.txt");

                         agent.publish("AIME MUHESI NON ACTIF");
                }
          }

            if (informationAgent.equals("MUMBERE VAKE")) {
                 presence [1] = presence [1] + 1;
                    if (presence [1] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[1] = rtc.getTimeStr();
                   heure_sortie[1] = ""; 
                   jourdelasemaine_arrivee [1] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                         enregistrementdatainsdcard ("2.txt" , "MUMBERE VAKE" , rtc.getDateStr() , heure_arrive[1] , "ARRIVEE" ,jourdelasemaine_arrivee[1]);

                         agent.publish("MUMBERE VAKE ACTIF");
                    }                    
                if  (presence [1] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[1] = rtc.getTimeStr();
                    jourdelasemaine_sortie [1] = rtc.getDOWStr();

                       temps_moi_presence[1]=temps_moi_presence[1]+calculminutesagent(heure_arrive[1],heure_sortie[1],jourdelasemaine_arrivee[1],jourdelasemaine_sortie[1]);
                       
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [1] = 0 ;
                         enregistrementdatainsdcard ("2.txt" , "MUMBERE VAKE" , rtc.getDateStr() , heure_sortie[1] , "SORTIE" ,jourdelasemaine_sortie[1]);
                         sautdeligne ("2.txt");

                         agent.publish("MUMBERE VAKE NON ACTIF");
                                        
                }
          }

                    if (informationAgent.equals("DELPHIN KAMABU")) {
                 presence [2] = presence [2] + 1 ;
                    if (presence [2] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[2] = rtc.getTimeStr();
                   heure_sortie[2] = ""; 
                    jourdelasemaine_arrivee [2] =rtc.getDOWStr () ;
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                         enregistrementdatainsdcard ("3.txt" , "DELPHIN KAMABU" , rtc.getDateStr() ,  heure_arrive[2] , "ARRIVEE" ,jourdelasemaine_arrivee[2]);

                         agent.publish("DELPHIN KAMABU ACTIF");
                    }                    
                if  (presence [2] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[2] = rtc.getTimeStr();  
                    jourdelasemaine_sortie [2] = rtc.getDOWStr(); 

                    temps_moi_presence[2]=temps_moi_presence[2]+calculminutesagent(heure_arrive[2],heure_sortie[2],jourdelasemaine_arrivee[2],jourdelasemaine_sortie[2]);
                    
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [2] = 0 ;

                         enregistrementdatainsdcard ("3.txt" , "DELPHIN KAMABU" , rtc.getDateStr() , heure_sortie[2] , "SORTIE" ,jourdelasemaine_sortie[2]);
                         sautdeligne ("3.txt");

                         agent.publish("DELPHIN KAMABU NON ACTIF");

                         
                }
          }

           if (informationAgent.equals("JANVIER KITOGA")) {
                 presence [3] = presence [3] + 1 ;
                    if (presence [3] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[3] = rtc.getTimeStr();
                   heure_sortie[3] = ""; 
                   jourdelasemaine_arrivee [3] =rtc.getDOWStr ();
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                         enregistrementdatainsdcard ("4.txt" , "JANVIER KITOGA" , rtc.getDateStr() , heure_arrive[3] , "ARRIVEE" ,jourdelasemaine_arrivee[3]);

                         agent.publish("JANVIER KITOGA ACTIF");
                    }                    
                if  (presence [3] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[3] = rtc.getTimeStr();
                    jourdelasemaine_sortie [3] = rtc.getDOWStr();

                    temps_moi_presence[3]=temps_moi_presence[3]+calculminutesagent(heure_arrive[3],heure_sortie[3],jourdelasemaine_arrivee[3],jourdelasemaine_sortie[3]);
                  
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [3] = 0 ;  
                         enregistrementdatainsdcard ("4.txt" , "JANVIER KITOGA" , rtc.getDateStr() , heure_sortie[3] , "SORTIE" ,jourdelasemaine_sortie[3]);
                         sautdeligne ("4.txt");

                         agent.publish("JANVIER KITOGA NON ACTIF");
                                      
                }
          }

          if (informationAgent.equals("BAHATI NATHANAEL")) {
                 presence [4] = presence [4] + 1 ;
                    if (presence [4] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[4] = rtc.getTimeStr();
                   heure_sortie[4] = ""; 
                   jourdelasemaine_arrivee [4] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                          enregistrementdatainsdcard ("5.txt" , "BAHATI NATHANAEL" , rtc.getDateStr() ,heure_arrive[4] , "ARRIVEE" ,jourdelasemaine_arrivee [4]);

                          agent.publish("BAHATI NATHANAEL ACTIF");
                    }                    
                if  (presence [4] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[4] = rtc.getTimeStr();   
                    jourdelasemaine_sortie [4] = rtc.getDOWStr();

                    temps_moi_presence[4]=temps_moi_presence[4]+calculminutesagent(heure_arrive[4],heure_sortie[4],jourdelasemaine_arrivee[4],jourdelasemaine_sortie[4]); 
                   
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [4] = 0 ;
                         enregistrementdatainsdcard ("5.txt" , "BAHATI NATHANAEL" , rtc.getDateStr() , heure_sortie[4] , "SORTIE" ,jourdelasemaine_sortie[4]);
                         sautdeligne ("5.txt");

                         agent.publish("BAHATI NATHANAEL NON ACTIF");
                                         
                }
          }


           if (informationAgent.equals("KIKA KAKURU")) {
                 presence [5] = presence [5] + 1 ;
                    if (presence [5] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);
                   
                   heure_arrive[5] = rtc.getTimeStr();
                   heure_sortie[5] = ""; 
                   jourdelasemaine_arrivee [5] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         enregistrementdatainsdcard ("6.txt" , "KIKA KAKURU" , rtc.getDateStr() , heure_arrive[5] , "ARRIVEE" ,jourdelasemaine_arrivee [5]);

                           agent.publish("KIKA KAKURU ACTIF");
                    }                    
                if  (presence [5] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[5] = rtc.getTimeStr(); 
                    jourdelasemaine_sortie [5] = rtc.getDOWStr();

                    temps_moi_presence[5]=temps_moi_presence[5]+calculminutesagent(heure_arrive[5],heure_sortie[5],jourdelasemaine_arrivee[5],jourdelasemaine_sortie[5]);
                    
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [5] = 0 ;
                         enregistrementdatainsdcard ("6.txt" , "KIKA KAKURU" , rtc.getDateStr() , heure_sortie[5] , "SORTIE" ,jourdelasemaine_sortie[5]);
                         sautdeligne ("6.txt");

                          agent.publish("KIKA KAKURU NON ACTIF"); 
                                    
                }
          }

           if (informationAgent.equals("MERVEIL KATALIKO")) {
                 presence [6] = presence [6] + 1 ;
                    if (presence [6] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[6] = rtc.getTimeStr();
                   heure_sortie[6] = ""; 
                   jourdelasemaine_arrivee [6] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    "); 
                         enregistrementdatainsdcard ("7.txt" , "MERVEIL KATALIKO" , rtc.getDateStr() ,  heure_arrive[6]  , "ARRIVEE" , jourdelasemaine_arrivee [6]); 

                         agent.publish("MERVEIL KATALIKO ACTIF"); 
                         
                    }                    
                if  (presence [6] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[6] = rtc.getTimeStr();
                    jourdelasemaine_sortie[6] = rtc.getDOWStr();

                    temps_moi_presence[6]=temps_moi_presence[6]+calculminutesagent(heure_arrive[6],heure_sortie[6],jourdelasemaine_arrivee[6],jourdelasemaine_sortie[6]);
                   
                                 
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [6] = 0 ;
                         enregistrementdatainsdcard ("7.txt" , "MERVEIL KATALIKO" , rtc.getDateStr() , heure_sortie[6] , "SORTIE" ,jourdelasemaine_sortie[6]);
                         sautdeligne ("7.txt");

                         agent.publish("MERVEIL KATALIKO NON ACTIF");
                                           
                }
          }


                  if (informationAgent.equals("FISTON SIKULI")) {
                 presence [7] = presence [7] + 1 ;
                    if (presence [7] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[7] = rtc.getTimeStr();
                   heure_sortie[7] = "";
                   jourdelasemaine_arrivee [7] =rtc.getDOWStr () ;  
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                         enregistrementdatainsdcard ("8.txt" , "FISTON KATALIKO" , rtc.getDateStr() ,heure_arrive[7], "ARRIVEE" ,jourdelasemaine_arrivee [7]);

                         agent.publish("FISTON SIKULI ACTIF");
                    }                    
                if  (presence [7] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);
                   
                   heure_sortie[7] = rtc.getTimeStr();
                   jourdelasemaine_sortie [7] = rtc.getDOWStr();
                   
                   temps_moi_presence[7]=temps_moi_presence[7]+calculminutesagent(heure_arrive[7],heure_sortie[7],jourdelasemaine_arrivee[7],jourdelasemaine_sortie[7]);
                   
                      
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [7] = 0 ;
                         enregistrementdatainsdcard ("8.txt" , "FISTON KATALIKO" , rtc.getDateStr() , heure_sortie[7] , "SORTIE" ,jourdelasemaine_sortie[7]);
                         sautdeligne ("8.txt");

                         agent.publish("FISTON SIKULI NON ACTIF");
                                         
                }
          }

                    if (informationAgent.equals("KAMBALE KAMBULA")) {
                 presence [8] = presence [8] + 1 ;
                    if (presence [8] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[8] = rtc.getTimeStr();
                   heure_sortie[8] = "";  
                   jourdelasemaine_arrivee [8] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                         enregistrementdatainsdcard ("9.txt" , "KAMBALE KAMBULA" , rtc.getDateStr() , heure_arrive[8] , "ARRIVEE" ,jourdelasemaine_arrivee[8]);

                         agent.publish("KAMBALE KAMBULA ACTIF");
                    }                    
                if  (presence [8] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[8] = rtc.getTimeStr(); 
                    jourdelasemaine_sortie[8] = rtc.getDOWStr();
                    
                    temps_moi_presence[8]=temps_moi_presence[8]+calculminutesagent(heure_arrive[8],heure_sortie[8],jourdelasemaine_arrivee[8],jourdelasemaine_sortie[8]);  
                   
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [8] = 0 ;
                         enregistrementdatainsdcard("9.txt" , "KAMBALE KAMBULA" , rtc.getDateStr() , heure_sortie[8] , "SORTIE" ,jourdelasemaine_sortie[8]);
                         sautdeligne ("9.txt");    

                         agent.publish("KAMBALE KAMBULA NON ACTIF");
                }
          }

               if (informationAgent.equals("GUYLAIN")) {
                 presence [9] = presence [9] + 1 ;
                    if (presence [9] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[9] = rtc.getTimeStr();
                   heure_sortie[9] = "";  
                   jourdelasemaine_arrivee [9] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         enregistrementdatainsdcard ("10.txt" , "GUYLAIN" , rtc.getDateStr() , heure_arrive[9] , "ARRIVEE" ,jourdelasemaine_arrivee[9]);  


                          agent.publish("GUYLAIN ACTIF");
                    }                    
                if  (presence [9] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[9] = rtc.getTimeStr(); 
                    jourdelasemaine_sortie [9] = rtc.getDOWStr(); 

                    temps_moi_presence[9]=temps_moi_presence[9]+calculminutesagent(heure_arrive[9],heure_sortie[9],jourdelasemaine_arrivee[9],jourdelasemaine_sortie[9]);
                   
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [9] = 0 ;
                         enregistrementdatainsdcard ("10.txt" , "GUYLAIN" , rtc.getDateStr() , heure_sortie[9] , "SORTIE" ,jourdelasemaine_sortie[9]);
                         sautdeligne ("10.txt"); 

                             agent.publish("GUYLAIN NON ACTIF");
                                        
                }
          }


                    if (informationAgent.equals("OLIVIER MUMBERE")) {
                 presence [10] = presence [10] + 1 ;
                    if (presence [10] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[10] = rtc.getTimeStr();
                   heure_sortie[10] = "";  
                    jourdelasemaine_arrivee [10] =rtc.getDOWStr () ;
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         enregistrementdatainsdcard ("11.txt" , "OLIVIER MUMBERE" , rtc.getDateStr() ,  heure_arrive[10] , "ARRIVEE" ,jourdelasemaine_arrivee[10]);

                            agent.publish("OLIVIER MUMBERE ACTIF");
                    }                    
                if  (presence [10] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[10] = rtc.getTimeStr();
                    jourdelasemaine_sortie [10] = rtc.getDOWStr();  
                    
                    temps_moi_presence[10]=temps_moi_presence[10]+calculminutesagent(heure_arrive[10],heure_sortie[10],jourdelasemaine_arrivee[10],jourdelasemaine_sortie[10]);
                    
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [10] = 0 ;
                         enregistrementdatainsdcard ("11.txt" , "OLIVIER MUMBERE" , rtc.getDateStr() , heure_sortie[10] , "SORTIE" ,jourdelasemaine_sortie[10]);
                         sautdeligne ("11.txt");  

                         agent.publish("OLIVIER MUMBERE NON ACTIF");
                                          
                }
          }

           if (informationAgent.equals("CHANCE KASEREKA")) {
                 presence [11] = presence [11] + 1 ;
                    if (presence [11] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[11] = rtc.getTimeStr();
                   heure_sortie[11] = "";  
                    jourdelasemaine_arrivee [11] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                         enregistrementdatainsdcard ("12.txt" , "CHANCE KASEREKA" , rtc.getDateStr() , heure_arrive[11] , "ARRIVEE" ,jourdelasemaine_arrivee[11]);

                          agent.publish("CHANCE KASEREKA ACTIF");
                    }                    
                if  (presence [11] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[11] = rtc.getTimeStr();
                    jourdelasemaine_sortie [11] = rtc.getDOWStr();
                    
                    temps_moi_presence[11]=temps_moi_presence[11]+calculminutesagent(heure_arrive[11],heure_sortie[11],jourdelasemaine_arrivee[11],jourdelasemaine_sortie[11]);   
                    
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [11] = 0 ; 
                           enregistrementdatainsdcard ("12.txt" , "CHANCE KASEREKA" , rtc.getDateStr() , heure_sortie[11] , "SORTIE" ,jourdelasemaine_sortie[11]);
                         sautdeligne ("12.txt");

                         agent.publish("CHANCE KASEREKA NON ACTIF");
                                         
                }
          }

          if (informationAgent.equals("EMMANUEL")) {
                 presence [12] = presence [12] + 1 ;
                    if (presence [12] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[12] = rtc.getTimeStr();
                   heure_sortie[12] = "";
                    jourdelasemaine_arrivee [12] =rtc.getDOWStr () ;  
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                         enregistrementdatainsdcard ("13.txt" , "EMMANUEL" , rtc.getDateStr() , heure_arrive[12] , "ARRIVEE" ,jourdelasemaine_arrivee [12]);

                         agent.publish("EMMANUEL ACTIF");
                         
                    }                    
                if  (presence [12] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[12] = rtc.getTimeStr();   
                    jourdelasemaine_sortie [12] = rtc.getDOWStr();
                   
                    temps_moi_presence[12]=temps_moi_presence[12]+calculminutesagent(heure_arrive[12],heure_sortie[12],jourdelasemaine_arrivee[12],jourdelasemaine_sortie[12]); 
                   
             
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [12] = 0 ;

                           enregistrementdatainsdcard ("13.txt" , "EMMANUEL" , rtc.getDateStr() , heure_sortie[12] , "SORTIE" ,jourdelasemaine_sortie[12]);
                         sautdeligne ("13.txt");

                          agent.publish("EMMANUEL NON ACTIF");
                                         
                }
          }


         

           if (informationAgent.equals("RODRIGUE MUMBERE")) {
                 presence [13] = presence [13] + 1 ;
                    if (presence [13] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[13] = rtc.getTimeStr();
                   heure_sortie[13] = "";
                    jourdelasemaine_arrivee [13] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    "); 
                         enregistrementdatainsdcard ("14.txt" , "RODRIGUE MUMBERE" , rtc.getDateStr() , heure_arrive[13] , "ARRIVEE" ,jourdelasemaine_arrivee[13]); 

                         agent.publish("RODRIGUE MUMBERE ACTIF");
                    }                    
                if  (presence [13] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[13] = rtc.getTimeStr();   
                    jourdelasemaine_sortie [13] = rtc.getDOWStr();

                    temps_moi_presence[13]=temps_moi_presence[13]+calculminutesagent(heure_arrive[13],heure_sortie[13],jourdelasemaine_arrivee[13],jourdelasemaine_sortie[13]);
                    
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [13] = 0 ;
                         enregistrementdatainsdcard ("14.txt" , "RODRIGUE MUMBERE" , rtc.getDateStr() , heure_sortie[13] , "SORTIE" ,jourdelasemaine_sortie[13]);
                         sautdeligne ("14.txt");

                         agent.publish("RODRIGUE MUMBERE NON ACTIF"); 
                                        
                }
          }

          if (informationAgent.equals("EDWINE LUKULIRWA")) {
                 presence [14] = presence [14] + 1 ;
                    if (presence [14] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[14] = rtc.getTimeStr();
                   heure_sortie[14] = ""; 
                    jourdelasemaine_arrivee [14] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    "); 
                         enregistrementdatainsdcard ("15.txt" , "EDWINE LUKULIRWA" , rtc.getDateStr() , heure_arrive[14] , "ARRIVEE" ,jourdelasemaine_arrivee [14]); 

                         agent.publish("EDWINE LUKULIRWA ACTIF");  
                    }                    
                if  (presence [14] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[14] = rtc.getTimeStr();   
                    jourdelasemaine_sortie [14] = rtc.getDOWStr();
                    
                    temps_moi_presence[14]=temps_moi_presence[14]+calculminutesagent(heure_arrive[14],heure_sortie[14],jourdelasemaine_arrivee[14],jourdelasemaine_sortie[14]);
                    
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [14] = 0 ; 
                         enregistrementdatainsdcard ("15.txt" , "EDWINE LUKULIRWA" , rtc.getDateStr() , heure_sortie[14] , "SORTIE" ,jourdelasemaine_sortie[14]);
                         sautdeligne ("15.txt");

                         agent.publish("EDWINE LUKULIRWA NON ACTIF");
                                         
                }
          }


           if (informationAgent.equals("KUBUYA ")) {
                 presence [15] = presence [15] + 1 ;
                    if (presence [15] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[15] = rtc.getTimeStr();
                   heure_sortie[15] = "";
                    jourdelasemaine_arrivee [15] =rtc.getDOWStr () ;  
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         enregistrementdatainsdcard ("16.txt" , "KUBUYA" , rtc.getDateStr() , heure_arrive[15] , "ARRIVEE" , jourdelasemaine_arrivee [15]); 

                          agent.publish("KUBUYA  ACTIF");
                    }                    
                if  (presence [15] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[15] = rtc.getTimeStr();   
                    jourdelasemaine_sortie [15] = rtc.getDOWStr();

                    temps_moi_presence[15]=temps_moi_presence[15]+calculminutesagent(heure_arrive[15],heure_sortie[15],jourdelasemaine_arrivee[15],jourdelasemaine_sortie[15]);
                    
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [15] = 0 ;
                         enregistrementdatainsdcard ("16.txt" , "KUBUYA" , rtc.getDateStr() , heure_sortie[15] , "SORTIE" ,jourdelasemaine_sortie[15]);
                         sautdeligne ("16.txt");

                         agent.publish("KUBUYA  NON ACTIF");
                                        
                }
          }


           if (informationAgent.equals("CHRISTIAN BALEKAGUE")) {
                 presence [16] = presence [16] + 1 ;
                    if (presence [16] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[16] = rtc.getTimeStr();
                   heure_sortie[16] = "";
                   jourdelasemaine_arrivee [16] =rtc.getDOWStr();
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                         enregistrementdatainsdcard ("17.txt" , "CHRISTIAN BALEKAGUE" , rtc.getDateStr() , heure_arrive[16] , "ARRIVEE" ,jourdelasemaine_arrivee [16]);

                         agent.publish("CHRISTIAN BALEKAGUE ACTIF");
                    }                    
                if  (presence [16] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                        heure_sortie[16] = rtc.getTimeStr();
                       jourdelasemaine_sortie [16] = rtc.getDOWStr();
                       
                       temps_moi_presence[16]=temps_moi_presence[16]+calculminutesagent(heure_arrive[16],heure_sortie[16],jourdelasemaine_arrivee[16],jourdelasemaine_sortie[16]);
                       
                       
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [16] = 0;
                         enregistrementdatainsdcard ("17.txt" , "CHRISTIAN BALEKAGUE" , rtc.getDateStr() , heure_sortie[16] , "SORTIE" ,jourdelasemaine_sortie[16]);
                         sautdeligne ("17.txt");

                          agent.publish("CHRISTIAN BALEKAGUE NON ACTIF");
                                        
                }
          }

             if (informationAgent.equals("JUSTIN")) {
                 presence [17] = presence [17] + 1 ;
                    if (presence [17] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[17] = rtc.getTimeStr();
                   heure_sortie[17] = "";
                    jourdelasemaine_arrivee [17] =rtc.getDOWStr () ;
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                         enregistrementdatainsdcard ("18.txt" , "JUSTIN" , rtc.getDateStr() , heure_arrive[17] , "ARRIVEE" , jourdelasemaine_arrivee [17]);

                         agent.publish("JUSTIN ACTIF");
                    }                    
                if  (presence [17] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[17] = rtc.getTimeStr();
                    jourdelasemaine_sortie [17] = rtc.getDOWStr();  
                   
                    temps_moi_presence[17]=temps_moi_presence[17]+calculminutesagent(heure_arrive[17],heure_sortie[17],jourdelasemaine_arrivee[17],jourdelasemaine_sortie[17]);
                     
                     
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [17] = 0; 
                        enregistrementdatainsdcard ("18.txt" , "JUSTIN" , rtc.getDateStr() , heure_sortie[17] , "SORTIE" , jourdelasemaine_sortie[17]);
                         sautdeligne ("18.txt");

                         agent.publish("JUSTIN NON ACTIF");
                }
          }


           if (informationAgent.equals("KAMBALE MUPIKA")) {
                 presence [18] = presence [18] + 1 ;
                    if (presence [18] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[18] = rtc.getTimeStr();
                   heure_sortie[18] = ""; 
                    jourdelasemaine_arrivee [18] =rtc.getDOWStr () ; 
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                         enregistrementdatainsdcard ("19.txt" , "KAMBALE MUPIKA" , rtc.getDateStr() , heure_arrive[18] , "ARRIVEE" ,jourdelasemaine_arrivee [18] );

                         agent.publish("KAMBALE MUPIKA ACTIF"); 
                    }                    
                if  (presence [18] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[18] = rtc.getTimeStr();   
                   jourdelasemaine_sortie [18] = rtc.getDOWStr();
                   
                   temps_moi_presence[18]=temps_moi_presence[18]+calculminutesagent(heure_arrive[18],heure_sortie[18],jourdelasemaine_arrivee[18],jourdelasemaine_sortie[18]);
                   
                   
                     
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [18] = 0 ;
                         enregistrementdatainsdcard ("19.txt" , "KAMBALE MUPIKA" , rtc.getDateStr() , heure_sortie[18] , "SORTIE" ,jourdelasemaine_sortie[18]);
                         sautdeligne ("19.txt");

                          agent.publish("KAMBALE MUPIKA NON ACTIF"); 
                                          
                }
          }


           if (informationAgent.equals("ALEXIS MALIRA")) {
                 presence [19] = presence [19] + 1 ;
                    if (presence [19] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[19] = rtc.getTimeStr();
                   heure_sortie[19] = "";  
                    jourdelasemaine_arrivee [19] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                         enregistrementdatainsdcard ("20.txt" , "ALEXIS MALIRA" , rtc.getDateStr() , heure_arrive[19] , "ARRIVEE" ,jourdelasemaine_arrivee [19]);

                         agent.publish("ALEXIS MALIRA ACTIF"); 
                    }                    
                if  (presence [19] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[19] = rtc.getTimeStr();
                    jourdelasemaine_sortie [19] = rtc.getDOWStr();
                    
                    temps_moi_presence[19]=temps_moi_presence[19]+calculminutesagent(heure_arrive[19],heure_sortie[19],jourdelasemaine_arrivee[19],jourdelasemaine_sortie[19]);
                       
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [19] = 0 ; 
                         enregistrementdatainsdcard ("20.txt" , "ALEXIS MALIRA" , rtc.getDateStr() , heure_sortie[19] , "SORTIE" ,jourdelasemaine_sortie[19]);
                         sautdeligne ("20.txt");

                         agent.publish("ALEXIS MALIRA NON ACTIF"); 
                                         
                }
          }

           if (informationAgent.equals("VICARD CARDTEST")) {
                 presence [20] = presence [20] + 1 ;
                    if (presence [20] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[20] = rtc.getTimeStr();
                   heure_sortie[20] = "";  
                    jourdelasemaine_arrivee [20] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                         enregistrementdatainsdcard ("21.txt" , "VICARD CARDTEST" , rtc.getDateStr() , heure_arrive[20] , "ARRIVEE" ,jourdelasemaine_arrivee [20]);

                         agent.publish("VICARD CARDTEST ACTIF"); 
                    }                    
                if  (presence [20] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[20] = rtc.getTimeStr();
                    jourdelasemaine_sortie [20] = rtc.getDOWStr();
                    
                    temps_moi_presence[20]=temps_moi_presence[20]+calculminutesagent(heure_arrive[20],heure_sortie[20],jourdelasemaine_arrivee[20],jourdelasemaine_sortie[20]);
                       
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [20] = 0 ; 
                         enregistrementdatainsdcard ("21.txt" , "VICARD CARDTEST" , rtc.getDateStr() , heure_sortie[20] , "SORTIE" ,jourdelasemaine_sortie[20]);
                         sautdeligne ("21.txt");

                         agent.publish("VICARD CARDTEST NON ACTIF"); 
                                         
                }
          }


           if (informationAgent.equals("PRINCE KAMBALE")) {
                 presence [21] = presence [21] + 1 ;
                    if (presence [21] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[21] = rtc.getTimeStr();
                   heure_sortie[21] = ""; 
                    jourdelasemaine_arrivee [21] =rtc.getDOWStr () ; 
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         enregistrementdatainsdcard ("22.txt" , "PRINCE KAMBALE" , rtc.getDateStr() , heure_arrive[21] , "ARRIVEE" ,jourdelasemaine_arrivee [21]);

                         agent.publish("PRINCE KAMBALE ACTIF"); 
 
                    }                    
                if  (presence [21] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[21] = rtc.getTimeStr();   
                    jourdelasemaine_sortie [21] = rtc.getDOWStr();

                    temps_moi_presence[21]=temps_moi_presence[21]+calculminutesagent(heure_arrive[21],heure_sortie[21],jourdelasemaine_arrivee[21],jourdelasemaine_sortie[21]);
                    
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [21] = 0 ;
                          enregistrementdatainsdcard ("22.txt" , "PRINCE KAMBALE" , rtc.getDateStr() , heure_sortie[21] , "SORTIE" ,jourdelasemaine_sortie[21]);
                         sautdeligne ("22.txt");

                         agent.publish("PRINCE KAMBALE NON ACTIF");
                }
          }

          
      if (informationAgent.equals("BLANCHARD VALISALIRE")) {
                 presence [22] = presence [22] + 1 ;
                    if (presence [22] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[22] = rtc.getTimeStr();
                   heure_sortie[22] = ""; 
                    jourdelasemaine_arrivee [22] =rtc.getDOWStr () ; 
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                         enregistrementdatainsdcard ("23.txt" , "BLANCHARD VALISALIRE" , rtc.getDateStr() , heure_arrive[22] , "ARRIVEE" ,jourdelasemaine_arrivee [22]);

                         agent.publish("BLANCHARD VALISALIRE ACTIF");
                    }                    
                if  (presence [22] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[22] = rtc.getTimeStr();
                    jourdelasemaine_sortie [22] = rtc.getDOWStr(); 
                     
                    temps_moi_presence[22]=temps_moi_presence[22]+calculminutesagent(heure_arrive[22],heure_sortie[22],jourdelasemaine_arrivee[22],jourdelasemaine_sortie[22]); 
                   
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [22] = 0 ;
                         enregistrementdatainsdcard ("23.txt" , "BLANCHARD VALISALIRE" , rtc.getDateStr() , heure_sortie[22] , "SORTIE" ,jourdelasemaine_sortie[22]);
                         sautdeligne ("23.txt");

                         agent.publish("BLANCHARD VALISALIRE NON ACTIF");
                }
          }


          if (informationAgent.equals("RIGO   KASEREKA")) {
                 presence [23] = presence [23] + 1 ;
                    if (presence [23] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[23] = rtc.getTimeStr();
                   heure_sortie[23] = "";  
                    jourdelasemaine_arrivee [23] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                          enregistrementdatainsdcard ("24.txt" , "RIGO KASEREKA" , rtc.getDateStr() , heure_arrive[23] , "ARRIVEE" ,jourdelasemaine_arrivee[23]);

                          agent.publish("RIGO KASEREKA ACTIF");
                    }                    
                if  (presence [23] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[23] = rtc.getTimeStr();
                    jourdelasemaine_sortie [23] = rtc.getDOWStr();  
                    
                    temps_moi_presence[23]=temps_moi_presence[23]+calculminutesagent(heure_arrive[23],heure_sortie[23],jourdelasemaine_arrivee[23],jourdelasemaine_sortie[23]); 
                    
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [23] = 0 ;
                         enregistrementdatainsdcard ("24.txt" , "RIGO KASEREKA" , rtc.getDateStr() , heure_sortie[23] , "SORTIE" ,jourdelasemaine_sortie[23]);
                         sautdeligne ("24.txt");

                          agent.publish("RIGO KASEREKA NON ACTIF");
                                    
                }
          }


          
          if (informationAgent.equals("SAMY KAMBALE")) {
                 presence [24] = presence [24] + 1 ;
                    if (presence [24] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[24] = rtc.getTimeStr();
                   heure_sortie[24] = ""; 
                    jourdelasemaine_arrivee [24] =rtc.getDOWStr () ; 
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                         enregistrementdatainsdcard ("25.txt" , "SAMY KAMBALE" , rtc.getDateStr() , heure_arrive[24] , "ARRIVEE" ,jourdelasemaine_arrivee[24]);

                         agent.publish("SAMY KAMBALE ACTIF");
                    }                    
                if  (presence [24] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[24] = rtc.getTimeStr(); 
                    jourdelasemaine_sortie [24] = rtc.getDOWStr();
                      
                    temps_moi_presence[24]=temps_moi_presence[24]+calculminutesagent(heure_arrive[24],heure_sortie[24],jourdelasemaine_arrivee[24],jourdelasemaine_sortie[24]);
                     
                     
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [24] = 0 ;
                         enregistrementdatainsdcard ("25.txt" , "SAMY KAMBALE" , rtc.getDateStr() , heure_sortie[24] , "SORTIE" ,jourdelasemaine_sortie[24]);
                         sautdeligne ("25.txt");  

                         agent.publish("SAMY KAMBALE NON ACTIF");
                                       
                }
          }
          if (informationAgent.equals("ERIC SHABANTU")) {
                 presence [25] = presence [25] + 1 ;
                    if (presence [25] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[25] = rtc.getTimeStr();
                   heure_sortie[25] = "";  
                    jourdelasemaine_arrivee [25] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         enregistrementdatainsdcard ("26.txt" , "ERIC SHABANTU" , rtc.getDateStr() , heure_arrive[25] , "ARRIVEE" ,jourdelasemaine_arrivee[25]); 

                          agent.publish("ERIC SHABANTU ACTIF");
                    }                    
                if  (presence [25] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[25] = rtc.getTimeStr();
                    jourdelasemaine_sortie [25] = rtc.getDOWStr(); 
                    
                    temps_moi_presence[25]=temps_moi_presence[25]+calculminutesagent(heure_arrive[25],heure_sortie[25],jourdelasemaine_arrivee[25],jourdelasemaine_sortie[25]);  
                   
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [25] = 0 ;
                         enregistrementdatainsdcard ("26.txt" , "ERIC SHABANTU" , rtc.getDateStr() , heure_sortie[25] , "SORTIE" ,jourdelasemaine_sortie[25]);
                         sautdeligne ("26.txt");

                         agent.publish("ERIC SHABANTU NON ACTIF");
                                        
                }
          }


               if (informationAgent.equals("CONSOLE KAVIRA")) {
                 presence [26] = presence [26] + 1 ;
                    if (presence [26] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[26] = rtc.getTimeStr();
                   heure_sortie[26] = "";
                    jourdelasemaine_arrivee [26] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    "); 
                         enregistrementdatainsdcard ("27.txt" , "CONSOLE KAVIRA" , rtc.getDateStr() , heure_arrive[26] , "ARRIVEE" ,jourdelasemaine_arrivee[26]);

                          agent.publish("CONSOLE KAVIRA ACTIF");
                    }                    
                if  (presence [26] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[26] = rtc.getTimeStr();  
                    jourdelasemaine_sortie [26] = rtc.getDOWStr(); 
                    
                    temps_moi_presence[26]=temps_moi_presence[26]+calculminutesagent(heure_arrive[26],heure_sortie[26],jourdelasemaine_arrivee[26],jourdelasemaine_sortie[26]);
                   
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [26] = 0 ;
                         enregistrementdatainsdcard ("27.txt" , "CONSOLE KAVIRA" , rtc.getDateStr() , heure_sortie[26] , "SORTIE" ,jourdelasemaine_sortie[26]);
                         sautdeligne ("27.txt");

                          agent.publish("CONSOLE KAVIRA NON ACTIF");
                                          
                }
          }


           if (informationAgent.equals("EZEKIEL MAMBO")) {
                 presence [27] = presence [27] + 1 ;
                    if (presence [27] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[27] = rtc.getTimeStr();
                   heure_sortie[27] = "";
                    jourdelasemaine_arrivee [27] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    "); 
                          enregistrementdatainsdcard ("28.txt" , "EZEKIEL MAMBO" , rtc.getDateStr() ,  heure_arrive[27] , "ARRIVEE" ,jourdelasemaine_arrivee[27]); 

                           agent.publish("EZEKIEL MAMBO ACTIF");
                    }                    
                if  (presence [27] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[27] = rtc.getTimeStr();
                    jourdelasemaine_sortie [27] = rtc.getDOWStr(); 
                    
                    temps_moi_presence[27]=temps_moi_presence[27]+calculminutesagent(heure_arrive[27],heure_sortie[27],jourdelasemaine_arrivee[27],jourdelasemaine_sortie[27]); 
                    
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [27] = 0 ;  
                         enregistrementdatainsdcard ("28.txt" , "EZEKIEL MAMBO" , rtc.getDateStr() , heure_sortie[27] , "SORTIE" ,jourdelasemaine_sortie[27]);
                         sautdeligne ("28.txt");

                          agent.publish("EZEKIEL MAMBO NON ACTIF");
                                        
                }
          }


          if (informationAgent.equals("MITTERAND KATEMBO")) {
                 presence [28] = presence [28] + 1 ;
                    if (presence [28] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[28] = rtc.getTimeStr();
                   heure_sortie[28] = ""; 
                    jourdelasemaine_arrivee [28] =rtc.getDOWStr () ; 
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                          enregistrementdatainsdcard ("29.txt" , "MITTERAND KATEMBO" , rtc.getDateStr() ,heure_arrive[28] , "ARRIVEE" ,jourdelasemaine_arrivee[28]);

                           agent.publish("MITTERAND KATEMBO ACTIF");
                    }                    
                if  (presence [28] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[28] = rtc.getTimeStr();  
                    jourdelasemaine_sortie [28] = rtc.getDOWStr();
                   
                    temps_moi_presence[28]=temps_moi_presence[28]+calculminutesagent(heure_arrive[28],heure_sortie[28],jourdelasemaine_arrivee[28],jourdelasemaine_sortie[28]);
                    
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [28] = 0 ;
                           enregistrementdatainsdcard ("29.txt" , "MITTERAND KATEMBO" , rtc.getDateStr() , heure_sortie[28] , "SORTIE" ,jourdelasemaine_sortie[28]);
                         sautdeligne ("29.txt");     

                         agent.publish("MITTERAND KATEMBO NON ACTIF");
                }
          }

       if (informationAgent.equals("YANNICK BAHATI")) {
                 presence [29] = presence [29] + 1 ;
                    if (presence [29] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[29] = rtc.getTimeStr();
                   heure_sortie[29] = ""; 
                    jourdelasemaine_arrivee [29] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                          enregistrementdatainsdcard ("30.txt" , "YANNICK BAHATI" , rtc.getDateStr() , heure_arrive[29] , "ARRIVEE" ,jourdelasemaine_arrivee[29]);

                          agent.publish("YANNICK BAHATI ACTIF");
                    }                    
                if  (presence [29] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[29] = rtc.getTimeStr();   
                    jourdelasemaine_sortie [29] = rtc.getDOWStr();
                    
                    temps_moi_presence[29]=temps_moi_presence[29]+calculminutesagent(heure_arrive[29],heure_sortie[29],jourdelasemaine_arrivee[29],jourdelasemaine_sortie[29]);
                    
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [29] = 0 ; 
                         enregistrementdatainsdcard ("30.txt" , "YANNICK BAHATI" , rtc.getDateStr() , heure_sortie[29] , "SORTIE" ,jourdelasemaine_sortie[29]);
                         sautdeligne ("30.txt");

                         agent.publish("YANNICK BAHATI NON ACTIF");
                                     
                }
          }


           if (informationAgent.equals("ARSENE KAMBALE")) {
                 presence [30] = presence [30] + 1 ;
                    if (presence [30] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[30] = rtc.getTimeStr();
                   heure_sortie[30] = "";  
                    jourdelasemaine_arrivee [30] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                         enregistrementdatainsdcard ("31.txt" , "ARSENE KAMBALE" , rtc.getDateStr() , heure_arrive[30] , "ARRIVEE" ,jourdelasemaine_arrivee[30]);

                         agent.publish("ARSENE KAMBALE ACTIF");
                    }                    
                if  (presence [30] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[30] = rtc.getTimeStr();
                   jourdelasemaine_sortie [30] = rtc.getDOWStr();
                  
                   temps_moi_presence[30]=temps_moi_presence[30]+calculminutesagent(heure_arrive[30],heure_sortie[30],jourdelasemaine_arrivee[30],jourdelasemaine_sortie[30]);
                   
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [30] = 0 ; 
                         enregistrementdatainsdcard ("31.txt" , "ARSENE KAMBALE" , rtc.getDateStr() , heure_sortie[30] , "SORTIE" ,jourdelasemaine_sortie[30]);
                         sautdeligne ("31.txt"); 

                         agent.publish("ARSENE KAMBALE NON ACTIF");
                                      
                }
          }

           if (informationAgent.equals("DONATIEN BIKUBA")) {
                 presence [31] = presence [31] + 1 ;
                    if (presence [31] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[31] = rtc.getTimeStr();
                   heure_sortie[31] = "";
                    jourdelasemaine_arrivee [31] =rtc.getDOWStr () ;  
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         enregistrementdatainsdcard ("32.txt" , "DONATIEN BIKUBA" , rtc.getDateStr() , heure_arrive[31] , "ARRIVEE" ,jourdelasemaine_arrivee[31]); 

                          agent.publish("DONATIEN BIKUBA ACTIF");
                    }                    
                if  (presence [31] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[31] = rtc.getTimeStr();  
                    jourdelasemaine_sortie [31] = rtc.getDOWStr();
                    
                    temps_moi_presence[31]=temps_moi_presence[31]+calculminutesagent(heure_arrive[31],heure_sortie[31],jourdelasemaine_arrivee[31],jourdelasemaine_sortie[31]);
                    
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [31] = 0 ;
                         enregistrementdatainsdcard ("32.txt" , "DONATIEN BIKUBA" , rtc.getDateStr() , heure_sortie[31] , "SORTIE" ,jourdelasemaine_sortie[31]);
                         sautdeligne ("32.txt");

                          agent.publish("DONATIEN BIKUBA NON ACTIF");

                                         
                }
          }

           if (informationAgent.equals("JOSEPH MUSHENGE")) {
                 presence [32] = presence [32] + 1 ;
                    if (presence [32] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[32] = rtc.getTimeStr();
                   heure_sortie[32] = ""; 
                    jourdelasemaine_arrivee [32] =rtc.getDOWStr ();
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    "); 
                         enregistrementdatainsdcard ("33.txt" , "JOSEPH MUSHENGE" , rtc.getDateStr() , heure_arrive[32] , "ARRIVEE" ,jourdelasemaine_arrivee [32]); 

                         agent.publish("JOSEPH MUSHENGE ACTIF"); 
                    }                    
                if  (presence [32] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                  heure_sortie[32] = rtc.getTimeStr();   
                  jourdelasemaine_sortie [32] = rtc.getDOWStr();
                  
                  temps_moi_presence[32]=temps_moi_presence[32]+calculminutesagent(heure_arrive[32],heure_sortie[32],jourdelasemaine_arrivee[32],jourdelasemaine_sortie[32]);
                  

                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [32] = 0 ;
                       enregistrementdatainsdcard ("33.txt" , "JOSEPH MUSHENGE" , rtc.getDateStr() , heure_sortie[32] , "SORTIE" ,jourdelasemaine_sortie[32]);
                         sautdeligne ("33.txt"); 

                         agent.publish("JOSEPH MUSHENGE NON ACTIF");
                                          
                }
          }

           if (informationAgent.equals("PATIENT MWAMBA      ")) {
                 presence [33] = presence [33] + 1 ;
                    if (presence [33] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[33] = rtc.getTimeStr();
                   heure_sortie[33] = ""; 
                    jourdelasemaine_arrivee [33] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                     enregistrementdatainsdcard ("34.txt" , "PATIENT MWAMBA" , rtc.getDateStr() , heure_arrive[33] , "ARRIVEE" ,jourdelasemaine_arrivee[33]);

                     agent.publish("PATIENT MWAMBA ACTIF"); 
                    }                    
                if  (presence [33] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[33] = rtc.getTimeStr(); 
                    jourdelasemaine_sortie [33] = rtc.getDOWStr();  
                    
                    temps_moi_presence[33]=temps_moi_presence[33]+calculminutesagent(heure_arrive[33],heure_sortie[33],jourdelasemaine_arrivee[33],jourdelasemaine_sortie[33]);
                    
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [33] = 0 ; 
                         enregistrementdatainsdcard ("34.txt" , "PATIENT MWAMBA" , rtc.getDateStr() , heure_sortie[33] , "SORTIE" ,jourdelasemaine_sortie[33]);
                         sautdeligne ("34.txt");

                          agent.publish("PATIENT MWAMBA NON ACTIF"); 
                                          
                }
          }


          if (informationAgent.equals("KAMBALE KASUKA")) {
                 presence [34] = presence [34] + 1 ;
                    if (presence [34] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[34] = rtc.getTimeStr();
                   heure_sortie[34] = "";
                    jourdelasemaine_arrivee[34] =rtc.getDOWStr () ; 
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                         enregistrementdatainsdcard ("35.txt" , "KAMBALE KASUKA" , rtc.getDateStr() , heure_arrive[34] , "ARRIVEE" ,jourdelasemaine_arrivee[34]);

                         agent.publish("KAMBALE KASUKA ACTIF"); 
                    }                    
                if  (presence [34] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[34] = rtc.getTimeStr();   
                    jourdelasemaine_sortie [34] = rtc.getDOWStr();
                   
                    temps_moi_presence[34]=temps_moi_presence[34]+calculminutesagent(heure_arrive[34],heure_sortie[34],jourdelasemaine_arrivee[34],jourdelasemaine_sortie[34]);
                    
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [34] = 0 ; 

                           enregistrementdatainsdcard ("35.txt" , "KAMBALE KASUKA" , rtc.getDateStr() , heure_sortie[34] , "SORTIE" ,jourdelasemaine_sortie[34]);
                           sautdeligne ("35.txt");

                           agent.publish("KAMBALE KASUKA NON ACTIF");
                                        
                }
          }


           if (informationAgent.equals("JULIEN KAMBALE")) {
                 presence [35] = presence [35] + 1 ;
                    if (presence [35] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[35] = rtc.getTimeStr();
                   heure_sortie[35] = ""; 
                    jourdelasemaine_arrivee [35] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                         enregistrementdatainsdcard ("36.txt" , "JULIEN KAMBALE" , rtc.getDateStr() , heure_arrive[35] , "ARRIVEE" ,jourdelasemaine_arrivee [35]);

                         agent.publish("JULIEN KAMBALE ACTIF");
                    }                    
                if  (presence [35] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[35] = rtc.getTimeStr(); 
                    jourdelasemaine_sortie [35] = rtc.getDOWStr(); 
                    
                    temps_moi_presence[35]=temps_moi_presence[35]+calculminutesagent(heure_arrive[35],heure_sortie[35],jourdelasemaine_arrivee[35],jourdelasemaine_sortie[35]); 
                    
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [35] = 0;

                         enregistrementdatainsdcard ("36.txt" , "JULIEN KAMBALE" , rtc.getDateStr() , heure_sortie[35] , "SORTIE" ,jourdelasemaine_sortie[35]);
                         sautdeligne ("36.txt");

                         agent.publish("JULIEN KAMBALE NON ACTIF");
                                          
                }
          }


           if (informationAgent.equals("KAMBALE KATAKA")) {
                 presence [36] = presence [36] + 1 ;
                    if (presence [36] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);
                   
                   heure_arrive[36] = rtc.getTimeStr();
                   heure_sortie[36] = ""; 
                    jourdelasemaine_arrivee [36] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                          enregistrementdatainsdcard ("37.txt" , "KAMBALE KATAKA" , rtc.getDateStr() , heure_arrive[36] , "ARRIVEE" ,jourdelasemaine_arrivee[36]);

                          agent.publish("KAMBALE KATAKA ACTIF");
                    }                    
                if  (presence [36] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[36] = rtc.getTimeStr();
                    jourdelasemaine_sortie [36] = rtc.getDOWStr();
                    
                    temps_moi_presence[36]=temps_moi_presence[36]+calculminutesagent(heure_arrive[36],heure_sortie[36],jourdelasemaine_arrivee[36],jourdelasemaine_sortie[36]);   
                     
                     
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [36] = 0 ; 
                         enregistrementdatainsdcard ("37.txt" , "KAMBALE KATAKA" , rtc.getDateStr() , heure_sortie[36] , "SORTIE" ,jourdelasemaine_sortie[36]);
                         sautdeligne ("37.txt");

                          agent.publish("KAMBALE KATAKA NON ACTIF");
                                        
                }
          }


           if (informationAgent.equals("DANNY KAKULE")) {
                 presence [37] = presence [37] + 1 ;
                    if (presence [37] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[37] = rtc.getTimeStr();
                   heure_sortie[37] = "";
                    jourdelasemaine_arrivee [37] =rtc.getDOWStr () ; 
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                          enregistrementdatainsdcard ("38.txt" , "DANNY KAKULE" , rtc.getDateStr() , heure_arrive[37] , "ARRIVEE" ,jourdelasemaine_arrivee [37]); 

                          agent.publish("DANNY KAKULE ACTIF");
                    }                    
                if  (presence [37] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[37] = rtc.getTimeStr();
                   jourdelasemaine_sortie [37] = rtc.getDOWStr();
                   
                   temps_moi_presence[37]=temps_moi_presence[37]+calculminutesagent(heure_arrive[37],heure_sortie[37],jourdelasemaine_arrivee[37],jourdelasemaine_sortie[37]);
                   
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [37] = 0 ; 
                          enregistrementdatainsdcard ("38.txt" , "DANNY KAKULE" , rtc.getDateStr() , heure_sortie[37] , "SORTIE" ,jourdelasemaine_sortie[37]);
                          sautdeligne ("38.txt");

                          agent.publish("DANNY KAKULE NON ACTIF");
                                        
                }
          }


          if (informationAgent.equals("ERIC KAMUSWEKERE")) {
                 presence [38] = presence [38] + 1 ;
                    if (presence [38] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[38] = rtc.getTimeStr();
                   heure_sortie[38] = "";
                    jourdelasemaine_arrivee [38] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    "); 
                         enregistrementdatainsdcard ("39.txt" , "ERIC KAMUSWEKERE" , rtc.getDateStr() , heure_arrive[38] , "ARRIVEE" ,jourdelasemaine_arrivee [38]); 

                         agent.publish("ERIC KAMUSWEKERE ACTIF");
                    }                    
                if  (presence [38] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[38] = rtc.getTimeStr(); 
                    jourdelasemaine_sortie [38] = rtc.getDOWStr();  
                    
                    temps_moi_presence[38]=temps_moi_presence[38]+calculminutesagent(heure_arrive[38],heure_sortie[38],jourdelasemaine_arrivee[38],jourdelasemaine_sortie[38]);
                     
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [38] = 0 ;
                          enregistrementdatainsdcard ("39.txt" , "ERIC KAMUSWEKERE" , rtc.getDateStr() , heure_sortie[38] , "SORTIE" ,jourdelasemaine_sortie[38]);
                          sautdeligne ("39.txt");

                          agent.publish("ERIC KAMUSWEKERE NON ACTIF");
                                         
                }
          }

            if (informationAgent.equals("ALEXIS")) {
                 presence [39] = presence [39] + 1 ;
                    if (presence [39] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[39] = rtc.getTimeStr();
                   heure_sortie[39] = "";  
                    jourdelasemaine_arrivee [39] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    "); 
                         enregistrementdatainsdcard ("40.txt" , "ALEXIS" , rtc.getDateStr() , heure_arrive[39] , "ARRIVEE" , jourdelasemaine_arrivee[39]);

                         agent.publish("ALEXIS ACTIF");
                          
                    }                    
                if  (presence [39] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[39] = rtc.getTimeStr();
                    jourdelasemaine_sortie [39] = rtc.getDOWStr();
                    
                    temps_moi_presence[39]=temps_moi_presence[39]+calculminutesagent(heure_arrive[39],heure_sortie[39],jourdelasemaine_arrivee[39],jourdelasemaine_sortie[39]);
                    
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [39] = 0 ; 

                         enregistrementdatainsdcard ("40.txt" , "ALEXIS" , rtc.getDateStr() , heure_sortie[39] , "SORTIE" ,jourdelasemaine_sortie[39]);
                          sautdeligne ("40.txt");

                          agent.publish("ALEXIS NON ACTIF");
                                          
                }
          }

           if (informationAgent.equals("NZANZU")) {
                 presence [40] = presence [40] + 1 ;
                    if (presence [40] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[40] = rtc.getTimeStr();
                   heure_sortie[40] = "";  
                    jourdelasemaine_arrivee [40] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                         enregistrementdatainsdcard ("41.txt" , "NZANZU" , rtc.getDateStr() , heure_arrive[40] , "ARRIVEE" ,jourdelasemaine_arrivee[40] );

                         agent.publish("NZANZU ACTIF");
                    }                    
                if  (presence [40] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[40] = rtc.getTimeStr(); 
                    jourdelasemaine_sortie [40] = rtc.getDOWStr();
                    
                    temps_moi_presence[40]=temps_moi_presence[40]+calculminutesagent(heure_arrive[40],heure_sortie[40],jourdelasemaine_arrivee[40],jourdelasemaine_sortie[40]);
                      
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [40] = 0 ;  
                         enregistrementdatainsdcard ("41.txt" , "NZANZU" , rtc.getDateStr() , heure_sortie[40] , "SORTIE" ,jourdelasemaine_sortie[40]);
                          sautdeligne ("41.txt");

                          agent.publish("NZANZU NON ACTIF");
                                   
                }
          }


           if (informationAgent.equals("PASCAL")) {
                 presence [41] = presence [41] + 1 ;
                    if (presence [41] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[41] = rtc.getTimeStr();
                   heure_sortie[41] = "";
                    jourdelasemaine_arrivee [41] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                         enregistrementdatainsdcard ("42.txt" , "PASCAL" , rtc.getDateStr() ,heure_arrive[41] , "ARRIVEE" ,jourdelasemaine_arrivee [41]);

                         agent.publish("PASCAL ACTIF");
                    }                    
                if  (presence [41] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[41] = rtc.getTimeStr();
                    jourdelasemaine_sortie [41] = rtc.getDOWStr();
                    
                    temps_moi_presence[41]=temps_moi_presence[41]+calculminutesagent(heure_arrive[41],heure_sortie[41],jourdelasemaine_arrivee[41],jourdelasemaine_sortie[41]);
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [41] = 0 ;
                          enregistrementdatainsdcard ("42.txt" , "PASCAL" , rtc.getDateStr() , heure_sortie[41] , "SORTIE" ,jourdelasemaine_sortie[41]);
                          sautdeligne ("42.txt"); 

                          agent.publish("PASCAL NON ACTIF");
                          
                                          
                }
          }



          
           if (informationAgent.equals("MICHEL MISHONA")) {
                 presence [42] = presence [42] + 1 ;
                    if (presence [42] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[42] = rtc.getTimeStr();
                   heure_sortie[42] = "";  
                    jourdelasemaine_arrivee [42] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                         enregistrementdatainsdcard ("43.txt" , "MICHEL MISHONA" , rtc.getDateStr() , heure_arrive[42] , "ARRIVEE" ,jourdelasemaine_arrivee[42]);

                         agent.publish("MICHEL MISHONA ACTIF");
                         
                    }                    
                if  (presence [42] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[42] = rtc.getTimeStr();  
                    jourdelasemaine_sortie [42] = rtc.getDOWStr();
                    
                    temps_moi_presence[42]=temps_moi_presence[42]+calculminutesagent(heure_arrive[42],heure_sortie[42],jourdelasemaine_arrivee[42],jourdelasemaine_sortie[42]);
                   
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [42] = 0 ; 
                         enregistrementdatainsdcard ("43.txt" , "MICHEL MISHONA" , rtc.getDateStr() , heure_sortie[42] , "SORTIE" ,jourdelasemaine_sortie[42]);
                          sautdeligne ("43.txt"); 

                          agent.publish("MICHEL MISHONA NON ACTIF");
                                          
                }
          }


         

           if (informationAgent.equals("CHRISTIAN BAKWANAMAH")) {
                 presence [43] = presence [43] + 1 ;
                    if (presence [43] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[43] = rtc.getTimeStr();
                   heure_sortie[43] = ""; 
                    jourdelasemaine_arrivee [43] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         enregistrementdatainsdcard ("44.txt" , "CHRISTIAN BAKWANAMAH" , rtc.getDateStr() , heure_arrive[43] , "ARRIVEE" ,jourdelasemaine_arrivee[43]);

                         agent.publish("CHRISTIAN BAKWANAMAH ACTIF");
                    }                    
                if  (presence [43] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[43] = rtc.getTimeStr();
                    jourdelasemaine_sortie [43] = rtc.getDOWStr();
                    
                    temps_moi_presence[43]=temps_moi_presence[43]+calculminutesagent(heure_arrive[43],heure_sortie[43],jourdelasemaine_arrivee[43],jourdelasemaine_sortie[43]);
                    
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [43] = 0 ;
                         enregistrementdatainsdcard ("44.txt" , "CHRISTIAN BAKWANAMAH" , rtc.getDateStr() , heure_sortie[43] , "SORTIE" ,jourdelasemaine_sortie[43]);
                          sautdeligne ("44.txt");

                           agent.publish("CHRISTIAN BAKWANAMAH NON ACTIF");
                                          
                }
          }

          if (informationAgent.equals("ARON ZUMBU")) {
                 presence [44] = presence [44] + 1 ;
                    if (presence [44] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);


                   heure_arrive[44] = rtc.getTimeStr();
                   heure_sortie[44] = "";  
                    jourdelasemaine_arrivee [44] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                          enregistrementdatainsdcard ("45.txt" , "ARON ZUMBU" , rtc.getDateStr() , heure_arrive[44] , "ARRIVEE" ,jourdelasemaine_arrivee [44]); 

                           agent.publish("ARON ZUMBU ACTIF");
                    }                    
                if  (presence [44] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[44] = rtc.getTimeStr(); 
                    jourdelasemaine_sortie [44] = rtc.getDOWStr();
                    
                    temps_moi_presence[44]=temps_moi_presence[44]+calculminutesagent(heure_arrive[44],heure_sortie[44],jourdelasemaine_arrivee[44],jourdelasemaine_sortie[44]);
                      
                      
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [44] = 0 ; 
                          enregistrementdatainsdcard ("45.txt" , "ARON ZUMBU" , rtc.getDateStr() , heure_sortie[44] , "SORTIE" ,jourdelasemaine_sortie[44]);
                          sautdeligne ("45.txt"); 

                          agent.publish("ARON ZUMBU NON ACTIF");
                                          
                }
          }

           if (informationAgent.equals("JAMES")) {
                 presence [45] = presence [45] + 1 ;
                    if (presence [45] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[45] = rtc.getTimeStr();
                   heure_sortie[45] = "";  
                    jourdelasemaine_arrivee [45] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                         enregistrementdatainsdcard ("46.txt" , "JAMES" , rtc.getDateStr() , heure_arrive[45] , "ARRIVEE" ,jourdelasemaine_arrivee[45]);

                         agent.publish("JAMES ACTIF");
                    }                    
                if  (presence [45] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[45] = rtc.getTimeStr();   
                    jourdelasemaine_sortie [45] = rtc.getDOWStr();
                    
                    temps_moi_presence[45]=temps_moi_presence[45]+calculminutesagent(heure_arrive[45],heure_sortie[45],jourdelasemaine_arrivee[45],jourdelasemaine_sortie[45]);
                    
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [45] = 0 ;
                         enregistrementdatainsdcard ("46.txt" , "JAMES" , rtc.getDateStr() , heure_sortie[45] , "SORTIE" ,jourdelasemaine_sortie[45]);
                          sautdeligne ("46.txt"); 

                          agent.publish("JAMES NON ACTIF");
                                          
                }
          }



          if (informationAgent.equals("SEMIDA KAVIRA")) {
                 presence [46] = presence [46] + 1 ;
                    if (presence [46] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[46] = rtc.getTimeStr();
                   heure_sortie[46] = "";  
                    jourdelasemaine_arrivee [46] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    "); 
                         enregistrementdatainsdcard ("47.txt" , "SEMIDA KAVIRA" , rtc.getDateStr() ,heure_arrive[46] , "ARRIVEE" ,jourdelasemaine_arrivee[46]);

                          agent.publish("SEMIDA KAVIRA ACTIF");
                    }                    
                if  (presence [46] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[46] = rtc.getTimeStr();   
                    jourdelasemaine_sortie [46] = rtc.getDOWStr();
                   
                    temps_moi_presence[46]=temps_moi_presence[46]+calculminutesagent(heure_arrive[46],heure_sortie[46],jourdelasemaine_arrivee[46],jourdelasemaine_sortie[46]);
                    
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [46] = 0 ;
                         enregistrementdatainsdcard ("47.txt" , "SEMIDA KAVIRA" , rtc.getDateStr() , heure_sortie[46] , "SORTIE" ,jourdelasemaine_sortie[46]);
                          sautdeligne ("47.txt");  

                        agent.publish("SEMIDA KAVIRA NON ACTIF");
                                                          
                }
          }

          if (informationAgent.equals("ALEX TSONGO")) {
                 presence [47] = presence [47] + 1 ;
                    if (presence [47] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[47] = rtc.getTimeStr();
                   heure_sortie[47] = "";  
                    jourdelasemaine_arrivee [47] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    "); 
                    enregistrementdatainsdcard ("48.txt" , "ALEX TSONGO" , rtc.getDateStr() , heure_arrive[47]  , "ARRIVEE" ,jourdelasemaine_arrivee[47]); 

                    agent.publish("ALEX TSONGO ACTIF");
                    
                    }                    
                if  (presence [47] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[47] = rtc.getTimeStr();
                    jourdelasemaine_sortie [47] = rtc.getDOWStr();
                    
                    temps_moi_presence[47]=temps_moi_presence[47]+calculminutesagent(heure_arrive[47],heure_sortie[47],jourdelasemaine_arrivee[47],jourdelasemaine_sortie[47]);   
                    
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [47] = 0 ; 
                         enregistrementdatainsdcard ("48.txt" , "ALEX TSONGO" , rtc.getDateStr() , heure_sortie[47] , "SORTIE" ,jourdelasemaine_sortie[47]);
                         sautdeligne ("48.txt");

                         agent.publish("ALEX TSONGO NON ACTIF");
                                      
                }
          }


           if (informationAgent.equals("GLORIA MALEMO")) {
                 presence [48] = presence [48] + 1 ;
                    if (presence [48] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[48] = rtc.getTimeStr();
                   heure_sortie[48] = "";  
                    jourdelasemaine_arrivee [48] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                          enregistrementdatainsdcard ("49.txt" , "GLORIA MALEMO" , rtc.getDateStr() ,  heure_arrive[48]  , "ARRIVEE" ,jourdelasemaine_arrivee[48]);

                          agent.publish("GLORIA MALEMO ACTIF");
                    }                    
                if  (presence [48] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[48] = rtc.getTimeStr();  
                    jourdelasemaine_sortie [48] = rtc.getDOWStr(); 
                    
                    temps_moi_presence[48]=  temps_moi_presence[48]+calculminutesagent(heure_arrive[48],heure_sortie[48],jourdelasemaine_arrivee[48],jourdelasemaine_sortie[48]); 
                    
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [48] = 0 ;
                          enregistrementdatainsdcard ("49.txt" , "GLORIA MALEMO" , rtc.getDateStr() , heure_sortie[48] , "SORTIE" ,jourdelasemaine_sortie[48]);
                         sautdeligne ("49.txt");

                         agent.publish("GLORIA MALEMO NON ACTIF");
                        
                
                }
          }

          if (informationAgent.equals("GYLAINE NZIAVAKE")) {
                 presence [49] = presence [49] + 1 ;
                    if (presence [49] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[49] = rtc.getTimeStr();
                   heure_sortie[49] = "";  
                    jourdelasemaine_arrivee [49] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                          enregistrementdatainsdcard ("50.txt" , "GYLAINE NZIAVAKE" , rtc.getDateStr() , heure_arrive[49] , "ARRIVEE" ,jourdelasemaine_arrivee[49]); 
                          
                          agent.publish("GYLAINE NZIAVAKE ACTIF"); 
                    }                    
                if  (presence [49] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[49] = rtc.getTimeStr();   
                    jourdelasemaine_sortie [49] = rtc.getDOWStr();
                    
                    temps_moi_presence[49]=temps_moi_presence[49]+calculminutesagent(heure_arrive[49],heure_sortie[49],jourdelasemaine_arrivee[49],jourdelasemaine_sortie[49]);
                    
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [49] = 0 ;
                         enregistrementdatainsdcard ("50.txt" , "GYLAINE NZIAVAKE" , rtc.getDateStr() , heure_sortie[49] , "SORTIE" ,jourdelasemaine_sortie[49]);
                         sautdeligne ("50.txt");

                         agent.publish("GYLAINE NZIAVAKE NON ACTIF"); 
                                        
                }
          }


           if (informationAgent.equals("JEAN MARIE BISIHIWA")) {
                 presence [50] = presence [50] + 1 ;
                    if (presence [50] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[50] = rtc.getTimeStr();
                   heure_sortie[50] = "";  
                    jourdelasemaine_arrivee [50] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                       enregistrementdatainsdcard ("51.txt" , "JEAN MARIE BISIHIWA" , rtc.getDateStr() , heure_arrive[50] , "ARRIVEE" ,jourdelasemaine_arrivee [50]);

                       agent.publish("JEAN MARIE BISIHIWA ACTIF");
                    }                    
                if  (presence [50] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[50] = rtc.getTimeStr();   
                    jourdelasemaine_sortie [50] = rtc.getDOWStr();
                    
                    temps_moi_presence[50]=temps_moi_presence[50]+calculminutesagent(heure_arrive[50],heure_sortie[50],jourdelasemaine_arrivee[50],jourdelasemaine_sortie[50]);
                     
                     
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [50] = 0 ; 
                         enregistrementdatainsdcard ("51.txt" , "JEAN MARIE BISIHIWA" , rtc.getDateStr() , heure_sortie[50] , "SORTIE" ,jourdelasemaine_sortie[50]);
                         sautdeligne ("51.txt");

                         agent.publish("JEAN MARIE BISIHIWA NON ACTIF");
                                         
                }
          }

      if (informationAgent.equals("THOMAS SEBISUSA")) {
                 presence [51] = presence [51] + 1 ;
                    if (presence [51] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[51] = rtc.getTimeStr();
                   heure_sortie[51] = "";  
                    jourdelasemaine_arrivee [51] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                          enregistrementdatainsdcard ("52.txt" , "THOMAS SEBISUSA" , rtc.getDateStr() , heure_arrive[51] , "ARRIVEE" ,jourdelasemaine_arrivee [51]);

                           agent.publish("THOMAS SEBISUSA ACTIF");
                    }                    
                if  (presence [51] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[51] = rtc.getTimeStr();   
                    jourdelasemaine_sortie [51] = rtc.getDOWStr();
                   
                    temps_moi_presence[51]=temps_moi_presence[51]+calculminutesagent(heure_arrive[51],heure_sortie[51],jourdelasemaine_arrivee[51],jourdelasemaine_sortie[51]);
                    
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [51] = 0 ;
                         enregistrementdatainsdcard ("52.txt" , "THOMAS SEBISUSA" , rtc.getDateStr() , heure_sortie[51] , "SORTIE" ,jourdelasemaine_sortie[51]);
                         sautdeligne ("52.txt");

                          agent.publish("THOMAS SEBISUSA NON ACTIF");
                }
          }


  
      if (informationAgent.equals("KAMBALE FATAKI")) {
                 presence [52] = presence [52] + 1 ;
                    if (presence [52] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[52] = rtc.getTimeStr();
                   heure_sortie[52] = "";  
                    jourdelasemaine_arrivee [52] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    "); 
                         enregistrementdatainsdcard ("53.txt" , "KAMBALE FATAKI" , rtc.getDateStr() , heure_arrive[52]  , "ARRIVEE" ,jourdelasemaine_arrivee [52]); 

                          agent.publish("KAMBALE FATAKI ACTIF");
                    }                    
                if  (presence [52] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[52] = rtc.getTimeStr();   
                    jourdelasemaine_sortie [52] = rtc.getDOWStr();
                    
                    temps_moi_presence[52]=temps_moi_presence[52]+calculminutesagent(heure_arrive[52],heure_sortie[52],jourdelasemaine_arrivee[52],jourdelasemaine_sortie[52]);
                   
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [52] = 0 ;
                         enregistrementdatainsdcard ("53.txt" , "KAMBALE FATAKI" , rtc.getDateStr() , heure_sortie[52] , "SORTIE" ,jourdelasemaine_sortie[52]);
                         sautdeligne ("53.txt"); 
                         
                            agent.publish("KAMBALE FATAKI NON ACTIF");             
                }
          }


          if (informationAgent.equals("HENRIQUE KABALA")) {
                 presence [53] = presence [53] + 1 ;
                    if (presence [53] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[53] = rtc.getTimeStr();
                   heure_sortie[53] = "";  
                    jourdelasemaine_arrivee [53] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                         enregistrementdatainsdcard ("54.txt" , "HENRIQUE KABALA" , rtc.getDateStr() , heure_arrive[53]  , "ARRIVEE" ,jourdelasemaine_arrivee [53]);

                          agent.publish("HENRIQUE KABALA ACTIF");
                    }                    
                if  (presence [53] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[53] = rtc.getTimeStr();   
                    jourdelasemaine_sortie [53] = rtc.getDOWStr();
                    
                    temps_moi_presence[53]=temps_moi_presence[53]+calculminutesagent(heure_arrive[53],heure_sortie[53],jourdelasemaine_arrivee[53],jourdelasemaine_sortie[53]);
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [53] = 0 ; 

                         enregistrementdatainsdcard ("54.txt" , "HENRIQUE KABALA" , rtc.getDateStr() , heure_sortie[53] , "SORTIE" ,jourdelasemaine_sortie[53]);
                         sautdeligne ("54.txt");

                          agent.publish("HENRIQUE KABALA NON ACTIF");


                                        
                }
          }

          if (informationAgent.equals("EMMANUEL MUHIGIRWA")) {
                 presence [54] = presence [54] + 1 ;
                    if (presence [54] == 1)  {
                  lcd.setCursor (0,2);
                  lcd.print ("BIENVENUE"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_arrive[54] = rtc.getTimeStr();
                   heure_sortie[54] = "";  
                    jourdelasemaine_arrivee [54] =rtc.getDOWStr () ;
                   
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");  
                         enregistrementdatainsdcard ("55.txt" , "EMMANUEL MUHIGIRWA" , rtc.getDateStr() ,  heure_arrive[54] , "ARRIVEE" ,jourdelasemaine_arrivee [54]);

                          agent.publish("EMMANUEL MUHIGIRWA ACTIF");
                         
                    }                    
                if  (presence [54] == 2) {
                  lcd.setCursor (0,2);
                  lcd.print ("AU REVOIR"); 
                lcd.setCursor (0,3); 
                   lcd.print (informationAgent);

                   heure_sortie[54] = rtc.getTimeStr();   
                    jourdelasemaine_sortie [54] = rtc.getDOWStr();
                    
                    temps_moi_presence[54]=temps_moi_presence[54]+calculminutesagent(heure_arrive[54],heure_sortie[54],jourdelasemaine_arrivee[54],jourdelasemaine_sortie[54]);
                   
                    
                      delay (3000); 
                        lcd.setCursor (0,2);
                       lcd.print("                    ");
                        lcd.setCursor (0,3); 
                         lcd.print ("                    ");
                         presence [54] = 0 ; 
                         enregistrementdatainsdcard ("55.txt" , "EMMANUEL MUHIGIRWA" , rtc.getDateStr() , heure_sortie[54] , "SORTIE" ,jourdelasemaine_sortie[54]);
                         sautdeligne ("55.txt");

                         agent.publish("EMMANUEL MUHIGIRWA NON ACTIF");


                                        
                }
          }

//           send_data_mqtt_cloud_agentactif();
//           send_data_mqtt_cloud_agentnonactif(); 
     
    }
    
    // server.handleClient();
}

//void handle_OnConnect() {
//  LEDstatus = LOW;
//  server.send(200, "text/html", SendHTML(false)); 
//}
//void handle_ledon() {
//  LEDstatus = HIGH;
//  server.send(200, "text/html", SendHTML(true)); 
//}
//
//void handle_ledoff() {
//  LEDstatus = LOW;
//  server.send(200, "text/html", SendHTML(false)); 
//}
//
//void handle_NotFound() {
//  server.send(404, "text/plain", "Not found");
//}
//
//String SendHTML(uint8_t led) {
//  
//   ptr = "<!DOCTYPE html>";
//  ptr +="<html>";
//  ptr +="<head>";
//
//   ptr += "<meta charset=\"utf-8\"/>" ; 
//  ptr +="<title>JERRYSON HOTEL</title>";
//  ptr +="</head>";
//  ptr +="<body style=background-color:powderblue>";
//
//
//   ptr += "<table width=\"1500\"height=\"50\"border=20 align=\"center\">"; 
//    ptr += "<caption>"; 
//
//     ptr += "<p><h1>JERRISON HOTEL</h1></p>";
//      ptr += "<p><h1>ATTENDANCE SYSTEM</h1></p>";
//
//    
//       ptr += "DATE:" ;
//     ptr += dateactuelle;
//      ptr += " HEURE:"; 
//       ptr += heureactuelle ; 
//        
//        
//       ptr += "</caption>";
//     
//     ptr += "<tr>"; 
//     ptr += "<td>NOMS</td>"; 
//     ptr += "<td>janvier</td>"; 
//     ptr += "<td> fevrier </td>"; 
//     ptr += " <td>mars</td>"; 
//     ptr += "<td>avril</td>";
//     ptr += "<td>mai</td>" ; 
//     ptr += "<td>juin</td>"; 
//     ptr += "<td>juillet</td>";
//     ptr += "<td>aout</td>"; 
//     ptr += " <td>septembre</td>" ; 
//      ptr += "<td>octobre</td>" ; 
//       ptr += "<td>novembre</td>";
//       ptr += "<td>decembre</td>";  
//        ptr += "</tr>"; 
//
//      ptr += "<tr>";
//      ptr += "<td>MERVEIL KATALIKO</td>";
//      affichagedatainwebsite (6,"January");  
//      affichagedatainwebsite (6,"February"); 
//      affichagedatainwebsite (6,"March"); 
//      affichagedatainwebsite (6,"April"); 
//      affichagedatainwebsite (6,"May"); 
//      affichagedatainwebsite (6,"June");
//       affichagedatainwebsite (6,"July");  
//      affichagedatainwebsite (6,"August"); 
//      affichagedatainwebsite (6,"September"); 
//      affichagedatainwebsite (6,"November"); 
//      affichagedatainwebsite (6,"December");   
//       ptr += "</tr>";
//
//      ptr += "<tr>";       
//      ptr += "<td>JANVIER KITOGA MUKAMBO</td>";
//      affichagedatainwebsite (3,"January");  
//      affichagedatainwebsite (3,"February"); 
//      affichagedatainwebsite (3,"March"); 
//      affichagedatainwebsite (3,"April"); 
//      affichagedatainwebsite (3,"May"); 
//      affichagedatainwebsite (3,"June");
//      affichagedatainwebsite (3,"July");  
//      affichagedatainwebsite (3,"August"); 
//      affichagedatainwebsite (3,"September"); 
//      affichagedatainwebsite (3,"November"); 
//      affichagedatainwebsite (3,"December"); 
//       ptr += "</tr>";
//
//       ptr += "<tr>";
//      ptr += "<td>MUMBERE VAKE</td>";
//       affichagedatainwebsite (1,"January");  
//      affichagedatainwebsite (1,"February"); 
//      affichagedatainwebsite (1,"March"); 
//      affichagedatainwebsite (1,"April"); 
//      affichagedatainwebsite (1,"May"); 
//      affichagedatainwebsite (1,"June"); 
//      affichagedatainwebsite (1,"July");  
//      affichagedatainwebsite (1,"August"); 
//      affichagedatainwebsite (1,"September"); 
//      affichagedatainwebsite (1,"November"); 
//      affichagedatainwebsite (1,"December"); 
//       ptr += "</tr>";
//
//        ptr += "<tr>";
//      ptr += "<td>KIKA KAKURU RICHARA</td>";
//        affichagedatainwebsite (5,"January");  
//      affichagedatainwebsite (5,"February"); 
//      affichagedatainwebsite (5,"March"); 
//      affichagedatainwebsite (5,"April"); 
//      affichagedatainwebsite (5,"May"); 
//      affichagedatainwebsite (5,"June");
//      affichagedatainwebsite (5,"July");   
//      affichagedatainwebsite (5,"August"); 
//      affichagedatainwebsite (5,"September"); 
//      affichagedatainwebsite (5,"November"); 
//      affichagedatainwebsite (5,"December");  
//       ptr += "</tr>";
//
//        ptr += "<tr>";
//      ptr += "<td>THOMAS SEBISUSA</td>";
//        affichagedatainwebsite (51,"January");  
//      affichagedatainwebsite (51,"February"); 
//      affichagedatainwebsite (51,"March"); 
//      affichagedatainwebsite (51,"April"); 
//      affichagedatainwebsite (51,"May"); 
//      affichagedatainwebsite (51,"June");
//      affichagedatainwebsite (51,"July");   
//      affichagedatainwebsite (51,"August"); 
//      affichagedatainwebsite (51,"September"); 
//      affichagedatainwebsite (51,"November"); 
//      affichagedatainwebsite (51,"December");
//       ptr += "</tr>";
//
//        ptr += "<tr>";
//      ptr += "<td>BAHATI NATHANAEL</td>";
//       affichagedatainwebsite (4,"January");  
//      affichagedatainwebsite (4,"February"); 
//      affichagedatainwebsite  (4,"March"); 
//      affichagedatainwebsite (4,"April"); 
//      affichagedatainwebsite (4,"May"); 
//      affichagedatainwebsite (4,"June"); 
//      affichagedatainwebsite (4,"July");  
//      affichagedatainwebsite (4,"August"); 
//      affichagedatainwebsite (4,"September"); 
//      affichagedatainwebsite (4,"November"); 
//      affichagedatainwebsite (4,"December");  
//       ptr += "</tr>";
//
//         ptr += "<tr>";
//      ptr += "<td> AIME MUHESI</td>";
//      affichagedatainwebsite (0,"January");  
//      affichagedatainwebsite (0,"February"); 
//      affichagedatainwebsite  (0,"March"); 
//      affichagedatainwebsite (0,"April"); 
//      affichagedatainwebsite (0,"May"); 
//      affichagedatainwebsite (0,"June"); 
//       affichagedatainwebsite (0,"July"); 
//      affichagedatainwebsite (0,"August"); 
//      affichagedatainwebsite (0,"September"); 
//      affichagedatainwebsite (0,"November"); 
//      affichagedatainwebsite (0,"December"); 
//       ptr += "</tr>";
//
//              ptr += "<tr>";
//      ptr += "<td>CHANCE KASEREKA</td>";        
//       affichagedatainwebsite (11,"January");  
//      affichagedatainwebsite (11,"February"); 
//      affichagedatainwebsite  (11,"March"); 
//      affichagedatainwebsite (11,"April"); 
//      affichagedatainwebsite (11,"May"); 
//      affichagedatainwebsite (11,"June"); 
//      affichagedatainwebsite (11,"July"); 
//     affichagedatainwebsite (11,"August"); 
//    affichagedatainwebsite (11,"September"); 
//      affichagedatainwebsite (11,"November"); 
//      affichagedatainwebsite (11,"December");
//         ptr += "</tr>";
//
//                ptr += "<tr>";      
//      ptr += "<td>FISTON SIKULI</td>\n";
//       affichagedatainwebsite (7,"January");  
//      affichagedatainwebsite (7,"February"); 
//      affichagedatainwebsite  (7,"March"); 
//      affichagedatainwebsite (7,"April"); 
//      affichagedatainwebsite (7,"May"); 
//      affichagedatainwebsite (7,"June");
//       affichagedatainwebsite (7,"July");  
//      affichagedatainwebsite (7,"August"); 
//      affichagedatainwebsite (7,"September"); 
//      affichagedatainwebsite (7,"November"); 
//      affichagedatainwebsite (7,"December"); 
//       ptr += "</tr>" ;
//
//       ptr += "<tr>";
//      ptr += "<td>GUILAIN</td>";
//       affichagedatainwebsite (9,"January");  
//      affichagedatainwebsite (9,"February"); 
//      affichagedatainwebsite  (9,"March"); 
//      affichagedatainwebsite (9,"April"); 
//      affichagedatainwebsite (9,"May"); 
//      affichagedatainwebsite (9,"June");
//       affichagedatainwebsite (9,"July");  
//      affichagedatainwebsite (9,"August"); 
//      affichagedatainwebsite (9,"September"); 
//      affichagedatainwebsite (9,"November"); 
//      affichagedatainwebsite (9,"December"); 
//       ptr += "</tr>";
//
//             ptr += "</tr>"; 
//      ptr += "<td>EMMANUEL</td>";
//      affichagedatainwebsite (12,"January");  
//      affichagedatainwebsite (12,"February"); 
//      affichagedatainwebsite  (12,"March"); 
//      affichagedatainwebsite (12,"April"); 
//      affichagedatainwebsite (12,"May"); 
//      affichagedatainwebsite (12,"June"); 
//       affichagedatainwebsite (12,"July"); 
//      affichagedatainwebsite (12,"August"); 
//      affichagedatainwebsite (12,"September"); 
//      affichagedatainwebsite (12,"November"); 
//     affichagedatainwebsite (12,"December"); 
//      ptr += "</tr>";
//
//               ptr += "<tr>";
//    ptr += "<td>KAMBALE KAMBULA</td>";
//     affichagedatainwebsite (8,"January");  
//      affichagedatainwebsite (8,"February"); 
//      affichagedatainwebsite  (8,"March"); 
//      affichagedatainwebsite (8,"April"); 
//      affichagedatainwebsite (8,"May"); 
//      affichagedatainwebsite (8,"June"); 
//       affichagedatainwebsite (8,"July"); 
//       affichagedatainwebsite (8,"August"); 
//      affichagedatainwebsite (8,"September"); 
//      affichagedatainwebsite (8,"November"); 
//      affichagedatainwebsite (8,"December"); 
//       ptr +=  "</tr>";
//
//        ptr += "<tr>";
//      ptr += "<td>OLIVIER MUMBERE</td>";
//     affichagedatainwebsite (10,"January");  
//      affichagedatainwebsite (10,"February"); 
//    affichagedatainwebsite  (10,"March"); 
//     affichagedatainwebsite (10,"April"); 
//      affichagedatainwebsite (10,"May"); 
//      affichagedatainwebsite (10,"June"); 
//       affichagedatainwebsite (10,"July"); 
//      affichagedatainwebsite (10,"August"); 
//     affichagedatainwebsite (10,"September"); 
//      affichagedatainwebsite (10,"November"); 
//      affichagedatainwebsite (10,"December"); 
//       ptr += " </tr>";  
//
//        ptr += "<tr>";
//      ptr += "<td>KABUYA</td>";
//     affichagedatainwebsite (15,"January");  
//      affichagedatainwebsite (15,"February"); 
//      affichagedatainwebsite  (15,"March"); 
//      affichagedatainwebsite (15,"April"); 
//      affichagedatainwebsite (15,"May"); 
//      affichagedatainwebsite (15,"June");
//       affichagedatainwebsite (15,"July");  
//      affichagedatainwebsite (15,"August"); 
//     affichagedatainwebsite (15,"September"); 
//      affichagedatainwebsite (15,"November"); 
//     affichagedatainwebsite (15,"December"); 
//      ptr += "</tr>"; 
//
//       ptr += "<tr>";
//      ptr += "<td>RODRIGUE MUMBERE MUYISA</td>";
//          affichagedatainwebsite (13,"January");  
//      affichagedatainwebsite (13,"February"); 
//      affichagedatainwebsite  (13,"March"); 
//      affichagedatainwebsite (13,"April"); 
//      affichagedatainwebsite (13,"May"); 
//      affichagedatainwebsite (13,"June"); 
//       affichagedatainwebsite (13,"July"); 
//      affichagedatainwebsite (13,"August"); 
//      affichagedatainwebsite (13,"September"); 
//      affichagedatainwebsite (13,"November"); 
//      affichagedatainwebsite (13,"December"); 
//       ptr += "</tr>";
//
//        ptr += "<tr>";
//      ptr += "<td>CHRISTIAN BALEKAGUE</td>";
//      affichagedatainwebsite (16,"January");  
//      affichagedatainwebsite (16,"February"); 
//      affichagedatainwebsite  (16,"March"); 
//     affichagedatainwebsite (16,"April"); 
//     affichagedatainwebsite (16,"May"); 
//     affichagedatainwebsite (16,"June");
//       affichagedatainwebsite (16,"July");  
//      affichagedatainwebsite (16,"August"); 
//      affichagedatainwebsite (16,"September"); 
//     affichagedatainwebsite (16,"November"); 
//      affichagedatainwebsite (16,"December"); 
//       ptr += "</tr>";
//
//       ptr += "<tr>";
//      ptr += "<td>JUSTIN</td>";
//      affichagedatainwebsite (17,"January");  
//      affichagedatainwebsite (17,"February"); 
//     affichagedatainwebsite  (17,"March"); 
//      affichagedatainwebsite (17,"April"); 
//      affichagedatainwebsite (17,"May"); 
//      affichagedatainwebsite (17,"June"); 
//       affichagedatainwebsite (17,"July"); 
//      affichagedatainwebsite (17,"August"); 
//      affichagedatainwebsite (17,"September"); 
//      affichagedatainwebsite (17,"November"); 
//      affichagedatainwebsite (17,"December"); 
//       ptr += "</tr>";
//
//        ptr += "<tr>";
//      ptr += "<td>EDWINE LUKULIRWA</td>";
//     affichagedatainwebsite (14,"January");  
//      affichagedatainwebsite (14,"February"); 
//      affichagedatainwebsite  (14,"March"); 
//      affichagedatainwebsite (14,"April"); 
//      affichagedatainwebsite (14,"May"); 
//      affichagedatainwebsite (14,"June"); 
//       affichagedatainwebsite (14,"July"); 
//      affichagedatainwebsite (14,"August"); 
//      affichagedatainwebsite (14,"September"); 
//      affichagedatainwebsite (14,"November"); 
//      affichagedatainwebsite (14,"December"); 
//       ptr += "</tr>";
//
//       ptr += "<tr>";
//      ptr += "<td>PRINCE KAMBALE</td>";
//      affichagedatainwebsite (21,"January");  
//      affichagedatainwebsite (21,"February"); 
//      affichagedatainwebsite  (21,"March"); 
//      affichagedatainwebsite (21,"April"); 
//      affichagedatainwebsite (21,"May"); 
//      affichagedatainwebsite (21,"June"); 
//       affichagedatainwebsite (21,"July"); 
//     affichagedatainwebsite (21,"August"); 
//     affichagedatainwebsite (21,"September"); 
//      affichagedatainwebsite (21,"November"); 
//     affichagedatainwebsite (21,"December"); 
//       ptr += "</tr>";
//
//    ptr += "<tr>";
//      ptr += "<td>KAMBALE MUPIKA</td>";
//         affichagedatainwebsite (18,"January");  
//      affichagedatainwebsite (18,"February"); 
//      affichagedatainwebsite  (18,"March"); 
//      affichagedatainwebsite (18,"April"); 
//      affichagedatainwebsite (18,"May"); 
//      affichagedatainwebsite (18,"June");
//       affichagedatainwebsite (18,"July");  
//      affichagedatainwebsite (18,"August"); 
//      affichagedatainwebsite (18,"September"); 
//      affichagedatainwebsite (18,"November"); 
//      affichagedatainwebsite (18,"December"); 
//       ptr += "</tr>";
//
//       ptr += "<tr>";
//     ptr += "<td>ALEXIS MALIRA</td>";
//       affichagedatainwebsite (19,"January");  
//      affichagedatainwebsite (19,"February"); 
//      affichagedatainwebsite  (19,"March"); 
//      affichagedatainwebsite (19,"April"); 
//     affichagedatainwebsite (19,"May"); 
//     affichagedatainwebsite (19,"June"); 
//       affichagedatainwebsite (19,"July"); 
//      affichagedatainwebsite (19,"August"); 
//      affichagedatainwebsite (19,"September"); 
//      affichagedatainwebsite (19,"November"); 
//      affichagedatainwebsite (19,"December"); 
//       ptr += "</tr>";
//       
//       ptr += "<tr>";
//      ptr += "<td>ERIC SHABANTU</td>";
//     affichagedatainwebsite (25,"January");  
//      affichagedatainwebsite (25,"February"); 
//      affichagedatainwebsite  (25,"March"); 
//      affichagedatainwebsite (25,"April"); 
//      affichagedatainwebsite (25,"May"); 
//      affichagedatainwebsite (25,"June");
//       affichagedatainwebsite (25,"July");  
//      affichagedatainwebsite (25,"August"); 
//      affichagedatainwebsite (25,"September"); 
//      affichagedatainwebsite (25,"November"); 
//      affichagedatainwebsite (25,"December"); 
//       ptr += "</tr>";
//
//       ptr += "<tr>";
//      ptr += "<td>BLANCHARD VAYISALIRE</td>";
//     affichagedatainwebsite (22,"January");  
//      affichagedatainwebsite (22,"February"); 
//      affichagedatainwebsite  (22,"March"); 
//      affichagedatainwebsite (22,"April"); 
//      affichagedatainwebsite (22,"May"); 
//      affichagedatainwebsite (22,"June"); 
//       affichagedatainwebsite (22,"July"); 
//      affichagedatainwebsite (22,"August"); 
//      affichagedatainwebsite (22,"September"); 
//      affichagedatainwebsite (22,"November"); 
//      affichagedatainwebsite (22,"December"); 
//       ptr += " </tr>";
//
//       ptr += "<tr>";
//      ptr += "<td>RIGO KASEREKA MUSAFIRI</td>";
//     affichagedatainwebsite (23,"January");  
//      affichagedatainwebsite (23,"February"); 
//      affichagedatainwebsite  (23,"March"); 
//      affichagedatainwebsite (23,"April"); 
//      affichagedatainwebsite (23,"May"); 
//        affichagedatainwebsite (23,"June");
//       affichagedatainwebsite (23,"July");  
//      affichagedatainwebsite (23,"August"); 
//      affichagedatainwebsite (23,"September"); 
//      affichagedatainwebsite (23,"November"); 
//      affichagedatainwebsite (23,"December"); 
//       ptr += "</tr>";
//
//        ptr += "<tr>";
//      ptr += "<td>SAMY KAMBALE</td>";
//     affichagedatainwebsite (24,"January");  
//      affichagedatainwebsite (24,"February"); 
//      affichagedatainwebsite  (24,"March"); 
//      affichagedatainwebsite (24,"April"); 
//      affichagedatainwebsite (24,"May"); 
//      affichagedatainwebsite (24,"June");
//       affichagedatainwebsite (24,"July");  
//      affichagedatainwebsite (24,"August"); 
//      affichagedatainwebsite (24,"September"); 
//      affichagedatainwebsite (24,"November"); 
//      affichagedatainwebsite (24,"December"); 
//       ptr += "</tr>";
//
//          ptr += "<tr>";
//      ptr += "<td>YANNICK BAHATI</td>";
//      affichagedatainwebsite (29,"January");  
//      affichagedatainwebsite (29,"February"); 
//      affichagedatainwebsite  (29,"March"); 
//      affichagedatainwebsite (29,"April"); 
//      affichagedatainwebsite (29,"May"); 
//      affichagedatainwebsite (29,"June"); 
//       affichagedatainwebsite (29,"July"); 
//      affichagedatainwebsite (29,"August"); 
//      affichagedatainwebsite (29,"September"); 
//      affichagedatainwebsite (29,"November"); 
//      affichagedatainwebsite (29,"December"); 
//      ptr += "</tr>";
//
//       ptr += "<tr>";
//      ptr += "<td>CONSOLE KAVIRA WASUKUNDI</td>";
//     affichagedatainwebsite (26,"January");  
//      affichagedatainwebsite (26,"February"); 
//     affichagedatainwebsite  (26,"March"); 
//     affichagedatainwebsite (26,"April"); 
//      affichagedatainwebsite (26,"May"); 
//      affichagedatainwebsite (26,"June");
//       affichagedatainwebsite (26,"July");  
//      affichagedatainwebsite (26,"August"); 
//      affichagedatainwebsite (26,"September"); 
//     affichagedatainwebsite (26,"November"); 
//     affichagedatainwebsite (26,"December"); 
//       ptr += "</tr>";
//
//         ptr += "<tr>";
//      ptr += "<td>EZEKIEL MIAMBO</td>";
//    affichagedatainwebsite (27,"January");  
//     affichagedatainwebsite (27,"February"); 
//      affichagedatainwebsite  (27,"March"); 
//      affichagedatainwebsite (27,"April"); 
//      affichagedatainwebsite (27,"May"); 
//     affichagedatainwebsite (27,"June");
//      affichagedatainwebsite (27,"July");  
//     affichagedatainwebsite (27,"August"); 
//      affichagedatainwebsite (27,"September"); 
//      affichagedatainwebsite (27,"November"); 
//     affichagedatainwebsite (27,"December"); 
//       ptr += "</tr>";
//
//       ptr += "<tr>";
//      ptr += "<td>ARSENE KAMBALE MBUSA</td>";
//    affichagedatainwebsite (30,"January");  
//      affichagedatainwebsite (30,"February"); 
//      affichagedatainwebsite  (30,"March"); 
//    affichagedatainwebsite (30,"April"); 
//     affichagedatainwebsite (30,"May"); 
//      affichagedatainwebsite (30,"June");
//       affichagedatainwebsite (30,"July");  
//      affichagedatainwebsite (30,"August"); 
//      affichagedatainwebsite (30,"September"); 
//      affichagedatainwebsite (30,"November"); 
//      affichagedatainwebsite (30,"December"); 
//       ptr += "</tr>";
//
//       ptr += "<tr>";
//      ptr += "<td>MITTERAND KATEMBO SAASITA</td>";
//    affichagedatainwebsite (28,"January");  
//      affichagedatainwebsite (28,"February"); 
//     affichagedatainwebsite  (28,"March"); 
//     affichagedatainwebsite (28,"April"); 
//      affichagedatainwebsite (28,"May"); 
//      affichagedatainwebsite (28,"June");
//       affichagedatainwebsite (28,"July");  
//      affichagedatainwebsite (28,"August"); 
//      affichagedatainwebsite (28,"September"); 
//      affichagedatainwebsite (28,"November"); 
//      affichagedatainwebsite (28,"December"); 
//       ptr += "</tr>";
//
//        ptr += "<tr>";
//      ptr += "<td>KAMBALE KASUKA</td>";
//       affichagedatainwebsite (34,"January");  
//      affichagedatainwebsite (34,"February"); 
//      affichagedatainwebsite  (34,"March"); 
//      affichagedatainwebsite (34,"April"); 
//      affichagedatainwebsite (34,"May"); 
//      affichagedatainwebsite (34,"June"); 
//       affichagedatainwebsite (34,"July"); 
//      affichagedatainwebsite (34,"August"); 
//      affichagedatainwebsite (34,"September"); 
//      affichagedatainwebsite (34,"November"); 
//      affichagedatainwebsite (34,"December");
//       ptr += "</tr>";
//
//          ptr += "<tr>";
//      ptr += "<td>DONATIEN BIKUBA</td>";
//      affichagedatainwebsite (31,"January");  
//      affichagedatainwebsite (31,"February"); 
//      affichagedatainwebsite  (31,"March"); 
//      affichagedatainwebsite (31,"April"); 
//      affichagedatainwebsite (31,"May"); 
//      affichagedatainwebsite (31,"June"); 
//       affichagedatainwebsite (31,"July"); 
//      affichagedatainwebsite (31,"August"); 
//      affichagedatainwebsite (31,"September"); 
//      affichagedatainwebsite (31,"November"); 
//      affichagedatainwebsite (31,"December");
//       ptr += "</tr>";
//
//        ptr += "<tr>";
//     ptr += "<td>PATIENT MWAMBA</td>";
//      affichagedatainwebsite (33,"January");  
//      affichagedatainwebsite (33,"February"); 
//      affichagedatainwebsite  (33,"March"); 
//      affichagedatainwebsite (33,"April"); 
//      affichagedatainwebsite (33,"May"); 
//      affichagedatainwebsite (33,"June"); 
//       affichagedatainwebsite (33,"July"); 
//      affichagedatainwebsite (33,"August"); 
//      affichagedatainwebsite (33,"September"); 
//      affichagedatainwebsite (33,"November"); 
//      affichagedatainwebsite (33,"December");
//       ptr += "</tr>";
//
//        ptr += "<tr>";
//      ptr += "<td>JOSEPH MUSHENGEZI</td>";
//       affichagedatainwebsite (32,"January");  
//       affichagedatainwebsite (32,"February"); 
//      affichagedatainwebsite  (32,"March"); 
//    affichagedatainwebsite (32,"April"); 
//      affichagedatainwebsite (32,"May"); 
//      affichagedatainwebsite (32,"June"); 
//       affichagedatainwebsite (32,"July"); 
//      affichagedatainwebsite (32,"August"); 
//      affichagedatainwebsite (32,"September"); 
//      affichagedatainwebsite (32,"November"); 
//      affichagedatainwebsite (32,"December");
//        ptr += "</tr>";
//
//          ptr += "<tr>";
//      ptr += "<td>KAMBALE KATAKA</td>";
//     affichagedatainwebsite (36,"January");  
//      affichagedatainwebsite (36,"February"); 
//      affichagedatainwebsite  (36,"March"); 
//      affichagedatainwebsite (36,"April"); 
//      affichagedatainwebsite (36,"May"); 
//      affichagedatainwebsite (36,"June"); 
//       affichagedatainwebsite (36,"July"); 
//      affichagedatainwebsite (36,"August"); 
//      affichagedatainwebsite (36,"September"); 
//      affichagedatainwebsite (36,"November"); 
//      affichagedatainwebsite (36,"December");;
//       ptr += "</tr>";
//
//           ptr += "<tr>";
//      ptr += "<td>ERIC KAMUSWEKERE</td>";
//        affichagedatainwebsite (38,"January");  
//      affichagedatainwebsite (38,"February"); 
//      affichagedatainwebsite  (38,"March"); 
//      affichagedatainwebsite (38,"April"); 
//     affichagedatainwebsite (38,"May"); 
//     affichagedatainwebsite (38,"June"); 
//      affichagedatainwebsite (38,"July"); 
//      affichagedatainwebsite (38,"August"); 
//    affichagedatainwebsite (38,"September"); 
//     affichagedatainwebsite (38,"November"); 
//      affichagedatainwebsite (38,"December");
//     ptr += "</tr>";
//
//     ptr += "<tr>";
//      ptr += "<td>DANNY KAKULE</td>";
//    affichagedatainwebsite (37,"January");  
//     affichagedatainwebsite (37,"February"); 
//     affichagedatainwebsite  (37,"March"); 
//      affichagedatainwebsite (37,"April"); 
//      affichagedatainwebsite (37,"May"); 
//     affichagedatainwebsite (37,"June");
//      affichagedatainwebsite (37,"July"); 
//     affichagedatainwebsite (37,"August"); 
//     affichagedatainwebsite (37,"September"); 
//      affichagedatainwebsite (37,"November"); 
//      affichagedatainwebsite (37,"December");
//       ptr += "</tr>";
//
//        ptr += "<tr>";
//      ptr += "<td>JULIEN KAMBALE KITANIKA</td>";
//     affichagedatainwebsite (35,"January");  
//      affichagedatainwebsite (35,"February"); 
//      affichagedatainwebsite  (35,"March"); 
//      affichagedatainwebsite (35,"April"); 
//      affichagedatainwebsite (35,"May"); 
//      affichagedatainwebsite (35,"June"); 
//      affichagedatainwebsite (35,"July"); 
//      affichagedatainwebsite (35,"August"); 
//      affichagedatainwebsite (35,"September"); 
//      affichagedatainwebsite (35,"November"); 
//      affichagedatainwebsite (35,"December");
//         ptr += "</tr>";
//
//         ptr += "<tr>";
//      ptr += "<td>NZANZU</td>";
//       affichagedatainwebsite (40,"January");  
//      affichagedatainwebsite (40,"February"); 
//      affichagedatainwebsite  (40,"March"); 
//      affichagedatainwebsite (40,"April"); 
//      affichagedatainwebsite (40,"May"); 
//      affichagedatainwebsite (40,"June");
//      affichagedatainwebsite (40,"July");  
//      affichagedatainwebsite (40,"August"); 
//     affichagedatainwebsite (40,"September"); 
//      affichagedatainwebsite (40,"November"); 
//      affichagedatainwebsite (40,"December");
//   ptr += "</tr>"; 
//
//    ptr += "<tr>";
//      ptr += "<td>MICHEL MISHONA</td>";
//     affichagedatainwebsite (42,"January");  
//      affichagedatainwebsite (42,"February"); 
//      affichagedatainwebsite  (42,"March"); 
//     affichagedatainwebsite (42,"April"); 
//      affichagedatainwebsite (42,"May"); 
//   affichagedatainwebsite (42,"June"); 
//      affichagedatainwebsite (42,"July"); 
//     affichagedatainwebsite (42,"August"); 
//    affichagedatainwebsite (42,"September"); 
//      affichagedatainwebsite (42,"November"); 
//     affichagedatainwebsite (42,"December");
//       ptr += "</tr>";
//
//        ptr += "<tr>";
//     ptr += "<td>ALEXIS</td>";
//       affichagedatainwebsite (39,"January");  
//      affichagedatainwebsite (39,"February"); 
//      affichagedatainwebsite  (39,"March"); 
//      affichagedatainwebsite (39,"April"); 
//      affichagedatainwebsite (39,"May"); 
//      affichagedatainwebsite (39,"June"); 
//      affichagedatainwebsite (39,"July"); 
//      affichagedatainwebsite (39,"August"); 
//      affichagedatainwebsite (39,"September"); 
//      affichagedatainwebsite (39,"November"); 
//      affichagedatainwebsite (39,"December");
//       ptr += "</tr>";
//
//        ptr += "<tr>";
//     ptr += "<td>GLORIA MALEMO</td>";
//    affichagedatainwebsite (48,"January");  
//      affichagedatainwebsite (48,"February"); 
//      affichagedatainwebsite  (48,"March"); 
//      affichagedatainwebsite (48,"April"); 
//      affichagedatainwebsite (48,"May"); 
//      affichagedatainwebsite (48,"June"); 
//     affichagedatainwebsite (48,"July"); 
//   affichagedatainwebsite (48,"August"); 
//   affichagedatainwebsite (48,"September"); 
//     affichagedatainwebsite (48,"November"); 
//      affichagedatainwebsite (48,"December");
//       ptr += "</tr>";
//
//         ptr += "<tr>";
//    ptr += "<td>CHRISTIAN BAKWANAMAHA</td>";
//          affichagedatainwebsite (43,"January");  
//      affichagedatainwebsite (43,"February"); 
//      affichagedatainwebsite  (43,"March"); 
//      affichagedatainwebsite (43,"April"); 
//      affichagedatainwebsite (43,"May"); 
//      affichagedatainwebsite (43,"June"); 
//      affichagedatainwebsite (43,"July"); 
//     affichagedatainwebsite (43,"August"); 
//      affichagedatainwebsite (43,"September"); 
//     affichagedatainwebsite (43,"November"); 
//      affichagedatainwebsite (43,"December");
//       ptr += "</tr>";
//
//       ptr += "<tr>";
//      ptr += "<td>AARON ZUMBU MATA</td>";
//      affichagedatainwebsite (44,"January");  
//      affichagedatainwebsite (44,"February"); 
//      affichagedatainwebsite  (44,"March"); 
//      affichagedatainwebsite (44,"April"); 
//      affichagedatainwebsite (44,"May"); 
//      affichagedatainwebsite (44,"June"); 
//      affichagedatainwebsite (44,"July");
//      affichagedatainwebsite (44,"August"); 
//      affichagedatainwebsite (44,"September"); 
//      affichagedatainwebsite (44,"November"); 
//      affichagedatainwebsite (44,"December");
//       ptr += "</tr>";
//
//        ptr += "<tr>";
//      ptr += "<td>SEMIDA KAVIRA</td>";
//       affichagedatainwebsite (46,"January");  
//      affichagedatainwebsite (46,"February"); 
//      affichagedatainwebsite  (46,"March"); 
//      affichagedatainwebsite (46,"April"); 
//      affichagedatainwebsite (46,"May"); 
//      affichagedatainwebsite (46,"June"); 
//      affichagedatainwebsite (46,"July"); 
//      affichagedatainwebsite (46,"August"); 
//      affichagedatainwebsite (46,"September"); 
//      affichagedatainwebsite (46,"November"); 
//      affichagedatainwebsite (46,"December");
//       ptr += "</tr>";
//       
//       ptr += "<tr>";
//     ptr += "<td>ALEX TSONGO KASONIA</td>";
//      affichagedatainwebsite (47,"January");  
//      affichagedatainwebsite (47,"February"); 
//      affichagedatainwebsite  (47,"March"); 
//      affichagedatainwebsite (47,"April"); 
//      affichagedatainwebsite (47,"May"); 
//     affichagedatainwebsite (47,"June"); 
//     affichagedatainwebsite (47,"July"); 
//     affichagedatainwebsite (47,"August"); 
//      affichagedatainwebsite (47,"September"); 
//      affichagedatainwebsite (47,"November"); 
//      affichagedatainwebsite (47,"December");
//      ptr += "</tr>";
//
//        ptr += "<tr>";
//    ptr += "<td>JAMES</td>";
//   affichagedatainwebsite (45,"January");  
//     affichagedatainwebsite (45,"February"); 
//     affichagedatainwebsite  (45,"March"); 
//     affichagedatainwebsite (45,"April"); 
//      affichagedatainwebsite (45,"May"); 
//    affichagedatainwebsite (45,"June"); 
//     affichagedatainwebsite (45,"July"); 
//      affichagedatainwebsite (45,"August"); 
//      affichagedatainwebsite (45,"September"); 
//      affichagedatainwebsite (45,"November"); 
//      affichagedatainwebsite (45,"December");
//       ptr += "</tr>\n";
//
//       ptr += "<tr>";
//    ptr += "<td>DELPHIN KAMABU</td>";
//   affichagedatainwebsite (2,"January");  
//     affichagedatainwebsite (2,"February"); 
//     affichagedatainwebsite  (2,"March"); 
//     affichagedatainwebsite (2,"April"); 
//      affichagedatainwebsite (2,"May"); 
//    affichagedatainwebsite (2,"June"); 
//     affichagedatainwebsite (2,"July"); 
//      affichagedatainwebsite (2,"August"); 
//      affichagedatainwebsite (2,"September"); 
//      affichagedatainwebsite (2,"November"); 
//      affichagedatainwebsite (2,"December");
//       ptr += "</tr>";
//
//        ptr += "<tr>";
//         ptr += "<td>GYLAINE NZIAVAKE</td>";
//   affichagedatainwebsite (49,"January");  
//     affichagedatainwebsite (49,"February"); 
//     affichagedatainwebsite  (49,"March"); 
//     affichagedatainwebsite (49,"April"); 
//      affichagedatainwebsite (49,"May"); 
//    affichagedatainwebsite (49,"June"); 
//     affichagedatainwebsite (49,"July"); 
//      affichagedatainwebsite (49,"August"); 
//      affichagedatainwebsite (49,"September"); 
//      affichagedatainwebsite (49,"November"); 
//      affichagedatainwebsite (49,"December");
//       ptr += "</tr>";
//
//      ptr += "<tr>";
//         ptr += "<td>JEAN MARIE BISIHIWA</td>\n";
//   affichagedatainwebsite (50,"January");  
//     affichagedatainwebsite (50,"February"); 
//     affichagedatainwebsite  (50,"March"); 
//     affichagedatainwebsite (50,"April"); 
//      affichagedatainwebsite (50,"May"); 
//    affichagedatainwebsite (50,"June"); 
//     affichagedatainwebsite (50,"July"); 
//      affichagedatainwebsite (50,"August"); 
//      affichagedatainwebsite (50,"September"); 
//      affichagedatainwebsite (50,"November"); 
//      affichagedatainwebsite (50,"December");
//       ptr += "</tr>";
//
//
//         ptr += "<tr>";
//         ptr += "<td>KAMBALE FATAKI</td>";
//   affichagedatainwebsite (52,"January");  
//     affichagedatainwebsite (52,"February"); 
//     affichagedatainwebsite  (52,"March"); 
//     affichagedatainwebsite (52,"April"); 
//      affichagedatainwebsite (52,"May"); 
//    affichagedatainwebsite (52,"June"); 
//     affichagedatainwebsite (52,"July"); 
//      affichagedatainwebsite (52,"August"); 
//      affichagedatainwebsite (52,"September"); 
//      affichagedatainwebsite (52,"November"); 
//      affichagedatainwebsite (52,"December");
//       ptr += "</tr>"; 
//       
//
//             ptr += "<tr>";
//         ptr += "<td>HENRIQUE KABALA</td>";
//   affichagedatainwebsite (53,"January");  
//     affichagedatainwebsite (53,"February"); 
//     affichagedatainwebsite  (53,"March"); 
//     affichagedatainwebsite (53,"April"); 
//      affichagedatainwebsite (53,"May"); 
//    affichagedatainwebsite (53,"June"); 
//     affichagedatainwebsite (53,"July"); 
//      affichagedatainwebsite (53,"August"); 
//      affichagedatainwebsite (53,"September"); 
//      affichagedatainwebsite (53,"November"); 
//      affichagedatainwebsite (53,"December");
//       ptr += "</tr>"; 
//
//        ptr += "<tr>";
//         ptr += "<td>EMMANUEL MUHIGIRWA</td>";
//   affichagedatainwebsite (54,"January");  
//     affichagedatainwebsite (54,"February"); 
//     affichagedatainwebsite  (54,"March"); 
//     affichagedatainwebsite (54,"April"); 
//      affichagedatainwebsite (54,"May"); 
//    affichagedatainwebsite (54,"June"); 
//     affichagedatainwebsite (54,"July"); 
//      affichagedatainwebsite (54,"August"); 
//      affichagedatainwebsite (54,"September"); 
//      affichagedatainwebsite (54,"November"); 
//      affichagedatainwebsite (54,"December");
//       ptr += "</tr>"; 
//
//        ptr += "<tr>";
//         ptr += "<td>VICAR ID TEST </td>";
//   affichagedatainwebsite (20,"January");  
//     affichagedatainwebsite (20,"February"); 
//     affichagedatainwebsite  (20,"March"); 
//     affichagedatainwebsite (20,"April"); 
//      affichagedatainwebsite (20,"May"); 
//    affichagedatainwebsite (20,"June"); 
//     affichagedatainwebsite (20,"July"); 
//      affichagedatainwebsite (20,"August"); 
//      affichagedatainwebsite (20,"September"); 
//      affichagedatainwebsite (20,"November"); 
//      affichagedatainwebsite (20,"December");
//       ptr += "</tr>"; 
//       
//     ptr += "</table>" ;
//
//      //ptr +="<p>"; 
//     // ptr += "#VICAR ind"; 
//     // ptr += "</p>"; 
//      //ptr +="<p>"; 
//    //  ptr += "#MALKIAH MACHINAH"; 
//     // ptr += "</p>"; 
//     
////
////  ptr += " <table width= \"1500\" height = \"50\" border = 20 align = \"center\">"; 
////       ptr += " <caption> <h2> TECHNICIEN </h2> </caption>";
////
////
////      ptr += "<tr>\n"; 
////     ptr += "<td> NOMS </td>\n"; 
////     ptr += "<td> janvier </td>\n"; 
////     ptr += "<td> fevrier </td>\n"; 
////     ptr += " <td> mars </td>\n "; 
////     ptr += "<td> avril </td>\n ";
////     ptr += "<td> mai </td>\n" ; 
////
////     ptr += "<td> juin </td>\n"; 
////     ptr += "<td> juillet </td>\n";
////     ptr += "<td> aout </td> \n "; 
////     ptr += " <td> septembre</td>\n" ; 
////      ptr += "<td> octobre </td> \n " ; 
////       ptr += "<td> novembre </td> \n";
////       ptr += "<td> decembre</td> \n";  
//
//
////
////
////        
////     ptr += "</table>";
////
////      ptr += "<table width= \"1500\" height = \"50\" border = 20 align = \"center\"> \n"; 
////
////
////       ptr += "<caption> <h2> PROPRETE </h2> </caption> \n";
////
////       ptr += "<tr> \n"; 
////     ptr += "<td> NOMS </td> \n"; 
////     ptr += "<td> janvier </td>\n"; 
////     ptr += "<td> fevrier </td>\n"; 
////     ptr += " <td> mars </td> \n "; 
////     ptr += "<td> avril </td> \n ";
////     ptr += "<td> mai </td> \n"; 
////
////     ptr += "<td> juin </td> \n"; 
////     ptr += "<td> juillet </td> \n";
////     ptr += "<td> aout </td> \n "; 
////     ptr += " <td> septembre</td>\n" ; 
////      ptr += "<td> octobre </td> \n " ; 
////       ptr += "<td> novembre </td> \n";
////       ptr += "<td> decembre</td> \n";  
////
////        ptr += " </tr> \n"; 
//
////        
////         ptr += "</table>";
////
////          ptr += "<table width= \"1500\" height = \"50\" border = 20 align = \"center\"> \n" ; 
////  ptr += "<caption> <h2> CUISINIER </h2> </caption> \n";
////
////    ptr += "<tr> \n"; 
////     ptr += "<td> NOMS </td> \n"; 
////     ptr += "<td> janvier </td>\n"; 
////     ptr += "<td> fevrier </td>\n"; 
////     ptr += " <td> mars </td> \n "; 
////     ptr += "<td> avril </td> \n ";
////     ptr += "<td> mai </td> \n" ; 
////
////     ptr += "<td> juin </td> \n"; 
////     ptr += "<td> juillet </td> \n";
////     ptr += "<td> aout </td> \n "; 
////     ptr += " <td> septembre</td>\n" ; 
////      ptr += "<td> octobre </td> \n " ; 
////       ptr += "<td> novembre </td> \n";
////       ptr += "<td> decembre</td> \n";
////         ptr += " </tr> \n ";
//
////      ptr += "<tr>\n";
////      ptr += "<td> JEAN MARIE BISIHIWA (maitre cuisinier)   </td>\n";
////      
////       ptr += " </tr> \n ";
////
////       ptr += "<tr>\n";
////      ptr += "<td> KAMBALE FATAKI (cuisinier) </td>\n";
////
////       ptr += " </tr> \n ";
////
////
////        
////            
////
////           ptr += "</table> \n "; 
////
////            ptr += " <table width= \"1500\" height = \"50\" border = 20 align = \"center\"> \n  ";
////             ptr += " <caption> <h2> VALET </h2> </caption> \n  ";
////
////           ptr += "<tr> \n"; 
////     ptr += "<td> NOMS </td> \n"; 
////     ptr += "<td> janvier </td>\n"; 
////     ptr += "<td> fevrier </td>\n"; 
////     ptr += " <td> mars </td> \n "; 
////     ptr += "<td> avril </td> \n ";
////     ptr += "<td> mai </td> \n" ; 
////
////     ptr += "<td> juin </td> \n"; 
////     ptr += "<td> juillet </td> \n";
////     ptr += "<td> aout </td> \n "; 
////     ptr += " <td> septembre</td>\n" ; 
////      ptr += "<td> octobre </td> \n " ; 
////       ptr += "<td> novembre </td> \n";
////       ptr += "<td> decembre</td> \n";
//
//   
////           ptr += "</table> \n"; 
////
////
////            ptr += " <table width= \"1500\" height = \"50\" border = 20 align = \"center\"> \n  ";
////             ptr += " <caption> <h2> RECEPTIONISTE </h2> </caption> \n  ";
////
////           ptr += "<tr> \n"; 
////     ptr += "<td> NOMS </td> \n"; 
////     ptr += "<td> janvier </td>\n"; 
////     ptr += "<td> fevrier </td>\n"; 
////     ptr += " <td> mars </td> \n "; 
////     ptr += "<td> avril </td> \n ";
////     ptr += "<td> mai </td> \n" ; 
////
////     ptr += "<td> juin </td> \n"; 
////     ptr += "<td> juillet </td> \n";
////     ptr += "<td> aout </td> \n "; 
////     ptr += " <td> septembre</td>\n" ; 
////      ptr += "<td> octobre </td> \n " ; 
////       ptr += "<td> novembre </td> \n";
////       ptr += "<td> decembre</td> \n";
////
////
////         ptr += "<tr>\n";
//
////        ptr += "<tr>\n";
//
////         
////
////  
////        ptr += "</table> \n"; 
////
////         ptr += " <table width= \"1500\" height = \"50\" border = 20 align = \"center\"> \n  ";
////             ptr += " <caption> <h2> LAVANDIER </h2> </caption> \n  ";
////
////           ptr += "<tr> \n"; 
////     ptr += "<td> NOMS </td> \n"; 
////     ptr += "<td> janvier </td>\n"; 
////     ptr += "<td> fevrier </td>\n"; 
////     ptr += " <td> mars </td> \n "; 
////     ptr += "<td> avril </td> \n ";
////     ptr += "<td> mai </td> \n" ; 
////
////     ptr += "<td> juin </td> \n"; 
////     ptr += "<td> juillet </td> \n";
////     ptr += "<td> aout </td> \n "; 
////     ptr += " <td> septembre</td>\n" ; 
////      ptr += "<td> octobre </td> \n " ; 
////       ptr += "<td> novembre </td> \n";
////       ptr += "<td> decembre</td> \n";
//
//
//
////
////      
////
////
////      
////      
////         ptr += "</table> \n";
////
////
////          ptr += " <table width= \"1500\" height = \"50\" border = 20 align = \"center\"> \n  ";
////             ptr += " <caption> <h2> PLONGEUR </h2> </caption> \n  ";
////
////         
////           ptr += "<tr> \n"; 
////     ptr += "<td> NOMS </td> \n"; 
////     ptr += "<td> janvier </td>\n"; 
////     ptr += "<td> fevrier </td>\n"; 
////     ptr += " <td> mars </td> \n "; 
////     ptr += "<td> avril </td> \n ";
////     ptr += "<td> mai </td> \n" ; 
////     ptr += "<td> juin </td> \n"; 
////     ptr += "<td> juillet </td> \n";
////     ptr += "<td> aout </td> \n "; 
////     ptr += " <td> septembre</td>\n" ; 
////      ptr += "<td> octobre </td> \n " ; 
////       ptr += "<td> novembre </td> \n";
////       ptr += "<td> decembre</td> \n";
////       ptr += " </tr> \n ";
//       
//     
////       ptr += "</table> \n"; 
////
////       ptr += " <table width= \"1500\" height = \"50\" border = 20 align = \"center\"> \n  ";
////             ptr += " <caption> <h2> SECURITE </h2> </caption> \n  ";
////     
////
////         
////     ptr += "<tr> \n"; 
////     ptr += "<td> NOMS </td> \n"; 
////     ptr += "<td> janvier </td>\n"; 
////     ptr += "<td> fevrier </td>\n"; 
////     ptr += " <td> mars </td> \n "; 
////     ptr += "<td> avril </td> \n ";
////     ptr += "<td> mai </td> \n" ; 
////
////     ptr += "<td> juin </td> \n"; 
////     ptr += "<td> juillet </td> \n";
////     ptr += "<td> aout </td> \n "; 
////     ptr += " <td> septembre</td>\n" ; 
////     ptr += "<td> octobre </td> \n " ; 
////     ptr += "<td> novembre </td> \n";
////     ptr += "<td> decembre</td> \n";
////     ptr += " </tr> \n ";
////
////      
////       
////        
////
////         ptr += "</table> \n"; 
////
////                ptr += " <table width= \"1500\" height = \"50\" border = 20 align = \"center\"> \n  ";
////             ptr += " <caption> <h2> ADMINISTRATION </h2> </caption> \n  ";
////     
////
////             ptr += "<tr> \n"; 
////     ptr += "<td> NOMS </td> \n"; 
////     ptr += "<td> janvier </td>\n"; 
////     ptr += "<td> fevrier </td>\n"; 
////     ptr += " <td> mars </td> \n "; 
////     ptr += "<td> avril </td> \n ";
////     ptr += "<td> mai </td> \n" ; 
////
////     ptr += "<td> juin </td> \n"; 
////     ptr += "<td> juillet </td> \n";
////     ptr += "<td> aout </td> \n "; 
////     ptr += " <td> septembre</td>\n" ; 
////      ptr += "<td> octobre </td> \n " ; 
////       ptr += "<td> novembre </td> \n";
////       ptr += "<td> decembre</td> \n";
////       ptr += " </tr> \n ";
//
////       
////
////      
////
////        
////        ptr += "<tr>\n";
////      ptr += "<td>  GYLAINE NZIAVAKE (Acheteur) </td>\n";
////      ptr += " <td> </td> \n" ;
////       ptr += " <td> </td> \n" ;
////        ptr += " <td> </td> \n" ;
////      ptr += " <td> </td> \n" ;
////      ptr += " <td> </td> \n" ;
////      ptr += " <td> </td> \n" ;
////      ptr += " <td> </td> \n" ;
////       ptr += " <td> </td> \n" ;
////      ptr += " <td> </td> \n" ;
////      ptr += " <td> </td> \n" ;
////      ptr += " <td> </td> \n" ;
////      ptr += " <td> </td> \n" ;
////       ptr += " </tr> \n ";
////
//////        ptr += "<tr>\n";
//////      ptr += "<td>   MICHEL MISHONA (OPS) </td>\n";
//////      ptr += " <td> </td> \n" ;
//////       ptr += " <td> </td> \n" ;
//////        ptr += " <td> </td> \n" ;
//////      ptr += " <td> </td> \n" ;
//////      ptr += " <td> </td> \n" ;
//////      ptr += " <td> </td> \n" ;
//////      ptr += " <td> </td> \n" ;
//////       ptr += " <td> </td> \n" ;
//////      ptr += " <td> </td> \n" ;
//////      ptr += " <td> </td> \n" ;
//////      ptr += " <td> </td> \n" ;
//////      ptr += " <td> </td> \n" ;
//////       ptr += " </tr> \n ";
////
////
////       
////
////      
////
////       
////
////       
////         ptr += "</table> \n";
////
////
////      
////      ptr += " <table width= \"1500\" height = \"50\" border = 20 align = \"center\"> \n  ";
////       ptr += " <caption> <h2> JARDINIER </h2> </caption> \n  ";
////
////
////       ptr += "<tr> \n"; 
////     ptr += "<td> NOMS </td> \n"; 
////     ptr += "<td> janvier </td>\n"; 
////     ptr += "<td> fevrier </td>\n"; 
////     ptr += " <td> mars </td> \n "; 
////     ptr += "<td> avril </td> \n ";
////     ptr += "<td> mai </td> \n" ; 
////
////     ptr += "<td> juin </td> \n"; 
////     ptr += "<td> juillet </td> \n";
////     ptr += "<td> aout </td> \n "; 
////     ptr += " <td> septembre</td>\n" ; 
////      ptr += "<td> octobre </td> \n " ; 
////       ptr += "<td> novembre </td> \n";
////       ptr += "<td> decembre</td> \n";
////       ptr += " </tr> \n ";
////
////
////       ptr += "<tr>\n";
////      ptr += "<td>   PASCAL (Jardinier) </td>\n";
////       affichagedatainwebsite (41,"January");  
////      affichagedatainwebsite (41,"February"); 
////      affichagedatainwebsite  (41,"March"); 
////      affichagedatainwebsite (41,"April"); 
////      affichagedatainwebsite (41,"May"); 
////      affichagedatainwebsite (41,"June");
////      affichagedatainwebsite (41,"July");  
////      affichagedatainwebsite (41,"August"); 
////      affichagedatainwebsite (41,"September"); 
////      affichagedatainwebsite (41,"November"); 
////      affichagedatainwebsite (41,"December");
////       ptr += " </tr> \n ";
////
////   ptr += "</table> \n"; 
////          
////    ptr += "<p > NB :  ce système permet de donner le temps en heure que les employés font et cela moi par moi </p>";
//        
//           
//  ptr +="</body>";
//  ptr +="</html>";
//  return ptr;
//  
//}


void MQTT_connect() {
  int8_t ret;
  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
