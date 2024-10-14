#include <QCoreApplication>
#include <QHostAddress>
#include <network/udpclient.h>

const int16_t TEST_PORT = 1337;
int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);
  switch (a.arguments().size()) {
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
  UdpClient fr(a.arguments().at(a.arguments().size() - 2)),
      sc(a.arguments().at(a.arguments().size() - 1), TEST_PORT);
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
