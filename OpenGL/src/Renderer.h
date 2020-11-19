#pragma once
#include "OpenGL.h"
#include "ForwardDecl.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Picker.h"
#include "Shader.h"

namespace OpenGL
{
    class Renderer
    {
        struct Coordinates
        {
            float mX[6] = { 0, 0, 0, 100000, 0, 0 };
            float mY[6] = { 0, 0, 0, 0, 100000, 0 };
            float mZ[6] = { 0, 0, 0, 0, 0, 100000 };
            std::vector<float> mXZ;
            std::vector<unsigned int> mIndexXZ;
            unsigned int mIndex[2] = { 0, 1 };
            VertexBuffer mVBX, mVBY, mVBZ, mVBXZ;
            VertexArray mVAX, mVAY, mVAZ, mVAXZ;
            IndexBuffer mIB, mIBXZ;
            mutable Shader mCoordShader;
            Coordinates()
            {
                mCoordShader.AttachProgram("res/shaders/DefaultCoord.shader");
                mIB.SetData(&mIndex[0], 2);
                VertexBufferLayout layout;
                layout.Push<float>(3);
                mVBX.SetData(&mX[0], sizeof(float) * 6);
                mVAX.AddBuffer(mVBX, layout);
                mVAX.UnBind();
				mVBY.SetData(&mY[0], sizeof(float) * 6);
				mVAY.AddBuffer(mVBY, layout);
				mVAY.UnBind();
				mVBZ.SetData(&mZ[0], sizeof(float) * 6);
				mVAZ.AddBuffer(mVBZ, layout);
				mVAZ.UnBind();
                mIB.UnBind();
                unsigned int xzIndex = 0;
                for (int x = 0; x <= 8; ++x) 
                {
                    mXZ.push_back(x - 4);
                    mXZ.push_back(0);
                    mXZ.push_back(-4);
                    mIndexXZ.push_back(xzIndex++);

					mXZ.push_back(x - 4);
					mXZ.push_back(0);
					mXZ.push_back(4);
                    mIndexXZ.push_back(xzIndex++);
                }
				for (int z = 0; z <= 8; ++z)
				{
					mXZ.push_back(-4);
					mXZ.push_back(0);
					mXZ.push_back(z - 4);
                    mIndexXZ.push_back(xzIndex++);

					mXZ.push_back(4);
					mXZ.push_back(0);
					mXZ.push_back(z - 4);
                    mIndexXZ.push_back(xzIndex++);
				}
                mIBXZ.SetData(&mIndexXZ[0], mIndexXZ.size());
				mVBXZ.SetData(&mXZ[0], sizeof(float) * mXZ.size());
                mVAXZ.AddBuffer(mVBXZ, layout);
                mVAXZ.UnBind();
            }
        };
    private:
        Camera* mpCamera;
        Coordinates mCoords;
        //Picker* mPicker;
        int mWidth, mHeight;
        float mMouseX, mMouseY;
        
    public:
        Renderer();
        void Init();
        void Resize(int width, int height);
        void SetSize(int width, int height);
        void SetCamera(Camera* pCamera);
        void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
        void DrawFaces(const DrawableObject &object) const;
        void DrawWireFrame(const DrawableObject& object) const;
        void DrawObjectId(const DrawableObject& obejct) const;
        void DrawObjectsId(Scene& scene) const;
        void Clear() const;
        void EnableDepthTest() const;
        void DisableDepthTest() const;
        void DrawCoordinates() const;
        void SetMousePoseition(float x, float y);
        unsigned int GetObjectId(int x, int y);
        inline Camera* GetCamera() const { return mpCamera; }

        void TestPick(const DrawableObject &object);
        static unsigned int IsReady();
    private:
		
    private:
        void MousePick();
        friend class GLFW::Previewer;
    };

}