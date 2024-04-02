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
  ComecarJogo,
  Torpedo,
  Submarino
};

EstadoInterface State = InterfaceInicial;

// Variáveis de debounce para cada botão
unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;
unsigned long lastDebounceTime3 = 0;
const long debounceDelay = 50; // debounce delay de 50 milissegundos

int buttonState1 = LOW;
int lastButtonState1 = LOW;
int button1;

int buttonState2 = LOW;
int lastButtonState2 = LOW;
int button2;

int buttonState3 = LOW;
int lastButtonState3 = LOW;
int button3;

bool mensagemMostrada = false;

void setup()
{
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  Serial.begin(9600);
}

void loop()
{
  // Verificação de debounce para cada botão
  debounceButton1();
  debounceButton2();
  debounceButton3();

  switch (State)
  {
  case InterfaceInicial:
  {
    if (!mensagemMostrada)
    {
      digitalWrite(ledPin1, HIGH);
      digitalWrite(ledPin2, HIGH);
      digitalWrite(ledPin3, HIGH);

      Serial.println("\n--------------------------------------\n");
      Serial.println("Pressione o botao (1) para iniciar");
      mensagemMostrada = true;
    }

    if (button1 == 1)
    { // Se o botão 1 for pressionado, muda para InterfaceDeSelecao
      State = InterfaceDeSelecao;
      mensagemMostrada = false; // Reset mensagemMostrada para o próximo estado
      button1 = 0;
    }
    else if (button2 == 1)
    {
      Serial.println("Pressione um botao valido");
      mensagemMostrada = false; // Reset mensagemMostrada para o próximo estado
      button2 = 0;
    }

    else if (button3 == 1)
    {
      Serial.println("Pressione um botao valido");
      mensagemMostrada = false; // Reset mensagemMostrada para o próximo estado
      button3 = 0;
    }
  }
  break;

  case InterfaceDeSelecao:
  {
    if (!mensagemMostrada)
    {
      digitalWrite(ledPin1, HIGH);
      digitalWrite(ledPin2, HIGH);
      digitalWrite(ledPin3, HIGH);

      Serial.println("\n--------------------------------------\n");
      Serial.println("Pressione o botao (1) para escolher a posicao dos navios");
      Serial.println("Pressione o botao (2) para iniciar o jogo");
      mensagemMostrada = true;
    }
    if (button1 == 1)
    {
      State = InterfaceDePosicaoDosNavios;
      mensagemMostrada = false;
      button1 = 0;
    }
    else if (button2 == 1)
    {
      State = ComecarJogo;
      mensagemMostrada = false;
      button2 = 0;
    }
    else if (button3 == 1)
    {
      Serial.println("Pressione um botao valido");
      mensagemMostrada = false;
      button3 = 0;
    }
  }
  break;

  case InterfaceDePosicaoDosNavios:
  {
    if (!mensagemMostrada)
    {
      digitalWrite(ledPin1, HIGH);
      digitalWrite(ledPin2, HIGH);
      digitalWrite(ledPin3, HIGH);
      Serial.println("\n--------------------------------------\n");
      Serial.println("Pressione o botao (1) para escolher a posicao do torpedo");
      Serial.println("Pressione o botao (2) para escolher a posicao do submarino");
      Serial.println("Pressione o botao (3) para retornar");
      mensagemMostrada = true;
    }

    if (button1 == 1)
    {
      State = Torpedo;
      mensagemMostrada = false;
      button1 = 0;
    }
    else if (button2 == 1)
    {
      State = Submarino;
      mensagemMostrada = false;
      button2 = 0;
    }
    else if (button3 == 1)
    {
      digitalWrite(ledPin1, LOW);
      digitalWrite(ledPin2, LOW);
      digitalWrite(ledPin3, HIGH);
      State = InterfaceDeSelecao;
      mensagemMostrada = false;
      button3 = 0;
    }
  }
  break;
  }
}

void debounceButton1()
{
  int reading = digitalRead(buttonPin1);

  if (reading != lastButtonState1)
    lastDebounceTime1 = millis();

  if ((millis() - lastDebounceTime1) > 50)
  {
    if (reading != buttonState1)
    {
      buttonState1 = reading;

      if (buttonState1 == HIGH)
      {
        button1 = 1;
      }
    }
  }

  lastButtonState1 = reading;
}

void debounceButton2()
{
  int reading = digitalRead(buttonPin2);

  if (reading != lastButtonState2)
    lastDebounceTime2 = millis();

  if ((millis() - lastDebounceTime2) > 50)
  {
    if (reading != buttonState2)
    {
      buttonState2 = reading;

      if (buttonState2 == HIGH)
      {
        button2 = 1;
      }
    }
  }

  lastButtonState2 = reading;
}

void debounceButton3()
{
  int reading = digitalRead(buttonPin3);

  if (reading != lastButtonState3)
    lastDebounceTime3 = millis();

  if ((millis() - lastDebounceTime3) > 50)
  {
    if (reading != buttonState3)
    {
      buttonState3 = reading;

      if (buttonState3 == HIGH)
      {
        button3 = 1;
      }
    }
  }

  lastButtonState3 = reading;
}
