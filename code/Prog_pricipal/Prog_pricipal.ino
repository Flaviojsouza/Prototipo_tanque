// importar biblioteca
#include <NewPing.h>

// Pinos digitais das variaveis de controle do projeto --> sinais temporarios com base em uma leitura
#define botao_on_off  7
#define valvula  8                                                //Sinal controla o modulo rele e a valvula

// sensor ultrasonico
#define trig 10                                                   // trigger (sinal enviado)
#define echo 11                                                   // echo ( sinal de retorno)
unsigned int pingSpeed = 100;                                     // How frequently are we going to send out a ping (in milliseconds). 100ms would be 10 times a second.
unsigned long pingTimer;                                          // Holds the next ping time. 

// Informações sobre o tanque                                           
#define Altura_do_tanque 45                                                         // --> altura em cm
float Altura_nivel_maximo = Altura_do_tanque - Altura_do_tanque * 0.65 ;            // --> capacidade maxima do tanque 65%     

// Declaração do sensor
NewPing sonar(trig, echo, Altura_do_tanque); // NewPing setup of pins and maximum distance.

// Nivel dos sinais --> sinal auto(HIGH ou 1) ; sinal baixo(LOW ou 0) ;
int sinal_botao_on_off = 0;                                      // --> sinal alterado apenas quando digitalread de botao_on_off é 1 ; estado Desligado / Ligado do sistema
int sinal_controle_sensor_ultrasonico = 0;                       // --> sinal alterado apenas quando o tanque está cheio ; estado tanque Vazio / Cheio 

// Declaração das funções do Projeto
void leitura_botao( int botao);
void Medir_distancia();

// Entrada e saida de sinais do arduino
void setup() {
  pinMode(botao_on_off, OUTPUT);
  pinMode(valvula, INPUT);
  pingTimer = millis();

}

//Código em loop
void loop() {
  leitura_botao(digitalRead(sinal_botao_on_off));

  if (millis() >= pingTimer) {   // pingSpeed milliseconds since last ping, do another ping.
    pingTimer += pingSpeed;      // Set the next ping time.
    sonar.ping_timer(Medir_distancia); // Send out the ping, calls "Medir_distancia" function where you can check the ping status.
  }

}

// Definição das funções
void leitura_botao( int botao){
  if( botao == 1){
    if(sinal_botao_on_off == 0){
      // Ligar circuito
      sinal_botao_on_off = 1;
    }else{
      // Desligar circuito
      sinal_botao_on_off = 0;
      digitalWrite(valvula, 0);
    }
  }
}

void Medir_distancia(){
  if (sonar.check_timer()) {
    float ultima_medicao = sonar.ping_result / US_ROUNDTRIP_CM;    //Cm
    if( ultima_medicao > Altura_nivel_maximo){
      sinal_controle_sensor_ultrasonico = 0;                       //Tanque ainda não está cheio
      if (sinal_botao_on_off == 1){
        digitalWrite(valvula, 1);                                  // --> Liga a valbula para encher o tanque
      }
    }else{
      sinal_controle_sensor_ultrasonico = 1;                      //Tanque está cheio
      digitalWrite(valvula, 0);                                   //se o tanque estiver cheio, independente do estado do sistema, a valvula fecha; Medida de segurança
    }
  }
}