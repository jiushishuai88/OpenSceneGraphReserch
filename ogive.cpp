#include "ogive.h"
#include "shapenodegenerator.h"

Ogive::Ogive():
    m_baseNormal(0,0,1)
{

}

Ogive::Ogive(eggData data):
    m_baseNormal(0,0,1),
    m_eggData(data)
{
    Update();
}

osg::Vec3 Ogive::GetBaseNormal() const
{
    return m_baseNormal;
}

eggData Ogive::GetEggData() const
{
    return m_eggData;
}

osg::Vec3 Ogive::ComputeCircleCenter(const osg::Vec3& p0,const osg::Vec3& p1,const float& arc) const
{
    osg::Vec3 Center;
    osg::Vec3 v1 = p0-p1;
    osg::Vec3 anchor = (p0+p1)/2;

    //绕y轴旋转
    osg::Vec3 v2 = osg::Matrix::rotate(-osg::PI/2,osg::Vec3(0,1,0)).preMult(v1);
    v2.normalize();
    float lenghV2 = (v1.length()/2)/tan(arc/2);
    v2*=lenghV2;
    Center = anchor+v2;
    return Center;
}

osg::ref_ptr<osg::MatrixTransform> Ogive::GetMt()
{
    osg::ref_ptr<osg::MatrixTransform> mt = ShapeNodeGenerator::GetInstance()->GetRoatationOfAxes(this);
    return mt;
}

void Ogive::Update()
{
    m_extPointLeft = {m_eggData.extRadiusLeft/2,0,m_eggData.length/2};
    m_extPointRight = {m_eggData.extRadiusRight/2,0,-m_eggData.length/2};
    m_innerPointLeft = {m_eggData.innerRadiusLeft/2,0,m_eggData.length/2};
    m_innerPointRight = {m_eggData.innerRadiusRight/2,0,-m_eggData.length/2};
    m_extCenter = ComputeCircleCenter(m_extPointLeft,m_extPointRight,m_eggData.arcExt);
    m_innerCenter = ComputeCircleCenter(m_innerPointLeft,m_innerPointRight,m_eggData.arcInner);
    osg::Vec3 vExtRightToCenter =  m_extPointRight-m_extCenter;
    osg::Vec3 vExtLeftToCenter =  m_extPointLeft-m_extCenter;
    m_extArcR = vExtRightToCenter.length();
    m_extArcFrom  =asin(vExtLeftToCenter.z()/m_extArcR);
    m_extArcTo =asin(vExtRightToCenter.z()/m_extArcR);

    osg::Vec3 vInnerRightToCenter = m_innerPointRight-m_innerCenter;
    osg::Vec3 vInnerLeftToCenter = m_innerPointLeft-m_innerCenter;
    m_innerArcR = vInnerRightToCenter.length();
    m_innerArFrom = asin(vInnerRightToCenter.z()/m_innerArcR);
    m_innerArTo = asin(vInnerLeftToCenter.z()/m_innerArcR);

    m_arcPoints = ComputePointsByRadius(m_extArcR,m_eggData.arcExt);
    m_circlePoints = ComputePointsByRadius(m_extArcR-abs(vExtRightToCenter.x()),osg::PI*2);
    m_pExtArray = GenerateCirclePoints(m_extCenter,m_extArcR,m_arcPoints,m_extArcFrom,m_extArcTo);
    m_pInnerArray = GenerateCirclePoints(m_innerCenter,m_innerArcR,m_arcPoints,m_innerArFrom,m_innerArTo);
}

osg::ref_ptr<osg::Vec3Array> Ogive::GenerateCirclePoints(const osg::Vec3 &center,const float &radius,const int &points,const float &arcFrom ,const float &arcTo)
{
  float angle = (arcTo-arcFrom)/points;
  osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
  for(int i = 0;i<points;++i)
  {
      v->push_back(osg::Vec3(radius*cosf(arcFrom+i*angle),0,radius*sinf(arcFrom+i*angle))+center);
  }
  return v;
}

ulong Ogive::ComputePointsByRadius(float r,float arc)
{
    return (ulong)ceil(10*2*arc*r);
}

osg::ref_ptr<osg::Vec3Array> Ogive::GetExtArcArray() const
{
    return m_pExtArray;
}
osg::ref_ptr<osg::Vec3Array> Ogive::GetInnerArcArray() const
{
    return m_pInnerArray;
}

 ulong Ogive::GetCirclePoints() const
 {
     return m_circlePoints;
 }

 void Ogive::SetData(const eggData& data)
 {
     m_eggData = data;
     Update();
 }

