#ifndef LOGINFAILREASON_H
#define LOGINFAILREASON_H

namespace Czateria {

enum class LoginFailReason {
  Unknown,
  BadCaptcha,
  BadPassword,
  NickRegistered,
  NoSuchUser,
  NaughtyNick
};
}

#endif // LOGINFAILREASON_H
