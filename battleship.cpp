// Variaveis dos pinos de entrada
const int ledPin1 = 2;
const int ledPin2 = 4;
const int ledPin3 = 6;
const int buttonPin1 = 8;
const int buttonPin2 = 10;
const int buttonPin3 = 12;

// Estados principais
enum EstadoInterface
{
  InterfaceInicial,
  InterfaceDeSelecao,
  InterfaceDePosicaoDosNavios,
  ArduinoEscolhe,
  Torpedo,
  Submarino,
  ComecarJogo,
  JogoFinalizado
};

EstadoInterface State = InterfaceInicial;

// Subestado para o Estado Torpedo
enum EstadoTorpedo
{
  x1,
  y1,
  x2,
  y2,
  Verificacao
};

// Variaveis que guardam as posicoes do torpedo
int x1T;
int y1T;
int x2T;
int y2T;

// Variáveis para controlar se a mensagem de prompt já foi exibida para cada estado
bool mensagemX1Mostrada = false;
bool mensagemY1Mostrada = false;
bool mensagemX2Mostrada = false;
bool mensagemY2Mostrada = false;
bool torpedo = false;

EstadoTorpedo StateTorpedo = x1;

// Subestado para o Estado Submarino
enum EstadoSubmarino
{
  x1Su,
  y1Su,
  x3Su,
  y3Su,
  VerificacaoSu
};

// Variaveis que guardam as posicoes do submarino
int x1S;
int y1S;
int x2S;
int y2S;
int x3S;
int y3S;

// Variáveis para controlar se a mensagem de prompt já foi exibida para cada estado
bool mensagemX1SMostrada = false;
bool mensagemY1SMostrada = false;
bool mensagemX3SMostrada = false;
bool mensagemY3SMostrada = false;
bool submarino = false;

EstadoSubmarino StateSubmarino = x1Su;

// Subestado para o estado de ComecarJogo
enum EstadoJogar
{
  Ataque,
  Defesa,
};

EstadoJogar StateJogar = Ataque;

// Struct que armazenara a posicao dos ataques
struct Atacar
{
  int x;
  int y;
};

// Variaveis auxiliares para atacar
Atacar ataquesErrados[20];
int totalAtaquesErrados = 0;
Atacar ataquesCertos[5];
int totalAtaquesCertos = 0;

bool mensagemXAMostrada = false;
bool mensagemYAMostrada = false;

int xA = -1;
int yA = -1;

// Subestado do Subestado de ataque
enum EstadoAtaque
{
  x,
  y,
  Ataq
};

EstadoAtaque StateAtaque = x;

// Struct que armazenara a posicao da defesa (ataque do arduino)
struct Defender
{
  int x;
  int y;
};

// Variaveis auxiliares para defender
Defender defesaErrados[20];
int totaldefesaErrados = 0;
Defender defesaCertos[5];
int totaldefesaCertos = 0;

// Coordenadas do Torpedeiro
int x1Ti, y1Ti, x2Ti, y2Ti;

// Coordenadas do Submarino
int x1Si, y1Si, x2Si, y2Si, x3Si, y3Si;

// Flag para assegurar a criação única do cenário
bool cenarioCriado = false;

// Variáveis de debounce para cada botão
unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;
unsigned long lastDebounceTime3 = 0;
unsigned long previousMillis = 0;
unsigned long previousMillisTimer = 0;
const long debounceDelay = 50; // debounce delay de 50 milissegundos

// Variaveis auxiliares para o debounce
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
bool botao3 = false;

// ------------------------------------------------------------------------------------------------------- //
// setup inicial

void setup()
{
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  randomSeed(analogRead(0));
  Serial.begin(9600);
}

// ------------------------------------------------------------------------------------------------------- //
// loop arduino

