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

enum EstadoTorpedo
{
  x1,
  y1,
  x2,
  y2,
  Verificacao
};

int x1T;
int y1T;
int x2T;
int y2T;

// Variáveis para controlar se a mensagem de prompt já foi exibida para cada estado
bool mensagemX1Mostrada = false;
bool mensagemY1Mostrada = false;
bool mensagemX2Mostrada = false;
bool mensagemY2Mostrada = false;

EstadoTorpedo StateTorpedo = x1;
bool torpedo = false;

enum EstadoSubmarino
{
  x1Su,
  y1Su,
  x3Su,
  y3Su,
  VerificacaoSu
};

int x1S;
int y1S;
int x2S;
int y2S;
int x3S;
int y3S;

bool mensagemX1SMostrada = false;
bool mensagemY1SMostrada = false;
bool mensagemX3SMostrada = false;
bool mensagemY3SMostrada = false;
bool submarino = false;

EstadoSubmarino StateSubmarino = x1Su;

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

bool valido;

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

  case Torpedo:
  {
    bool erroEncontrado = false;

    if (!mensagemMostrada)
    {
      Serial.println("\n--------------------------------------\n");
      Serial.println("Por favor, insira as coordenadas do torpedo.");
      mensagemMostrada = true; // Corrige para 'true' para indicar que a mensagem foi mostrada
    }

    switch (StateTorpedo)
    {
    case x1:
    {
      if (!mensagemX1Mostrada)
      {
        Serial.println("Digite a primeira posicao horizontal (x): ");
        mensagemX1Mostrada = true;
      }
      if (Serial.available() > 0)
      {
        x1T = Serial.parseInt();
        Serial.println(x1T);
        StateTorpedo = y1; // Transiciona para o próximo estado
        // Reseta a flag para permitir que a próxima mensagem seja mostrada
        mensagemY1Mostrada = false;
      }
    }
    break;

    case y1:
    {
      if (!mensagemY1Mostrada)
      {
        Serial.println("Digite a primeira posicao vertical (y): ");
        mensagemY1Mostrada = true;
      }
      if (Serial.available() > 0)
      {
        y1T = Serial.parseInt();
        Serial.println(y1T);
        StateTorpedo = x2;
        mensagemX2Mostrada = false;
      }
    }
    break;

    case x2:
    {
      if (!mensagemX2Mostrada)
      {
        Serial.println("Digite a segunda posicao horizontal (x): ");
        mensagemX2Mostrada = true;
      }
      if (Serial.available() > 0)
      {
        x2T = Serial.parseInt();
        Serial.println(x2T);
        StateTorpedo = y2;
        mensagemY2Mostrada = false;
      }
    }
    break;

    case y2:
    {
      if (!mensagemY2Mostrada)
      {
        Serial.println("Digite a segunda posicao vertical (y): ");
        mensagemY2Mostrada = true;
      }
      if (Serial.available() > 0)
      {
        y2T = Serial.parseInt();
        Serial.println(y2T);
        StateTorpedo = Verificacao; // Transiciona para estado Completo
      }
    }
    break;

    case Verificacao:
    {
      if (!valoresEntre0e4(x1T, y1T, x2T, y2T))
      {
        erroEncontrado = true;
        Serial.println("Erro: Valores devem estar entre 0 e 4.");
      }
      else if (!saoAdjacentes(x1T, y1T, x2T, y2T))
      {
        erroEncontrado = true;
        Serial.println("Erro: Os pontos devem ser adjacentes verticalmente ou horizontalmente.");
      }
      if (erroEncontrado)
      {
        Serial.println("Coloque novamente os pontos.");
        StateTorpedo = x1;
        mensagemX1Mostrada = false;
        mensagemY1Mostrada = false;
        mensagemX2Mostrada = false;
        mensagemY2Mostrada = false;
      }
      else
      {
        Serial.println("Pontos adicionados com sucesso.");
        torpedo = true;
        State = InterfaceDePosicaoDosNavios;
        StateTorpedo = x1;
        mensagemX1Mostrada = false;
        mensagemY1Mostrada = false;
        mensagemX2Mostrada = false;
        mensagemY2Mostrada = false;
        mensagemMostrada = false;
      }
    }
    break;
    }
  }
  break;

  case Submarino:
  {
    bool erroEncontrado = false;

    if (!mensagemMostrada)
    {
      Serial.println("\n--------------------------------------\n");
      Serial.println("Por favor, insira as coordenadas do submarino.");
      mensagemMostrada = true; // A mensagem foi mostrada
    }

    switch (StateSubmarino)
    {
    case x1Su:
    {
      if (!mensagemX1SMostrada)
      {
        Serial.println("Digite a primeira posicao horizontal do submarino (x1S): ");
        mensagemX1SMostrada = true;
      }
      if (Serial.available() > 0)
      {
        x1S = Serial.parseInt();
        Serial.println(x1S);
        StateSubmarino = y1Su; // Transiciona para o próximo estado
        // Reseta a flag para permitir que a próxima mensagem seja mostrada
        mensagemY1SMostrada = false;
      }
    }
    break;

    case y1Su:
    {
      if (!mensagemY1SMostrada)
      {
        Serial.println("Digite a primeira posicao vertical do submarino (y1S): ");
        mensagemY1SMostrada = true;
      }
      if (Serial.available() > 0)
      {
        y1S = Serial.parseInt();
        Serial.println(y1S);
        StateSubmarino = x3Su;
        // Reseta a flag para permitir que a próxima mensagem seja mostrada
        mensagemX3SMostrada = false;
      }
    }
    break;

    case x3Su:
    {
      if (!mensagemX3SMostrada)
      {
        Serial.println("Digite a ultima posicao horizontal do submarino (x3S): ");
        mensagemX3SMostrada = true;
      }
      if (Serial.available() > 0)
      {
        x3S = Serial.parseInt();
        Serial.println(x3S);
        StateSubmarino = y3Su;
        // Reseta a flag para permitir que a próxima mensagem seja mostrada
        mensagemY3SMostrada = false;
      }
    }
    break;

    case y3Su:
    {
      if (!mensagemY3SMostrada)
      {
        Serial.println("Digite a ultima posicao vertical do submarino (y3S): ");
        mensagemY3SMostrada = true;
      }
      if (Serial.available() > 0)
      {
        y3S = Serial.parseInt();
        Serial.println(y3S);
        StateSubmarino = VerificacaoSu; // Prepara para verificação
      }
    }
    break;

    case VerificacaoSu:
    {
      // Calcular x2S e y2S com base em x1S, y1S, x3S, y3S
      x2S = (x1S + x3S) / 2; // Ponto intermediário X
      y2S = (y1S + y3S) / 2; // Ponto intermediário Y

      if (!valoresEntre0e4(x1S, y1S, x3S, y3S) || !valoresEntre0e4(x2S, y2S, x2S, y2S))
      {
        erroEncontrado = true;
        Serial.println("Erro: Valores devem estar entre 0 e 4.");
      }
      else if (!saoAdjacentes(x1S, y1S, x2S, y2S) || !saoAdjacentes(x2S, y2S, x3S, y3S))
      {
        erroEncontrado = true;
        Serial.println("Erro: Os pontos do submarino devem ser adjacentes verticalmente ou horizontalmente e formar uma linha reta.");
      }
      else if (!coordenadasUnicas(x1T, y1T, x2T, y2T, x1S, y1S, x2S, y2S, x3S, y3S))
      {
        erroEncontrado = true;
        Serial.println("Erro: As coordenadas do submarino não podem coincidir com as do torpedo.");
      }

      if (erroEncontrado)
      {
        Serial.println("Coloque novamente os pontos do submarino.");
        StateSubmarino = x1Su;
        // Reset das flags
        mensagemX1SMostrada = false;
        mensagemY1SMostrada = false;
        mensagemX3SMostrada = false;
        mensagemY3SMostrada = false;
      }
      else
      {
        Serial.println("Submarino adicionado com sucesso.");
        submarino = true;
        State = InterfaceDePosicaoDosNavios; // Transiciona para o próximo estado
        // Reset das variáveis e flags para possíveis futuras inserções
        StateSubmarino = x1Su;
        mensagemX1SMostrada = false;
        mensagemY1SMostrada = false;
        mensagemX3SMostrada = false;
        mensagemY3SMostrada = false;
        mensagemMostrada = false;
      }
    }
    break;
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

// Funcao para verificar se esta dentro de 0 e 4
bool valoresEntre0e4(int x1, int y1, int x2, int y2)
{
  return x1 >= 0 && x1 <= 4 &&
         y1 >= 0 && y1 <= 4 &&
         x2 >= 0 && x2 <= 4 &&
         y2 >= 0 && y2 <= 4;
}

// Funcao para verificar se são adjacentes
bool saoAdjacentes(int x1, int y1, int x2, int y2)
{
  // Verifica adjacência horizontal
  if (y1 == y2 && (x1 == x2 + 1 || x1 == x2 - 1))
  {
    return true;
  }
  // Verifica adjacência vertical
  if (x1 == x2 && (y1 == y2 + 1 || y1 == y2 - 1))
  {
    return true;
  }
  return false;
}

// Funcao para verificar se os pontos coincidem
bool coordenadasUnicas(int x1T, int y1T, int x2T, int y2T, int x1S, int y1S, int x2S, int y2S, int x3S, int y3S)
{
  // Verifica se algum ponto do torpedo é igual a algum ponto do submarino
  if ((x1T == x1S && y1T == y1S) || (x1T == x2S && y1T == y2S) || (x1T == x3S && y1T == y3S) ||
      (x2T == x1S && y2T == y1S) || (x2T == x2S && y2T == y2S) || (x2T == x3S && y2T == y3S))
  {
    return false; // Ponto coincidente encontrado
  }
  return true; // Nenhum ponto coincidente encontrado
}
