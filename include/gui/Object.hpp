#ifndef GUI_OBJECT_HPP
#define GUI_OBJECT_HPP

#include <lib/NormalCalculation.hpp>
#include <lib/Object.hpp>

#include <memory>

#include <QUrl>
#include <QVector3D>
#include <Qt3DCore/QComponent>
#include <Qt3DCore/QEntity>
#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QGeometry>
#include <Qt3DRender/QGeometryRenderer>

namespace gui {

class Object : public Qt3DRender::QGeometryRenderer
{
  Q_OBJECT
  Q_PROPERTY(QVector3D min READ min NOTIFY objectChanged)
  Q_PROPERTY(QVector3D max READ max NOTIFY objectChanged)
  Q_PROPERTY(QVector3D centroid READ centroid NOTIFY objectChanged)
  Q_PROPERTY(int nbVertices READ nbVertices NOTIFY objectChanged)
  Q_PROPERTY(int nbFaces READ nbFaces NOTIFY objectChanged)

public:
  typedef lib::NormalCalculation::Method CalculNormalMethod;
  Q_ENUM(CalculNormalMethod)

  Object(QNode* parent = nullptr);
  virtual ~Object();

  inline QVector3D min() const
  {
    return { _object.min().x, _object.min().y, _object.min().z };
  }
  inline QVector3D max() const
  {
    return { _object.max().x, _object.max().y, _object.max().z };
  }
  inline QVector3D centroid() const
  {
    return { _object.centroid().x, _object.centroid().y, _object.centroid().z };
  }

  inline size_t nbVertices() const { return _object.vertices().size(); }
  inline size_t nbFaces() const { return _object.faces().size(); }

public slots:
  void load();
  void calculateNormals(const CalculNormalMethod m);

  void readOFF(const QUrl& fileName);
  void writeOFF(const QUrl& fileName) const;
  void writeOBJ(const QUrl& fileName) const;

signals:
  void objectChanged();

private:
  lib::Object _object;
  std::unique_ptr<lib::NormalCalculation> _normalCalculation;

  Qt3DRender::QBuffer* _buffVertices;
  Qt3DRender::QBuffer* _buffIndexes;
  Qt3DRender::QBuffer* _buffNormals;
  Qt3DRender::QAttribute* _attVertices;
  Qt3DRender::QAttribute* _attIndexes;
  Qt3DRender::QAttribute* _attNormals;
};

} // namespace gui

#endif
