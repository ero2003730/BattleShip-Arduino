

const int ledPin1 = 2;
const int ledPin2 = 4;
const int ledPin3 = 6;
const int buttonPin1 = 8;
const int buttonPin2 = 10;
const int buttonPin3 = 12;

enum EstadoInterface
{
  InterfaceInicial,
  InterfaceDeSelecao,
  InterfaceDePosicaoDosNavios,
  ComecarJogo
};

unsigned long previousMillis = 0;
unsigned long lastDebounceTime = 0;

int State = 0;
int buttonState;           // the current reading from the input pin
int lastButtonState = LOW; // the previous reading from the input pin

bool mensagemMostrada = false;

void setup()
{
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(buttonPin1, OUTPUT);
  pinMode(buttonPin2, OUTPUT);
  pinMode(buttonPin3, INPUT);

  Serial.begin(9600);
}

void loop()
{
  switch (State)
  {
  case InterfaceInicial:
  {
    // Aciona os LEDs apenas uma vez
    if (!mensagemMostrada)
    {
      digitalWrite(ledPin1, HIGH);
      digitalWrite(ledPin2, HIGH);
      digitalWrite(ledPin3, HIGH);

      Serial.println("Pressione o botao (1) para iniciar\n");
      mensagemMostrada = true; // Impede que a mensagem seja exibida novamente
    }

    if (Serial.available() > 0)
    {
      int flag = Serial.read(); // Lê o caractere recebido

      // Converte o caractere para número e compara
      if (flag == '1')
      {
        State = InterfaceDeSelecao;
        mensagemMostrada = false; // Reseta a flag para a próxima vez
      }

      else
      {
        Serial.println("Usuario digitou algo diferente de 1, escreva novamente\n");
        mensagemMostrada = false;
      }
    }
  }
  break;

  case InterfaceDeSelecao:
  {
    // Aciona os LEDs apenas uma vez
    if (!mensagemMostrada)
    {
      digitalWrite(ledPin1, LOW);
      digitalWrite(ledPin2, LOW);
      digitalWrite(ledPin3, LOW);

      Serial.println("Estado proximo\n");
      mensagemMostrada = true; // Impede que a mensagem seja exibida novamente
    }
  }
  }
}