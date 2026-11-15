Membros:
Gustavo Ribeiro Britto Rezende - 17894830
Eric Azuma - 17887791
Pedro Sinzato - 17919490

Relatorio_DESCRITIVO.pdf

Explicação da implementação do projeto e de como executar o programa.

Relatorio_EXPERIMENTAL

Análise assindótica, tempos e exemplos de execução.

TRABALHO/

Implementação do trabalho. Em readers/ estão os arquivos dos readers, em datasets/ estão os datasets.

Obs: O dataset escolhido pelo grupo (combustivel.csv) estava com um problema que, em alguns casos, ao invés de "," foi utilizado ";" para separar os campos. O dataset foi reformatado para resolver esse problema. Portanto, o dataset disponível no link na planilha não é o mesmo do contido na pasta datasets/

exemplos/

Exemplos de execução completos. Devido ao tamanho dos arquivos contendo os resultados da pesquisa, foi mostrada apenas parte deles no Relatorio_EXPERIMENTAL.

Considerações finais:

Makefile utiliza o compilador CLANG, caso se utilize outro compilador, deve-se altera o campo: CC = clang. Além disso, deve-se utilizar -lm com o GCC pois o projeto utiliza-se da biblioteca math.h.

Foi utilizado na main.c a função goto com o intuito de reduzir o tamanho do código. O professor Rudinei Goularte proibiu o uso dessa função da dísciplina de Introdução a Computação I. Como ele não está ministrando as aulas dessa dísciplina, não sei se a proibição continua.
