#include<Arduino.h>
#include<stdint.h>

// Definimos algunas variables globales 
#define LED_1 27
#define LED_2 14
#define LED_3 13
#define POT_1 12

char opcion;
int seleccion;
String tiempo;
String LED;
String Tiempo;
int coma;
String cadena;
int valor = 0;
float voltaje = 0;

// Definimos prototipos de función
void Modulo_UART();
void Lect_ADC();
void Seleccion_LEDS();
void LEDS_seleccion();

void setup() {
  // Configuración inicial
  pinMode(POT_1, INPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT); 

  // Inicializar comunicación UART
  Serial.begin(115200);
}

void loop() {
  Modulo_UART();  // Mostrar el menú

  if (opcion == '1') {  // Comparar con caracteres individuales
    Lect_ADC();
    delay(100);
  }
  else if (opcion == '2') {
    Seleccion_LEDS();
  }
}

void LEDS_seleccion(void) {
  Serial.print("Usted ha seleccionado la led:");
  Serial.print(seleccion);
  Serial.print("y el tiempo");
  Serial.print(tiempo);
  
  if (seleccion == 1) {
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, LOW);
    delay(tiempo.toInt());  // Convertir String a int
    digitalWrite(LED_1, LOW);
  }
  else if (seleccion == 2) {
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, HIGH);
    digitalWrite(LED_3, LOW);
    delay(tiempo.toInt());
    digitalWrite(LED_2, LOW);
  }
  else if (seleccion == 3) {
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, HIGH);
    delay(tiempo.toInt());
    digitalWrite(LED_3, LOW);
  }
  else {
    Serial.println("Error: LED inválido");
  }
}

void Modulo_UART(void) {
  // Mostrar el menú al usuario
  Serial.println("1. Lectura ADC");
  Serial.println("\n");
  Serial.println("2. Controlar LEDs");
  
  if (Serial.available() > 0) {
    opcion = Serial.read();  // Leer solo un carácter
  }
}

void Lect_ADC(void) {
  // Leer el valor del potenciómetro (ADC)
  valor = analogRead(POT_1);
  voltaje = valor * (3.3 / 4095.0);  // Convertir a voltaje (resolución de 12 bits)
  Serial.print("El voltaje es: ");
  Serial.print(voltaje);
  Serial.println(" V");
  delay(1000);
}

void Seleccion_LEDS(void) {
  // Pedir al usuario que elija el LED y el tiempo
  Serial.println("Ingrese el LED a encender (1, 2, 3) y el tiempo en milisegundos separados por coma (Ej: 1,500):");
  
  while (Serial.available() == 0) {
    // Esperar a que el usuario ingrese algo
  }

  // Leer la cadena ingresada por el usuario
  cadena = Serial.readStringUntil('\n'); 
  
  // Buscar la coma en la cadena
  coma = cadena.indexOf(',');

  // Verificar que la coma exista en la cadena
  if (coma != -1) {
    // Extraer el número de LED (parte antes de la coma)
    LED = cadena.substring(0, coma);
    // Extraer el tiempo (parte después de la coma)
    Tiempo = cadena.substring(coma + 1);

    // Convertir los valores de String a entero
    seleccion = LED.toInt();
    tiempo = Tiempo;

    // Validar que los datos ingresados sean válidos
    if ((seleccion >= 1 && seleccion <= 3) && (tiempo.toInt() > 0)) {
      LEDS_seleccion();  // Encender el LED seleccionado con el tiempo especificado
    } else {
      Serial.println("Error: Asegúrese de ingresar un LED válido (1, 2, 3) y un tiempo mayor a 0.");
    }
  } else {
    Serial.println("Formato inválido. Use el formato n,m (Ej: 1,500).");
  }
}
