#include <NewPing.h>

// Pinos digitais das variáveis de controle do projeto --> sinais temporários com base em uma leitura
#define botao_on_off  7
#define valvula  8                                                // Sinal controla o módulo relé e a válvula

// sensor ultrassônico
#define trig 10                                                   // trigger (sinal enviado)
#define echo 11                                                   // echo (sinal de retorno)
unsigned int pingSpeed = 100;                                     // Frequência de envio do ping (em milissegundos). 100ms seria 10 vezes por segundo.
unsigned long pingTimer;                                          // Armazena o próximo tempo de ping. 

// Informações sobre o tanque                                           
#define Altura_do_tanque 20                                       // --> altura em cm
float Altura_nivel_maximo = Altura_do_tanque - Altura_do_tanque * 0.65; // --> capacidade máxima do tanque 65%     

// Declaração do sensor
NewPing sonar(trig, echo, Altura_do_tanque); // Configuração do NewPing com pinos e distância máxima.

// Níveis dos sinais --> sinal alto (HIGH ou 1) ; sinal baixo (LOW ou 0) ;
int sinal_botao_on_off = 0;                                      // --> estado Desligado / Ligado do sistema
int estado_anterior_botao = LOW;                                 // --> Armazena o estado anterior do botão
int sinal_controle_sensor_ultrasonico = 0;                       // --> estado tanque Vazio / Cheio 

// Declaração das funções do Projeto
void leitura_botao(); 
void Medir_distancia();

// Configuração dos pinos de entrada e saída do Arduino
void setup() {
  pinMode(botao_on_off, INPUT);
  pinMode(valvula, OUTPUT);  // Corrigido para OUTPUT
  digitalWrite(valvula, LOW);  // Inicia com a válvula desligada
  pingTimer = millis();
}

// Código em loop
void loop() {
  leitura_botao();  // Verifica o estado do botão

  if (millis() >= pingTimer) {   // Se passou o tempo de ping, envia outro ping
    pingTimer += pingSpeed;      // Define o próximo tempo de ping
    sonar.ping_timer(Medir_distancia); // Envia o ping, chama a função "Medir_distancia" para verificar o status do ping
  }
}

// Função para leitura e tratamento do estado do botão
void leitura_botao() {
  int leitura_botao_atual = digitalRead(botao_on_off);

  // Verifica se o botão foi pressionado (mudança de estado LOW -> HIGH)
  if (leitura_botao_atual == HIGH && estado_anterior_botao == LOW) {
    sinal_botao_on_off = !sinal_botao_on_off; // Inverte o estado do sistema (liga/desliga)
    // digitalWrite(valvula, sinal_botao_on_off); // Atualiza o estado da válvula conforme o estado do sistema
  }

  estado_anterior_botao = leitura_botao_atual; // Atualiza o estado anterior do botão
}

// Função para medir a distância e controlar a válvula conforme o nível do tanque
void Medir_distancia() {
  if (sonar.check_timer()) {
    float ultima_medicao = sonar.ping_result / US_ROUNDTRIP_CM; // Converte a medição para centímetros
    if (ultima_medicao > Altura_nivel_maximo) {
      sinal_controle_sensor_ultrasonico = 0; // Tanque ainda não está cheio
      if (sinal_botao_on_off == 1) {
        digitalWrite(valvula, HIGH); // Liga a válvula para encher o tanque
      }
    } else {
      sinal_controle_sensor_ultrasonico = 1; // Tanque está cheio
      digitalWrite(valvula, LOW); // Fecha a válvula para segurança
    }
  }
}
