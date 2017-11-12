#include <QApplication>
#include <QOpenGLContext>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <gui/Object.hpp>

int
main(int argc, char* argv[])
{
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication app(argc, argv);

  QSurfaceFormat format;
  if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);
  }
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  format.setSamples(4);
  QSurfaceFormat::setDefaultFormat(format);

  qmlRegisterType<gui::Object>("lib", 1, 0, "Object");

  QQmlApplicationEngine engine;
  engine.load(QUrl(QStringLiteral("qrc:///qml/main.qml")));

  return app.exec();
}
