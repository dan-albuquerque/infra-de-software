#include <stdio.h>
/*
  executavel tem as instacias dos recursos
  exe: ./banker 10 3 5
  tenho 10 instancias do recurso 1, 3 do recurso 2 e 5 do recurso 3
*/
/*
  costumer tem o numero maximo de instancias 
  ex: 7,5,3
  no cliente 0 temos ate no maximo 7 instancias do recurso 1, 5 do recurso 2 e 3 do recurso 3
*/
/*
  commands tem os pedidos dos clientes
  ex: RQ 0 0 2 1
  primeiro 0 eh o cliente - cliente 0 nesse caso
  o cliente 0 quer 0 do recurso 1, quer 2 do recurso 2, quer 1 do recurso 3
  RQ eh pedidno pra alocar recurso, RL eh release ta liberando recursos
  * imprime informações do cenario atual -> imprime a matriz
*/
/*
  result diz linha a linha do commands e as mensagens respectivas a cada comando, se pode alocar, se pode liberar, e se pode ter deadlock ou nao
*/

int main(int argc ,char *argv[])
{
  for(int i=1; i<argc; i++)
  {
    printf("%s\n", argv[i]);
  }
  return 0;
}
