#include "ogive.h"
#include "shapenodegenerator.h"
#include <osg/Drawable>
#include <osg/Geometry>

Ogive::Ogive():
    m_baseNormal(0,0,1),
    m_pExtArray(new osg::Vec3Array),
    m_pInnerArray(new osg::Vec3Array),
    m_pGeom(new osg::Geometry)
{

}

Ogive::Ogive(eggData data):
    m_baseNormal(0,0,1),
    m_pExtArray(new osg::Vec3Array),
    m_pInnerArray(new osg::Vec3Array),
    m_pGeom(new osg::Geometry),
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


void Ogive::Update()
{
    m_extPointUp = {m_eggData.extRadiusLeft/2,0,m_eggData.length/2};
    m_extPointDown = {m_eggData.extRadiusRight/2,0,-m_eggData.length/2};
    m_innerPointUp = {m_eggData.innerRadiusLeft/2,0,m_eggData.length/2};
    m_innerPointDown = {m_eggData.innerRadiusRight/2,0,-m_eggData.length/2};
    m_extCenter = ComputeCircleCenter(m_extPointUp,m_extPointDown,m_eggData.arcExt);
    m_innerCenter = ComputeCircleCenter(m_innerPointUp,m_innerPointDown,m_eggData.arcInner);
    osg::Vec3 vExtRightToCenter =  m_extPointDown-m_extCenter;
    m_extArcR = vExtRightToCenter.length();

    osg::Vec3 vInnerRightToCenter = m_innerPointDown-m_innerCenter;
    m_innerArcR = vInnerRightToCenter.length();
    float maxArc= (abs(m_eggData.arcExt)>abs(m_eggData.arcInner))?m_eggData.arcExt:m_eggData.arcInner;

    m_arcPoints = ComputePointsByRadius(m_extArcR,maxArc);
    m_circlePoints = ComputePointsByRadius(m_extArcR-abs(vExtRightToCenter.x()),osg::PI*2);
    if(abs(m_eggData.arcExt)>0.01)
    {
       m_pExtArray = GenerateArcPoints(m_extCenter,m_extPointUp,m_extPointDown,m_arcPoints);
    }
    else
    {
        m_pExtArray->clear();
        m_pExtArray->push_back(m_extPointUp);
        m_pExtArray->push_back(m_extPointDown);
    }
    if(abs(m_eggData.arcInner)>0.01)
    {
       m_pInnerArray = GenerateArcPoints(m_innerCenter,m_innerPointDown,m_innerPointUp,m_arcPoints);
    }
    else
    {
        m_pInnerArray->clear();
        m_pInnerArray->push_back(m_innerPointDown);
        m_pInnerArray->push_back(m_innerPointUp);
    }
    GenerateGeom();
}

osg::ref_ptr<osg::Vec3Array> Ogive::GenerateArcPoints(const osg::Vec3 &center,const float &radius,const int &points,const float &arcFrom ,const float &arcTo)
{

  float angle = (arcTo-arcFrom)/points;
  osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
  for(int i = 0;i<points;++i)
  {
      v->push_back(osg::Vec3(radius*cosf(arcFrom+i*angle),0,radius*sinf(arcFrom+i*angle))+center);
  }
  return v;
}

osg::ref_ptr<osg::Vec3Array> Ogive::GenerateArcPoints(const osg::Vec3 &center,const osg::Vec3 &pointFrom,const osg::Vec3 &pointTo,const int &points)
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

 osg::ref_ptr<osg::Geometry> Ogive::GetGeometry()
 {
    return m_pGeom;
 }

 void Ogive::GenerateSurface(osg::Geometry* geom,int* index1,int* index2,int col)
 {
     int lineNum = 2;//默认为2条线形成面
     osg::ref_ptr<osg::DrawElementsUInt> indices =new osg::DrawElementsUInt(GL_TRIANGLE_STRIP, col*lineNum);
     for(int i=0;i<col;++i)
     {
          (*indices)[i*lineNum] =index1[i];
          (*indices)[i*lineNum+1] = index2[i];
     }

     geom->addPrimitiveSet( indices.get() );
 }

 void Ogive::LoopPoints( osg::ref_ptr<osg::Vec3Array> pArr)
  {
     pArr->push_back(pArr->front());
  }

 void Ogive::V3ArrayTransform(osg::Vec3Array* pArr,const osg::Matrixd& mat)
 {
     for(uint i= 0;i < pArr->getNumElements();++i)
     {
         (*pArr)[i] = mat.preMult((*pArr)[i]);
     }
 }

 osg::ref_ptr<osg::Vec3Array> Ogive::GenerateCircle(float radius,int points)
 {
   float angle = (osg::PI*2)/points;
   osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
   for(int i = 0;i<points;++i)
   {
       osg::Vec3 v3(radius*cosf(i*angle),radius*sinf(i*angle),0);
       v->push_back(osg::Vec3(radius*cosf(i*angle),radius*sinf(i*angle),0));
   }
   return v;
 }

 void Ogive::GenerateGeom()
 {
     osg::ref_ptr<osg::Vec3Array> arcCombineArray = new osg::Vec3Array();
     osg::ref_ptr<osg::Vec3Array> pExtArray = GetExtArcArray();
     arcCombineArray->insert(arcCombineArray->end(),pExtArray->begin(),pExtArray->end());
     osg::ref_ptr<osg::Vec3Array> pInnerArray = GetInnerArcArray();
     arcCombineArray->insert(arcCombineArray->end(),pInnerArray->begin(),pInnerArray->end());

     int numElements = arcCombineArray->getNumElements();
     //生成顶点序列号
     int* indexArray = new int[numElements*m_circlePoints];
     for(int i = 0;i<numElements*m_circlePoints;++i)
     {
        indexArray[i]=i;
     }
     osg::ref_ptr<osg::Vec3Array> v= new osg::Vec3Array;
     osg::ref_ptr<osg::Vec2Array> texcoords= new osg::Vec2Array;
     //通过外轮廓生成三维顶点
     for(int i =0;i<numElements;++i)
     {
         osg::Vec3f v3=(*arcCombineArray)[i];
         osg::ref_ptr<osg::Vec3Array> v3Points= GenerateCircle(v3.x(),m_circlePoints-1);
         V3ArrayTransform(v3Points,osg::Matrix::translate(osg::Vec3(0,0,v3.z())));
         LoopPoints(v3Points);
         v->insert(v->end(),v3Points->begin(),v3Points->end());
     }
     //计算纹理UV值，纹理贴图只贴表面
     int extNumElements = pExtArray->getNumElements();
       for(uint i =0;i<numElements;++i)
       {
           for(uint j=0;j<m_circlePoints;++j)
           {
               texcoords->push_back(osg::Vec2f(float(j)/float((m_circlePoints-1)),float(extNumElements-1-i)/float((extNumElements-1))));
           }
       }

     m_pGeom->setVertexArray(v.get());
     //arg0:the index of text2d
     m_pGeom->setTexCoordArray(0,texcoords);
     for(int i= 0;i<numElements;++i)
     {
          GenerateSurface(m_pGeom,indexArray+i*m_circlePoints,indexArray+((i+1)%numElements)*m_circlePoints,m_circlePoints);
     }
     delete []indexArray;
 }
