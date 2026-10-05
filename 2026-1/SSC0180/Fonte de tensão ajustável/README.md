## Fonte de Tensão Ajustável entre 3V a 12V com capacidade de 100mA

---

#### Link para o projeto sem o LM317 no Falstaad

https://is.gd/WkK3F9

### Apresentação

Grupo 2: Pedro Sinzato - 17919490, Mauricio Pinheiro Lacerda 17915183, Eric Azuma -  17887791

Projeto desenvolvido para a discíplina SSC0180 - Eletrônica para Computação, ministrada pelo professor Eduardo do Valle Simões, para o curso de Bacharelado em Ciências da Computação ICMC São Carlos, no primeiro semestre de 2026.
O trabalho consiste na construção de uma fonte de tensão ajustável entre 3V a 12V com capacidade de 100mA, para tal, segue o circuito minímo abaixo.

Gitlab do professor: https://gitlab.com/simoesusp

É importante resaltar que:
1. O projeto mínimo tem como base um circuito feito em um software de CAD, assim, não considera diversos fatores do mundo real.
2. Devido a tolerância dos resistores, existe uma ocilação de ±0.5V no circuito construido. Vale ressaltar que caso você decida replicar o projeto, deve-se atentar que o resultado pode ser diferente do desenvolvido por nós pelas questões apresentadas acima, portanto, ao montar o circuito, espere que seja necessário a mudança nos valores dos resistores utilizados.Além disso, considere o porjeto mínimo apenas como uma referência e não como algo que deve ser obrigatóriamente seguido, como irão ver, o nosso próprio projeto difere do projeto mínimo.

Nas fotos do projeto desenvolvido, optamos por um potênciometro multivoltas de 1K que, apesar de mais caro, oferece maior precisão, além disso, utilizados um capacitor de 1000uF por já estar disponível. Por fim, tivemos que ajustar os resistor de 120Ω para 150Ω e o de 200Ω para 220Ω (Todos os resistores utilizados são de 1/2W). Esses itens possuim um * depois do nome na tabela a seguir.

Os cálculos dos resistores foram feitos com base na fórmula presente da documentação do LM317 (disponível no git) e o do capacitor com base nas simulações do pSpice.

Link ao vídeo do projeto funcionando: https://youtu.be/kuH94GLydRo

### Projeto Mínimo
| Compontes     | Quantidade    | Preço |Link                                                                                           |
| ------------- |:-------------:| -----:|----------------------------------------------------------------------------------------------:|
| KBL01         | 1X            | 1.5R$ | N/A                          
| Cap. 470uF 50V| 1X            | 1.5R$ | https://www.baudaeletronica.com.br/produto/capacitor-eletrolitico-470uf-50v-105c.html         |
| LM317T        | 1x            | 3R$   | https://www.baudaeletronica.com.br/produto/regulador-de-tensao-ajustavel-lm317t-to-220.html|  |
| Pot. 1k        | 1X            | 2.25R$| https://www.saravati.com.br/trimpot-multivoltas-25-voltas-3296w-vertical-1k-ohm.html          |
| R. 120Ω       | 1x            |0.12R$ | N/A
| R. 200Ω       |               |0.12R$ | N/A
| Pot. 1k 10 R.* | 1x            |27.6R$ | https://www.mercadolivre.com.br/potenciometro-multi-voltas-1k-ohms/p/MLB36791303|
|Cap. 1000uF 50V*|1x |1.92R$|https://www.baudaeletronica.com.br/produto/capacitor-eletrolitico-1000uf-50v-105c.html|