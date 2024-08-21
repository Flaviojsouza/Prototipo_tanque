// Pinos digitais das variaveis de controle do projeto -->sinais temporarios com base em uma leitura
#define botao_on_off  7;
#define valvula  8;                                                       //Sinal controla o modulo rele e a valvula
#define sensor_ultrasonico  12;

// Informações sobre o tanque
unsigned float Altura_do_tanque = 0;
unsigned float Altura_nivel_maximo = Altura_do_tanque * 0.65 ;            // --> capacidade maxima do tanque 65%                              

// Nivel dos sinais --> sinal auto(HIGH ou 1) ; sinal baixo(LOW ou 0) ;
unsigned int sinal_botao_on_off = 0;                                      // --> sinal alterado apenas quando digitalread de botao_on_off é 1
unsigned int sinal_controle_sensor_ultrasonico = 0;                       // --> sinal alterado apenas quando o tanque está cheio

// Declaração das funções do Projeto
void leitura_botao(unsigned int botao);

// Entrada e saida de sinais do arduino
void setup() {
  pinMode(botao, OUTPUT);
  pinMode(valvula, INPUT);
  pinMode(sensor_ultrasonico, OUTPUT)

}

//Código em loop
void loop() {
  leitura_botao(digitalRead(sinal_botao_on_off));
}

// Definição das funções
void leitura_botao(unsigned int botao){
  if( botao == 1){
    if(sinal_botao_on_off == 0){
      // Ligar circuito
      sinal_botao_on_off = 1;
      digitalWrite(valvula, 1);
    }else{
      // Desligar circuito
      sinal_botao_on_off = 0;
      digitalWrite(valvula, 0);
    }
  }
}