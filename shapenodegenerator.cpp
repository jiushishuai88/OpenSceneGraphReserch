/******************************************************************************
*
* Copyright (c) 20XX-20XX, XXXXXXXX有限公司
*
* All rights reserved.
*
* 文 件 名: shapenodegenerator.cpp
*
* 作    者: guchuan
*
* 创建日期: 2021.10
*
* 描    述: osg图形生成器，用于生成各种图形的原始数据
*
******************************************************************************/
#pragma execution_character_set("utf-8")

#include "shapenodegenerator.h"
#include <osg/Geometry>
#include <osg/Geode>
#include <osgUtil/Tessellator>
#include <osg/ShapeDrawable>
#include <osg/Texture2D>
#include <osg/Material>
#include <osgDB/ReadFile>
#include <osgUtil/SmoothingVisitor>

 ShapeNodeGenerator* ShapeNodeGenerator::m_pSelf =nullptr;

ShapeNodeGenerator::ShapeNodeGenerator()
{

}

 ShapeNodeGenerator* ShapeNodeGenerator::GetInstance()
 {
     if(m_pSelf == nullptr)
     {
         m_pSelf = new ShapeNodeGenerator();
     }
     return m_pSelf;
 }

osg::ref_ptr<osg::MatrixTransform> ShapeNodeGenerator::GetRectangleGrid(const osg::Vec3& upleft,float width,float height,
                                          osg::Vec3& nTo,int wDim,int hDim)
{
    osg::Vec3 nFrom(0,0,1);
    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    float xStep = width/wDim;
    float yStep = -height/hDim;
    for(int h = 0;h<hDim+1;++h)
    {
        for(int w=0;w<wDim+1;++w)
        {
            v->push_back(osg::Vec3(w*xStep,h*yStep,0));
        }
    }

    osg::ref_ptr<osg::DrawElementsUInt> indices = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS,hDim*wDim*4);
    osg::ref_ptr<osg::Vec3Array> n=new osg::Vec3Array();
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    n->push_back(nFrom);
    int colorindex = 0;
    for(int h=0;h<hDim;++h)
    {
        for(int w=0;w<wDim;++w)
        {
            osg::ref_ptr<osg::Vec3Array> vecQuads = new osg::Vec3Array();
            vecQuads->push_back( (*v)[h*(wDim+1)+w+1]);
            vecQuads->push_back( (*v)[h*(wDim+1)+w]);
            vecQuads->push_back( (*v)[(h+1)*(wDim+1)+w]);
            vecQuads->push_back( (*v)[(h+1)*(wDim+1)+w+1]);
            osg::ref_ptr<osg::Geometry> geom=new osg::Geometry();
            geom->setVertexArray(vecQuads.get());
            geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));
            geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP,0,4));

            osg::ref_ptr<osg::Vec4Array> colorArray=new osg::Vec4Array();
            if((colorindex%2) == 1)
            {
               colorArray->push_back(osg::Vec4(1,0,0,1));
            }
            else
            {
               colorArray->push_back(osg::Vec4(0,1,0,1));
            }
            ++colorindex;
            colorArray->push_back(osg::Vec4(0,0,0,1));
            geom->setColorArray(colorArray.get());
            geom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);

            geom->setNormalArray(n.get());//添加法线到几何体中
            geom->setNormalBinding(osg::Geometry::BIND_OVERALL);//将法线进行绑定
            geode->addDrawable(geom.get());
        }
    }
    osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
    mt->setMatrix(osg::Matrix::rotate(nFrom,nTo)*osg::Matrix::translate(upleft));
    mt->addChild(geode.get());
    return mt;
}

