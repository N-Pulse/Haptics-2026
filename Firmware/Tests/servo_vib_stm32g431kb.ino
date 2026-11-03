/*
  Contrôle d'un servo et de 3 moteurs de vibration via le moniteur série
  Version STM32 (NUCLEO-G431KB, core Arduino "STM32duino")
  ----------------------------------------------------------------------
  Installation : Gestionnaire de cartes -> "STM32 MCU based boards" (STMicroelectronics)
    Outils -> Carte : "Nucleo-32", Board part number : "Nucleo G431KB"
    Outils -> U(S)ART support : "Enabled (generic 'Serial')"
  La bibliothèque Servo est incluse dans le core STM32duino (rien à installer).

  Brochage (étiquettes Arduino Nano du connecteur Nucleo-32) :
    Servo (signal) : D13
    vib1           : D2
    vib2           : D3
    vib3           : D4
    D7             : forcée à LOW (sert de "masse")

  Le port série passe par le ST-LINK (port COM virtuel du câble USB).

  Moniteur série : 9600 bauds, fin de ligne = "Nouvelle ligne" (ou "Les deux, NL & CR")

  Commandes (insensibles à la casse) :
    servo <angle>       -> angle entre -90 et +90 (0 = position centrale)
                           ex : servo 45   /   servo -90
    vib1 [nb]           -> le moteur 1 vibre nb petites fois (3 par défaut)
    vib2 [nb]              ex : vib2        /   vib3 5
    vib3 [nb]              chaque impulsion : 100 ms ON puis 100 ms OFF
    stop                -> arrête tous les moteurs de vibration
    aide                -> affiche la liste des commandes
*/

#include <Servo.h>

// ---------- Brochage ----------
const uint32_t PIN_SERVO  = D12;
const uint32_t PIN_VIB[3] = {D2, D3, D4};
const uint32_t PIN_GND    = D7;

// ---------- Paramètres ----------
const unsigned long DUREE_ON  = 300;   // ms : durée d'une impulsion
const unsigned long DUREE_OFF = 75;   // ms : pause entre deux impulsions
const long NB_IMPULSIONS_DEFAUT = 3;
const long NB_IMPULSIONS_MAX    = 50;  // garde-fou
const uint8_t LONGUEUR_MAX_LIGNE = 32;

// Mêmes largeurs d'impulsion que la bibliothèque Servo d'Arduino (0°..180°)
const int SERVO_MIN_US  = 544;
const int SERVO_MAX_US  = 2400;

Servo servo;

// État de chaque moteur (gestion non bloquante avec millis)
bool          vibActif[3]     = {false, false, false};  // séquence en cours ?
bool          vibOn[3]        = {false, false, false};  // moteur alimenté en ce moment ?
uint8_t       vibRestantes[3] = {0, 0, 0};              // impulsions restant à faire
unsigned long vibDernier[3]   = {0, 0, 0};              // instant du dernier changement

String ligne = "";

// Prototypes
void traiterCommande(String cmd);
bool estEntier(const String &s);
void afficherAide();

// ------------------------------------------------------------
void setup() {
  // D7 en "masse" (sortie à 0 V)
  pinMode(PIN_GND, OUTPUT);
  digitalWrite(PIN_GND, LOW);

  // Moteurs de vibration éteints au démarrage
  for (uint8_t i = 0; i < 3; i++) {
    pinMode(PIN_VIB[i], OUTPUT);
    digitalWrite(PIN_VIB[i], LOW);
  }

  // Servo en position centrale (0° logique = 90° physique)
  servo.attach(PIN_SERVO, SERVO_MIN_US, SERVO_MAX_US);
  servo.write(90);

  Serial.begin(9600);
  Serial.println(F("Pret."));
  afficherAide();
}

// ------------------------------------------------------------
void loop() {
  // Lecture non bloquante des caractères reçus
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      if (ligne.length() > 0) {
        traiterCommande(ligne);
        ligne = "";
      }
    } else if (ligne.length() < LONGUEUR_MAX_LIGNE) {
      ligne += c;
    }
  }

  // Séquences d'impulsions (ON / OFF alternés)
  unsigned long maintenant = millis();
  for (uint8_t i = 0; i < 3; i++) {
    if (!vibActif[i]) continue;

    if (vibOn[i]) {
      // Fin d'une impulsion -> on coupe
      if (maintenant - vibDernier[i] >= DUREE_ON) {
        digitalWrite(PIN_VIB[i], LOW);
        vibOn[i] = false;
        vibDernier[i] = maintenant;
        vibRestantes[i]--;
        if (vibRestantes[i] == 0) {
          vibActif[i] = false;
          Serial.print(F("vib"));
          Serial.print(i + 1);
          Serial.println(F(" : termine"));
        }
      }
    } else {
      // Fin de la pause -> impulsion suivante
      if (maintenant - vibDernier[i] >= DUREE_OFF) {
        digitalWrite(PIN_VIB[i], HIGH);
        vibOn[i] = true;
        vibDernier[i] = maintenant;
      }
    }
  }
}

