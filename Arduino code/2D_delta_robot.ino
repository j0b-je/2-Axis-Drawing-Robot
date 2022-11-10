#include <VarSpeedServo.h> //deze bibliotheek maakt het mogelijk om de draaisnelheid van de servos aan te passen
#include <math.h> // deze bibliotheek maakt het mogelijk om sinus cosinus en tangens te gebruiken

VarSpeedServo servo1;
VarSpeedServo servo2;

int Tel = 0; //deze variabelen is bedoeld om tussen de coordinaten te varieren

float L = 104.5; //lengte van de bovenarm
float B = 71;// lengte onderarm. dit was eerst 90mm maar was later terug gegaan naar 71m

float SxR = 30;
float SyR = 10;

float SxL = -30;
float SyL = 10;

float X1 = 0;// het X coordinaat waar de servo start
float Y1 = 0;// het Y coordinaat waar de servo start

float X2 = 0;// het X coordinaat waar de servo naar toe gaat
float Y2 = 0;// het Y coordinaat waar de servo naar toe gaat

int FX = 100;// de hoe veel tussen stappen er tussen twee punten wordt genomen
// de waardes hier onder worden gebruikt voor het berekenen van de tussenpunten
float TPX = 0;
float DeltaX = 0;

float TPY = 0;
float DeltaY = 0;

float ResX = 0;
float ResY = 0;

int TellerTP = 0;

float CX = -5;// deze waarden wordt gebruikt om de coordinaten van het figuur op te schuifen.
float CY = -5;

void setup() {
  servo1.attach(10);
  servo2.attach(11);
  Serial.begin(9600);
}

void loop()
{
  // hieronder worden de X en Y coordinaten in gevuld. X2 en Y2 zijn er om de tussen coordinaten te berekenen
  if (Tel == 0) {

    X1 = -17.45 + CX;
    Y1 = 184.95 + CY;

    X2 = -3.73 + CX;
    Y2 = 133.73 + CY;

  }
  if (Tel == 1) {

    X1 = -3.73 + CX;
    Y1 = 133.73 + CY;

    X2 = -54.95 + CX;
    Y2 = 120 + CY;
  }

  if (Tel == 2) {

    X1 = -54.95 + CX;
    Y1 = 120 + CY;

    X2 = 15 + CX;
    Y2 = 82.5 + CY;

  }

  if (Tel == 3) {

    X1 = 10 + CX;
    Y1 = 82.5 + CY;

    X2 = 47.5 + CX;
    Y2 = 147.45 + CY;
  }


  if (Tel == 4) {

    X1 = 47.5 + CX;
    Y1 = 147.5 + CY;

    X2 = -17.45 + CX;
    Y2 = 184.95 + CY;
  }

  if (Tel == 5) {

    X1 = -17.45 + CX;
    Y1 = 184.95 + CY;

    X2 = -54.95 + CX;
    Y2 = 120 + CY;
  }

  if (Tel == 6) {

    delay(200000);
  }
  //Tussen en uiteindlijke positie berekenen
  DeltaX = X2 - X1;
  ResX = DeltaX / FX;

  DeltaY = Y2 - Y1;
  ResY = DeltaY / FX;

  for (int TellerTP = 0; FX >= TellerTP; TellerTP++) {

    TPX = X1 + (ResX * TellerTP);         //Tussenpositie voor X

    TPY = Y1 + (ResY * TellerTP);         //Tussenpositie voor X

    double xy = sqrt(pow(TPX, 2) + pow(TPY, 2));

    //Rechterzijde hoek berekenen

    double S = sqrt(pow(SxR, 2) + (SyR, 2));

    double P = sqrt(pow((TPX - SxR), 2) + pow((TPY - SyR), 2));

    double beta = acos((pow(L, 2) - pow(B, 2) - pow(P, 2)) / (-2 * B * P));
    double gamma = acos((pow(xy, 2) - pow(P, 2) - pow(S, 2)) / (-2 * P * S));
    double omega = atan(SyR / SxR);

    double aR = PI - (double)beta - (double)gamma + (double)omega;

    double ServohoekR = (((double)aR * 4068) / 71);

    //Linkerzijde hoek berekenen

    double SL = sqrt(pow(SxL, 2) + (SyL, 2));

    double PL = sqrt(pow((TPX - SxL), 2) + pow((TPY - SyL), 2));

    double betaL = acos((pow(L, 2) - pow(B, 2) - pow(PL, 2)) / (-2 * B * PL));
    double gammaL = acos((pow(xy, 2) - pow(PL, 2) - pow(SL, 2)) / (-2 * PL * SL));
    double omegaL = atan(SyL / SxL);

    double aL = PI - (double)betaL - (double)gammaL - (double)omegaL;

    double ServohoekL = 180 - ((double)aL * 4068) / 71;


    Serial.println((double)ServohoekR);
    Serial.println((double)ServohoekL);

    Serial.println((float)DeltaX);
    Serial.println((float)DeltaY);

    servo1.write(ServohoekL, 20);
    servo2.write(ServohoekR, 20);

    servo1.wait();//hier wacht het programma tot dat de servos de gewenste stand hebben bereikt
    servo2.wait();

  }

  Tel ++;//de teller om hoog zodat de volgende coordinaten bekend worden.

}
