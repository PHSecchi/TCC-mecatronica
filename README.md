#TCC Mecatrônica
Este projeto é o Trabalho de Conclusão de Curso (TCC) do curso de Tecnologia em Mecatrônica Industrial, do Instituto Federal de São Paulo, campus Catanduva.

O projeto consistiu na criação de um protótipo de um termômetro infantil, juntamente com um aplicativo para verificação da temperatura e orientações para os pais.

#Eletrônica
A eletrônica do projeto é composta por um ESP8266 modelo 01 e um sensor de temperatura TMP102. Ambos os componentes foram selecionados considerando a possibilidade de uma versão comercial futura, apesar de esse não ser o objetivo principal do projeto. Essa foi uma característica fundamental na escolha dos dispositivos.

O software *termometro_esp01*, presente no microcontrolador, desempenha duas principais funções:

Servidor web para que o aplicativo possa se conectar e verificar o valor atual da temperatura;
Aferição da temperatura via conexão I2C com o sensor TMP102.
Ao analisar o código, é possível observar que a aferição da temperatura ocorre em várias etapas, para garantir que o ESP8266 não falhe como servidor. Além disso, o TMP102 envia dois bytes de dados, sendo necessária a conversão dos valores de binário para decimal.

#Aplicativo
O aplicativo foi desenvolvido na plataforma App Inventor, criada pelo MIT. Trata-se de uma ferramenta simples e intuitiva, projetada para desenvolvimento rápido.

A aplicação conta com uma tela inicial que exibe o valor da temperatura, e quatro outras telas com os Critérios de Alerta na Criança com Febre (CAF), segundo o artigo de Manuel Salgado. O objetivo do aplicativo é verificar periodicamente o valor apresentado pelo ESP8266 via rede sem fio e atualizá-lo automaticamente na tela.

Para mais detalhes ou dúvidas, sugiro consultar o arquivo *monografia* disponível neste repositório.