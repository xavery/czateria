#ifndef FILEBASEDLOGGER_H
#define FILEBASEDLOGGER_H

#include <czatlib/chatsessionlistener.h>

#include <memory>

#include <QFile>
#include <QHash>
#include <unordered_map>

// Qt containers do not accept move-only objects by design (QTBUG-59188), which
// is why we need to use std containers here.
// however support for std::hash for QStrings was added in 5.15 (QTBUG-33428),
// so a version check is needed to avoid a duplicate definition.
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
namespace std {
template <> struct hash<QString> {
  std::size_t operator()(const QString &s) const noexcept { return qHash(s); }
};
} // namespace std
#endif

struct AppSettings;
class QFileInfo;

class FileBasedLogger : public Czateria::ChatSessionListener {
public:
  FileBasedLogger(const AppSettings &settings);

  void onRoomMessage(const Czateria::ChatSession *session,
                     const Czateria::Message &message) override;
  void onPrivateMessageReceived(const Czateria::ChatSession *session,
                                const Czateria::Message &message) override;
  void onPrivateMessageSent(const Czateria::ChatSession *session,
                            const Czateria::Message &message) override;
  void onUserJoined(const Czateria::ChatSession *session,
                    const QString &nickname) override;
  void onUserLeft(const Czateria::ChatSession *session,
                  const QString &nickname) override;

private:
  QFile *getOpenFile(const QFileInfo &fileInfo);
  template <typename F>
  void writeLogEntry(const QFileInfo &fileInfo, const QDateTime &timestamp,
                     F &&outputGenFn);

  const AppSettings &mSettings;
  std::unordered_map<QString, std::unique_ptr<QFile>> mOpenFiles;
};

#endif // FILEBASEDLOGGER_H
