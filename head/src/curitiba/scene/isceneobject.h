//#ifndef ISCENEOBJECT_H
//#define ISCENEOBJECT_H
//
//#include <string>
//
//#include <curitiba/geometry/iboundingvolume.h>
//#include <curitiba/math/itransform.h>
//#include <curitiba/render/irenderable.h>
//
////
//#include <curitiba/event/ilistener.h>
////
//
//#include <fstream>
//
//namespace curitiba
//{
//	namespace scene
//	{
//		class IScene;
//
//		class ISceneObject : public curitiba::event_::IListener
//		{
//		public:
//			virtual int getId () = 0;
//			virtual void setId (int id) = 0;
//
//			virtual std::string& getName () = 0;
//			virtual void setName (const std::string &name) = 0;
//
//			virtual std::string getType (void) = 0;
//
//			virtual void unitize(float min, float max) = 0;
//
//			virtual bool isStatic() = 0;
//			virtual void setStaticCondition(bool aCondition) = 0;
//			
//			virtual ~ISceneObject() {};
//
//			virtual const curitiba::geometry::IBoundingVolume* getBoundingVolume () = 0;
//			virtual void setBoundingVolume (curitiba::geometry::IBoundingVolume *b) = 0;
//
//			virtual const curitiba::math::ITransform& getTransform () = 0;
//			virtual void setTransform (curitiba::math::ITransform *t) = 0; //who's in charge of the disposal of the transform?
//			virtual void burnTransform (void) = 0;
//			virtual curitiba::math::ITransform *_getTransformPtr (void) = 0;
//
//			virtual curitiba::render::IRenderable& getRenderable (void) = 0;
//			virtual curitiba::render::IRenderable* _getRenderablePtr (void) = 0;
//			virtual void setRenderable (curitiba::render::IRenderable *renderable) = 0;
//
//			virtual void writeSpecificData (std::fstream &f) = 0;
//			virtual void readSpecificData (std::fstream &f) = 0;
//
//			virtual void prepareTriangleIDs(bool ids) = 0;
//		};
//	};
//};
//#endif // ISCENEOBJECT_H
//
