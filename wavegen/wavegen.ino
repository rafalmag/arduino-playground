//waveform generator

//Include LCD library
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

#define pinrotA 10
#define pinrotB 11
#define pinrotX 12

#define nsamp 256
#define dacmax 256

const byte nclk=42; //number of clock cycles of the loop
long int freq; //frequency in mHz
long unsigned int phase;
long unsigned int phase_inc;
byte mode=4; // 0:waveform change >=1:frequency change
byte iwave=0;
#define nwave 20

void setup() {
  TIMSK0 &= ~_BV(TOIE0); // disable timer0 overflow interrupt
  DDRD=255; // pin0-7 on output
  PORTD=0; // all outputs low
 
  //rotary encoder pins
  pinMode(pinrotA, INPUT_PULLUP);
  pinMode(pinrotB, INPUT_PULLUP);
  pinMode(pinrotX, INPUT_PULLUP);

  //enable pin change interrupts on rotary encoder
  PCICR =0x01;          // Enable PCINT0 interrupt
  PCMSK0 = 0b00011100;  // mask for pins 10,11,12
  
  //setup TIMER1 for 1kHz 50% duty cycle pulse on pin D9
  pinMode(9,OUTPUT);
  TCCR1A=B10000000;
  TCCR1B=B00010001;
  OCR1A =4000;
  ICR1  =8000;

  freq=100000;
  phase=0;
  calc_phase_inc();

  lcd.begin(16, 2);
  refresh_display();
  setwave();
}

const float pi=3.14159265;
byte waveform[nsamp];
void setwave(){
  for (int isamp=0; isamp<nsamp; ++isamp){
    float phip=(isamp+0.5)/nsamp;
    float phi=2*pi*phip;
    int val=0;

    //sine
    if (iwave== 0)val=(sin(phi)+1.0)*dacmax/2;

    //square
    if (iwave== 1)val=(dacmax-1)*(phip>0.5);

    //triangle
    if (iwave== 2)val=abs(dacmax*(1.0-2.0*phip));

    //rising sawtooth
    if (iwave== 3)val=dacmax*phip;

    //falling sawtooth
    if (iwave== 4)val=dacmax*(1.0-phip);

    //uniform noise
    if (iwave== 5)val=random(dacmax);

    //gaussian noise
    if (iwave== 6){
      for(byte i=0; i<8; i++)val+=random(dacmax/8);
    }
    
    //pulse20 (20% duty cycle)
    if (iwave== 7)val=(dacmax-1)*(phip<=0.20);

    //pulse10 (10% duty cycle)
    if (iwave== 8)val=(dacmax-1)*(phip<=0.10);

    //pulse05 (5% duty cycle)
    if (iwave== 9)val=(dacmax-1)*(phip<=0.05);
    
    //delta function (1/256 duty cycle)
    if (iwave==10)val=(dacmax-1)*(isamp==0);

    //sine with 2nd harmonic
    if (iwave==11)val=((sin(phi)+0.5*sin(2*phi))/1.30+1)*dacmax/2;

    //sine with 3rd harmonic
    if (iwave==12)val=((sin(phi)+0.333*sin(3*phi))/0.943+1)*dacmax/2;

    //chopped sine
    if (iwave==13)val=(1.2*sin(phi)+1.0)*dacmax/2;
    
    //sinc function with 10 peaks/cycle
    if (iwave==14)val=(sin(10.5*(phi-pi))/(10.5*(phi-pi))+0.22)/1.22*dacmax;
    
    //sinc function with 20 peaks/cycle
    if (iwave==15)val=(sin(20.5*(phi-pi))/(20.5*(phi-pi))+0.22)/1.22*dacmax;

    //sinabs function
    if (iwave==16)val=sin(phi/2)*dacmax;

    //trapezoid
    if (iwave==17 and phip>0.00)val=4*phip*dacmax;
    if (iwave==17 and phip>0.25)val=(dacmax-1);
    if (iwave==17 and phip>0.50)val=(3-4*phip)*dacmax;
    if (iwave==17 and phip>0.75)val=0;

    //step2
    if (iwave==18 and phip>0.25)val=dacmax/2;
    if (iwave==18 and phip>0.50)val=dacmax-1;
    if (iwave==18 and phip>0.75)val=dacmax/2;

    //step4
    if (iwave==19 and phip>0.125)val=1*dacmax/4;
    if (iwave==19 and phip>0.250)val=2*dacmax/4;
    if (iwave==19 and phip>0.375)val=3*dacmax/4;
    if (iwave==19 and phip>0.500)val=dacmax-1;
    if (iwave==19 and phip>0.625)val=3*dacmax/4;
    if (iwave==19 and phip>0.750)val=2*dacmax/4;
    if (iwave==19 and phip>0.875)val=1*dacmax/4;

    val=max(val,0);
    val=min(val,dacmax-1);
    waveform[isamp]=val;
  }
}


