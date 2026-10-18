----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int socket(int domain, int type, int protocol); Crea un socket, es decir, el punto que utilizará un programa para comunicarse con otro a través de la red.

  domain   ---> AF_INET: para direcciones IPV4.
                AF_INET6: para direcciones IPv6.
                AF_UNIX / AF_LOCAL: Comunicación local entre procesos del mismo equipo. 
              
  type     ---> SOCK_STREAM: Conexión orientada a flujo, fiable y basada en TCP (garantiza entrega y orden).
                SOCK_DGRAM: Basada en datagramas, no orientada a conexión y basada en UDP (más rápida, sin garantía de entrega).
                SOCK_SEQPACKET: Paquete secuencial basado en datagramas.

  protocol ---> Un protocolo especifico, si se deja a 0 el SO elige el mas conveniente

Devuelve: Un número que identifica el socket dentro del programa, o -1 si falla.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint16_t htons(uint16_t hostshort); Convierte un número de 16 bits al formato de bytes que utiliza la red. En la práctica se usa para preparar el número de puerto.

  hostshort ---> El valor que se quiere convertir.

Devuelve: El mismo valor representado en el orden de bytes de la red. 
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint32_t htonl(uint32_t hostlong); Convierte un número de 32 bits al formato de bytes que utiliza la red. En la práctica se usa al preparar la dirección del servidor.

  hostlong ---> El valor que se quiere convertir. En la práctica aparece htonl(INADDR_ANY) al preparar la dirección del servidor; 
                INADDR_ANY permite recibir conexiones por sus direcciones IPv4 locales. 
                
Devuelve: El valor convertido al orden de bytes de la red. 
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Int inet_pton(int af, const char *restrict src, void *restrict dst); Transforma una dirección IP escrita como texto, por ejemplo 127.0.0.1, 
                                                                     en el formato binario que necesitan las funciones de comunicación.
 
  af  ---> AF_INET: para direcciones IPV4.
           AF_INET6: para direcciones IPv6.
          
  src ---> Contiene la IP, por ejemplo "127.0.0.1". 
  
  dst ---> Indica dónde guardar la dirección convertida

Devuelve: 1 si convierte la IP, 0 si el texto no es una IP válida y -1 si la familia no está soportada 
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen); Conecta el socket del cliente con el servidor para que puedan empezar a intercambiar datos. 

  sockfd  ---> El socket del cliente.

  *addr   ---> Contiene la IP y el puerto del servidor.

  addrlen ---> Indica los bytes que ocupa esa estructura; se obtiene con sizeof(serv_addr).

Devuelve: 0 si consigue conectar o -1 si hay un error.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen); Asocia el socket del servidor a una dirección IP local y a un puerto. 
                                                                      Así se establece por dónde recibirá las conexiones de los clientes.
  
  sockfd  ---> El socket que se quiere configurar.

  addr    ---> Contiene la IP local y el puerto donde estará el servicio

  addrlen ---> El tamaño de esa estructura, como sizeof(address).

Devuelve: 0 si consigue asociarlo o -1 si falla, por ejemplo porque el puerto está ocupado.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int listen(int sockfd, int backlog); Prepara el socket del servidor para recibir solicitudes de conexión de los clientes.

  sockfd  ---> Socket que recibirá las solicitudes.
  
  backlog ---> Indica el límite solicitado para las conexiones pendientes de aceptar. En listen(sock, 3), el 3 se refiere a esa cola, no al total de clientes que puede atender el servidor.

Devuelve: 0 si funciona o -1 si hay un error.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int accept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len); Acepta la conexión de un cliente y devuelve un nuevo socket para comunicarse con él. 
                                                                                            El socket original sigue disponible para aceptar otros clientes.
  socket      ---> Socket de escucha.
  
  address     ---> Donde se guarda la IP y el puerto del cliente.

  address_len ---> Indica el espacio disponible para esa dirección y después recoge el tamaño utilizado.

Devuelve: El nuevo socket conectado, que se guarda en new_socket, o -1 si falla. El socket original sigue disponible para aceptar otros clientes.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ssize_t send(int sockfd, const void *buf, size_t len, int flags); Envía datos al otro extremo de una conexión.

  sockfd ---> El socket conectado

  buf    ---> Contiene lo que se quiere enviar

  len    ---> Indica cuántos bytes enviar

  flags  --->  0: Comportamiento predeterminado (bloqueante, envío normal).
               MSG_OOB: Envía datos fuera de banda (out-of-band data) en sockets que lo soporten (como SOCK_STREAM).
               MSG_DONTROUTE: No utiliza una puerta de enlace (gateway); los datos se envían solo a hosts en redes directamente conectadas.
               MSG_DONTWAIT: Activa la operación no bloqueante para esta llamada específica; si el envío se bloqueara, devuelve EAGAIN o EWOULDBLOCK en lugar de esperar.
               MSG_EOR: Termina un registro (si el protocolo lo soporta, como en sockets SOCK_SEQPACKET).
               MSG_NOSIGNAL: Evita que se genere la señal SIGPIPE si intentas escribir en un socket orientado a conexión que ya ha sido cerrado por el peer (devolviendo el error EPIPE en su lugar).
               MSG_MORE: Indica que el llamador tiene más datos por enviar (permite acumular paquetes, similar a la opción TCP_CORK).

Devuelve: Cuántos bytes ha aceptado para enviar, o -1 si falla. Puede aceptar menos de los pedidos y habrá que enviar los restantes.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ssize_t recv(int sockfd, void *buf, size_t len, int flags); Recibe los datos que llegan por la conexión y los guarda en una zona de memoria del programa, llamada buffer.
                                                            Si todavía no hay datos, puede quedarse esperando.

  sockfd ---> El socket conectado

  buf    ---> Zona de memoria donde se guardarán los datos.

  len    ---> Indica cuántos bytes puede escribir como máximo.

  flags  --->  0: Comportamiento predeterminado (bloqueante, envío normal).
               MSG_PEEK: Lee los datos del búfer del socket sin eliminarlos de la cola; una llamada posterior a recv() volverá a leer los mismos datos.
               MSG_OOB: Procesa o solicita datos out-of-band (fuera de banda, es decir, de alta prioridad o urgentes enviados por el otro extremo).
               MSG_WAITALL: Hace que la función no regrese hasta que se haya llenado por completo el búfer solicitado o hasta que ocurra un error.
               MSG_DONTROUTE: Omite las tablas de enrutamiento estándar (menos común en recv, más asociado a send, aunque soportado en algunas pilas de red).

Devuelve: Los bytes recibidos; 0 si el otro extremo cerró su envío y ya no quedan datos; o -1 si falla. Puede recibir solo una parte de lo enviado y no añade automáticamente el final de cadena.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void* memset(void* dest, int ch, std::size_t count); Rellena una zona de memoria con un mismo valor.

  dest  ---> La zona de memoria a partir de la cual se vae rellenar.

  ch    ---> El byte que se repetirá.

  count ---> Cuántos bytes modificar.

Devuelve: La dirección de la memoria modificada. No borra los datos que todavía están pendientes de recibir por la conexión.
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pid_t fork(void); Crear un nuevo proceso (proceso hijo) duplicando el proceso actual (proceso padre).

Devuelve: <0: Hubo un error al crear el proceso.  
           0: Se retorna en el proceso hijo.
          >0: Se retorna en el proceso padre (devuelve el PID del nuevo proceso creado).