void loop()
{
  // Verificação de debounce para cada botão
  debounceButton1();
  debounceButton2();
  debounceButton3();

  // ------------------------------------------------------------------------------------------------------- //
  // switch dos estados principais
  switch (State)
  {

  // ------------------------------------------------------------------------------------------------------- //
  // primeiro estado onde se inicia o processo
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
    { // Se o botão 2 for pressionado, isso é um erro
      Serial.println("Pressione um botao valido");
      mensagemMostrada = false; // Reset mensagemMostrada para o próximo estado
      button2 = 0;
    }

    else if (button3 == 1)
    { // Se o botão 3 for pressionado, isso é um erro
      Serial.println("Pressione um botao valido");
      mensagemMostrada = false; // Reset mensagemMostrada para o próximo estado
      button3 = 0;
    }
  }
  break;

  // ------------------------------------------------------------------------------------------------------- //
  // interface de selecao. Para escolher entre colocar navios ou iniciar
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
      if (torpedo && submarino)
      {
        State = ArduinoEscolhe;
        mensagemMostrada = false;
      }
      else if (torpedo && !submarino)
      {
        Serial.println("Adicione o submarino antes");
        mensagemMostrada = false;
      }
      else if (!torpedo && submarino)
      {
        Serial.println("Adicione o torpedo antes");
        mensagemMostrada = false;
      }
      else
      {
        Serial.println("Adicione o torpedo e o submarino antes");
        mensagemMostrada = false;
      }
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

  // ------------------------------------------------------------------------------------------------------- //
  // interface de selecao. Para escolher qual navio vamos selecionar
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

    // ------------------------------------------------------------------------------------------------------- //
    // se botao 1 for clicado entao ir para o estado do Torpedo
    if (button1 == 1)
    {
      State = Torpedo;
      mensagemMostrada = false;
      button1 = 0;
    }

    // ------------------------------------------------------------------------------------------------------- //
    // se botao 2 for clicado entao ir para o estado do Submarino
    else if (button2 == 1)
    {
      State = Submarino;
      mensagemMostrada = false;
      button2 = 0;
    }

    // ------------------------------------------------------------------------------------------------------- //
    // se botao 2 for clicado entao voltar para o estado de Selecao
    else if (button3 == 1)
    {
      State = InterfaceDeSelecao;
      mensagemMostrada = false;
      button3 = 0;
    }
  }
  break;

  // ------------------------------------------------------------------------------------------------------- //
  // inserir os valores no navio torpedeiro
  case Torpedo:
  {
    bool erroEncontrado = false;

    if (!mensagemMostrada)
    {
      Serial.println("\n--------------------------------------\n");
      Serial.println("Por favor, insira as coordenadas do torpedo.");
      mensagemMostrada = true; // Corrige para 'true' para indicar que a mensagem foi mostrada
    }

    // ------------------------------------------------------------------------------------------------------- //
    // realizar uma maquina de estados que transita entre os estados (posicoes) do torpedeiro
    switch (StateTorpedo)
    {

    // ------------------------------------------------------------------------------------------------------- //
    // pegar o x da primeira linha
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

    // ------------------------------------------------------------------------------------------------------- //
    // pegar o y da primeira linha
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
        StateTorpedo = x2; // Transiciona para o próximo estado
        // Reseta a flag para permitir que a próxima mensagem seja mostrada
        mensagemX2Mostrada = false;
      }
    }
    break;

    // ------------------------------------------------------------------------------------------------------- //
    // pegar o x da segunda linha
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
        StateTorpedo = y2; // Transiciona para o próximo estado
        // Reseta a flag para permitir que a próxima mensagem seja mostrada
        mensagemY2Mostrada = false;
      }
    }
    break;

    // ------------------------------------------------------------------------------------------------------- //
    // pegar o y da segunda linha
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
        StateTorpedo = Verificacao; // Transiciona para estado Completo (de verificacao)
      }
    }
    break;

    // ------------------------------------------------------------------------------------------------------- //
    // etapa de verificacao das posicoes do torpedeiro
    case Verificacao:
    {

      // ------------------------------------------------------------------------------------------------------- //
      // verificando para ver se as posicoes estao entre 0 e 4
      if (!valoresEntre0e4(x1T, y1T, x2T, y2T))
      {
        erroEncontrado = true;
        Serial.println("Erro: Valores devem estar entre 0 e 4.");
      }

      // ------------------------------------------------------------------------------------------------------- //
      // verificando para ver se as posicoes (x1,y1) e (x2,y2) são adjacentes
      else if (!saoAdjacentes(x1T, y1T, x2T, y2T))
      {
        erroEncontrado = true;
        Serial.println("Erro: Os pontos devem ser adjacentes verticalmente ou horizontalmente.");
      }

      // ------------------------------------------------------------------------------------------------------- //
      // se a flag for verdadeira, encontrou um erro, então tentar novamente colocar os valores
      if (erroEncontrado)
      {
        Serial.println("Coloque novamente os pontos.");
        StateTorpedo = x1;
        mensagemX1Mostrada = false;
        mensagemY1Mostrada = false;
        mensagemX2Mostrada = false;
        mensagemY2Mostrada = false;
      }

      // ------------------------------------------------------------------------------------------------------- //
      // se nenhum erro for encontrado, entao voltar para a seção de escolher os navios
      else
      {
        Serial.println("Pontos adicionados com sucesso.");
        button1 = 0;
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

    default:
      break;
    }
  }
  break;

  // ------------------------------------------------------------------------------------------------------- //
  // inserir os valores no navio submarino
  case Submarino:
  {
    bool erroEncontrado = false;

    if (!mensagemMostrada)
    {
      Serial.println("\n--------------------------------------\n");
      Serial.println("Por favor, insira as coordenadas do submarino.");
      mensagemMostrada = true; // A mensagem foi mostrada
    }

    // ------------------------------------------------------------------------------------------------------- //
    // realizar uma maquina de estados que transita entre os estados (posicoes) do submarino
    switch (StateSubmarino)
    {

    // ------------------------------------------------------------------------------------------------------- //
    // pegar o x da primeira linha
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

    // ------------------------------------------------------------------------------------------------------- //
    // pegar o y da primeira linha
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
        StateSubmarino = x3Su; // Transiciona para o próximo estado
        // Reseta a flag para permitir que a próxima mensagem seja mostrada
        mensagemX3SMostrada = false;
      }
    }
    break;

    // ------------------------------------------------------------------------------------------------------- //
    // pegar o x da terceira linha
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
        StateSubmarino = y3Su; // Transiciona para o próximo estado
        // Reseta a flag para permitir que a próxima mensagem seja mostrada
        mensagemY3SMostrada = false;
      }
    }
    break;

    // ------------------------------------------------------------------------------------------------------- //
    // pegar o y da terceira linha
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

    // ------------------------------------------------------------------------------------------------------- //
    // etapa de verificacao das posicoes do torpedeiro
    case VerificacaoSu:
    {
      // Calcular x2S e y2S com base em x1S, y1S, x3S, y3S
      x2S = (x1S + x3S) / 2; // Ponto intermediário X
      y2S = (y1S + y3S) / 2; // Ponto intermediário Y

      // ------------------------------------------------------------------------------------------------------- //
      // verificando para ver se as posicoes estao entre 0 e 4
      if (!valoresEntre0e4(x1S, y1S, x3S, y3S) || !valoresEntre0e4(x2S, y2S, x2S, y2S))
      {
        erroEncontrado = true;
        Serial.println("Erro: Valores devem estar entre 0 e 4.");
      }

      // ------------------------------------------------------------------------------------------------------- //
      // verificando para ver se as posicoes (x1,y1), (x2,y2) e (x2,y2), (x3,y3) são adjacentes
      else if (!saoAdjacentes(x1S, y1S, x2S, y2S) || !saoAdjacentes(x2S, y2S, x3S, y3S))
      {
        erroEncontrado = true;
        Serial.println("Erro: Os pontos do submarino devem ser adjacentes verticalmente ou horizontalmente e formar uma linha reta.");
      }

      // ------------------------------------------------------------------------------------------------------- //
      // verificando para ver se os pontos do submarino e os do torpedo coincidem
      else if (!coordenadasUnicas(x1T, y1T, x2T, y2T, x1S, y1S, x2S, y2S, x3S, y3S))
      {
        erroEncontrado = true;
        Serial.println("Erro: As coordenadas do submarino não podem coincidir com as do torpedo.");
      }

      // ------------------------------------------------------------------------------------------------------- //
      // se a flag for verdadeira, encontrou um erro, então tentar novamente colocar os valores
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

      // ------------------------------------------------------------------------------------------------------- //
      // se nenhum erro for encontrado, entao voltar para a seção de escolher os navios
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
        button1 = 0;
      }
    }
    break;

    default:
      break;
    }
  }
  break;

  // ------------------------------------------------------------------------------------------------------- //
  // Estado que gera o topedo e submarino aleatorio para o arduino
  case ArduinoEscolhe:
  {
    // Geração aleatória do Torpedeiro
    x1Ti = random(0, 4); // Gera X inicial (0 a 4)
    y1Ti = random(0, 4); // Gera Y inicial (0 a 4)

    // Escolha a direção do torpedeiro (horizontal ou vertical)
    if (random(0, 2) == 0)
    { // Horizontal
      x2Ti = x1Ti + 1;
      y2Ti = y1Ti;
    }
    else
    { // Vertical
      x2Ti = x1Ti;
      y2Ti = y1Ti + 1;
    }

    bool posicaoValida; // Indica se a posição gerada é válida

    // ------------------------------------------------------------------------------------------------------- //
    // uma estrtura de do while para repetir e achar uma combinacao valida para o submarino
    do
    {
      posicaoValida = true; // Assume inicialmente que a posição é válida

      x1Si = random(0, 4); // Gera X inicial para o submarino
      y1Si = random(0, 4); // Gera Y inicial para o submarino

      // Decide a direção do submarino (0 para horizontal, 1 para vertical)
      int direcao = random(0, 2);

      if (direcao == 0)
      {                  // Horizontal
        x2Si = x1Si + 1; // Ponto médio
        x3Si = x1Si + 2; // Ponto final
        y2Si = y3Si = y1Si;
      }
      else
      {                  // Vertical
        y2Si = y1Si + 1; // Ponto médio
        y3Si = y1Si + 2; // Ponto final
        x2Si = x3Si = x1Si;
      }

      // Verifica se o submarino está dentro do grid
      if (x3Si > 4 || y3Si > 4)
      {
        posicaoValida = false;
      }

      // Verifica se alguma posição do submarino coincide com as do torpedeiro
      if ((x1Si == x1Ti && y1Si == y1Ti) || (x1Si == x2Ti && y1Si == y2Ti) ||
          (x2Si == x1Ti && y2Si == y1Ti) || (x2Si == x2Ti && y2Si == y2Ti) ||
          (x3Si == x1Ti && y3Si == y1Ti) || (x3Si == x2Ti && y3Si == y2Ti))
      {
        posicaoValida = false;
      }

    } while (!posicaoValida); // Repete até que uma posição válida seja encontrada

    cenarioCriado = true; // Marca que o cenário foi criado

    if (!mensagemMostrada)
    {
      Serial.print("Posição inicial do Torpedeiro (X1, Y1): (");
      Serial.print(x1Ti);
      Serial.print(", ");
      Serial.print(y1Ti);
      Serial.println(")");

      Serial.print("Posição final do Torpedeiro (X2, Y2): (");
      Serial.print(x2Ti);
      Serial.print(", ");
      Serial.print(y2Ti);
      Serial.println(")");

      Serial.print("Posição inicial do Submarino (X1, Y1): (");
      Serial.print(x1Si);
      Serial.print(", ");
      Serial.print(y1Si);
      Serial.println(")");

      Serial.print("Posições média e final do Submarino (X2, Y2), (X3, Y3): (");
      Serial.print(x2Si);
      Serial.print(", ");
      Serial.print(y2Si);
      Serial.print(") e (");
      Serial.print(x3Si);
      Serial.print(", ");
      Serial.print(y3Si);
      Serial.println(")");
      mensagemMostrada = true;
    }

    // Depois de printar tudo comecar o jogo
    State = ComecarJogo;
    button1 = 0;
    button2 = 0;
    button3 = 0;
  }
  break;

  // ------------------------------------------------------------------------------------------------------- //
  // Estado que o jogo finalmente comeca
  case ComecarJogo:
  {

    // ------------------------------------------------------------------------------------------------------- //
    // Subestado para controlar entre ataque e defesa
    switch (StateJogar)
    {

    // ------------------------------------------------------------------------------------------------------- //
    // Estado de Ataque
    case Ataque:
    {
      // verificacao para ver se ja passou 30 segundos
      if ((millis() - previousMillis) <= 30000)
      {
        digitalWrite(ledPin1, HIGH);
        digitalWrite(ledPin2, LOW);
        digitalWrite(ledPin3, LOW);

        // caso seja botao 1 ou 2, printar os valores das coordenas corretas ou incorretas
        if (button1 == 1 || button2 == 1)
        {
          // verificacao para ver se acertou alguma coordenada
          if (totalAtaquesCertos == 0)
            Serial.println("O jogador nao acertou nenhuma coordenada");
          else
          {
            Serial.println("Coordenadas corretas:");

            for (int i = 0; i < totalAtaquesCertos; i++)
            {
              Serial.print("(");
              Serial.print(ataquesCertos[i].x);
              Serial.print(", ");
              Serial.print(ataquesCertos[i].y);
              Serial.println(")");
            }
          }

          // verificacao para ver se ja errou alguma coordenada
          if (totalAtaquesErrados == 0)
            Serial.println("O jogador nao errou nenhuma coodenada");
          else
          {
            Serial.println("Coordenadas erradas:");

            for (int i = 0; i < totalAtaquesErrados; i++)
            {
              Serial.print("(");
              Serial.print(ataquesErrados[i].x);
              Serial.print(", ");
              Serial.print(ataquesErrados[i].y);
              Serial.println(")");
            }
          }
          button1 = 0;
          button2 = 0;
        }

        // caso seja botao 3, entao finalizar o jogo
        if (button3 == 1)
        {

          if (!botao3)
          {
            previousMillis = millis();
            Serial.println("Jogador encerrou o jogo");
            botao3 = true; // setar para falar que veio do botao3
            previousMillisTimer = millis();
          }
          State = JogoFinalizado; // mudar de estado para o final
          button3 = 0;
        }

        bool alvoAcertado = false;

        // ------------------------------------------------------------------------------------------------------- //
        // subestado do Ataque para o x
        switch (StateAtaque)
        {

        // ------------------------------------------------------------------------------------------------------- //
        // caso seja primeiro para colocar o valor de x
        case x:
        {
          if (!mensagemXAMostrada)
          {
            Serial.println("Por favor, insira a coordenada x do ataque.");
            mensagemXAMostrada = true;
          }
          if (Serial.available() > 0)
          {
            xA = Serial.parseInt();
            Serial.println(xA);
            StateAtaque = y; // mudar o estado para o y
          }
        }
        break;

        // ------------------------------------------------------------------------------------------------------- //
        // caso seja a hora de colocar eixo y
        case y:
        {
          if (!mensagemYAMostrada)
          {
            Serial.println("Por favor, insira a coordenada y do ataque.");
            mensagemYAMostrada = true;
          }
          if (Serial.available() > 0)
          {
            yA = Serial.parseInt();
            Serial.println(yA);
            StateAtaque = Ataq; // mudar o estado para finalmente o ataque
          }
        }
        break;

        // ------------------------------------------------------------------------------------------------------- //
        // estado Ataque que as verificacoes acontecem
        case Ataq:
        {
          // Verificação de coordenada já utilizada, basicamente verificando se o usuario ja tentou aquele ponto
          bool coordenadaJaUtilizada = false;
          for (int i = 0; i < totalAtaquesCertos; i++)
          {
            if (ataquesCertos[i].x == xA && ataquesCertos[i].y == yA)
            {
              coordenadaJaUtilizada = true;
              break;
            }
          }
          for (int i = 0; i < totalAtaquesErrados; i++)
          {
            if (ataquesErrados[i].x == xA && ataquesErrados[i].y == yA)
            {
              coordenadaJaUtilizada = true;
              break;
            }
          }

          // caso ja tenha atacado aquela posicao pedir uma valida
          if (coordenadaJaUtilizada)
          {
            // Reseta xA e yA após o ataque para evitar reprocessamento
            xA = -1;
            yA = -1;
            Serial.println("Você já atacou essa coordenada. Por favor, escolha outra.");
          }

          // caso ainda nao tenha feito aquele ataque fazer as verificacoes
          else
          { // Verifica se acertou o torpedeiro inimigo
            if ((xA == x1Ti && yA == y1Ti) || (xA == x2Ti && yA == y2Ti))
              alvoAcertado = true;

            // Verifica se acertou o submarino inimigo
            if ((xA == x1Si && yA == y1Si) || (xA == x2Si && yA == y2Si) || (xA == x3Si && yA == y3Si))
              alvoAcertado = true;

            // se o alvo foi acertado entao salvar as posicoes no vetor correto
            if (alvoAcertado)
            {
              Serial.println("Alvo localizado!");
              ataquesCertos[totalAtaquesCertos].x = xA;
              ataquesCertos[totalAtaquesCertos].y = yA;
              totalAtaquesCertos++;
            }

            // se o alvo foi errado entao salvar as posicoes no vetor errado
            else
            {
              Serial.println("Alvo perdido!");
              ataquesErrados[totalAtaquesErrados].x = xA;
              ataquesErrados[totalAtaquesErrados].y = yA;
              totalAtaquesErrados++;
            }

            // se acertar 5 vezes, significa que acertou todos entao encerrar jogo
            if (totalAtaquesCertos == 5)
            {
              digitalWrite(ledPin1, HIGH);
              digitalWrite(ledPin2, HIGH);
              digitalWrite(ledPin3, HIGH);
              Serial.println("Parabens voce ganhou!");
              State = JogoFinalizado; // transicao para estado final
            }

            // se ainda nao tiver acertado, e os valores tiverem sido inseridos
            else
            {
              if (xA != -1 && yA != -1)
              {
                StateJogar = Defesa; // mudar para a vez do arduino
                // Reseta xA e yA após o ataque para evitar reprocessamento
                xA = -1;
                yA = -1;
                StateAtaque = x;
                previousMillis = millis();
              }
            }
          }
        }
        }
      }

      // caso tenha passado 30 segundos passar a vez
      else
      {
        Serial.println("Passou do tempo, sua vez foi 'pulada'");
        StateJogar = Defesa; // mudar para a vez do arduino
        xA = -1;
        yA = -1;
        StateAtaque = x;
        previousMillis = millis();
      }
    }
    break;

    // ------------------------------------------------------------------------------------------------------- //
    // estado de Defesa, que é o ataque do Arduino
    case Defesa:
    {
      bool coordenadaJaUtilizada;
      Defender coordenadaDefesa;

      // estrutura de do while para achar uma posicao que ainda nao foi utilizado
      do
      {
        coordenadaJaUtilizada = false;
        coordenadaDefesa.x = random(0, 5); // Gera um número entre 0 e 4
        coordenadaDefesa.y = random(0, 5); // Gera um número entre 0 e 4

        // Verifica se a coordenada já foi utilizada nos ataques certos
        for (int i = 0; i < totaldefesaCertos; i++)
        {
          if (defesaCertos[i].x == coordenadaDefesa.x && defesaCertos[i].y == coordenadaDefesa.y)
          {
            coordenadaJaUtilizada = true;
            break;
          }
        }

        // Verifica se a coordenada já foi utilizada nos ataques errados, se ainda não foi marcada como utilizada
        if (!coordenadaJaUtilizada)
        {
          for (int i = 0; i < totaldefesaErrados; i++)
          {
            if (defesaErrados[i].x == coordenadaDefesa.x && defesaErrados[i].y == coordenadaDefesa.y)
            {
              coordenadaJaUtilizada = true;
              break;
            }
          }
        }
      } while (coordenadaJaUtilizada); // Repete até encontrar uma coordenada não utilizada

      bool defesaAcertou = false; // Inicializa como falso

      // Verifica se acertou o torpedeiro
      if ((coordenadaDefesa.x == x1T && coordenadaDefesa.y == y1T) || (coordenadaDefesa.x == x2T && coordenadaDefesa.y == y2T))
      {
        defesaAcertou = true;
      }

      // Verifica se acertou o submarino
      if (!defesaAcertou)
      { // Só verifica se ainda não acertou o torpedeiro
        if ((coordenadaDefesa.x == x1S && coordenadaDefesa.y == y1S) ||
            (coordenadaDefesa.x == x2S && coordenadaDefesa.y == y2S) ||
            (coordenadaDefesa.x == x3S && coordenadaDefesa.y == y3S))
        {
          defesaAcertou = true;
        }
      }

      // Antes de verificar o acerto, imprime a coordenada escolhida pela defesa
      Serial.print("Coordenada da defesa: (");
      Serial.print(coordenadaDefesa.x);
      Serial.print(", ");
      Serial.print(coordenadaDefesa.y);
      Serial.println(")");

      // Após verificar, registra o resultado conforme acerto ou erro
      if (defesaAcertou)
      {
        defesaCertos[totaldefesaCertos++] = coordenadaDefesa; // Armazena a defesa correta
        Serial.println("Defesa acertou!");
      }
      else
      {
        defesaErrados[totaldefesaErrados++] = coordenadaDefesa; // Armazena a defesa errada
        Serial.println("Defesa errou.");
      }

      // Verifica se o jogo terminou, se acertar 5 vezes acertou todos os pontos
      if (totaldefesaCertos == 5)
      {
        Serial.println("Jogo encerrado, Arduino ganhou");
        digitalWrite(ledPin1, HIGH);
        digitalWrite(ledPin2, HIGH);
        digitalWrite(ledPin3, HIGH);
        State = JogoFinalizado; // transicao para o estado final
      }

      // caso nao tenha passado entao voltar para o jogador atacar
      else
      {
        StateJogar = Ataque; // Passa a vez para o jogador
        StateAtaque = x;
        mensagemXAMostrada = false;
        mensagemYAMostrada = false;
      }
    }
    break;

    default:
      break;
    }
  }
  break;

  // ------------------------------------------------------------------------------------------------------- //
  // ultimo estado, que eh quando o jogo finaliza
  case JogoFinalizado:
  {
    // verificaco para ver se veio do botao3, caso o usuario tenha encerrado por vontade propria
    if (botao3)
    {
      // logica para fazer piscar os leds por 3 segundos
      if ((millis() - previousMillis) < 3000)
      {
        if ((millis() - previousMillisTimer) >= 200)
        {
          previousMillisTimer = millis();

          if (digitalRead(ledPin1) == HIGH)
          {
            digitalWrite(ledPin1, LOW);
            digitalWrite(ledPin2, LOW);
            digitalWrite(ledPin3, LOW);
          }

          else
          {
            digitalWrite(ledPin1, HIGH);
            digitalWrite(ledPin2, HIGH);
            digitalWrite(ledPin3, HIGH);
          }
        }
      }

      // quando passar 3 segundos entao apenas desligar os leds
      else
      {
        digitalWrite(ledPin1, LOW);
        digitalWrite(ledPin2, LOW);
        digitalWrite(ledPin3, LOW);
      }
    }

    // caso nao venha do botao3, mas sim caso alguem tenha ganhado, entao apenas desligar os leds
    else
    {
      digitalWrite(ledPin1, LOW);
      digitalWrite(ledPin2, LOW);
      digitalWrite(ledPin3, LOW);
    }
  }
  break;

  default:
    break;
  }
}

// ------------------------------------------------------------------------------------------------------- //
// funcao do debounce para o primeiro botao
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

// ------------------------------------------------------------------------------------------------------- //
// funcao do debounce para o segundo botao
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

// ------------------------------------------------------------------------------------------------------- //
// funcao do debounce para o terceiro botao
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
