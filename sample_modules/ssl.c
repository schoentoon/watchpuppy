#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void init()
{
  SSL_load_error_strings();
  SSL_library_init();
  ERR_load_BIO_strings();
  OpenSSL_add_all_algorithms();
};

#define PORT 4433

int check()
{
  int output = 0;
  SSL_CTX* ctx = SSL_CTX_new(SSLv23_client_method());
  SSL* ssl = NULL;
  struct sockaddr_in addr;
  int fd = socket(PF_INET, SOCK_STREAM, 0);
  bzero(&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
    output = -1;
    goto exit;
  }
  ssl = SSL_new(ctx);
  SSL_set_fd(ssl, fd);
  if (SSL_connect(ssl) == -1)
    output = -1;
exit:
  close(fd);
  SSL_CTX_free(ctx);
  if (ssl)
    SSL_free(ssl);
  return output;
};