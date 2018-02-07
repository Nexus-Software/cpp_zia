#include "Socket.hh"

nzm::Socket::Socket():
	_isInit(false)
{

}

int nzm::Socket::getFd() const
{
  return this->_fd;
}

int nzm::Socket::initServer(short port)
{
  struct sockaddr_in 	sin;
  int 			i;

  if (this->_isInit)
    throw ModuleNetworkException("Socket already init");

  if ((this->_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    throw ModuleNetworkException("Socket init error");

  this->_isInit = true;

  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  sin.sin_addr.s_addr = INADDR_ANY;
  i = 1;
  if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEPORT, &i, sizeof(int)) < 0)
    throw ModuleNetworkException("Socket fail set options");
  if (bind(this->_fd, (struct sockaddr*)&sin, sizeof(sin)) < 0 || listen(this->_fd, 100) < 0)
    throw ModuleNetworkException("Fail bind and listen");
  return this->_fd;
}

int nzm::Socket::initClient(int fdServer)
{
  struct sockaddr_in 	client_sin;
  socklen_t 		client_sin_len;

  if (this->_isInit)
    throw ModuleNetworkException("Socket already init");

  client_sin_len = sizeof(client_sin);
  if ((this->_fd = accept(fdServer, (struct sockaddr *)&client_sin,
		   &client_sin_len)) < 0)
    throw ModuleNetworkException("Fail accept");

  this->_isInit = true;

  return this->_fd;
}