#include "Wire.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#include <SPI.h>
#include <SD.h>

const int CS = 4;

int tensao_vc = A1;
int corrente_vc = A2;
int sal_acucar = A3;

float VC_tensao, VC_corrente;

#define n 10 //pontos da media deslocada
int vetor[n]; //vetor para media deslocada

float media(int);
float media_deslocada(int);
void detectar(int);

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  lcd.begin (16, 2);

  Serial.println("Inicializando Micro-cartao-SD...");
  if (!SD.begin(CS))
  {
    Serial.println("Error no cartao.");
  }
  Serial.println("Micro-cartao-SD iniciado corretamente");

  pinMode(tensao_vc, INPUT);
  pinMode(corrente_vc, INPUT);
  pinMode(sal_acucar, INPUT);

  lcd.setCursor(0, 0);
  lcd.print("LAIS - Detector");
  lcd.display();
}

void loop()
{
  detectar(sal_acucar);

  VC_tensao = media(tensao_vc);
  VC_corrente = media(corrente_vc);

  File arquivo = SD.open("dados.csv", FILE_WRITE);

  if (arquivo)
  {
    arquivo.print("Tensão voltametria ciclica: ");
    arquivo.println(VC_tensao);

    arquivo.print("Corrente voltametria ciclica: ");
    arquivo.println(VC_corrente);
  }

  else
  {
    Serial.println("Erro ao abrir o arquivo dados.csv");
  }
  delay(500); // Tempo de gravação

}

float media(int x)
{
  float soma, valor;
  soma = 0.0;

  for (int i = 0; i < 10; i++)
  {
    valor = analogRead(x);
    soma += valor;
  }

  return valor/10;
}

float media_deslocada(int x)
{
  for (int i = n - 1; i > 0; i--)
  {
    vetor[i] = vetor[i - 1];
  }
  vetor[0] = analogRead(x);

  float soma = 0;

  for (int i = 0; i < n; i++)
  {
    soma += vetor[i];
  }

  return soma / n;
}

void detectar(int x)
{
  float detector = media_deslocada(x);

  //valor depende do material do eletrodo
  if (detector >= 690)
  {
    Serial.println("Detector - ÁGUA COM SAL");
    lcd.setCursor(0, 1);
    lcd.print("AGUA C/ SAL");
  }

  if (detector >= 530 && detector < 690)
  {
    Serial.println("Detector - ÁGUA");
    lcd.setCursor(0, 1);
    lcd.print("AGUA");
  }

  if (detector >= 300 && detector < 530) {
    Serial.println("Detector - AGUA COM AÇÚCAR");
    lcd.setCursor(0, 1);
    lcd.print("AGUA C/ ACUCAR");
  }
}
