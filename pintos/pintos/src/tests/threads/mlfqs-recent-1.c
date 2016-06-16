/* Checa se foram cumpridas todas as especificações para o escalonador avançado passado na especificação do projeto
(os testes para o escalonador de prioridades podem ser os originais, exceto os que usam doação de prioridade.)
O teste primeiramente checa se o argumento de prioridade passado em thread_create é ignorado, checando se a mesma é iniciada com prioridade máxima.
 Em seguida, tenta modificar a prioridade da thread, e a modificação não deve ocorrer.
Depois, testa se após um TIME_SLICE (quantum), a thread desce de prioridade.
Se descer, a thread principal deve voltar a executar, sendo a mesma posta pra dormir até o fim da thread 2.
*/   

#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "devices/timer.h"

void Priority_test (void);

struct semaphore sema;

void
test_mlfqs_recent_1 (void) 
{
  ASSERT (thread_mlfqs);
  sema_init(&sema, 0);
  thread_create ("T0", 0, Priority_test, NULL);
  msg("Retornando para thread principal");
  msg("Dorme thread principal até que a thread 2 libere o semáforo");
  sema_down(&sema);
  msg("got it");


}

void
Priority_test (void) {
  int64_t start = timer_ticks();
  ASSERT(thread_current()->priority != 0);
  msg("Criacao de thread: OK");
  msg("Tentando modificar prioridade");
  thread_set_priority(0);
  ASSERT(thread_current()->priority != 0);
  msg("Modificacao ignorada");
  msg("Esperando ate descer na fila de prioridade");
  int64_t i = timer_ticks();
  while (thread_current()->priority == PRI_MAX) {
    if (i-start > 4) {
      fail("fail");
    }
    i = timer_ticks();
  }
  ASSERT(thread_current()->priority != PRI_MAX);
  msg("Thread 2 desceu na fila de prioridade com sucesso");
  msg("thread 2 finished");
  sema_up(&sema);
  
}

