# Atester - POSIX Lab

Este é o repositório de um programa que executa testes para diferentes códigos na linguagem C. Em suma, arquivo exemplo `atester.c` contém os testes que são realizados pelo programa, que seguem:

 - Cause erro e termine com falha de segmentação.
 - Cause erro e termine com divisão por zero.
 - Fique em loop infinito.
 - Faça muito trabalho, mas eventualmente acabe.
 - Tenha asserts que falham e passem no mesmo teste.
 - Tenha testes que façam muitos prints.
 - Tenha testes que sejam rápidos e testes que sejam lentos.

Para a realização dos testes foram utilizados alguns recursos e especificidades que podem ser relatadas a seguir:

 - Execução dos testes de maneira isolada.
 - Execução dos testes em paralelo.
 - Execução de um teste específico.
 - Tempo limite de execução para os testes.
 - Relatório de erros dos testes que falharem.
 - Organizar saída padrão e de erros para cada teste.
 - Contagem de tempo.

Para cada um dos recursos foi implementado uma solução. No caso da execução dos testes de maneira isolada e para execução dos testes em paralelo foram criados processos para cada um dos testes como podemos ver abaixo:

    for (int i = 0; i < size; i++) {
		pid_t child = fork();
	}

Salvando o atributo pid de cada um dos processos em uma `struct` pudemos realizar a checagem da saída de cada um dos testes caso tivessem a saida com erros ou daqueles que falharam/passaram como a seguir:

	pid_t child = wait(&st);
	.
	.
	.
	if (child == dictionary[i].id)

Dessa forma conseguimos resolver qual processo estava rodando cada teste e fazer o relatório especificado pelo recurso relatório de erros dos testes que falharem. Além disso guardamos na `struct` o nome do teste, o tempo de seu ínicio e o arquivo que será utilizado para sua saída:

	typedef  struct {
		int id;
		char  name[50];
		double start;
		int out;
	} process;

A execução de testes de maneira específica (ou seja a realização de somente um teste) é feita passando para o arquivo compilado o nome de alguns dos testes existentes como a seguir:

	./atester test1

Isso vai ativar a parte do código específica para rodar um único teste que funciona basicamente da seguinte forma:

 - Declara o número de testes a serem executados.
 - Inicializa uma variável para contar o número de testes que passarem.
 - Inicializa o tempo de contagem do teste.
 - Cria um arquivo para escrita da saída do teste.
 - Faz o processo no qual o teste vai ser executado.
 - Aciona um alarme de 2 segundos para o filho, se no caso ultrapassar esse tempo será interrompido de acordo com o recurso tempo limite de execução para os testes.
 - Faz um `dup2` redirecionando a saída do processo para o arquivo criado.
 - Caso o teste passar, o filho retornará 0, se o teste falhar o processo retornará 1 e no terceiro caso de erro retornará o erro.
 - A saída do processo será recolhida pelo `wait` que aguarda a finalização do processo filho.
 - Com a finalização do processo podemos verificar se a saída foi um erro, uma falha ou que o teste teve sucesso utilizando  `WIFEXITED` , `WEXITSTATUS`, `WIFSIGNALED` e `WIFSIGNALED` que interpreta a saída do processo.
 - Todos os dados do processo vão para um arquivo separado utilizando o `dup2`.
 - Os dados organizados são lidos e colocados no terminal do programa pai.

Esse processo é muito parecido com mais de um teste rodando simultaneamente. A diferença é que é utilizado uma espécie de dicionário para guardar os dados de cada um dos processos e facilitar a leitura de suas saídas. Para garantir a realização dos testes de maneira isolada e paralela continua-se utilizando o conceito de processos filhos que faram toda a parte do teste. Para cada um dos processos é criado um arquivo que guarda toda a as saídas dos processos. Posteriormente essa saída é lida pelo processo pai e impressa no terminal. Os arquivos são deletados também verificando a sua utilização temporária.

Para o relatório dos erros foi realizado um estudo do erro. Caso o erro seja especificamente do tempo de 2 segunos, a impressão do erro no terminal fica da forma seguir:

	test3: [ERRO] [TIME]

De outra forma é impresso o erro e em quanto tempo o teste foi executado.
Dessa forma fica aberto a novas versões, atualizações e recomendações.

Agradecimento aos professores Igor Montagner e António Júnior por coordearem o projeto.