#include <gui/Object.hpp>

#include <fstream>
#include <sstream>

#include <QDataStream>
#include <QDebug>
#include <QFile>

using namespace gui;
using namespace std;

Object::Object(QNode* parent)
  : Qt3DRender::QGeometryRenderer(parent)
  , _buffVertices(nullptr)
  , _buffIndexes(nullptr)
  , _buffNormals(nullptr)
  , _attVertices(nullptr)
  , _attIndexes(nullptr)
  , _attNormals(nullptr)
{
  _buffVertices =
    new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, this);
  _buffIndexes =
    new Qt3DRender::QBuffer(Qt3DRender::QBuffer::IndexBuffer, this);
  _buffNormals =
    new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, this);

  _attVertices = new Qt3DRender::QAttribute(this);
  _attVertices->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
  _attVertices->setDataType(Qt3DRender::QAttribute::Float);
  _attVertices->setDataSize(3);
  _attVertices->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
  _attVertices->setBuffer(_buffVertices);
  _attVertices->setByteStride(3 * sizeof(float));

  _attIndexes = new Qt3DRender::QAttribute(this);
  _attIndexes->setDataType(Qt3DRender::QAttribute::UnsignedInt);
  _attIndexes->setBuffer(_buffIndexes);
  _attIndexes->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);

  _attNormals = new Qt3DRender::QAttribute(this);
  _attNormals->setName(Qt3DRender::QAttribute::defaultNormalAttributeName());
  _attNormals->setDataType(Qt3DRender::QAttribute::Float);
  _attNormals->setDataSize(3);
  _attNormals->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
  _attNormals->setBuffer(_buffVertices);
  _attNormals->setByteStride(3 * sizeof(float));

  auto* geometry = new Qt3DRender::QGeometry(this);
  geometry->addAttribute(_attVertices);
  geometry->addAttribute(_attIndexes);
  geometry->addAttribute(_attNormals);

  auto* renderer = this;
  renderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Triangles);
  renderer->setInstanceCount(1);
  renderer->setIndexOffset(0);
  renderer->setFirstInstance(0);
  renderer->setGeometry(geometry);
}

Object::~Object() {}

void
Object::load()
{
  {
    QByteArray bytes;
    QDataStream stream(&bytes, QIODevice::WriteOnly);
    stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    stream.setByteOrder(QDataStream::LittleEndian);
    for (const lib::Vertex& v : _object.vertices()) {
      stream << float(v.x);
      stream << float(v.y);
      stream << float(v.z);
    }
    _buffVertices->setData(bytes);
    _attVertices->setCount(nbVertices());
  }
  {
    QByteArray bytes;
    QDataStream stream(&bytes, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);
    for (const lib::Face& f : _object.faces()) {
      stream << uint32_t(f.v0);
      stream << uint32_t(f.v1);
      stream << uint32_t(f.v2);
    }
    _buffIndexes->setData(bytes);
    _attIndexes->setCount(nbFaces() * 3);
  }

  calculateNormals(lib::NormalCalculation::Method::pThread);

  {
    QByteArray bytes;
    QDataStream stream(&bytes, QIODevice::WriteOnly);
    stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    stream.setByteOrder(QDataStream::LittleEndian);
    for (const lib::Vector& v : _normalCalculation->vertexNormal()) {
      stream << float(v.x);
      stream << float(v.y);
      stream << float(v.z);
    }
    _buffNormals->setData(bytes);
    _attNormals->setCount(nbVertices());
  }
}

void
Object::calculateNormals(const CalculNormalMethod m)
{
  _normalCalculation.reset(lib::NormalCalculation::factory(m, _object));
  // TODO mettre du temps !
  _normalCalculation->calculate();
}

void
Object::readOFF(const QString& fileName)
{
  QFile file(fileName);
  //  ifstream file(fileName.toLocalFile().toStdString(), std::ios::binary);
  if (file.open(QIODevice::ReadOnly)) {
    //  file.setTextModeEnabled(false);
    stringstream ss;
    //  ss << file.rdbuf();
    ss << file.readAll().toStdString();
    _object = lib::Object::readOFF(ss);
    emit objectChanged();
    load();
  } else {
    qDebug() << "Prout";
  }
}

void
Object::writeOFF(const QUrl& fileName) const
{
  stringstream ss;
  _object.writeOFF(ss);
  ofstream file(fileName.toLocalFile().toStdString(),
                std::ios::binary | std::ios::out);
  file << ss.rdbuf();
}

void
Object::writeOBJ(const QUrl& fileName) const
{
  // TODO: si on a pas fait de calcul ça ne marche pas !
  stringstream ss;
  _object.writeOBJ(ss, _normalCalculation->vertexNormal());
  ofstream file(fileName.toLocalFile().toStdString(),
                std::ios::binary | std::ios::out);
  file << ss.rdbuf();
}
