#pragma once

#pragma warning (disable : 4005)
#pragma warning (disable : 4251)
#pragma warning (disable : 4819)


#include <d3d11.h>
#include <DirectXMath.h>

#include <Effects11/d3dx11effect.h>
#include <DirectXTK/DDSTextureLoader.h>
#include <DirectXTK/WICTextureLoader.h>
#include <DirectXTK/SpriteBatch.h>
#include <DirectXTK/SpriteFont.h>
#include <DirectXTK/ScreenGrab.h>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <DirectXCollision.h>

#include <d3dcompiler.h>

#include <typeinfo.h>

#include <opencv2\imgcodecs.hpp>
#include <opencv2\imgproc.hpp>


using namespace DirectX;
using namespace cv;

#define DIRECTINPUT_VERSION  0x0800
#include <dinput.h>

#include <process.h>

#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>

#include <fstream>
#include <iostream>
#include <string>
#include "json/json.hpp"
using json = nlohmann::json;

#include "Engine_Typedef.h"
#include "Engine_Function.h"
#include "Engine_Functor.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"


//IMGUI 쓰려면 없에야함; 
//#ifdef _DEBUG
//
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//
//#ifndef DBG_NEW 
//
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
//#define new DBG_NEW 
//
//#endif
//
//#endif	//_DEBUG
//#include <tchar.h>
//imgui

using namespace Engine;
using namespace std;