osg::ref_ptr<osg::MatrixTransform> ShapeNodeGenerator::GetCircleGrid(const osg::Vec3& center,float radius,int rDim,int arcDim,
                                              osg::Vec3& nTo)
{
    int points = 20000;
    points =(points/arcDim)*arcDim;//取倍数
    osg::Vec3 nFrom(0,0,1);
    QVector<osg::ref_ptr<osg::Vec3Array>> arrPtr;
    float angle = 2*osg::PI/points;
    float deltaR = radius/rDim;
    for(int i = 0;i<rDim+1;++i)
    {
       osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
       arrPtr.push_back(v);
       float r = i*deltaR;
       for(int i = 0;i<points;++i)
       {
           v->push_back(osg::Vec3(r*cosf(i*angle),r*sinf(i*angle),0));
       }
    }

    int pointsPerArc =points/arcDim;
    int colorindex = 0;
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    for(int rIndex = 0;rIndex<rDim;++rIndex)
    {
        for(int arcIndex=0;arcIndex<arcDim;++arcIndex)
        {
            osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
            for(int i=0;i<pointsPerArc;++i)
            {
                osg::ref_ptr<osg::Vec3Array> ptr = arrPtr[rIndex+1];
                int id =arcIndex*pointsPerArc;
                v->push_back((*ptr)[id+i]);
            }

            for(int i=0;i<pointsPerArc;++i)
            {
                osg::ref_ptr<osg::Vec3Array> ptr = arrPtr[rIndex];
                int id =(arcIndex+1)*pointsPerArc;
                v->push_back((*ptr)[id-i-1]);
            }
            osg::ref_ptr<osg::Geometry> gemo = new osg::Geometry();
            gemo->setVertexArray(v.get());
            gemo->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POLYGON,0,v->size()));
            gemo->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP,0,v->size()));

            osg::ref_ptr<osg::Vec4Array> colorArray=new osg::Vec4Array();
            if((colorindex%3) == 1)
            {
               colorArray->push_back(osg::Vec4(1,0,0,1));
            }
            else if((colorindex%3) == 2)
            {
               colorArray->push_back(osg::Vec4(0,1,0,1));
            }
            else
            {
               colorArray->push_back(osg::Vec4(0,0,1,1));
            }
            ++colorindex;
            //colorArray->push_back(osg::Vec4(0,0,0,1));
            gemo->setColorArray(colorArray.get());
            gemo->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);

            osg::ref_ptr<osg::Vec3Array> n=new osg::Vec3Array();
            n->push_back(nFrom);
            gemo->setNormalArray(n.get());
            gemo->setNormalBinding(osg::Geometry::BIND_OVERALL);

            //创建分格化对象(支持凹多边形)
            osg::ref_ptr<osgUtil::Tessellator> tscx = new osgUtil::Tessellator;
            //设置分格类型为几何体
            tscx->setTessellationType(osgUtil::Tessellator::TESS_TYPE_POLYGONS);
            //设置只显示轮廓线为false。设置环绕规则
            tscx->setWindingType(osgUtil::Tessellator::TESS_WINDING_ODD);
            //使用分格化
            tscx->retessellatePolygons(*(gemo.get()));
            geode->addDrawable(gemo);

        }
    }
     nTo.normalize();
     osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
     mt->setMatrix(osg::Matrix::rotate(nFrom,nTo)*osg::Matrix::translate(center));
     mt->addChild(geode);
     return mt;
}

  osg::ref_ptr<osg::MatrixTransform> ShapeNodeGenerator::GetCylinder(const osg::Vec3& center,float radius,float height ,osg::Vec3 nTo,bool useText2d)
  {
      osg::Vec3 nFrom(0,0,1);
      osg::ref_ptr<osg::Geode> geode = new osg::Geode();
      osg::ref_ptr<osg::Cylinder> cylinder    = new osg::Cylinder( osg::Vec3(0,0,0),radius,height);
      osg::ref_ptr<osg::ShapeDrawable> sd = new osg::ShapeDrawable(cylinder);
      osg::Vec4 color(233./256.,231./256.,239./256.,1);
      sd->setColor(color);
      geode->addDrawable(sd);
      if(useText2d)
      {
          geode->getOrCreateStateSet()->setTextureAttribute(0, new osg::Texture2D(osgDB::readImageFile("D:/WorkSpace/624/OsgOpenGL/OsgOpenGl/texture2d/wenli1.jpeg")));
          geode->getOrCreateStateSet()->setTextureMode(0, GL_TEXTURE_2D, osg::StateAttribute::ON);

      }
      //设置材质
      osg::ref_ptr<osg::Material> material = new osg::Material;
      //设置正面散射颜色
      material->setDiffuse(osg::Material::FRONT, color);
      //设置正面镜面颜色
      material->setSpecular(osg::Material::FRONT, osg::Vec4(0.2, 0.2, 0.2, .5));
      //设置正面指数
      material->setShininess(osg::Material::FRONT, 90.0);
      material->setColorMode(osg::Material::AMBIENT);
      geode->getOrCreateStateSet()->setAttribute(material.get());

      osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
      mt->setMatrix(osg::Matrix::rotate(nFrom,nTo)*osg::Matrix::translate(center));
      mt->addChild(geode);
      return mt;
  }

   osg::ref_ptr<osg::MatrixTransform> ShapeNodeGenerator::GetCone(const osg::Vec3& center,float radius,float height ,osg::Vec3 nTo)
   {
       osg::Vec3 nFrom(0,0,1);
       osg::ref_ptr<osg::Geode> geode = new osg::Geode();
       osg::ref_ptr<osg::Cone> cone    = new osg::Cone( osg::Vec3(0,0,0),radius,height);
       osg::ref_ptr<osg::ShapeDrawable> sd = new osg::ShapeDrawable(cone);
       osg::Vec4 color(233./256.,231./256.,239./256.,0.8);
       sd->setColor(color);
       geode->addDrawable(sd);
       //geode->getOrCreateStateSet()->setTextureAttribute(0, new osg::Texture2D(osgDB::readImageFile("D:/WorkSpace/624/OsgOpenGL/OsgOpenGl/texture2d/wenli1.jpeg")));
       //geode->getOrCreateStateSet()->setTextureMode(0, GL_TEXTURE_2D, osg::StateAttribute::ON);
       //设置材质
       osg::ref_ptr<osg::Material> material = new osg::Material;
       //设置正面散射颜色
       material->setDiffuse(osg::Material::FRONT, color);
       //设置正面镜面颜色
       material->setSpecular(osg::Material::FRONT, osg::Vec4(0.2, 0.2, 0.2, .5));
       //设置正面指数
       material->setShininess(osg::Material::FRONT, 90.0);
       material->setColorMode(osg::Material::AMBIENT);
       geode->getOrCreateStateSet()->setAttribute(material.get());

       osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
       mt->setMatrix(osg::Matrix::rotate(nFrom,nTo)*osg::Matrix::translate(center));
       mt->addChild(geode);
       return mt;
   }

   void ShapeNodeGenerator::GenerateSurface(osg::Geometry* geom,int* index1,int* index2,int col)
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

   osg::ref_ptr<osg::MatrixTransform> ShapeNodeGenerator::GetPipe(PipeData* pData)
   {
       ulong points = ComputePointsByRadius(pData->extRadius);
       osg::ref_ptr<osg::Vec2Array> vec2Arr = new osg::Vec2Array();
       vec2Arr->push_back(osg::Vec2(pData->length/2,pData->extRadius));
       vec2Arr->push_back(osg::Vec2(-pData->length/2,pData->extRadius));
       vec2Arr->push_back(osg::Vec2(-pData->length/2,pData->innerRadius));
       vec2Arr->push_back(osg::Vec2(pData->length/2,pData->innerRadius));

       int numElements = vec2Arr->getNumElements();
       int* indexArray = new int[numElements*points];
       for(int i = 0;i<numElements*points;++i)
       {
          indexArray[i]=i;
       }
       osg::ref_ptr<osg::Vec3Array> v= new osg::Vec3Array;
       for(int i =0;i<numElements;++i)
       {
           osg::Vec2f v2=(*vec2Arr)[i];
           osg::ref_ptr<osg::Vec3Array> v3Points= GenerateCirclePoints(v2.y(),points-1);
           V3ArrayTransform(v3Points,osg::Matrix::translate(osg::Vec3(0,0,v2.x())));
           LoopPoints(v3Points);
           v->insert(v->end(),v3Points->begin(),v3Points->end());
       }
       osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
       geom->setVertexArray(v.get());
       for(int i= 0;i<numElements;++i)
       {
            GenerateSurface(geom,indexArray+i*points,indexArray+((i+1)%numElements)*points,points);
       }


        osgUtil::SmoothingVisitor::smooth( *geom );

        osg::ref_ptr<osg::Vec4Array> colorArray=new osg::Vec4Array();
        colorArray->push_back(osg::Vec4(1,1,0,1));
        geom->setColorArray(colorArray.get());
        geom->setColorBinding(osg::Geometry::BIND_OVERALL);

        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        geode->addDrawable(geom.get());

        osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
        //mt->setMatrix(osg::Matrix::rotate(nFrom,nTo)*osg::Matrix::translate(center));
        mt->addChild(geode);
        delete []indexArray;

      return mt;
   }

