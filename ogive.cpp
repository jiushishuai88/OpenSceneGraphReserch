#include "ogive.h"
#include "shapenodegenerator.h"

Ogive::Ogive():
    m_baseNormal(0,0,1),
    m_pExtArray(new osg::Vec3Array),
    m_pInnerArray(new osg::Vec3Array)
{

}

Ogive::Ogive(eggData data):
    m_baseNormal(0,0,1),
    m_pExtArray(new osg::Vec3Array),
    m_pInnerArray(new osg::Vec3Array),
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
    if(abs(arc)>0.01)
    {
        osg::Vec3 v1 = p0-p1;
        osg::Vec3 anchor = (p0+p1)/2;

        //绕y轴旋转
        osg::Vec3 v2 = osg::Matrix::rotate(-osg::PI/2,osg::Vec3(0,1,0)).preMult(v1);
        v2.normalize();
        float lenghV2 = (v1.length()/2)/tan(arc/2);
        v2*=lenghV2;
        Center = anchor+v2;
    }
    else
    {
        Center = {0,0,0};
    }
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
    m_extArcR = vExtRightToCenter.length();

    osg::Vec3 vInnerRightToCenter = m_innerPointRight-m_innerCenter;
    m_innerArcR = vInnerRightToCenter.length();
    float maxArc= (abs(m_eggData.arcExt)>abs(m_eggData.arcInner))?m_eggData.arcExt:m_eggData.arcInner;

    m_arcPoints = ComputePointsByRadius(m_extArcR,maxArc);
    m_circlePoints = ComputePointsByRadius(m_extArcR-abs(vExtRightToCenter.x()),osg::PI*2);
    if(abs(m_eggData.arcExt)>0.01)
    {
       m_pExtArray = GenerateCirclePoints(m_extCenter,m_extPointLeft,m_extPointRight,m_arcPoints);
    }
    else
    {
        m_pExtArray->clear();
        m_pExtArray->push_back(m_extPointLeft);
        m_pExtArray->push_back(m_extPointRight);
    }
    if(abs(m_eggData.arcInner)>0.01)
    {
       m_pInnerArray = GenerateCirclePoints(m_innerCenter,m_innerPointRight,m_innerPointLeft,m_arcPoints);
    }
    else
    {
        m_pInnerArray->clear();
        m_pInnerArray->push_back(m_innerPointRight);
        m_pInnerArray->push_back(m_innerPointLeft);
    }
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

osg::ref_ptr<osg::Vec3Array> Ogive::GenerateCirclePoints(const osg::Vec3 &center,const osg::Vec3 &pointFrom,const osg::Vec3 &pointTo,const int &points)
{
     osg::Vec3 vFrom =pointFrom-center;
     osg::Vec3 vTo =pointTo-center;
     osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
     osg::Quat quatTo;
     quatTo= {osg::Matrix::rotate(vFrom,vTo).getRotate()};
     osg::Quat quatFrom;
     quatFrom = {osg::Matrix::rotate(vFrom,vFrom).getRotate()};
     for(int i=0;i<points;++i)
     {
         osg::Quat quatInter;
         double interpolation = double(i)/double((points-1));
         quatInter.slerp(interpolation,quatFrom,quatTo);
         osg::Vec3 point = osg::Matrix::rotate(quatInter).preMult(vFrom)+center;
         v->push_back(point);
     }
     return v;
}

ulong Ogive::ComputePointsByRadius(float r,float arc)
{
    return (ulong)abs(ceil(10*2*arc*r));
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