// ------------------------------------------------------------
void traiterCommande(String cmd) {
  cmd.trim();
  cmd.toLowerCase();

  // --- servo <angle> ---
  if (cmd.startsWith("servo")) {
    String arg = cmd.substring(5);
    arg.trim();
    if (!estEntier(arg)) {
      Serial.println(F("Erreur : angle manquant ou invalide (ex : servo 45)"));
      return;
    }
    long angle = arg.toInt();
    if (angle < -90 || angle > 90) {
      Serial.println(F("Erreur : l'angle doit etre entre -90 et +90"));
      return;
    }
    servo.write((int)(angle + 90));   // -90..+90  ->  0..180
    Serial.print(F("Servo -> "));
    Serial.print(angle);
    Serial.println(F(" deg"));
    return;
  }

  // --- vib1 / vib2 / vib3 [nombre d'impulsions] ---
  if (cmd.startsWith("vib") && cmd.length() >= 4) {
    char n = cmd.charAt(3);
    if (n < '1' || n > '3') {
      Serial.println(F("Erreur : utiliser vib1, vib2 ou vib3"));
      return;
    }
    uint8_t idx = n - '1';

    long nb = NB_IMPULSIONS_DEFAUT;
    String arg = cmd.substring(4);
    arg.trim();
    if (arg.length() > 0) {
      if (!estEntier(arg) || arg.toInt() <= 0 || arg.toInt() > NB_IMPULSIONS_MAX) {
        Serial.println(F("Erreur : nombre d'impulsions invalide (1 a 50)"));
        return;
      }
      nb = arg.toInt();
    }

    // Démarre la séquence par une impulsion immédiate
    digitalWrite(PIN_VIB[idx], HIGH);
    vibActif[idx]     = true;
    vibOn[idx]        = true;
    vibRestantes[idx] = (uint8_t)nb;
    vibDernier[idx]   = millis();

    Serial.print(F("vib"));
    Serial.print(idx + 1);
    Serial.print(F(" : "));
    Serial.print(nb);
    Serial.println(F(" impulsion(s)"));
    return;
  }

  // --- stop ---
  if (cmd == "stop") {
    for (uint8_t i = 0; i < 3; i++) {
      digitalWrite(PIN_VIB[i], LOW);
      vibActif[i] = false;
      vibOn[i] = false;
      vibRestantes[i] = 0;
    }
    Serial.println(F("Tous les moteurs de vibration sont arretes"));
    return;
  }

  // --- aide ---
  if (cmd == "aide" || cmd == "help") {
    afficherAide();
    return;
  }

  Serial.print(F("Commande inconnue : "));
  Serial.println(cmd);
  Serial.println(F("Tapez 'aide' pour la liste des commandes."));
}

// ------------------------------------------------------------
// Vérifie qu'une chaîne est un entier (signe + ou - accepté en tête)
bool estEntier(const String &s) {
  if (s.length() == 0) return false;
  uint8_t debut = 0;
  if (s.charAt(0) == '-' || s.charAt(0) == '+') {
    if (s.length() == 1) return false;
    debut = 1;
  }
  for (uint8_t i = debut; i < s.length(); i++) {
    if (!isDigit(s.charAt(i))) return false;
  }
  return true;
}

// ------------------------------------------------------------
void afficherAide() {
  Serial.println(F("--- Commandes ---"));
  Serial.println(F("servo <angle>     : -90 a +90 (ex : servo 45)"));
  Serial.println(F("vib1 [nb]         : moteur 1, nb impulsions (defaut 3)"));
  Serial.println(F("vib2 [nb]         : moteur 2"));
  Serial.println(F("vib3 [nb]         : moteur 3"));
  Serial.println(F("stop              : arrete toutes les vibrations"));
  Serial.println(F("aide              : affiche cette aide"));
}
