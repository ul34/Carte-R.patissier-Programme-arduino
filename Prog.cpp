 volatile unsigned long timer;
volatile unsigned long timera;
volatile unsigned long pulse_duration; 
volatile unsigned long pulse_durationa,REL,DEF;
volatile unsigned long  w = 0; 
int Q,V;
int ET = 0;
volatile byte flag = 0;
 int RETA = 0;
float Com =0.006;
int LL=0;
int RO,ROL = 0;
int R = 0;
int PO = 0;
float CA,VA,EP,EI,DE,lastEP;
float CP = 0.01;
float CI = 0.001;
float CD = 0.002;
long GOM = 0;
long varCompteur = 0;





void setup() {
  DDRB  |= B00100000;
   Serial.begin(57600);
   
  PCICR  |= (1 << PCIE0);
  PCMSK0 |= (1 << PCINT0); 
  TCCR2A |= (0 << 0);
 TCCR2A |= (0 << 1);
  TCCR2B = 0b00000010;       
  TIFR2 = 0b00000001;       
  TCNT2 = 56;        

}

void loop() {

    PO = analogRead(0)+100;
    if(PO >= 1023) PO = 1023;
     
   
    VA = analogRead(2);
     
     
  if(micros()-GOM >= 4000)   {
    GOM = micros();
    CA = PO*0.088669;
     
    
    EP = VA - CA;
    EP = EP/100;

    EI += EP;
    if(EI >= 5) EI = 5;
    if(EI <= -5) EI = -5;
    
    DE = EP - lastEP;

    lastEP =  EP;

      Com = CP*EP + CI*EI + CD*DE;

    }

      
    if(PO >= 150 && RETA <= 1000 ){
       RETA =RETA+ 1;
       ET = 1;
       Com = 0.005;
    } 

     

   if(PO < 150 ){
      ET = 0;
      RETA = 0;
      }
    
    if(Com >= 0.006) Com = 0.006;
     if(Com <= 0.001) Com = 0.001;
      
     w = Com/0.000228; 

      if ( TIFR2  & B00000001 ) {        
    TCNT2 = 56;         
   if(RO == 1){   
    varCompteur = 0;
    RO = 0;
   }

   if(ROL == 1){   
    varCompteur = 0;
    ROL = 0;
   }
     
    TIFR2 = B00000001;
    varCompteur++;  
    
    }


      if (  varCompteur >= w && flag == 1 && ET == 1) {
     
     PORTB |= B00100000;  
     flag = 0;
     LL = 1;
     varCompteur = 0;
     ROL = 1;
 } 

 if (LL == 1 &&  varCompteur >= 2 ) {
    PORTB &= B00000000; 
        
    varCompteur = 0;
    LL = 0;
   }

}



ISR(PCINT0_vect)
{
    
 if (PINB & B00000001) {                                         
       
        timer = micros() ; 
         
          }else  {

         flag = 1;
             varCompteur = 0;
              RO = 1;
          }

  
   }  


    
