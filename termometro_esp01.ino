#include <ESP8266WiFi.h> //Biblioteca responsavel pela conexao do Esp com a rede 
#include <Wire.h> //Biblioteca responsavel pela comunicacao I2C

//Funcoes===============================================================
void iniWiFi();
void verWiFi();
void verireq();
void tmrsensor();
void calctmp01();
void calctmp02();
void calctmp03();
void trterro();
void reqi2c();
void zrvar();

//Variaveis=============================================================
const char* ssid = "Esp";//Nome da rede
const char* password = "senha123"; //Senha da rede
const int16_t I2C_SLAVE = 0x48; //Endereco do TMP102 para a comunicacao I2C
boolean auxver = false; //Variavel auxiliar da funcao verWiFi
int msb; //Byte mais significativo da comunicacao I2C e auxiliar das funcoes para o calculo da temperatura
int lsb; //Byte menos significativo da comunicacao I2C e auxiliar das funcoes para o calculo da temperatura
int bimsb[12]; //Vetor com bits mais signifiativos
int bilsb[4]; //Vetor com bits menos signifiativos
int pot = 0; // Varialvel utilizada no calculo da potencia
float temp = 0; //Variavel da temperatura
int i; //Variavel auxiliar das funcoes para o calculo da temperatura
int k = 4; //Variavel gerenciador das funcoes

WiFiServer server(80); //Inicializacao do Esp em modo servidor

//=====================================================================
void setup() {
Serial.begin(115200); //iniciando a comunicacao serial
Wire.begin(0,2);//iniciando a comunicacao I2C utilizando o GPIO 0 como SDA e o 2 como SCL
delay(10);

//Calcula o valor de temperatura ao iniciar
reqi2c();
calctmp01();
calctmp02();
calctmp03();

iniWiFi(); //funcao para iniciar a conexao wifi
}

//=====================================================================
void loop() {
if (WiFi.status() != WL_CONNECTED || auxver == false) { //Verifica se o Esp esta conectado ou o valor da variavel auxver
verWiFi(); //Chama a funcao
}
verireq();//Chama a funcao

switch (k) {//Verifica o valor da variavel k e dependendo deste valor ele chama a funcao correspondente
case 0: calctmp01(); break;
case 1: calctmp02(); break;
case 2: calctmp03(); break;
default: reqi2c(); break;
}
//envia o valor da temperatura para o monitor serial.
Serial.print("Temperatura:");
Serial.println(temp, 3);
delay(1000);
}

//[Inicia a conexao com a rede]=========================================
void iniWiFi() {
Serial.println();
Serial.println();
Serial.print("Conectando a rede ");
Serial.println(ssid);
WiFi.begin(ssid, password);//Conecta na rede

while (WiFi.status() != WL_CONNECTED) { //Verifica se a conexao foi realizada
delay(500);
Serial.print(".");
}
server.begin(); //inicializa do servidor
}

//[Verifica a conexao com a rede]=======================================
void verWiFi() {
if (WiFi.status() == WL_CONNECTED) { //Verifica se ha conexao
Serial.println("");
Serial.println("O seu ESP esta conectado!");
Serial.print("Endereco IP: ");
Serial.println(WiFi.localIP());
server.begin();//inicializa do servidor
Serial.println("Servidor iniciado!");
auxver = true; //Altera o valor da variavel para que a mensagem nao repita contantemente
}
else {
Serial.println("");
Serial.println("O seu ESP nao se consegue se conectar...");
auxver = false; //Altera o valor da variavel para repetir a mensagem da conexao realizada
}
}

//[Gerencia as requisicoes]=============================================
void verireq() {
WiFiClient client = server.available(); //Verifica se ha solicitacao
if (!client) { // Se nao houver ele retorna para a funcao loop
return;
}
Serial.println("Novo acesso!");
client.setTimeout(1000); //Tempo para realizar a conexao

while (client.available()) { //Enquanto houver comunicacao com o cliente
client.read();//funcao responsavel por enviar os dados de temperatura
}
client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"); //Configura texto em HTML
client.print(temp); //Variavel de temperatura

}

//[Calculo da temperatura - parte 1 ]===================================
void calctmp01() {

// Converte o byte mais significativos de decimal para binario e os aloca em oito celulas do vetor bimsb
for (i = 4; i < 12; i++) {
bimsb[i] = msb % 2;
msb = msb / 2;

}
// Converte parte do byte menos significativos de decimal para binario e os aloca nas quatro celulas do vetor bilsb
for (i = 0; i < 4; i++) {
bilsb[i] = lsb % 2;
lsb = lsb / 2;
}
k++; //Altera o valor da variavel k para passar para o proximo passo
}

//[Calculo da temperatura - parte 2 ]===================================
void calctmp02() {
// Converte o resto do byte menos significativos de decimal para binario e os aloca nas quatro celulas restantes do vetor bimPsb
for (i = 0; i < 4 ; i++) {
bimsb[i] = lsb % 2;
lsb = lsb / 2;
}
// Converte os doze bits mais significativos de binario para  decimal
msb = 0;
pot = 1;
for (i = 0; i < 12; i++) {
msb = msb + (pot * bimsb[i]);
pot = pot * 2;

}
// Converte os quatro bits restantes de binario para  decimal
lsb = 0;
pot = 1;
for (i = 0; i < 4; i++) {
lsb = lsb + (pot * bilsb[i]);
pot = pot * 2;

}
k++;//Altera o valor da variavel k para passar para o proximo passo
}

//[Calculo da temperatura - parte 3 ]===================================
void calctmp03() {
//Verifica valor de lsb, para realizar a equacao correta.
if (lsb == 0) {
temp = (msb * 0.0625);
}
else {
temp = (msb * (0.0625 / lsb));
}
k++;//Altera o valor da variavel k para passar para o proximo passo
zrvar(); //funcao para zerar as variaveis
}

//[Requisita e recebe os dados do sensor ]==============================
void reqi2c()
{ Wire.requestFrom(I2C_SLAVE, 2);    //requisita 2 Bytes para o TMP102
while (Wire.available()) { //Enquanto ouver comunicacao
msb = Wire.read(); // Recebe o valor do byte mais significativo
lsb = Wire.read(); // Recebe o valor do byte menos significativo

k = 0; //Altera o valor da variavel k para passar para o proximo passo


}
}

//[Zera os valores da variaveis ]=======================================
void zrvar() {
msb = 0;
lsb = 0;
bimsb[12] = 0;
bilsb[4] = 0;
pot = 0;
i = 0;
}
