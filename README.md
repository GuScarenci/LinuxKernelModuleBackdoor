# LinuxKernelModuleBackdoor

## Instruções 
O trabalho consiste na implementação de um módulo de kernel para Linux que atue como um agente backdoor para coletar informações de dentro do kernel. Essas informações devem ser enviadas via Socket para uma outra máquina remota a fim de armazenar e visualizar o conteúdo coletado. A quantidade e a forma de informação coletada é livre, no entanto, deve-se coletar no mínimo informações de entrada do usuário via teclado e snapshots da memória de vídeo. 

## Introdução 
O projeto consiste em um módulo de Kernel cuja funcionalidade envolve coletar informações da máquina monitorada e transmissão destas para um receptor via socket. As informações coletadas envolvem entradas de teclado, novos dispositivos USB conectados à máquina e prints da tela do sistema. Estas são enviadas via socket para uma outra máquina monitoradora remota.

## Lógica do Código 
### Informações Gerais 
O código foi implementado com o kernel do sistema operacional Ubuntu 22.04.2 LTS em mente, sendo testado tanto na versão x86_64 quanto aarch64 e em VMs. A utilização de qualquer outra versão de sistema operacional não tem seu funcionamento garantido e pode causar imprevisibilidades. Cada funcionalidade, isto é, cada coleta de dados da máquina, além da sua transmissão, foi implementada em arquivos separados. 

### Organização de Arquivos
Os arquivos `deviceLogger.c`,  `keyboardLogger` e `??` e seus respectivos arquivos `.h` lidam com a coleta de dispositivos conectados, entradas de teclado e prints do sistema respectivamente. O arquivo `??` possui todas as principais operações de envio de mensagem via socket. Já o arquivo `backdoor.c` representa a parte principal do código, as operações de inicialização e finalização do módulo. Para construir o módulo, utiliza-se um `Makefile` na pasta raíz e um `Kbuild` na pasta `src`. Enquanto aquele realiza as operações principais de compilação, limpeza, inserção e remoção do módulo, este cuida de juntar os arquivos "objetos".

## Resultados 


## Orientações para Execução do Código
Primeiramente, na máquina a ser monitorada, certifica-se de que esteja rodando a versão 22.04.2 LTS de Ubuntu, e que o interpretador de python 3 e o compilador gcc estejam instalados.

Caso `neofetch` não esteja instalado, instala-o pelo comando:
`sudo apt install neofetch`

Para verificar a versão do sistema operacional, utilize o comando:
`neofetch`

Para verificar se gcc está instalado:
`gcc --version`

Para instalar gcc caso não esteja:
`sudo apt-get install gcc`

Para verificar se python3 está instalado:
`python3 --version`

Para instalar python3 caso não esteja:
`sudo apt install python3`

Depois de certificar-se de que todas essas ferramentas estão instaladas, vá para o diretório raíz do arquivo na linha de comando. Dali, compile o módulo com o comando:
`make all`

Caso deseja-se recompilar o projeto utiliza o comando a seguir para certificar-se de que a compilação anterior seja removida:
`make clean`

Agora, na máquina monitoradora, certifica-se também de que possui python3 instalado. No diretório raíz, executa-se o comando:
`python3 server.py`

Em seguida, anexa o módulo ao Kernel da máquina monitorada digitando:
`make insert`

Após a execução, quando quiser remover o módulo da máquina monitorada isto realiza-se pelo comando:
`make remove`

### feito por Artur Weber, Gabriel Libardi e Gustavo Scarenci 