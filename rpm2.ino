
hw_timer_t * timer = NULL;
hw_timer_t * timer2 = NULL;
//portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;


void IRAM_ATTR handleInterrupt()
{

  portENTER_CRITICAL_ISR(&mux);
  uint64_t TempVal = timerRead(timer2);        // value of timer at interrupt // cause stack
  PeriodCount = TempVal - StartValue;         // period count between rising edges in 0.000001 of a second
  StartValue = TempVal;                       // puts latest reading as start for next calculation
  portEXIT_CRITICAL_ISR(&mux);
}
void setup(void) {

  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, CHANGE);
  pinMode(interruptPin, INPUT_PULLUP);// sets pin AWARE
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, CHANGE);// cause stack
  timer2 = timerBegin(2, 80, true);
  timerStart(timer2);
}
void loop(void) {

portENTER_CRITICAL(&mux);
  Freg = 1000000.00 / PeriodCount;        // PeriodCount in 0.000001 of a second
  portEXIT_CRITICAL(&mux);
//  Serial.print("Freg:            ");Serial.println(Freg);
  if (PeriodCount == PeriodCounttemp) {PeriodCount = 0;}
  PeriodCounttemp = PeriodCount;          //check if stuck
  if (Freg > 10000) {                     //NOISE
    Freg = 0;
  }
  RPM = (30*Freg)*Co_F;                   //60*1/2*freq
  if (RPM <= 8000) {
    RPM_Real = RPM;
  }
//------------MOVING AVRAGE--------------
  readingsrpm[readIndexrpm] = RPM_Real;
  readIndexrpm = readIndexrpm + 1;
  if (readIndexrpm >= 5) {
    readIndexrpm = 0;
  }
  totalrpm = readingsrpm[0] + readingsrpm[1] + readingsrpm[2] + readingsrpm[3] + readingsrpm[4];
  averagerpm = totalrpm / 5;
  string_rpm = String(averagerpm);
}
