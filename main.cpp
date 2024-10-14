#include "base/mutatorloader.h"
#include <QCoreApplication>
#include <QFileInfo>
#include <QHostAddress>
#include <network/udpclient.h>

const int16_t TEST_PORT = 1337;
int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);
  int pluginArgEntry = a.arguments().indexOf("--plugin");
  MutatorLoader loader;
  QStringList args = a.arguments();
  while (pluginArgEntry >= 0) {
    QString path = args.value(pluginArgEntry + 1);
    if (!QFileInfo::exists(path)) {
      qWarning() << "Plugin not found at " << pluginArgEntry + 1 << "argument"
                 << path;
      return -1;
    }
    if (!loader.load(path.toLatin1())) {
      qWarning() << "Cannot load " << path;
    }
    Mutator *currentMutator = loader.createFrom(loader.size() - 1);
    qWarning() << "Loaded" << currentMutator->id().c_str();
    delete currentMutator;
    args.removeAt(pluginArgEntry);
    args.removeAt(pluginArgEntry);
    pluginArgEntry = a.arguments().indexOf("--plugin", pluginArgEntry + 1);
  }
  qWarning() << "Plugin count" << loader.size();

  switch (args.size()) {
  case 1:
    qWarning() << "Provide ip   for all clients";
    break;
  case 2:
    qWarning() << "Provide ip  for second client";
    break;
  case 3:
    break;
  default:
    qWarning() << QStringLiteral("Too many clients, picking last 2 : %1 and %2")
                      .arg(a.arguments().at(a.arguments().size() - 2))
                      .arg(a.arguments().at(a.arguments().size() - 1));
    ;
    break;
  }
  UdpClient fr(args.at(args.size() - 2)),
      sc(args.at(args.size() - 1), TEST_PORT);
  if (loader.size() > 0)
    fr.setSendMutator(loader.createFrom(loader.size() - 1));
  if (!fr.isHostValid()) {
    qWarning() << "Provide valid ip  for first client. Address : "
               << fr.getHost().toString();
  } else if (!fr.isHostValid()) {
    if (!sc.isHostValid()) {
      qWarning() << "Provide valid ip  for second client. Address : "
                 << sc.getHost().toString();
    }
  }
  Q_ASSERT(fr.setTarget(sc.getHost(), TEST_PORT));
  QString msg_body = QStringLiteral("12345678");
  QByteArray test(msg_body.toLatin1());
  fr.appendMessage(test.data(), test.size());
  QByteArray testSmall(msg_body.toLatin1().first(2));
  fr.appendMessage(testSmall.data(), testSmall.size());
  Q_ASSERT(fr.sendAll() > 0);
  return a.exec();
}
