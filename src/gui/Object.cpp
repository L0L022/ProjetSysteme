#include <gui/Object.hpp>

#include <fstream>
#include <sstream>

#include <QDataStream>
#include <QDebug>

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

  _attVertices = new Qt3DRender::QAttribute(
    _buffVertices,
    Qt3DRender::QAttribute::defaultPositionAttributeName(),
    Qt3DRender::QAttribute::Float,
    3,
    0,
    0,
    0,
    this);
  //_attIndexes = new Qt3DRender::QAttribute(_buffIndexes,
  // Qt3DRender::QAttribute::UnsignedInt, 1, 0, 0, 1*sizeof(uint32_t), this);
  _attNormals = new Qt3DRender::QAttribute(
    _buffNormals,
    Qt3DRender::QAttribute::defaultNormalAttributeName(),
    Qt3DRender::QAttribute::Float,
    3,
    0,
    0,
    0,
    this);

  auto* geometry = new Qt3DRender::QGeometry(this);
  geometry->addAttribute(_attVertices);
  // geometry->addAttribute(_attIndexes);
  geometry->addAttribute(_attNormals);

  auto* renderer = this; // new Qt3DRender::QGeometryRenderer(this);
  renderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Triangles);
  renderer->setInstanceCount(1);
  renderer->setIndexOffset(0);
  renderer->setFirstInstance(0);
  renderer->setGeometry(geometry);
  // addComponent(renderer);
}

Object::~Object() {}

void
Object::load()
{
  // QByteArray bytes;
  // QDataStream stream(&bytes, QIODevice::WriteOnly);
  // stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
  // stream.setByteOrder(QDataStream::LittleEndian);
  // for (const lib::Vertex& v : _object.vertices()) {
  //   stream << float(v.x);
  //   stream << float(v.y);
  //   stream << float(v.z);
  // }
  // _buffVertices->setData(bytes);
  // _attVertices->setCount(nbVertices());
  //
  // QByteArray bytesI;
  // QDataStream streamI(&bytesI, QIODevice::WriteOnly);
  // streamI.setFloatingPointPrecision(QDataStream::SinglePrecision);
  // streamI.setByteOrder(QDataStream::LittleEndian);
  // for (const lib::Face& f : _object.faces()) {
  //   stream << uint32_t(f.v0);
  //   stream << uint32_t(f.v1);
  //   stream << uint32_t(f.v2);
  // }
  // _buffIndexes->setData(bytesI);
  // _attIndexes->setCount(nbFaces());

  {
    QByteArray bytes;
    QDataStream stream(&bytes, QIODevice::WriteOnly);
    stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    stream.setByteOrder(QDataStream::LittleEndian);
    for (const lib::Face& f : _object.faces()) {
      lib::Vertex v = _object.vertices()[f.v0];
      stream << float(v.x);
      stream << float(v.y);
      stream << float(v.z);
      v = _object.vertices()[f.v1];
      stream << float(v.x);
      stream << float(v.y);
      stream << float(v.z);
      v = _object.vertices()[f.v2];
      stream << float(v.x);
      stream << float(v.y);
      stream << float(v.z);
    }
    _buffVertices->setData(bytes);
    _attVertices->setCount(nbFaces() * 3);
  }

  calculateNormals(lib::NormalCalculation::Method::Sequential);

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
Object::readOFF(const QUrl& fileName)
{
  ifstream file(fileName.toLocalFile().toStdString(), std::ios::binary);
  stringstream ss;
  ss << file.rdbuf();
  _object = lib::Object::readOFF(ss);
  emit objectChanged();
  load();
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
