// Incluir as bibliotecas necessarias
#include <OneWire.h> // Responsável por coordenar a comunicacao com o sensor de temperatura (caso haja)
#include <DallasTemperature.h> // Responsável por receber os dados do sensor de temperatura (caso haja) e os interpretar

#define pino_pH A0 // Define o pino do Arduino que recebera os dados do sensor de pH

// Pre configurar a comunicacao entre o DS18B20 e o Arduino
#define ONE_WIRE_BUS 2 // Define o pino do Arduino que recebera os dados do sensor de temperatura (caso haja) transferidos pelo oneWire 

int medida_pH; // Variável que armazenará o valor correspondente à voltagem (em LSB) vinda do sensor de pH
float voltagem_pH; // Variável que armazenará o valor correspondente à voltagem vinda do sensor de pH
float pH; // Variável que armazenará o valor de pH obtido (varia de 0 a 14

OneWire oneWire(ONE_WIRE_BUS); // Estabelece comunicacao via oneWire com o DS18B20 (caso haja) atraves do pino ja definido

DallasTemperature sensor(&oneWire); // Aponta para a biblioteca a presenca de sensores de temperatura (caso haja) conectados atraves da comunicacao estabelecida

// Comandos executados ao inicializar o Arduino
void setup() {

  Serial.begin(9600); // Inicia a porta serial com velocidade de 9600

  pinMode(pino_pH, INPUT); // Declara o pino do sensor para entrada de dados

  sensor.begin(); // Inicia os sensores de temperatura (caso haja)
}

// Comandos que serao executados repetidamente (loop)
void loop() {

  medida_pH = analogRead(pino_pH);  // Lê a voltagem enviada pelo sensor e a armazena como um valor entre 0 e 1023 e a armazena na variável "medida_pH"

  voltagem_pH = 5 / 1023.0 * medida_pH; // Converte o valor adquirido, obtendo a voltagem em Volts (V) e a armazena na variável "voltagem_pH"

  pH = 7 + ((2.5 - voltagem_pH) / 0.18); // Interpreta o valor adquirido, obtendo o pH  e a armazena na variável "pH"
  //pH = 7 + ((2.5 - (5 / 1023.0 * medida_pH)) / 0.18);
  // OBS: Essa converção é realizada com o auxílio da equação: pH = 7 + ((2.5 - (5 / 1023.0 * medida_pH)) / 0.18)

  if(pH<0 or pH>14){ // Caso corroa um erro na medição, o valor do pH será algum número fora da faixa 0 - 14
    Serial.print("ERRO: pH INVÁLIDO (pH = ");
    Serial.print(pH);
    Serial.print(")");
    Serial.println();
  }else{ // Caso a medição ocorra normalmente

    sensor.requestTemperatures(); // Solicita ao sensor os dados de temperatura (caso haja)

    float tempC = sensor.getTempCByIndex(0); // Recebe o dado de temperatura, o converte para graus Celsius e o armazena na variavel tempC

    if(tempC == -127.0){ // Caso não haja um sensor de temperatura, tempC será igual a -127.0, e o programa prossegue sem o dado de temperatura, apenas com o pH

      Serial.print("pH: "); // Imprime o rótulo "pH: "
      Serial.print(pH, 2); // Imprime o valor do pH obtido (0 a 14, com aproximação de 2 casas decimais)

      Serial.println(""); // Imprime uma quebra de linha

      /* O resultado fica como:
    
         pH: 7.00 
      
      */
      
    }else{ // Caso tempC seja diferente de -127.0, é porque foi detectado um sensor de temperatura e o programa prosseguirá com o dado de temperatura e pH
      
      Serial.print("pH: "); // Imprime o rótulo "pH: "
      Serial.print(pH, 2); // Imprime o valor do pH obtido (0 a 14, com aproximação de 2 casas decimais)

      Serial.print(", Temperatura: "); // Imprime o rótulo ", Temperatura: "
      Serial.print(tempC); // Imprime o valor da temperatura obtida (em graus Celsius)
      Serial.print(" °C"); // Imprime o rótulo " °C"

      Serial.println(""); // Imprime uma quebra de linha

      /* O resultado fica como:
    
         pH: 7.00, Temperatura: 25.00 °C
      
      */
    }
  }
  
  delay(3000); // Estabelece um intervalo em milisegundos entre as medicoes (3000 ms = 3 s)

}
