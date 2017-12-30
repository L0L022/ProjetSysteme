#include <lib/SequentialNormalCalculation.hpp>
#include <tests/TestNormalCalculation.hpp>

#include <QDebug>
#include <fstream>
#include <memory>
#include <string>

using namespace tests;
using namespace std::string_literals;

Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(lib::NormalCalculation::Method)

TestNormalCalculation::TestNormalCalculation(QObject* parent)
  : QObject(parent)
{
}

void
TestNormalCalculation::calculate_data()
{
  QTest::addColumn<std::string>("fileName");
  QTest::addColumn<lib::NormalCalculation::Method>("method");

  QTest::newRow("bouddha1m.off/OpenMP")
    << "bouddha1m.off"s << lib::NormalCalculation::Method::OpenMP;
  QTest::newRow("bouddha1m.off/pThread")
    << "bouddha1m.off"s << lib::NormalCalculation::Method::pThread;
}

void
TestNormalCalculation::calculate()
{
  QFETCH(std::string, fileName);
  QFETCH(lib::NormalCalculation::Method, method);

  std::ifstream file(fileName);
  lib::Object object = lib::Object::readOFF(file);

  lib::SequentialNormalCalculation seq(object);
  std::unique_ptr<lib::NormalCalculation> nc(
    lib::NormalCalculation::factory(method, object));

  seq.calculate();
  nc->calculate();

//  qDebug() << seq.faceNormal().size() << seq.vertexNormal().size();
//  qDebug() << nc->faceNormal().size() << nc->vertexNormal().size();

  //for (size_t i = 0; i < seq.vertexNormal().size(); ++i)
   //   std::cout << seq.vertexNormal()[i] << " <-> " << nc->vertexNormal()[i] << "\n";

  QCOMPARE(seq.faceNormal().size(), nc->faceNormal().size());
  QCOMPARE(seq.vertexNormal().size(), nc->vertexNormal().size());

  for (size_t i = 0; i < seq.faceNormal().size(); ++i) {
      QCOMPARE(seq.faceNormal()[i].x+1, nc->faceNormal()[i].x+1);
      QCOMPARE(seq.faceNormal()[i].y+1, nc->faceNormal()[i].y+1);
      QCOMPARE(seq.faceNormal()[i].z+1, nc->faceNormal()[i].z+1);
  }

  for (size_t i = 0; i < seq.vertexNormal().size(); ++i) {
      QCOMPARE(seq.vertexNormal()[i].x+1, nc->vertexNormal()[i].x+1);
      QCOMPARE(seq.vertexNormal()[i].y+1, nc->vertexNormal()[i].y+1);
      QCOMPARE(seq.vertexNormal()[i].z+1, nc->vertexNormal()[i].z+1);
  }
}

QTEST_MAIN(TestNormalCalculation)
//#include "testqstring.moc"
