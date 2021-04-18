 
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
  DDRB  |= B00100000; // on configure la broche de commande du triac en sortie.
    
   
  PCICR  |= (1 << PCIE0); // on active l'interruption qui detectera le passage par 0.
  PCMSK0 |= (1 << PCINT0); 
  TCCR2A |= (0 << 0); // on configure notre interruption timer en mode normal
 TCCR2A |= (0 << 1);
  TCCR2B = 0b00000010; // on régle notre prédiviseur    
  TIFR2 = 0b00000001;  // on initialise notre drapeaux a 0      
  TCNT2 = 56; // le drapeaux se levera au bout de 200 interruptions TIMER.        

}

void loop() {

    PO = analogRead(0)+100; // lecture de la consigne
    if(PO >= 1023) PO = 1023;
     
   
    VA = analogRead(2); // lecture de la vitesse
     
     
  if(micros()-GOM >= 4000)   { // exécution du régulateur P.I.D toute 0.004 seconde
    GOM = micros();
    CA = PO*0.088669;
     
    
    EP = VA - CA; // Erreur proportionelle
    EP = EP/100;

    EI += EP; // erreur intégrale
    if(EI >= 5) EI = 5; // on limite les valeurs intégrale du régulateur P.I.D entre 5 et -5
    if(EI <= -5) EI = -5;
    
    DE = EP - lastEP; // erreur dérivée 

    lastEP =  EP;

      Com = CP*EP + CI*EI + CD*DE; // Sortie régulateur P.I.D

    }

      
    if(PO >= 150 && RETA <= 1000 ){ // Ce teste de condition permet d'éviter un démarage brusque de votre moteur.
       RETA =RETA+ 1;
       ET = 1;
       Com = 0.005; // déclenchement du triac avec un retard de 5 ms
    } 

     

   if(PO < 150 ){ // Si la valeur de sortie du potentiométre et plus petite que 150 le moteur et etein
      ET = 0;
      RETA = 0;
      }
    
    if(Com >= 0.006) Com = 0.006; // limitation des valeurs de sortie du régulateur P.I.D entre 6ms a 1ms
     if(Com <= 0.001) Com = 0.001;
      
     w = Com/0.000228; 

      if ( TIFR2  & B00000001 ) { // Si Flag TOV2 mis à 1      
    TCNT2 = 56; // on réinitialise le compteur.        
   if(RO == 1){  // Si le signal secteur passe de l'état haut a l'état bas on mes le compteur de temps a 0. 
    varCompteur = 0;
    RO = 0;
   }

   if(ROL == 1){   // Si la commande du triac est a l'état haut on mes le compteur de temps a 0.
    varCompteur = 0;
    ROL = 0;
   }
     
    TIFR2 = B00000001; // on baisse le drapeau du timer 
    varCompteur++;  // on incrémente notre compteur de temps a chaque lever de drapeaux
    
    }


      if (  varCompteur >= w && flag == 1 && ET == 1) { // broche de commande du triac a 1.
     
     PORTB |= B00100000;  // broche du triac mis a 1
     flag = 0;
     LL = 1; // Cette variable indique au systéme si le triac 
     varCompteur = 0;
     ROL = 1;
 } 

 if (LL == 1 &&  varCompteur >= 2 ) { // broche du triac a 0.
    PORTB &= B00000000; // broche du triac mis a 0
        
    varCompteur = 0;
    LL = 0;
   }

}



ISR(PCINT0_vect) // routine interruption Zero cross
{
    
 if (PINB & B00000001) { // detection du passage  a Zero.                                        
       
         
         
          }else  {

         flag = 1;// on autorise la broche du triac a passé a 1.
             varCompteur = 0;
              RO = 1;
          }

  
   }  


    
