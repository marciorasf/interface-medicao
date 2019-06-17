/*
 * Código para leitura de Tensão, Corrente, Iluminância e Temperatura de projeto de lâmpada LED
 * Código para Arduino
 * Associado ao código em MATLAB, que retorna Potência e Energia
 * 
 * ALUNOS:  Daniel Guimarães Sousa
 *          Filipe Laureano Raso
 *        
 * PROFESSOR: Dalton Martini Colombo
 */

void setup() {
  //Configura os pinos do conversor AD como entradas.
  pinMode(A0,INPUT); //Temperatura
  pinMode(A1,INPUT); //Tensão
  pinMode(A2,INPUT); //Iluminância
  pinMode(A3,INPUT); //Corrente 

  //Configura o registrador ADMUX
  ADMUX = 0x00;
  ADMUX |= 0x40; //Coloca a referencia de tensão em AVcc
  ADMUX |= 0x00; //Escolhe primeiramente a entrada analógica ADC0 

  //Configura a frequência de amostragem e habilita interrupções
  ADCSRA = 0x00;
  ADCSRA |= 0x06; //Ajusta a frequência de amostragem por canal 
    
  ADCSRA |= 0x08; //Habilita as interrupções.
  ADCSRA |= 0x20; //Habilita o auto-trigger.

  //Configura a fonte do trigger para o free running mode
  ADCSRB = 0x00;

  //Desabilita os buffers das entradas digitais.
  DIDR0 = 0xFF;

  //Habilita as interrupções globais do microprocessador.
  SREG |= 0x80;

  ADCSRA |= 0x80; //Habilita o conversor AD
  ADCSRA |= 0x40; //Inicia a aquisição

  //Configuração da comunicação serial.
  Serial.begin(115200);
}

//Constantes
#define tamanhoAC 200
#define tamanhoDC 200
#define CORRENTE 0
#define TEMPERATURA 1
#define TENSAO 2
#define LUMINOSIDADE 3

//Vetores de dados
volatile unsigned int vetorCorrente[tamanhoAC];
volatile unsigned int vetorTemperatura[tamanhoDC];
volatile unsigned int vetorTensao[tamanhoAC];
volatile unsigned int vetorLuminosidade[tamanhoDC];

//Variáveis de controle
volatile bool procStatus = false;
volatile int contadorAC = 0;
volatile int contadorDC = 0;
volatile unsigned long labelTempo = millis();
volatile bool enviar = true;

void enviaPontos(){
 
  for(int i=0;i<tamanhoAC;i++){
    Serial.print(vetorTensao[i]);
    Serial.print(";");
    Serial.print(vetorCorrente[i]);
    Serial.print(";");
    Serial.print(vetorTemperatura[(i-i%(tamanhoAC/tamanhoDC))/(tamanhoAC/tamanhoDC)]);
    Serial.print(";");
    Serial.print(vetorLuminosidade[(i-i%(tamanhoAC/tamanhoDC))/(tamanhoAC/tamanhoDC)]);
    Serial.println(";");
  }
}

void loop() {
  //Verifica se já passou 2 segundos para enviar novos vetores
  if(millis()-labelTempo >= 2000){
    enviar = true;
  }
  //Verifica se está na hora de transmitir os dados.
  if(procStatus == true && enviar==true){

    labelTempo = millis();
    enviar = false;
    
    enviaPontos();
 
  }
  noInterrupts();
  procStatus = false;
  interrupts();
}

ISR(ADC_vect){
  int canal;
  int amostra;
  
  //Le a última amostra coletada.
  amostra = ADCL; //Lê o byte inferior.
  amostra+= ADCH<<8; //Lê o byte superior.

  //Lê o o canal do conversor AD.
  canal =  ADMUX & 0x0F;
  
  //Cálculos para exclusivamente encher os buffers.
  if(procStatus == false){
    if(canal==CORRENTE){
      ADMUX+=1;
      
      if(contadorAC < tamanhoAC){
        vetorCorrente[contadorAC] = amostra;
      }
    }
    
    else if(canal==TEMPERATURA){
     ADMUX+=1;
      
      if(contadorDC < tamanhoDC && contadorAC%(tamanhoAC/tamanhoDC)==0){
        vetorTemperatura[contadorDC] = amostra;
      }      
    }
    else if(canal==TENSAO){
      ADMUX+=1;
      if(contadorAC < tamanhoAC){
        vetorTensao[contadorAC] = amostra;
        contadorAC++;
      }
    }
    else if(canal==LUMINOSIDADE){
     ADMUX-=3;
      
      if(contadorDC < tamanhoDC && contadorAC%(tamanhoAC/tamanhoDC)==0){
       vetorLuminosidade[contadorDC] = amostra;
        contadorDC++;
      }      
    }
}

    //Zera os flags criados para enviar novo conjunto de amostras
    if(contadorAC == tamanhoAC && contadorDC == tamanhoDC){
      contadorAC = 0;
      contadorDC = 0;
      procStatus = true;
    }
  }
  