void ShapeNodeGenerator::GetOgivePipe(OgivePipeData* pData)
{
   float arcRadius;
   //计算圆弧半径
   arcRadius=(pData->length/2)/sinf(pData->arc/2);
   ulong points = ComputePointsByRadius(pData->extRadius,pData->arc);

}

osg::ref_ptr<osg::Vec3Array> ShapeNodeGenerator::GenerateCirclePoints(float radius,int points,float arcFrom ,float arc)
{
  float angle = arc/points;
  osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
  for(int i = 0;i<points;++i)
  {
      v->push_back(osg::Vec3(radius*cosf(arcFrom+i*angle),radius*sinf(arcFrom+i*angle),0));
  }
  return v;
}

void ShapeNodeGenerator::LoopPoints( osg::ref_ptr<osg::Vec3Array> pArr)
 {
    pArr->push_back(pArr->front());
 }


 ulong ShapeNodeGenerator::ComputePointsByRadius(float r,float arc)
 {
     return ceil(100*2*arc*r);
 }

 void ShapeNodeGenerator::V3ArrayTransform(osg::Vec3Array* pArr,const osg::Matrixd& mat)
 {
     for(uint i= 0;i < pArr->getNumElements();++i)
     {
         (*pArr)[i] = mat.preMult((*pArr)[i]);
     }
 }
