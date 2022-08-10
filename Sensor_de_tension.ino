//VARIABLES MODULO SMS
#include <SoftwareSerial.h>
#define Gsm_tx 2
#define Gsm_rx 3
SoftwareSerial SIM800L(Gsm_tx, Gsm_rx);
String Jose = "2302651912";
String Leo = "1138679616";
String Seba = "2944806952";
String Mariano = "2944676771";
String Cris = "1156060958";

//VARIABLES MEDICION DE TENSION
const int tensionPin = A1;
int decimalPrecision = 2;                                                                               
int VoltageAnalogInputPin = A1;                                                                         
float voltajeFinalRMS;                                                                                 
float voltajeMedio;                                                                                     
float voltajeActual  = 0;                                                                             
float voltajeUltimaLectura  = 0;                                                                       
float voltajeContadorMuestras = 0;                                                                    
float voltajeSumaMuestras = 0;                                                                          
float mediaVoltajeRMS ;                                                                                 
float adjusteMediaVoltajeRMS; 
//Tiempos para determinar corte de luz
unsigned long tiempoSinLuz = 0;
unsigned long tiempoConLuz = 0;
unsigned long tiempo = 0;
int flag_Luz=1;
int entro=0;

void setup()
{
  Serial.begin(9600); 
  SIM800L.begin(9600);
  Serial.println("Inicia");  
  
}

void loop() {
  tiempo=millis();
  if(micros() >= voltajeUltimaLectura + 1000 )                                                        
  {            
    voltajeActual = (analogRead(VoltageAnalogInputPin)- 512);                  
    voltajeSumaMuestras = voltajeSumaMuestras + sq(voltajeActual) ;                           
    voltajeContadorMuestras++;                                                                 
    voltajeUltimaLectura = micros() ;                                                          
  }

  if(voltajeContadorMuestras >= 1000)                                                                       
  {                             
    voltajeMedio = voltajeSumaMuestras/voltajeContadorMuestras;                                
    mediaVoltajeRMS = (sqrt(voltajeMedio))*1.5;                                                 
    adjusteMediaVoltajeRMS = mediaVoltajeRMS ;                                                                                                                                                                                     /* square root of the average value*/                                                                                                             
    voltajeFinalRMS = mediaVoltajeRMS ;                                       
    if(voltajeFinalRMS <= 2.5) voltajeFinalRMS = 0;                                           
    Serial.print(" El valor de voltaje RMS es: ");                                             
    Serial.print(voltajeFinalRMS,decimalPrecision);
    Serial.println(" V ");
    voltajeSumaMuestras =0;                                                                    
    voltajeContadorMuestras=0;  
    
    if (voltajeFinalRMS>190){
      tiempoConLuz=millis();
      if((tiempo-tiempoSinLuz)>10000 and flag_Luz==0)
      {
        flag_Luz=1;
        Enviar_msj(Jose, "Volvio la luz - Pab5");
        delay(120000);      
        Enviar_msj(Leo, "Volvio la luz - Pab5");
        delay(120000);
        Enviar_msj(Seba, "Volvio la luz - Pab5");
        delay(120000);
        Enviar_msj(Cris, "Volvio la luz - Pab5");
        delay(120000);
        Enviar_msj(Mariano, "Volvio la luz - Pab5");
        delay(120000);
      }
    }
    
    if (voltajeFinalRMS<180) 
    {
      tiempoSinLuz=millis();
      if(int(tiempo-tiempoConLuz)>10000 and flag_Luz==1)
      {
        flag_Luz=0;
        Enviar_msj(Jose, "Corte de luz en Pab5");
        delay(120000);
        Enviar_msj(Leo, "Corte de luz - Pab5");
        delay(120000);
        Enviar_msj(Seba, "Corte de luz - Pab5");
        delay(120000);
        Enviar_msj(Cris, "Corte de luz - Pab5");
        delay(120000);
        Enviar_msj(Mariano, "Corte de luz - Pab5");
        delay(120000);
      }                                   
    }
    
  }//ContadorMuestras
}//Loop

void Enviar_msj(String numero, String msj)
{
  //Se establece el formato de SMS en ASCII
  String config_numero = "AT+CMGS=\"+54" + numero + "\"\r\n";
  //configurar modulo como modo SMS
  SIM800L.print("AT+CMGF=1\r\n");
  delay(500);
  //Enviar comando para un nuevos SMS al numero establecido
  SIM800L.print(config_numero);
  delay(500);
  //Enviar contenido del SMS
  SIM800L.print(msj);
  delay(500);
  //Enviar Ctrl+Z
  SIM800L.write((char)26);
  delay(500);
  Serial.print("Mensaje enviado: "); Serial.println(msj);
  SIM800L.write("AT+CMGDA=DEL ALL\r\n");
}