//calculate the phase increment. 2^32/16E6=268.435456
void calc_phase_inc(){
  phase_inc=0.268435456*nclk*freq;    
}

//regular running: generate waves
void loop() {
  phase+=phase_inc;
  int redphase=phase>>24;
  PORTD=waveform[redphase];
}

//react to pin change of the rotary encoder
volatile byte prevrotstat=0;
ISR (PCINT0_vect){
  byte rotstat=(PINB&B00011100)>>2;
  bool pressed=((rotstat&B100)==0);
  // RM: swapped rigth-left
  bool left=(((rotstat&B11)==0) and ((prevrotstat&B11)==B10));
  bool right=(((rotstat&B11)==0) and ((prevrotstat&B11)==B01));

  if(pressed and right and mode<9)mode++;
  if(pressed and left and mode>0)mode--;
  if( not pressed and right){
    if(mode==0 and iwave<nwave-1){iwave++; setwave();}     
    if(mode==1)freq+=100000000;
    if(mode==2)freq+=10000000;
    if(mode==3)freq+=1000000;
    if(mode==4)freq+=100000;
    if(mode==5)freq+=10000;
    if(mode==6)freq+=1000;
    if(mode==7)freq+=100;
    if(mode==8)freq+=10;
    if(mode==9)freq+=1;
    if(freq>=100000000)freq=100000000;
    calc_phase_inc();
  }
  if( not pressed and left){
    if(mode==0 and iwave>0){iwave--; setwave();}     
    if(mode==1)freq-=100000000;
    if(mode==2)freq-=10000000;
    if(mode==3)freq-=1000000;
    if(mode==4)freq-=100000;
    if(mode==5)freq-=10000;
    if(mode==6)freq-=1000;
    if(mode==7)freq-=100;
    if(mode==8)freq-=10;
    if(mode==9)freq-=1;
    if(freq<0)freq=0;
    calc_phase_inc();
  }
  if (left or right)refresh_display();
  prevrotstat=rotstat;
}

// fetch the nth digit of a number
long unsigned int pow10[]={1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000};
byte dig(long unsigned int num,byte e){
  return (num%pow10[e+1])/pow10[e];
}


void refresh_display(){
  lcd.clear();
  lcd.print("WAVE:");
  if (iwave== 0)lcd.print("sine");
  if (iwave== 1)lcd.print("square");
  if (iwave== 2)lcd.print("triangle");
  if (iwave== 3)lcd.print("sawrise");
  if (iwave== 4)lcd.print("sawfall");
  if (iwave== 5)lcd.print("uninoise");
  if (iwave== 6)lcd.print("gaussnoise");
  if (iwave== 7)lcd.print("pulse20");
  if (iwave== 8)lcd.print("pulse10");
  if (iwave== 9)lcd.print("pulse05");
  if (iwave==10)lcd.print("delta");
  if (iwave==11)lcd.print("harm2");
  if (iwave==12)lcd.print("harm3");
  if (iwave==13)lcd.print("chopsine");
  if (iwave==14)lcd.print("sinc10");
  if (iwave==15)lcd.print("sinc20");
  if (iwave==16)lcd.print("sinabs");
  if (iwave==17)lcd.print("trapezoid");
  if (iwave==18)lcd.print("step2");
  if (iwave==19)lcd.print("step4");

  lcd.setCursor(0,1);
  lcd.print("f=");
  lcd.print(dig(freq,8));
  lcd.print(dig(freq,7));
  lcd.print(dig(freq,6));
  lcd.print(",");
  lcd.print(dig(freq,5));
  lcd.print(dig(freq,4));
  lcd.print(dig(freq,3));
  lcd.print(".");
  lcd.print(dig(freq,2));
  lcd.print(dig(freq,1));
  lcd.print(dig(freq,0));
  lcd.print("Hz");
  if(mode==0)lcd.setCursor(5,0);
  if(mode==1)lcd.setCursor(2,1);
  if(mode==2)lcd.setCursor(3,1);
  if(mode==3)lcd.setCursor(4,1);
  if(mode==4)lcd.setCursor(6,1);
  if(mode==5)lcd.setCursor(7,1);
  if(mode==6)lcd.setCursor(8,1);
  if(mode==7)lcd.setCursor(10,1);
  if(mode==8)lcd.setCursor(11,1);
  if(mode==9)lcd.setCursor(12,1);
  lcd.blink();

}
